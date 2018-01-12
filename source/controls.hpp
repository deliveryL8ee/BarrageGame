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
#include "MyBullet.hpp"
#include "Enemy.hpp"
#include "EnemyBullet.hpp"


void computeMatricesFromInputs();
int getFrag();
bool isCollide(float r1, float r2, float x1, float x2, float y1, float y2);
void CollisionAll();
glm::mat4 getModelMatrix();

#endif
