#include <iostream>
#include <vector>
#include <cmath>

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

//Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

//Include Shader
#include "shader.hpp"

//Include Controller 
#include "controls.hpp"

//Include Bullet 
#include "MyBullet.hpp"

//Include Enemy
#include "Enemy.hpp"

//Include Enemy
#include "EnemyBullet.hpp"

int main() {
	float width = 768.0f;
	float height = 768.0f;
	

	if(!glfwInit()) {
		std::cout << "GLFWの初期化に失敗しました。" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Windowを開き、OpenGLコンテキストを作る
	window = glfwCreateWindow(width, height, "BarrageGame", NULL, NULL);
	if(window == NULL) {
		std::cout << "GLFWウィンドウのオープンに失敗しました。もしIntelのGPUならば、3.3に対応していません。チュートリアルのバージョン2.1を試してください。" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = true;

	if(glewInit() != GLEW_OK) {
		std::cout << "GLEWの初期化に失敗しました。" << std::endl;
		return -1;
	}
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glClearColor(0.0, 0.0, 0.0, 0.0);

	//デプステストを有効にする
	glEnable(GL_DEPTH_TEST);
	//前のものよりもカメラに近ければフラグメントを受け入れる
	glDepthFunc(GL_LESS);


	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");
	GLuint c_programID = LoadShaders("MyBulletVertexShader.vertexshader", "MyBulletFragmentShader.fragmentshader");
	GLuint e_programID = LoadShaders("EnemyVertexShader.vertexshader", "EnemyFragmentShader.fragmentshader");
	GLuint e_c_programID = LoadShaders("EnemyBulletVertexShader.vertexshader", "EnemyBulletFragmentShader.fragmentshader");


	//射影行列
	glm::mat4 Projection = glm::ortho(0.0f, width, 0.0f, height, 0.0f, 5.0f);

	//カメラ行列
	glm::mat4 View = glm::lookAt(
			glm::vec3(0,0,1),
			glm::vec3(0,0,0),
			glm::vec3(0,1,0)
			);

	//モデル行列
	glm::mat4 Model = glm::mat4(1.0f);
	glm::mat4 MVP = Projection * View * Model;


     glm::mat4 c_Projection = glm::ortho(0.0f, width, 0.0f, height, 0.0f, 5.0f);
     glm::mat4 c_View = glm::lookAt(glm::vec3(0,0,1),glm::vec3(0,0,0),glm::vec3(0,1,0));
     glm::mat4 c_VP = c_Projection * c_View;

	glm::mat4 e_MVP = Projection * View * Model;

     glm::mat4 e_c_Projection = glm::ortho(0.0f, width, 0.0f, height, 0.0f, 5.0f);
     glm::mat4 e_c_View = glm::lookAt(glm::vec3(0,0,1),glm::vec3(0,0,0),glm::vec3(0,1,0));
     glm::mat4 e_c_VP = e_c_Projection * e_c_View;


	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint c_MatrixID = glGetUniformLocation(c_programID, "c_VP");
	GLuint e_MatrixID = glGetUniformLocation(e_programID, "e_MVP");
	GLuint e_c_MatrixID = glGetUniformLocation(e_c_programID, "e_c_VP");


	/*プレイヤーを表す配列*/
	//頂点情報
	static const GLfloat player_vertex_buffer_data[] = { 
		-1.0f, 1.0f, 0.0f,
		-1.0f,-1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		-1.0f,-1.0f, 0.0f,
		1.0f,-1.0f, 0.0f,
	};

	//色情報
	static const GLfloat player_color_buffer_data[] = { 
		0.583f,  0.771f,  0.014f,
		0.609f,  0.115f,  0.436f,
		0.327f,  0.483f,  0.844f,
		0.822f,  0.569f,  0.201f,
		0.435f,  0.602f,  0.223f,
		0.310f,  0.747f,  0.185f,
	};

	/*弾幕を表す配列*/
	//頂点情報->vertex、色情報->colorにpush_back()
	const int n_gon = 16;

	circle_vertex_buffer_data.push_back(0.0f);
	circle_vertex_buffer_data.push_back(0.0f);
	circle_vertex_buffer_data.push_back(0.0f);

	for(int i = 0; i < n_gon+1; i++) {
		circle_vertex_buffer_data.push_back(cos(M_PI*2/n_gon*i));
		circle_vertex_buffer_data.push_back(sin(M_PI*2/n_gon*i));
		circle_vertex_buffer_data.push_back(0.0f);
	}


	//各情報をOpenGLに渡すためのバッファ
	GLuint p_vertexbuffer;
	glGenBuffers(1, &p_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, p_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(player_vertex_buffer_data), player_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint p_colorbuffer;
	glGenBuffers(1, &p_colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, p_colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(player_color_buffer_data), player_color_buffer_data, GL_STATIC_DRAW);

	GLuint c_vertexbuffer;
	glGenBuffers(1, &c_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, c_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, circle_vertex_buffer_data.size()*sizeof(GLfloat), &circle_vertex_buffer_data[0], GL_STATIC_DRAW);

	GLuint c_colorbuffer;
	glGenBuffers(1, &c_colorbuffer);

	GLuint c_modelbuffer;
	glGenBuffers(1, &c_modelbuffer);

	GLuint e_c_vertexbuffer;
	glGenBuffers(1, &e_c_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, e_c_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, e_circle_vertex_buffer_data.size()*sizeof(GLfloat), &e_circle_vertex_buffer_data[0], GL_STATIC_DRAW);

	GLuint e_c_colorbuffer;
	glGenBuffers(1, &e_c_colorbuffer);

	GLuint e_c_modelbuffer;
	glGenBuffers(1, &e_c_modelbuffer);


	/*
	 * == メインループ =========================================================================
	 */

	while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
			glfwWindowShouldClose(window) == 0){

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//declare buffers
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		glEnableVertexAttribArray(5);
		glEnableVertexAttribArray(6);
		glEnableVertexAttribArray(7);

		glEnableVertexAttribArray(10);
		glEnableVertexAttribArray(11);
		glEnableVertexAttribArray(12);
		glEnableVertexAttribArray(13);
		glEnableVertexAttribArray(14);
		glEnableVertexAttribArray(15);

		/*プレイヤーの描画開始*/
		glUseProgram(programID);
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		//プレイヤーの移動計算
		computeMatricesFromInputs();
		glm::mat4 ModelMatrix = getModelMatrix();
		MVP = Projection * View * ModelMatrix;

		//vertexbuffers
		glBindBuffer(GL_ARRAY_BUFFER,p_vertexbuffer);
		glVertexAttribPointer(
				0,			//属性
				3,			//サイズ
				GL_FLOAT,		//型
				GL_FALSE,		//正規r
				0,			//ストライド
				(void*)0		//配列バッファオフセット
				);

		//colorbuffers
		glBindBuffer(GL_ARRAY_BUFFER,p_colorbuffer);
		glVertexAttribPointer(1,	3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawArrays(GL_TRIANGLES, 0, 3*2);

		/*プレイヤーの描画終了*/


		/*弾幕の描画開始*/
		glUseProgram(c_programID);
		glUniformMatrix4fv(c_MatrixID, 1, GL_FALSE, &c_VP[0][0]);

		//弾幕に透明度を持たせるためのブレンド関数
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//弾幕の移動計算
		for(auto bullet : BulletList){
			bullet->tick();
		}

		//Bullet vertexbuffers
		glBindBuffer(GL_ARRAY_BUFFER,c_vertexbuffer);
		glVertexAttribPointer(2,	3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		//Bullet colorbuffers
		glBindBuffer(GL_ARRAY_BUFFER, c_colorbuffer);
		glBufferData(GL_ARRAY_BUFFER, circle_color_buffer_data.size()*sizeof(GLfloat), &circle_color_buffer_data[0], GL_STATIC_DRAW);

		glVertexAttribPointer(3,	4, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glVertexAttribDivisor(3, 1);

		//Bullet modelbuffers
		glBindBuffer(GL_ARRAY_BUFFER, c_modelbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4)*ModelMatrixVector.size(), &ModelMatrixVector[0], GL_DYNAMIC_DRAW);

		glVertexAttribPointer(4,	4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)*0));
		glVertexAttribPointer(5,	4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)*1));
		glVertexAttribPointer(6,	4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)*2));
		glVertexAttribPointer(7,	4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)*3));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glVertexAttribDivisor(4, 1); // 色：弾ごとに一つ->1
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glVertexAttribDivisor(7, 1);

		glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, n_gon+2, BulletList.size());

		/*弾幕の描画終了*/


		/*敵の描画開始*/
		getFrag();	//敵の動きを制御するフラグを確立

		for(auto enemy : EnemyGroup){
			enemy->tick();

			enemy->draw(e_programID, e_MatrixID);
		}
		CollisionAll();
		
		/*敵の描画終了*/


		/*敵弾幕の描画開始*/
		glUseProgram(e_c_programID);
		glUniformMatrix4fv(e_c_MatrixID, 1, GL_FALSE, &e_c_VP[0][0]);

		//敵弾幕に透明度を持たせるためのブレンド関数
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//敵弾幕の移動計算
		for(auto bullet : e_BulletList){
			bullet->tick();
		}

		//EnemyBullet vertexbuffers
		glBindBuffer(GL_ARRAY_BUFFER,e_c_vertexbuffer);
		glVertexAttribPointer(10, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		//EnemyBullet colorbuffers
		glBindBuffer(GL_ARRAY_BUFFER, e_c_colorbuffer);
		glBufferData(GL_ARRAY_BUFFER, e_circle_color_buffer_data.size()*sizeof(GLfloat), &e_circle_color_buffer_data[0], GL_STATIC_DRAW);

		glVertexAttribPointer(11, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glVertexAttribDivisor(11, 1);

		//EnemyBullet modelbuffers
		glBindBuffer(GL_ARRAY_BUFFER, e_c_modelbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4)*e_ModelMatrixVector.size(), &e_ModelMatrixVector[0], GL_DYNAMIC_DRAW);

		glVertexAttribPointer(12, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)*0));
		glVertexAttribPointer(13, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)*1));
		glVertexAttribPointer(14, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)*2));
		glVertexAttribPointer(15, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)*3));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glVertexAttribDivisor(12, 1); // 色：弾ごとに一つ->1
		glVertexAttribDivisor(13, 1);
		glVertexAttribDivisor(14, 1);
		glVertexAttribDivisor(15, 1);

		glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, n_gon+2, e_BulletList.size());

		/*敵弾幕の描画終了*/


		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(4);
		glDisableVertexAttribArray(5);
		glDisableVertexAttribArray(6);
		glDisableVertexAttribArray(7);

		glDisableVertexAttribArray(10);
		glDisableVertexAttribArray(11);
		glDisableVertexAttribArray(12);
		glDisableVertexAttribArray(13);
		glDisableVertexAttribArray(14);
		glDisableVertexAttribArray(15);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteBuffers(1, &p_vertexbuffer);
	glDeleteBuffers(1, &p_colorbuffer);
	glDeleteBuffers(1, &c_vertexbuffer);
	glDeleteBuffers(1, &c_colorbuffer);
	glDeleteBuffers(1, &c_modelbuffer);

	for(auto enemy : EnemyGroup) {
		enemy->deleteVertex();
	}

	glDeleteBuffers(1, &e_c_vertexbuffer);
	glDeleteBuffers(1, &e_c_colorbuffer);
	glDeleteBuffers(1, &e_c_modelbuffer);


	glDeleteProgram(programID);
	glDeleteProgram(c_programID);
	glDeleteProgram(e_programID);
	glDeleteProgram(e_c_programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	glfwTerminate();


}
