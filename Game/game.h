#pragma once
#include "scene.h"
#include "light_list.h"

class Game : public Scene
{
public:
	
	Game();
	Game(float angle,float relationWH,float near, float far);
	void Init();
	void Update(const glm::mat4 &MVP,const glm::mat4 &Model,const int  shaderIndx);
	void ControlPointUpdate();
	void WhenRotate();
	void WhenTranslate();
	void Motion();
	~Game(void);

    void calc_color_data(light_list &lights, hittable_list &world, float viewport_width, float viewport_heigt, int image_width, int image_height, int threads_per_row);

    void add_color_mat_texture(int image_width, int image_height, float **const *color_mat);
};

