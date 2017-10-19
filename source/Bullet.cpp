#include "Bullet.hpp"

Bullet::Bullet(): Bullet(0.0f, 0.0f, 0.0f, 0.0f, 0) {
}

Bullet::Bullet(float posX, float posY, float speedX, float speedY, int id): posX(posX), posY(posY), speedX(speedX), speedY(speedY), id(id){
	createVertex();
}

Bullet::Bullet(const Bullet& rhs): Bullet(rhs.posX, rhs.posY, rhs.speedX, rhs.speedY, rhs.id){
}


void Bullet::createVertex(){
	const int n_gon = 16;

	circle_vertex_buffer_data.push_back(0.0f);
	circle_vertex_buffer_data.push_back(0.0f);
	circle_vertex_buffer_data.push_back(0.0f);
	circle_color_buffer_data.push_back(1.0f);
	circle_color_buffer_data.push_back(1.0f);
	circle_color_buffer_data.push_back(1.0f);

	for(int i = 0; i < n_gon+1; i++) {
		circle_vertex_buffer_data.push_back(cos(M_PI*2/n_gon*i));
		circle_vertex_buffer_data.push_back(sin(M_PI*2/n_gon*i));
		circle_vertex_buffer_data.push_back(0.0f);
		circle_color_buffer_data.push_back(1.0f);
		circle_color_buffer_data.push_back(1.0f);
		circle_color_buffer_data.push_back(1.0f);
	}

	glGenBuffers(1, &c_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, c_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, circle_vertex_buffer_data.size()*sizeof(GLfloat), &circle_vertex_buffer_data[0], GL_STATIC_DRAW);

	glGenBuffers(1, &c_colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, c_colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, circle_color_buffer_data.size()*sizeof(GLfloat), &circle_color_buffer_data[0], GL_STATIC_DRAW);


     BltProjection = glm::ortho(0.0f, width, 0.0f, height, 0.0f, 5.0f);
     BltView = glm::lookAt(glm::vec3(0,0,1),glm::vec3(0,0,0),glm::vec3(0,1,0));
	BltModel = glm::mat4(1.0f);
	BltMVP = BltProjection * BltView * BltModel;

}

void Bullet::deleteVertex() {
	glDeleteBuffers(1, &c_vertexbuffer);
	glDeleteBuffers(1, &c_colorbuffer);
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
}

void Bullet::setID(int id) {
	this->id = id;
}

//アクティブな弾幕を進め、そうでなければ削除する
void Bullet::tick(){
	posX += speedX;
	posY += speedY;

	if(posY-10.0f > height+10.0f) {
		deleteBullet(id);			//使い終わった弾幕を削除する
		
	}

}


//弾幕の頂点情報を渡して描画する
void Bullet::draw(GLuint MatrixID) {
	BltModel = glm::translate(glm::mat4(), glm::vec3(posX,posY-10.0f,1.0f)) * glm::scale(glm::mat4(1.0f),glm::vec3(10.0f));
	BltMVP = BltProjection * BltView * BltModel;

	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &BltMVP[0][0]);

	//declare buffers
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//vertexbuffers
	glBindBuffer(GL_ARRAY_BUFFER,c_vertexbuffer);
	glVertexAttribPointer(
			0,			//属性
			3,			//サイズ
			GL_FLOAT,		//型
			GL_FALSE,		//正規化
			0,			//ストライド
			(void*)0		//配列バッファオフセット
			);

	//colorbuffers
	glBindBuffer(GL_ARRAY_BUFFER,c_colorbuffer);
	glVertexAttribPointer(
			1,			//属性
			3,			//サイズ
			GL_FLOAT,		//型
			GL_FALSE,		//正規化
			0,			//ストライド
			(void*)0		//配列バッファオフセット
			);

	glDrawArrays(GL_TRIANGLE_FAN, 0, circle_vertex_buffer_data.size()*sizeof(GLfloat));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);


}

std::vector<Bullet*> BulletList;

