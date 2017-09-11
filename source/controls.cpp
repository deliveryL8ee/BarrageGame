// Include GLFW
#include <GLFW/glfw3.h>
#include <iostream>
extern GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.hpp"

float width = 1024.0f;
float height = 768.0f;

float scaleV = 10.0f;

//変換させる行列,ウィンドウの中央が初期位置
glm::mat4 OriginalVector = glm::mat4(1.0f);
//拡大縮小行列
glm::mat4 ScalingMatrix = glm::scale(OriginalVector, glm::vec3(scaleV));
//平行移動行列
glm::mat4 TranslationMatrix;
//行列変換後のモデル行列
glm::mat4 ModelMatrix;

glm::mat4 getModelMatrix(){
     return ModelMatrix;
}


// Initial model position
float Xpos = width/2.0f;
float Ypos = height/2.0f;

// Initial Field of View
float initialFoV = 45.0f;

float speed = 5.0f; // 3 units / second


void computeMatricesFromInputs(){

	std::cout << "(" << Xpos << " ," << Ypos << ")" << std::endl;
     
     // Move forward
     if (glfwGetKey(window, 'W') == GLFW_PRESS){
	  Ypos += 1.0f * speed;
	  if(Ypos > height-scaleV) Ypos = height-scaleV;
     }
     // Move backward
     if (glfwGetKey(window, 'S') == GLFW_PRESS){
	  Ypos -= 1.0f * speed;
	  if(Ypos < 0.0f+scaleV) Ypos = 0.0f+scaleV;
     }
     // Strafe right
     if (glfwGetKey(window, 'D') == GLFW_PRESS){
	  Xpos += 1.0f * speed;
	  if(Xpos > width-scaleV) Xpos = width-scaleV;
     }
     // Strafe left
     if (glfwGetKey(window, 'A') == GLFW_PRESS){
	  Xpos -= 1.0f * speed;
	  if(Xpos < 0.0f+scaleV) Xpos = 0.0f+scaleV;
     }

     TranslationMatrix = glm::translate(glm::mat4(), glm::vec3(Xpos,Ypos,1.0f));
     ModelMatrix = TranslationMatrix * ScalingMatrix * OriginalVector;

}
