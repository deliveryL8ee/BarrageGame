// Include GLFW
#include <GLFW/glfw3.h>
#include <iostream>
extern GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Bullet {
     private:
     float posX;
     float posY;
     float speedX;
     float speedY;

     public:
     Bullet(): Bullet(0.0f, 0.0f, 0.0f, 0.0f) {
     }

     Bullet(float posX, float posY, float speedX, float speedY): posX(posX), posY(posY), speedX(speedX), speedY(speedY){
     }

     Bullet(const Bullet& rhs): Bullet(rhs.posX, rhs.posY, rhs.speedX, rhs.speedY){
     }

     Bullet& operator=(const Bullet& rhs){
	  this->posX   = rhs.posX;
	  this->posY   = rhs.posY;
	  this->speedX = rhs.speedX;
	  this->speedY = rhs.speedY;
     }


     //1フレーム毎に動かす
     void tick(){
	  posX += speedX;
	  posX += speedY;
     }

};
