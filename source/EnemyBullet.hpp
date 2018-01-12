#ifndef ENEMYBULLET_HPP
#define ENEMYBULLET_HPP

#include <vector>
#include <cmath>

//Include GLEW
#include <GL/glew.h>


// Include GLFW
#include <GLFW/glfw3.h>
#include <iostream>


// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

extern GLFWwindow* window;


#include "shader.hpp"

#include "controls.hpp"

class EnemyBullet {
     float width = 768.0f;
     float height = 768.0f;

     private:
     float posX;
     float posY;
     float speedX;
     float speedY;
	int id;

	glm::mat4 c_Model;

     public:
     EnemyBullet();
     EnemyBullet(float posX, float posY, float speedX, float speedY, int id);
     EnemyBullet(const EnemyBullet& rhs);

     EnemyBullet& operator=(const EnemyBullet& rhs){
	  this->posX   = rhs.posX;
	  this->posY   = rhs.posY;
	  this->speedX = rhs.speedX;
	  this->speedY = rhs.speedY;
	  this->id   = rhs.id;
     }

     void setParameter(float E_posX, float E_posY, float E_speedX, float E_speedY, int E_id);
	void deleteBullet(int id);
	void setID(int id);
	void getPosition(float *x, float *y);
     void tick();

};

extern std::vector<EnemyBullet*> e_BulletList;
extern std::vector<GLfloat> e_circle_vertex_buffer_data;		//頂点情報
extern std::vector<GLfloat> e_circle_color_buffer_data;		//色情報
extern std::vector<glm::mat4> e_ModelMatrixVector;


#endif
