#ifndef BULLET_HPP
#define BULLET_HPP


// Include GLFW
#include <GLFW/glfw3.h>
#include <iostream>
extern GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "controls.hpp"

class Bullet {
     float width = 1024.0f;
     float height = 768.0f;

     private:
     float posX;
     float posY;
     float speedX;
     float speedY;

     bool isActive;

     public:
     Bullet();
     Bullet(float posX, float posY, float speedX, float speedY);
     Bullet(const Bullet& rhs);

     Bullet& operator=(const Bullet& rhs){
	  this->posX   = rhs.posX;
	  this->posY   = rhs.posY;
	  this->speedX = rhs.speedX;
	  this->speedY = rhs.speedY;
     }

     void setParameter(float P_posX, float P_posY, float P_speedX, float P_speedY);
     void activate();
     //1フレーム毎に動かす
     void tick();

};

extern Bullet myBullet;


#endif
