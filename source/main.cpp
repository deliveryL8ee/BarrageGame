#include <iostream>

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

     GLuint MatrixID = glGetUniformLocation(programID, "MVP");


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
     glm::mat4 Model = glm::mat4(1.0f);

     glm::mat4 MVP = Projection * View * Model;


     static const GLfloat g_vertex_buffer_data[] = { 
	  //プレイヤー
	  -1.0f, 1.0f, 0.0f,
	  -1.0f,-1.0f, 0.0f,
	   1.0f, 1.0f, 0.0f,
	   1.0f, 1.0f, 0.0f,
	  -1.0f,-1.0f, 0.0f,
	   1.0f,-1.0f, 0.0f,
     };

     static const GLfloat g_color_buffer_data[] = { 
	  0.583f,  0.771f,  0.014f,
	  0.609f,  0.115f,  0.436f,
	  0.327f,  0.483f,  0.844f,
	  0.822f,  0.569f,  0.201f,
	  0.435f,  0.602f,  0.223f,
	  0.310f,  0.747f,  0.185f,
     };

     GLuint vertexbuffer;
     glGenBuffers(1, &vertexbuffer);
     glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
     glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

     GLuint colorbuffer;
     glGenBuffers(1, &colorbuffer);
     glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
     glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

     while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
	       glfwWindowShouldClose(window) == 0){

	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	  computeMatricesFromInputs();
	  /*カメラの位置を取得する*/
	  glm::mat4 ModelMatrix = getModelMatrix();
	  glm::mat4 MVP = Projection * View * ModelMatrix;

	  glUseProgram(programID);
	  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

	  //declare buffers
	  glEnableVertexAttribArray(0);
	  glEnableVertexAttribArray(1);

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


	  //三角形を描く
	  glDrawArrays(GL_TRIANGLES, 0, 3*2); //頂点0 ~ 3*2 (3頂点 = 1三角形 -> 2三角形 = 四角形 = 6頂点)

	  glDisableVertexAttribArray(0);
	  glDisableVertexAttribArray(1);

	  glfwSwapBuffers(window);
	  glfwPollEvents();
     }

	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	glfwTerminate();

}
