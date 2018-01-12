#include <random>
#include "EnemyBullet.hpp"

EnemyBullet::EnemyBullet(): EnemyBullet(0.0f, 0.0f, 0.0f, 0.0f, 0) {
	e_circle_color_buffer_data.push_back(1.0f);
	e_circle_color_buffer_data.push_back(1.0f);
	e_circle_color_buffer_data.push_back(1.0f);
	e_circle_color_buffer_data.push_back(0.8f);
}

EnemyBullet::EnemyBullet(float posX, float posY, float speedX, float speedY, int id): posX(posX), posY(posY), speedX(speedX), speedY(speedY), id(id){
}

EnemyBullet::EnemyBullet(const EnemyBullet& rhs): EnemyBullet(rhs.posX, rhs.posY, rhs.speedX, rhs.speedY, rhs.id){
}

void EnemyBullet::setParameter(float E_posX, float E_posY, float E_speedX, float E_speedY, int E_id) {
	posX = E_posX;
	posY = E_posY;
	speedX = E_speedX;
	speedY = E_speedY;
	id = E_id;
}

//受け取ったIDに該当する弾幕に新しく生成した弾幕情報を上書きし、削除する
void EnemyBullet::deleteBullet(int id) {
	delete e_BulletList[id];
	e_BulletList[id] = e_BulletList[e_BulletList.size()-1];
	e_BulletList[id]->setID(id);
	e_BulletList.pop_back();

	e_ModelMatrixVector[id] = e_ModelMatrixVector[e_ModelMatrixVector.size()-1];
	e_ModelMatrixVector.pop_back();
}

void EnemyBullet::setID(int id) {
	this->id = id;
}

void EnemyBullet::getPosition(float *x, float *y) {
	*x = posX;
	*y = posY;
}

/*
 * アクティブな弾幕を進め、そうでなければ削除する
 * かつ弾幕のモデル行列を更新する
 */
void EnemyBullet::tick(){
	posX += speedX;
	posY += speedY;

	std::cout << "(" << posX << ", " << posY << ")" << std::endl;

	if(posX+10.0f < 0.0f || posX-10.0f > width+10.0f || posY+10.0f < 0.0 || posY-10.0f > height+10.0f) {
		deleteBullet(id);			//使い終わった弾幕を削除する
	}
		

	c_Model = glm::translate(glm::mat4(), glm::vec3(posX,posY-10.0f,1.0f)) * glm::scale(glm::mat4(1.0f),glm::vec3(10.0f));
	e_ModelMatrixVector[id] = c_Model;

}

std::vector<EnemyBullet*> e_BulletList;
std::vector<GLfloat> e_circle_vertex_buffer_data;
std::vector<GLfloat> e_circle_color_buffer_data;
std::vector<glm::mat4> e_ModelMatrixVector;

