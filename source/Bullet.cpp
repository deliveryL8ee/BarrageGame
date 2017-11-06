#include "Bullet.hpp"

Bullet::Bullet(): Bullet(0.0f, 0.0f, 0.0f, 0.0f, 0) {
}

Bullet::Bullet(float posX, float posY, float speedX, float speedY, int id): posX(posX), posY(posY), speedX(speedX), speedY(speedY), id(id){
}

Bullet::Bullet(const Bullet& rhs): Bullet(rhs.posX, rhs.posY, rhs.speedX, rhs.speedY, rhs.id){
}


void Bullet::setParameter(float P_posX, float P_posY, float P_speedX, float P_speedY, int P_id) {
	posX = P_posX;
	posY = P_posY;
	speedX = P_speedX;
	speedY = P_speedY;
	id = P_id;

}

//受け取ったIDに該当する弾幕に新しく生成した弾幕情報を上書きし、削除する
void Bullet::deleteBullet(int id) {
	delete BulletList[id];
	BulletList[id] = BulletList[BulletList.size()-1];
	BulletList[id]->setID(id);
	BulletList.pop_back();

	ModelMatrixVector[id] = ModelMatrixVector[ModelMatrixVector.size()-1];
	ModelMatrixVector.pop_back();
}

void Bullet::setID(int id) {
	this->id = id;
}

/*
 * アクティブな弾幕を進め、そうでなければ削除する
 * かつ弾幕のモデル行列を更新する
 */
void Bullet::tick(){
	posX += speedX;
	posY += speedY;

	if(posY-10.0f > height+10.0f) {
		deleteBullet(id);			//使い終わった弾幕を削除する
	}
		

	c_Model = glm::translate(glm::mat4(), glm::vec3(posX,posY-10.0f,1.0f)) * glm::scale(glm::mat4(1.0f),glm::vec3(10.0f));
	ModelMatrixVector[id] = c_Model;

}

std::vector<Bullet*> BulletList;
std::vector<GLfloat> circle_vertex_buffer_data;
std::vector<GLfloat> circle_color_buffer_data;
std::vector<glm::mat4> ModelMatrixVector;

