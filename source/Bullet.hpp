#ifndef BULLET_HPP
#define BULLET_HPP

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

class Bullet {
     float width = 1024.0f;
     float height = 768.0f;

     private:
     float posX;
     float posY;
     float speedX;
     float speedY;
	int id;

	glm::mat4 c_Model;

     public:
     Bullet();
     Bullet(float posX, float posY, float speedX, float speedY, int id);
     Bullet(const Bullet& rhs);

     Bullet& operator=(const Bullet& rhs){
	  this->posX   = rhs.posX;
	  this->posY   = rhs.posY;
	  this->speedX = rhs.speedX;
	  this->speedY = rhs.speedY;
	  this->id   = rhs.id;
     }

     void setParameter(float P_posX, float P_posY, float P_speedX, float P_speedY, int P_id);
	void deleteBullet(int id);
	void setID(int id);
     void tick();
     void draw(GLuint MatrixID);

};

extern std::vector<Bullet*> BulletList;
extern std::vector<GLfloat> circle_vertex_buffer_data;		//頂点情報
extern std::vector<GLfloat> circle_color_buffer_data;		//色情報
extern std::vector<glm::mat4> ModelMatrixVector;


#endif
