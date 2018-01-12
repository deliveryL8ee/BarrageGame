#include <random>
#include "MyBullet.hpp"

MyBullet::MyBullet(): MyBullet(0.0f, 0.0f, 0.0f, 0.0f, 0) {
	//乱数によって弾幕を様々な色に設定する
	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_real_distribution<float> rand1(0,1);
	std::uniform_real_distribution<float> rand2(1,2);

	circle_color_buffer_data.push_back(1.0f);
	circle_color_buffer_data.push_back(rand1(mt));
	circle_color_buffer_data.push_back(1.0f);
	circle_color_buffer_data.push_back(rand2(mt)/2);
}

MyBullet::MyBullet(float posX, float posY, float speedX, float speedY, int id): posX(posX), posY(posY), speedX(speedX), speedY(speedY), id(id){
}

MyBullet::MyBullet(const MyBullet& rhs): MyBullet(rhs.posX, rhs.posY, rhs.speedX, rhs.speedY, rhs.id){
}


void MyBullet::setParameter(float P_posX, float P_posY, float P_speedX, float P_speedY, int P_id) {
	posX = P_posX;
	posY = P_posY;
	speedX = P_speedX;
	speedY = P_speedY;
	id = P_id;
}

//受け取ったIDに該当する弾幕に新しく生成した弾幕情報を上書きし、削除する
void MyBullet::deleteBullet(int id) {
	delete BulletList[id];
	BulletList[id] = BulletList[BulletList.size()-1];
	BulletList[id]->setID(id);
	BulletList.pop_back();

	ModelMatrixVector[id] = ModelMatrixVector[ModelMatrixVector.size()-1];
	ModelMatrixVector.pop_back();
}

void MyBullet::setID(int id) {
	this->id = id;
}

void MyBullet::getPosition(float *x, float *y) {
	*x = posX;
	*y = posY;
}

/*
 * アクティブな弾幕を進め、そうでなければ削除する
 * かつ弾幕のモデル行列を更新する
 */
void MyBullet::tick(){
	posX += speedX;
	posY += speedY;

	if(posY-10.0f > height+10.0f) {
		deleteBullet(id);			//使い終わった弾幕を削除する
	}
		

	c_Model = glm::translate(glm::mat4(), glm::vec3(posX,posY-10.0f,1.0f)) * glm::scale(glm::mat4(1.0f),glm::vec3(10.0f));
	ModelMatrixVector[id] = c_Model;

}

std::vector<MyBullet*> BulletList;
std::vector<GLfloat> circle_vertex_buffer_data;
std::vector<GLfloat> circle_color_buffer_data;
std::vector<glm::mat4> ModelMatrixVector;

