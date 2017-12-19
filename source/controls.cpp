
#include "controls.hpp"

float width = 768.0f;
float height = 768.0f;

float scaleVec = 10.0f;

//変換させる行列,ウィンドウの中央が初期位置
glm::mat4 OriginalVector = glm::mat4(1.0f);
//拡大縮小行列
glm::mat4 ScalingMatrix = glm::scale(OriginalVector, glm::vec3(scaleVec));
//平行移動行列
glm::mat4 TranslationMatrix;
//行列変換後のモデル行列
glm::mat4 ModelMatrix;

glm::mat4 getModelMatrix(){
     return ModelMatrix;
}

//pattern of Enemy movement
int frag = 1;

// Initial model position
float posX = width/2.0f;
float posY = height/2.0f;

float e_posX = width/2.0f;
float e_posY = 3*height/4.0f;

//speed of player moving
float speed = 5.0f;


//change pattern of Enemy movement
int getFrag(){
	//Get fragment by pressing key of number
     if (glfwGetKey(window, '1') == GLFW_PRESS){
		frag = 1;
     }
     if (glfwGetKey(window, '2') == GLFW_PRESS){
		frag = 2;
     }
     if (glfwGetKey(window, '3') == GLFW_PRESS){
		frag = 3;
     }
     if (glfwGetKey(window, '4') == GLFW_PRESS){
		frag = 4;
     }
     if (glfwGetKey(window, '5') == GLFW_PRESS){
		frag = 5;
     }
}

void computeMatricesFromInputs(){
     // Move forward
     if (glfwGetKey(window, 'W') == GLFW_PRESS){
	  posY += 1.0f * speed;
	  if(posY > height-scaleVec) posY = height-scaleVec;
     }
     // Move backward
     if (glfwGetKey(window, 'S') == GLFW_PRESS){
	  posY -= 1.0f * speed;
	  if(posY < 0.0f+scaleVec) posY = 0.0f+scaleVec;
     }
     // Strafe right
     if (glfwGetKey(window, 'D') == GLFW_PRESS){
	  posX += 1.0f * speed;
	  if(posX > width-scaleVec) posX = width-scaleVec;
     }
     // Strafe left
     if (glfwGetKey(window, 'A') == GLFW_PRESS){
	  posX -= 1.0f * speed;
	  if(posX < 0.0f+scaleVec) posX = 0.0f+scaleVec;
     }
     
     if (glfwGetKey(window, 'P') == GLFW_PRESS) {
		if(EnemyGroup.size() < 20){
			EnemyGroup.push_back(new Enemy());							//敵の作成
			EnemyGroup[EnemyGroup.size()-1]->setParameter(e_posX, e_posY, 5.0f, 5.0f, 0.0f, 1.0f, EnemyGroup.size()-1, frag);		//敵にパラメータをセット
		}
	}

     if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		BulletList.push_back(new Bullet());							//弾幕の作成
		BulletList[BulletList.size()-1]->setParameter(posX, posY+scaleVec, 0.0f, 15.0f, BulletList.size()-1);		//弾幕にパラメータをセット
		BulletList.push_back(new Bullet());
		BulletList[BulletList.size()-1]->setParameter(posX, posY+scaleVec, 2.0f, 15.0f, BulletList.size()-1);
		BulletList.push_back(new Bullet());
		BulletList[BulletList.size()-1]->setParameter(posX, posY+scaleVec, -2.0f, 15.0f, BulletList.size()-1);

		ModelMatrixVector.push_back(glm::mat4());
		ModelMatrixVector.push_back(glm::mat4());
		ModelMatrixVector.push_back(glm::mat4());
     }


     TranslationMatrix = glm::translate(glm::mat4(), glm::vec3(posX,posY,1.0f));
     ModelMatrix = TranslationMatrix * ScalingMatrix * OriginalVector;

}

