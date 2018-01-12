
#include "Enemy.hpp"

Enemy::Enemy(): Enemy(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0, 1) {
}

Enemy::Enemy(float posX, float posY, float speedX, float speedY, float t, float life, int id, int frag): posX(posX), posY(posY), speedX(speedX), speedY(speedY), t(t), life(life), id(id), frag(frag){
	createVertex();
}

Enemy::Enemy(const Enemy& rhs): Enemy(rhs.posX, rhs.posY, rhs.speedX, rhs.speedY, rhs.t, rhs.life, rhs.id, rhs.frag){
}

void Enemy::createVertex(){
	/*エネミーを表す配列*/
	//頂点情報
	static const GLfloat enemy_vertex_buffer_data[] = { 
		-1.0f, 1.0f, 0.0f,
		-1.0f,-1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		-1.0f,-1.0f, 0.0f,
		1.0f,-1.0f, 0.0f,
	};

	//色情報
	static const GLfloat enemy_color_buffer_data[] = { 
		1.000f,  0.000f,  0.000f,
		0.609f,  0.000f,  0.000f,
		0.927f,  0.000f,  0.000f,
		0.422f,  0.000f,  0.000f,
		0.335f,  0.000f,  0.000f,
		0.210f,  0.000f,  0.000f,
	};

	glGenBuffers(1, &e_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, e_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(enemy_vertex_buffer_data), enemy_vertex_buffer_data, GL_STATIC_DRAW);

	glGenBuffers(1, &e_colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, e_colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(enemy_color_buffer_data), enemy_color_buffer_data, GL_STATIC_DRAW);

     e_Projection = glm::ortho(0.0f, width, 0.0f, height, 0.0f, 5.0f);
     e_View = glm::lookAt(glm::vec3(0,0,1),glm::vec3(0,0,0),glm::vec3(0,1,0));
	e_Model = glm::mat4(1.0f);
	e_MVP = e_Projection * e_View * e_Model;


}

void Enemy::deleteVertex() {
	glDeleteBuffers(1, &e_vertexbuffer);
	glDeleteBuffers(1, &e_colorbuffer);
}

void Enemy::setParameter(float E_posX, float E_posY, float E_speedX, float E_speedY, float E_t, float E_life, int E_id, int E_frag) {
	posX = E_posX;
	posY = E_posY;
	speedX = E_speedX;
	speedY = E_speedY;
	t = E_t;
	life = E_life;
	id = E_id;
	frag = E_frag;
}

//受け取ったIDに該当する敵に新しく生成した敵情報を上書きし、削除する
void Enemy::deleteEnemy(int id) {
	delete EnemyGroup[id];
	EnemyGroup[id] = EnemyGroup[EnemyGroup.size()-1];
	EnemyGroup[id]->setID(id);
	EnemyGroup.pop_back();
}

void Enemy::setID(int id) {
	this->id = id;
}

void Enemy::getPosition(float *x, float *y) {
	*x = posX;
	*y = posY;
}

void Enemy::damege(){
	life -= 0.005f;
}

/*
 * アクティブな敵を動かし、そうでなければ削除する
 * フラグを引数として受け取り、それによって座標計算を変える
 */
void Enemy::tick(){
	//fragment 1 is "straight line"
	if(frag == 1){
		if(posY > height-scaleVec) speedY *= -1;
		if(posY < height/2.0f+scaleVec) speedY *= -1;
		if(posX > width-scaleVec) speedX *= -1;
		if(posX < 0.0f+scaleVec) speedX *= -1;
		posX += speedX;
		posY += speedY;
	}

	//fragment 2 is "circle"
	if(frag == 2){
		posX = 100 * cos(t * M_PI) + width/2.0f;
		posY = 50 * sin(t * M_PI) + 3*height/4.0f;
		t += 0.01;
	}

	//fragment 3 is "Asteroid"
	if(frag == 3){
		posX = 100 * pow(cos(t*M_PI), 3) + width/2.0f;
		posY = 100 * pow(sin(t*M_PI), 3) + 3*height/4.0f;
		t += 0.01;
	}

	//fragment 4 is "cardioid"
	if(frag == 4){
		posX = 100 * (1+cos(t*M_PI)) * cos(t*M_PI) + 3*width/8.0f;
		posY = 100 * (1+cos(t*M_PI)) * sin(t*M_PI) + 3*height/4.0f;
		t += 0.01;
	}

	//fragment 5 is "Lissajous"
	if(frag == 5){
		posX = 100 * sin(3*t*M_PI) + width/2.0f;
		posY = 100 * sin(4*t*M_PI) + 3*height/4.0f;
		t += 0.005;
	}


	//life of Enemy was lost
	if(life <= DEAD) {
		deleteEnemy(id);			//倒した敵を削除する
	}
		
}

void Enemy::draw(GLuint e_programID, GLuint e_MatrixID) {
     e_Model = glm::translate(glm::mat4(),glm::vec3(posX,posY,1.0f)) * glm::scale(glm::mat4(1.0f),glm::vec3(10.0f));
	e_MVP = e_Projection * e_View * e_Model;

	glEnableVertexAttribArray(8);
	glEnableVertexAttribArray(9);

	glUseProgram(e_programID);
	glUniformMatrix4fv(e_MatrixID, 1, GL_FALSE, &e_MVP[0][0]);

	//vertexbuffers
	glBindBuffer(GL_ARRAY_BUFFER,e_vertexbuffer);
	glVertexAttribPointer(8,	3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//colorbuffers
	glBindBuffer(GL_ARRAY_BUFFER,e_colorbuffer);
	glVertexAttribPointer(9,	3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDrawArrays(GL_TRIANGLES, 0, 3*2);


	glDisableVertexAttribArray(8);
	glDisableVertexAttribArray(9);

}

std::vector<Enemy*> EnemyGroup;

