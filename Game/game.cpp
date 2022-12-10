#include "game.h"
#include "ray.h"
#include "sphere.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <thread>
#include "hittable_list.h"
#include "light.h"
#include "directional_light.h"
#include "light_list.h"
#include "plane.h"
#include "spotlight.h"

using namespace glm;
#define color_size_bytes 4
#define to_index(i, j) i * image_width * color_size_bytes + j * color_size_bytes
#define MAX_DEPTH 5
#define THREADS_PER_ROW 2
#define air_constant 1.0f
#define material_constant 1.5f
static std::vector<std::tuple<glm::vec4, char>> objects;
static std::vector<glm::vec4> ambient_color;
static std::vector<glm::vec4> object_colors;
static std::vector<glm::vec4> light_intensity;
static std::vector<glm::vec4> direct_lights;
static std::vector<glm::vec4> spotlights;
static std::vector<glm::vec4> eye_camera;
const float infinity = std::numeric_limits<float>::infinity();
const double pi = 3.1415926535897932385;

static void WriteToTxt(unsigned char *data, int image_width, int image_height, std::string name) {
    std::ofstream txt_file;
    txt_file.open(name, std::ofstream::out);
    if (txt_file.is_open()) {
        for (int i = 0; i < image_height; i++) {
            for (int j = 0; j < image_width; j++) {
                txt_file << (int) data[to_index(i, j)] << ",";
            }
            txt_file << '\n';
        }
    } else {
        std::cout << "error in opening file" << std::endl;
    }
    txt_file.close();
}

//setting coords of vec
glm::vec4 set_coords(std::vector<std::string> &coords) {
    glm::vec4 temp;
    //stof - string to float
    temp.x = std::stof(coords[0]);
    temp.y = std::stof(coords[1]);
    temp.z = std::stof(coords[2]);
    temp.w = std::stof(coords[3]);
    return temp;
}

//for checking vec coordinates
void print_vec4(std::string type, glm::vec4 &vec) {
    std::cout << type << " " << vec.x << " " << vec.y << " " << vec.z << " " << vec.w << std::endl;
}

void print_vec3(std::string type, glm::vec3 &vec) {
    std::cout << type << " " << static_cast<int>(255.999 * vec.x) << " " << static_cast<int>(255.999 * vec.y) << " "
              << static_cast<int>(255.999 * vec.z) << std::endl;
}

// for string delimiter
std::vector<std::string> split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}

//reading specified file
void read_file() {
    std::ifstream in("../res/scene4.txt", std::ios::in);

    if (!in) {
        printf("\ncan't open input file\n");
        exit(1);
    }
    std::string line;
    std::vector<std::string> temp_coords;


    while (std::getline(in, line)) { //while didn't reach EOF
        // EYE VEC
        if (line.substr(0, 2) == "e ") {
            std::string e(line.substr(2)); //take eye coordinates
            temp_coords = split(e, " ");
            glm::vec4 eye;
            eye = set_coords(temp_coords);
            eye_camera.push_back(eye);
            print_vec4("eye", eye); //vec coordinates check
        }
            // AMBIENT VEC
        else if (line.substr(0, 2) == "a ") {
            std::string a(line.substr(2)); //take ambient coordinates
            temp_coords = split(a, " ");
            glm::vec4 ambient;
            ambient = set_coords(temp_coords);
            ambient_color.push_back(ambient);
            print_vec4("ambient", ambient); //vec coordinates check
        }
            // OBJECT VEC
        else if (line.substr(0, 2) == "o ") {
            std::string a(line.substr(2)); //take ambient coordinates
            temp_coords = split(a, " ");
            glm::vec4 object;
            object = set_coords(temp_coords);
            objects.push_back(std::make_tuple(object, 'o'));
            print_vec4("object", object); //vec coordinates check
        }
            // COLOR OF OBJECT VEC
        else if (line.substr(0, 2) == "c ") {
            std::string a(line.substr(2)); //take ambient coordinates
            temp_coords = split(a, " ");
            glm::vec4 color;
            color = set_coords(temp_coords);
            object_colors.push_back(color);
            print_vec4("color", color); //vec coordinates check
        }
            // DIRECT LIGHT VEC
        else if (line.substr(0, 2) == "d ") {
            std::string a(line.substr(2)); //take ambient coordinates
            temp_coords = split(a, " ");
            glm::vec4 direct;
            direct = set_coords(temp_coords);
            direct_lights.push_back(direct);
            print_vec4("direct", direct); //vec coordinates check
        }
            // SPOTLIGHT VEC
        else if (line.substr(0, 2) == "p ") {
            std::string a(line.substr(2)); //take ambient coordinates
            temp_coords = split(a, " ");
            glm::vec4 spotlight;
            spotlight = set_coords(temp_coords);
            spotlights.push_back(spotlight);
            print_vec4("spotlight", spotlight); //vec coordinates check
        }
            // LIGHT INTENSITY VEC
        else if (line.substr(0, 2) == "i ") {
            std::string a(line.substr(2)); //take ambient coordinates
            temp_coords = split(a, " ");
            glm::vec4 intensity;
            intensity = set_coords(temp_coords);
            light_intensity.push_back(intensity);
            print_vec4("intensity", intensity); //vec coordinates check
        }
            // REFLECTIVE OBJECT VEC
        else if (line.substr(0, 2) == "r ") {
            std::string a(line.substr(2)); //take ambient coordinates
            temp_coords = split(a, " ");
            glm::vec4 reflective;
            reflective = set_coords(temp_coords);
            objects.push_back(std::make_tuple(reflective, 'r'));
            print_vec4("reflective", reflective); //vec coordinates check
        }
            // TRANSPARENT OBJECT VEC
        else if (line.substr(0, 2) == "t ") {
            std::string a(line.substr(2)); //take ambient coordinates
            temp_coords = split(a, " ");
            glm::vec4 transparent;
            transparent = set_coords(temp_coords);
            objects.push_back(std::make_tuple(transparent, 't'));
            print_vec4("transparent", transparent); //vec coordinates check
        }

    }
    in.close();
}


/*  ************************************************** end parser **************************************************** */

inline float degrees_to_radians(float degrees) {
    return degrees * pi / 180.0;
}

ray calculate_reflected_ray(ray in_ray, glm::vec3 normal, glm::vec3 point){
    normal = glm::normalize(normal);
    glm::vec3 dir = glm::normalize(in_ray.dir) - 2.0f * normal * (glm::dot(glm::normalize(in_ray.dir), normal));
    return ray(point + 0.005f * dir, dir);
}

ray get_snell_ray(ray ray_in, float ni, float nr, hit_record hr){
    glm::vec3 L = glm::normalize(ray_in.dir) * (-1.0f);
    float theta_i = glm::dot(L, glm::normalize(hr.normal));
    float theta_r = asin(glm::clamp((ni / nr) * sin(theta_i), -1.f, 1.f));
    glm::vec3 Tvec = ((ni / nr) * cos(theta_i) - cos(theta_r)) * glm::normalize(hr.normal) - (ni / nr) * L;
    Tvec = glm::normalize(Tvec);
    return ray(hr.point + Tvec, Tvec);
}

void max_vec(glm::vec3& v, float x){
    v.x = std::fmax(v.x, x);
    v.y = std::fmax(v.y, x);
    v.z = std::fmax(v.z, x);
}

glm::vec3 ray_color(const ray& r, const hittable& world, light_list& light_sources, int depth) {
    if (depth == 0)
        return glm::vec3(0.0f,0.0f,0.0f);
    hit_record rec;
    if (world.hit(r, 0.0001f, infinity, rec)) {
        float n1;
        float n2;
        if (glm::dot(r.dir, rec.normal) > 0.0f){ // ray is coming from inside (important for snell's law aka transparency)
            n1 = material_constant;
            n2 = air_constant;
        } else { // ray is coming from outside
            n1 = air_constant;
            n2 = material_constant;
        }
        glm::vec3 output = (light_sources.get_illumination(r, rec, const_cast<hittable &>(world))) * rec.mat.base_color + // light from light sources
               ray_color(calculate_reflected_ray(r, rec.normal, rec.point), world, light_sources, depth - 1) * rec.mat.reflective + // light from refractions
                ray_color(get_snell_ray(r, n1, n2, rec), world, light_sources, depth - 1) * rec.mat.transperancy; // light from transparency
        max_vec(output, 0.0f);
        return output;
    }
    return glm::vec3(0.0f,0.0f,0.0f); //infinity plane
}

float get_random(){
    return (float)rand() /(RAND_MAX + 1);
//    return 0;
}

float color_clamp(float x){
    return  glm::clamp(x, 0.0f, 255.0f);
}

const int sample_per_pixel = 25;
void Game::calc_color_data(light_list& lights, hittable_list& world, float viewport_width, float viewport_height, int image_width, int image_height, int threads_per_row) {
    auto*** color_mat = new float**[image_width]();
    for(int i = 0; i < image_width; i ++){
        color_mat[i] = new float*[image_height]();
        for(int j = 0; j < image_height; j ++)
            color_mat[i][j] = new float[3]();
    }
    std::cout << "light sources amount: " << lights.light_sources.size() << '\n';
    std::cout << "objs amount: " << world.objects.size() << '\n';
    glm::vec3 eye = glm::vec3(eye_camera[0].x, eye_camera[0].y, eye_camera[0].z); //origin
    glm::vec3 horizontal = glm::vec3(viewport_width, 0, 0); // X axis
    glm::vec3 vertical = glm::vec3(0, viewport_height, 0); // Y axis
    glm::vec3 focal_length = glm::vec3(0, 0, eye_camera[0].z); // distance from camera to screen
    glm::vec3 lower_left_corner =
            eye - horizontal / 2.0f - vertical / 2.0f - focal_length;

    auto foo =[&world, &lights, &color_mat, &lower_left_corner, &eye](int image_width, int image_height, int y0, int x0, int threads_per_row) {
        for (int y = y0; y < image_height; y++) {
            for (int x = x0; x < image_width; x++) {
                glm::vec3 pixel_color = glm::vec3(0.0f,0.0f,0.0f);
                for (int s = 0; s < sample_per_pixel; s++) {
                    float u = (float(x) + get_random()) / (float) (threads_per_row*(image_width - x0) - 1);
                    float v = (float(y)+ get_random()) / (float) (threads_per_row*(image_height - y0) - 1);
                    glm::vec3 xDir = glm::vec3(2 * u, 0, 0);
                    glm::vec3 yDir = glm::vec3(0, 2 * v, 0);
                    ray r(eye, lower_left_corner + xDir + yDir - eye);
                    pixel_color += ray_color(r, world, lights, MAX_DEPTH);
                }
                pixel_color = pixel_color / (float)sample_per_pixel;
                color_mat[y][x][0] = color_clamp(pixel_color.x);
                color_mat[y][x][1] = color_clamp(pixel_color.y);
                color_mat[y][x][2] = color_clamp(pixel_color.z);
            }
        }
    };
    std::thread threads[threads_per_row * threads_per_row];
    for (int i = 0; i < threads_per_row; i ++)
        for (int j = 0; j < threads_per_row; j ++)
            threads[threads_per_row * i + j] = std::thread(foo, (1+ i) *(image_width / threads_per_row), (1 + j) * (image_height / threads_per_row), j* (image_height / threads_per_row), i* (image_height / threads_per_row), threads_per_row);
    for (int i = 0; i < threads_per_row * threads_per_row; i ++)
        threads[i].join();
    add_color_mat_texture(image_width, image_height, color_mat);
}

void Game::add_color_mat_texture(int image_width, int image_height, float **const *color_mat) {
    unsigned char *data = new unsigned char[image_width * image_height * color_size_bytes];
    int index = 0;
    for (int i = 0; i < image_height; i++) {
        for (int j = 0; j < image_width; j++) {
            data[index] = static_cast<int>(color_mat[image_height - i - 1][j][0]);
            data[index + 1] = static_cast<int>(color_mat[image_height - i - 1][j][1]);
            data[index + 2] = static_cast<int>(color_mat[image_height - i - 1][j][2]);
            data[index + 3] = static_cast<int>(255);
            index += 4;
        }
    }
    AddTexture(image_width, image_height, data);
//    WriteToTxt(data, image_width, image_height, "bla.txt");
}


static void printMat(const glm::mat4 mat) {
    std::cout << " matrix:" << std::endl;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++)
            std::cout << mat[j][i] << " ";
        std::cout << std::endl;
    }
}

Game::Game() : Scene() {
}

Game::Game(float angle, float relationWH, float near1, float far1) : Scene(angle, relationWH, near1, far1) {
}

glm::vec3 vec4_to_vec3(glm::vec4 v){
    return glm::vec3(v.x, v.y, v.z);
}

void vec4_to_obj(glm::vec4 v, hittable_list *world, float transparency, float reflective, glm::vec3 color, float shiny){
    if (v.w > 0.0f) { // sphere
        world->add(make_shared<sphere>(vec4_to_vec3(v), v.w, material(color, reflective, transparency, shiny)));
    } else { // plane
        world->add(make_shared<plane>(vec4_to_vec3(v), v.w, material(color, reflective, transparency, shiny)));
    }
}

hittable_list* file_to_world(){
    auto *world = new hittable_list();
    int color_index = 0;
    for (auto tuple : objects){
        switch(std::get<1>(tuple)) {
            case 'o':
                vec4_to_obj(std::get<0>(tuple), world, 0.0f, 0.0f, vec4_to_vec3(object_colors[color_index]), object_colors[color_index].w);
                break;
            case 'r':
                vec4_to_obj(std::get<0>(tuple), world, 0.0f, 1.0f, glm::vec3(0.f,0.f,0.f), object_colors[color_index].w);
                break;
            case 't':
                vec4_to_obj(std::get<0>(tuple), world, 1.0f, 0.0f, glm::vec3(0.1f,0.1f,0.1f), object_colors[color_index].w);
                break;
        }
        color_index ++;
    }
    return world;
}

light_list* file_to_lights(){
    glm::vec4 AmbientLight = ambient_color[0];
    auto *lights = new light_list(vec4_to_vec3(AmbientLight));
    int spotLight_index = 0;
    int intensity_index = 0;
    for (glm::vec4 light : direct_lights) {
        if (light.w == 1.0){ // spotlight
            lights->add(make_shared<spotlight>(vec4_to_vec3(spotlights[spotLight_index]), vec4_to_vec3(light), std::acos(spotlights[spotLight_index].w),
                        vec4_to_vec3(light_intensity[intensity_index])));
            spotLight_index ++;
        } else { //directional light
            lights->add(make_shared<directional_light>(vec4_to_vec3(light), vec4_to_vec3(light_intensity[intensity_index])));
        }
        intensity_index ++;
    }
    return lights;
}

void Game::Init() {
    read_file();
//    std::cout << object.size() << std::endl; //checking count of object = V
    AddShader("../res/shaders/pickingShader");
    AddShader("../res/shaders/basicShader");
    calc_color_data(*file_to_lights(), *file_to_world(), 2.0, 2.0, 256, 256, THREADS_PER_ROW);

    AddShape(Plane, -1, TRIANGLES);

    pickedShape = 0;

    SetShapeTex(0, 0);
    MoveCamera(0, zTranslate, 10);
    pickedShape = -1;

    //ReadPixel(); //uncomment when you are reading from the z-buffer
}

void Game::Update(const glm::mat4 &MVP, const glm::mat4 &Model, const int shaderIndx) {
    Shader *s = shaders[shaderIndx];
    int r = ((pickedShape + 1) & 0x000000FF) >> 0;
    int g = ((pickedShape + 1) & 0x0000FF00) >> 8;
    int b = ((pickedShape + 1) & 0x00FF0000) >> 16;
    s->Bind();
    s->SetUniformMat4f("MVP", MVP);
    s->SetUniformMat4f("Normal", Model);
    s->SetUniform4f("lightDirection", 0.0f, 0.0f, -1.0f, 0.0f);
    if (shaderIndx == 0)
        s->SetUniform4f("lightColor", r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
    else
        s->SetUniform4f("lightColor", 0.7f, 0.8f, 0.1f, 1.0f);
    s->Unbind();
}

void Game::WhenRotate() {
}

void Game::WhenTranslate() {
}

void Game::Motion() {
    if (isActive) {
    }
}

Game::~Game(void) {
}


