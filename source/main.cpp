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
#include "Bullet.hpp"

int main() {
     float width = 1024.0f;
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

     GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.flagmentshader");


     //射影行列
     //glm::ortho(left, right, bottom, top, near, far)
     glm::mat4 Projection = glm::ortho(0.0f, width, 0.0f, height, 0.0f, 5.0f);

     //カメラ行列
     glm::mat4 View = glm::lookAt(
	       glm::vec3(0,0,1),
	       glm::vec3(0,0,0),
	       glm::vec3(0,1,0)
	       );
     
     //モデル行列
     glm::mat4 Model = glm::translate(glm::mat4(), glm::vec3(100.0f,100.0f,1.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(100.0f));

     glm::mat4 MVP = Projection * View * glm::mat4(1.0f);

     glm::mat4 MVP2 = Projection * View * Model;

     GLuint MatrixID = glGetUniformLocation(programID, "MVP");


	//プレイヤーを表す配列
	//頂点情報
     static const GLfloat g_vertex_buffer_data[] = { 
	  -1.0f, 1.0f, 0.0f,
	  -1.0f,-1.0f, 0.0f,
	   1.0f, 1.0f, 0.0f,
	   1.0f, 1.0f, 0.0f,
	  -1.0f,-1.0f, 0.0f,
	   1.0f,-1.0f, 0.0f,
     };

	//色情報
     static const GLfloat g_color_buffer_data[] = { 
	  0.583f,  0.771f,  0.014f,
	  0.609f,  0.115f,  0.436f,
	  0.327f,  0.483f,  0.844f,
	  0.822f,  0.569f,  0.201f,
	  0.435f,  0.602f,  0.223f,
	  0.310f,  0.747f,  0.185f,
     };

	//可変長の弾幕配列
	std::vector <GLfloat> circle_vertex_buffer_data;		//頂点情報
	std::vector <GLfloat> circle_color_buffer_data;		//色情報

	circle_vertex_buffer_data.push_back(0);
	circle_vertex_buffer_data.push_back(0);
	circle_vertex_buffer_data.push_back(0);
	circle_color_buffer_data.push_back(1);
	circle_color_buffer_data.push_back(1);
	circle_color_buffer_data.push_back(1);
	
	const int n_gon = 16;

	for(int i = 0; i < n_gon+1; i++) {
		circle_vertex_buffer_data.push_back(cos(M_PI*2/n_gon*i));
		circle_vertex_buffer_data.push_back(sin(M_PI*2/n_gon*i));
		circle_vertex_buffer_data.push_back(0);
		circle_color_buffer_data.push_back(1);
		circle_color_buffer_data.push_back(1);
		circle_color_buffer_data.push_back(1);
	}


     GLuint vertexbuffer;
     glGenBuffers(1, &vertexbuffer);
     glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
     glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

     GLuint colorbuffer;
     glGenBuffers(1, &colorbuffer);
     glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
     glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

     GLuint c_vertexbuffer;
     glGenBuffers(1, &c_vertexbuffer);
     glBindBuffer(GL_ARRAY_BUFFER, c_vertexbuffer);
     glBufferData(GL_ARRAY_BUFFER, circle_vertex_buffer_data.size()*sizeof(GLfloat), &circle_vertex_buffer_data[0], GL_STATIC_DRAW);

     GLuint c_colorbuffer;
     glGenBuffers(1, &c_colorbuffer);
     glBindBuffer(GL_ARRAY_BUFFER, c_colorbuffer);
     glBufferData(GL_ARRAY_BUFFER, circle_color_buffer_data.size()*sizeof(GLfloat), &circle_color_buffer_data[0], GL_STATIC_DRAW);

     while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
	       glfwWindowShouldClose(window) == 0){

	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	  //プレイヤーの移動計算
	  computeMatricesFromInputs();
	  glm::mat4 ModelMatrix = getModelMatrix();
	  glm::mat4 MVP = Projection * View * ModelMatrix;

	  //弾幕の移動計算
	  myBullet.tick();


	  /*プレイヤーの描写開始*/
	  glUseProgram(programID);
	  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);


	  //declare buffers
	  glEnableVertexAttribArray(0);
	  glEnableVertexAttribArray(1);
	  glEnableVertexAttribArray(2);
	  glEnableVertexAttribArray(3);

	  //vertexbuffers
	  glBindBuffer(GL_ARRAY_BUFFER,vertexbuffer);
	  glVertexAttribPointer(
		    0,			//属性
		    3,			//サイズ
		    GL_FLOAT,		//型
		    GL_FALSE,		//正規化
		    0,			//ストライド
		    (void*)0		//配列バッファオフセット
		    );

	  //colorbuffers
	  glBindBuffer(GL_ARRAY_BUFFER,colorbuffer);
	  glVertexAttribPointer(
		    1,			//属性
		    3,			//サイズ
		    GL_FLOAT,		//型
		    GL_FALSE,		//正規化
		    0,			//ストライド
		    (void*)0		//配列バッファオフセット
		    );

	  glDrawArrays(GL_TRIANGLES, 0, 3*2); //頂点0 ~ 3*2 (3頂点 = 1三角形 -> 2三角形 = 四角形 = 6頂点)
	  /*プレイヤーの描写終了*/


	  /*弾幕の描写開始*/
	  //myBullet.draw();
	  glUseProgram(programID);
	  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP2[0][0]);

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
	  /*弾幕の描写終了*/

	  glDisableVertexAttribArray(0);
	  glDisableVertexAttribArray(1);
	  glDisableVertexAttribArray(2);
	  glDisableVertexAttribArray(3);

	  glfwSwapBuffers(window);
	  glfwPollEvents();
     }

	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteBuffers(1, &c_vertexbuffer);
	glDeleteBuffers(1, &c_colorbuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	glfwTerminate();

}
