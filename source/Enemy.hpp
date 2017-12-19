#ifndef ENEMY_HPP
#define ENEMY_HPP

#define DEAD 0.0

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

class Enemy {
     float width = 768.0f;
     float height = 768.0f;

     float scaleVec = 10.0f;

     private:
     float posX;
     float posY;
     float speedX;
     float speedY;
     float t;
	float life;
	int id;
	int frag;

	GLuint e_vertexbuffer;
	GLuint e_colorbuffer;

	glm::mat4 e_Projection;
	glm::mat4 e_View;
	glm::mat4 e_Model;
	glm::mat4 e_MVP;

     public:
     Enemy();
     Enemy(float posX, float posY, float speedX, float speedY, float t, float life, int id, int frag);
     Enemy(const Enemy& rhs);

     Enemy& operator=(const Enemy& rhs){
	  this->posX   = rhs.posX;
	  this->posY   = rhs.posY;
	  this->speedX = rhs.speedX;
	  this->speedY = rhs.speedY;
	  this->t = rhs.t;
	  this->life = rhs.life;
	  this->id   = rhs.id;
	  this->frag = rhs.frag;
     }

	void createVertex();
	void deleteVertex();
     void setParameter(float E_posX, float E_posY, float E_speedX, float E_speedY, float E_t, float E_life, int E_id, int E_frag);
	void deleteEnemy(int id);
	void setID(int id);
     void tick();
     void isCollide();
     void draw(GLuint e_programID, GLuint e_MatrixID);

};

extern std::vector<Enemy*> EnemyGroup;


#endif
