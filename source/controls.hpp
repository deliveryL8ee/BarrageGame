#ifndef CONTROLS_HPP
#define CONTROLS_HPP

//Include GLEW
#include <GL/glew.h>


// Include GLFW
#include <GLFW/glfw3.h>
#include <iostream>
extern GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Bullet.hpp"


void computeMatricesFromInputs();
bool PressSPACE();
glm::mat4 getModelMatrix();

#endif
