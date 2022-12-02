#include "game.h"
#include "ray.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <sstream>

#define color_size_bytes 4
#define to_index(i, j) i * image_width * color_size_bytes + j * color_size_bytes
#define set_val(ARR, INDEX, vec) ARR[INDEX] = vec.x; ARR[INDEX+1] = vec.y; ARR[INDEX+2] = vec.z; ARR[INDEX+3] = 255;
static std::vector<glm::vec4> spheres;
static std::vector<glm::vec4> planes;
static std::vector<glm::vec4> ambient_color;
static std::vector<glm::vec4> object_colors;
static std::vector<glm::vec4> light_intensity;
static std::vector<glm::vec4> direct_lights;
static std::vector<glm::vec4> spotlights;
static std::vector<glm::vec4> eye_camera;

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
    std::ifstream in("../res/scene.txt", std::ios::in);

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
            if (object.w <= 0) //if it's a plane
                planes.push_back(object);
            else // it's a sphere
                spheres.push_back(object);
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
            if (reflective.w <= 0) //if it's a plane
                planes.push_back(reflective);
            else // it's a sphere
                spheres.push_back(reflective);
            print_vec4("reflective", reflective); //vec coordinates check
        }
            // TRANSPARENT OBJECT VEC
        else if (line.substr(0, 2) == "t ") {
            std::string a(line.substr(2)); //take ambient coordinates
            temp_coords = split(a, " ");
            glm::vec4 transparent;
            transparent = set_coords(temp_coords);
            if (transparent.w <= 0) //if it's a plane
                planes.push_back(transparent);
            else // it's a sphere
                spheres.push_back(transparent);
            print_vec4("transparent", transparent); //vec coordinates check
        }

    }
    in.close();
}


/*  ************************************************** end parser **************************************************** */


glm::vec3 ray_color(const ray &r) {
    glm::vec3 unit_direction = glm::normalize(r.direction());
    float t = 0.5f * (unit_direction.y + 1.0f);
    float diffuse = 1.0f;
    float specular = 0.7f;
    return (1.0f - t) * glm::vec3(1.0, 1.0, 1.0) +
    t * glm::vec3(0.5, 0.7, 1.0);


}

void Game::calc_color_data(float viewport_width, float viewport_height, int image_width, int image_height) {
    float color_mat[image_width][image_height][3];
    glm::vec3 eye = glm::vec3(eye_camera[0].x, eye_camera[0].y, eye_camera[0].z); //origin
    glm::vec3 horizontal = glm::vec3(viewport_width, 0, 0); // X axis
    glm::vec3 vertical = glm::vec3(0, viewport_height, 0); // Y axis
    glm::vec3 focal_length = glm::vec3 (0,0,1.0f); // distance from camera to screen
    glm::vec3 lower_left_corner =
            eye - horizontal / 2.0f - vertical / 2.0f - focal_length;
    unsigned char *data = new unsigned char[image_width * image_height * color_size_bytes];
    for (int i = image_height -1 ; i >=0 ; i--) {
        for (int j = 0; j < image_width; j++) {
            float u = float(j) / (float) (image_width - 1);
            float v = float(i) / (float) (image_height - 1);
            ray r(eye, lower_left_corner + u * horizontal + v * vertical - eye);
            glm::vec3 pixel_color = ray_color(r);
//            print_vec3("color", pixel_color);
            color_mat[i][j][0] = pixel_color.x;
            color_mat[i][j][1] = pixel_color.y;
            color_mat[i][j][2] = pixel_color.z;
        }
    }
    int index = 0;
    for (int i = 0; i < image_height; i++) {
        for (int j = 0; j < image_width; j++) {
            data[index] = static_cast<int>(color_mat[i][j][0]*255.999);
            data[index+1] = static_cast<int>(color_mat[i][j][1]*255.999);
            data[index+2] = static_cast<int>(color_mat[i][j][2]*255.999);
            data[index+3] = static_cast<int>(255);
            index+=4;
        }

    }

//    WriteToTxt(data,image_width,image_height,"data");
    AddTexture(image_width, image_height, data);
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

void Game::Init() {
    read_file();
//    std::cout << spheres.size() << std::endl; //checking count of spheres = V
    AddShader("../res/shaders/pickingShader");
    AddShader("../res/shaders/basicShader");
    calc_color_data(2.0, 2.0 ,256,256 );

//    AddTexture("../res/textures/box0.bmp", false);

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


