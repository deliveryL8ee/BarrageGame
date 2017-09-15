#include "Bullet.hpp"

Bullet::Bullet(): Bullet(0.0f, 0.0f, 0.0f, 0.0f) {
}

Bullet::Bullet(float posX, float posY, float speedX, float speedY): posX(posX), posY(posY), speedX(speedX), speedY(speedY){
}

Bullet::Bullet(const Bullet& rhs): Bullet(rhs.posX, rhs.posY, rhs.speedX, rhs.speedY){
}


void Bullet::setParameter(float P_posX, float P_posY, float P_speedX, float P_speedY) {
     posX = P_posX;
     posY = P_posY;
     speedX = P_speedX;
     speedY = P_speedY;
}

void Bullet::activate(){
     isActive = true;
}

void Bullet::tick(){
     std::cout << "(" << posX << "," << posY << ")" << std::endl;
     if (isActive == true) {
	  posX += speedX;
	  posY += speedY;
	  std::cout << "(" << posX << "," << posY << ")" << std::endl;
	  
	  if(posY > height) {
	       isActive = false;
	  }

     }
}

/*void Bullet::draw() {
}
*/

Bullet myBullet;
