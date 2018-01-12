#ifndef MYBULLET_HPP
#define MYBULLET_HPP

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

class MyBullet {
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
     MyBullet();
     MyBullet(float posX, float posY, float speedX, float speedY, int id);
     MyBullet(const MyBullet& rhs);

     MyBullet& operator=(const MyBullet& rhs){
	  this->posX   = rhs.posX;
	  this->posY   = rhs.posY;
	  this->speedX = rhs.speedX;
	  this->speedY = rhs.speedY;
	  this->id   = rhs.id;
	  
	  return (*this);
     }

     void setParameter(float P_posX, float P_posY, float P_speedX, float P_speedY, int P_id);
	void deleteBullet(int id);
	void setID(int id);
	void getPosition(float *x, float *y);
     void tick();

};

extern std::vector<MyBullet*> BulletList;
extern std::vector<GLfloat> circle_vertex_buffer_data;		//頂点情報
extern std::vector<GLfloat> circle_color_buffer_data;		//色情報
extern std::vector<glm::mat4> ModelMatrixVector;


#endif
