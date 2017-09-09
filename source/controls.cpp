// Include GLFW
#include <GLFW/glfw3.h>
extern GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.hpp"

glm::mat4 ModelMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getModelMatrix(){
     return ModelMatrix;
}
glm::mat4 getProjectionMatrix(){
     return ProjectionMatrix;
}


// Initial model position
float Xpos = 0.0f; 
float Ypos = 0.0f; 

// Initial Field of View
float initialFoV = 45.0f;

float speed = 0.25f; // 3 units / second


void computeMatricesFromInputs(){
     // Move forward
     if (glfwGetKey(window, 'W') == GLFW_PRESS){
	  if(Xpos != 0.0f || Xpos != 768.0f)
	       Ypos += 1.0f * speed;
     }
     // Move backward
     if (glfwGetKey(window, 'S') == GLFW_PRESS){
	  if(Xpos != 0.0f || Xpos != 768.0f)
	       Ypos -= 1.0f * speed;
     }
     // Strafe right
     if (glfwGetKey(window, 'D') == GLFW_PRESS){
	  if(Xpos != 0.0f || Xpos != 1024.0f)
	       Xpos += 1.0f * speed;
     }
     // Strafe left
     if (glfwGetKey(window, 'A') == GLFW_PRESS){
	  if(Xpos != 0.0f || Xpos != 1024.0f)
	       Xpos -= 1.0f * speed;
     }

     float FoV = initialFoV;

     ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
     ModelMatrix = glm::translate(glm::mat4(), glm::vec3(Xpos,Ypos,1.0f));

}
