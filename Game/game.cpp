#include "game.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#include <sstream>


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
void print_vec(std::string type, glm::vec4 &vec) {
    std::cout << type << " " << vec.x << " " << vec.y << " " << vec.z << " " << vec.w << std::endl;
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
            print_vec("eye", eye); //vec coordinates check
        }
            // AMBIENT VEC
        else if (line.substr(0, 2) == "a ") {
            std::string a(line.substr(2)); //take ambient coordinates
            temp_coords = split(a, " ");
            glm::vec4 ambient;
            ambient = set_coords(temp_coords);
            print_vec("ambient", ambient); //vec coordinates check
        }
            // OBJECT VEC
        else if (line.substr(0, 2) == "o ") {
            std::string a(line.substr(2)); //take ambient coordinates
            temp_coords = split(a, " ");
            glm::vec4 object;
            object = set_coords(temp_coords);
            print_vec("object", object); //vec coordinates check
        }
            // COLOR OF OBJECT VEC
        else if (line.substr(0, 2) == "c ") {
            std::string a(line.substr(2)); //take ambient coordinates
            temp_coords = split(a, " ");
            glm::vec4 color;
            color = set_coords(temp_coords);
            print_vec("color", color); //vec coordinates check
        }
            // DIRECT LIGHT VEC
        else if (line.substr(0, 2) == "d ") {
            std::string a(line.substr(2)); //take ambient coordinates
            temp_coords = split(a, " ");
            glm::vec4 direct;
            direct = set_coords(temp_coords);
            print_vec("direct", direct); //vec coordinates check
        }
            // SPOTLIGHT VEC
        else if (line.substr(0, 2) == "p ") {
            std::string a(line.substr(2)); //take ambient coordinates
            temp_coords = split(a, " ");
            glm::vec4 spotlight;
            spotlight = set_coords(temp_coords);
            print_vec("spotlight", spotlight); //vec coordinates check
        }
            // LIGHT INTENSITY VEC
        else if (line.substr(0, 2) == "i ") {
            std::string a(line.substr(2)); //take ambient coordinates
            temp_coords = split(a, " ");
            glm::vec4 intensity;
            intensity = set_coords(temp_coords);
            print_vec("intensity", intensity); //vec coordinates check
        }
            // REFLECTIVE OBJECT VEC
        else if (line.substr(0, 2) == "r ") {
            std::string a(line.substr(2)); //take ambient coordinates
            temp_coords = split(a, " ");
            glm::vec4 reflective;
            reflective = set_coords(temp_coords);
            print_vec("reflective", reflective); //vec coordinates check
        }
            // REFLECTIVE OBJECT VEC
        else if (line.substr(0, 2) == "r ") {
            std::string a(line.substr(2)); //take ambient coordinates
            temp_coords = split(a, " ");
            glm::vec4 reflective;
            reflective = set_coords(temp_coords);
            print_vec("reflective", reflective); //vec coordinates check
        }
            // TRANSPARENT OBJECT VEC
        else if (line.substr(0, 2) == "t ") {
            std::string a(line.substr(2)); //take ambient coordinates
            temp_coords = split(a, " ");
            glm::vec4 transparent;
            transparent = set_coords(temp_coords);
            print_vec("transparent", transparent); //vec coordinates check
        }

    }
    in.close();
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
    AddShader("../res/shaders/pickingShader");
    AddShader("../res/shaders/basicShader");

    AddTexture("../res/textures/box0.bmp", false);

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


