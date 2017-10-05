
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
float posX = width/2.0f;
float posY = height/2.0f;

// Initial Field of View
float initialFoV = 45.0f;
//speed of player moving
float speed = 5.0f;

int Bpoint = 0;

void computeMatricesFromInputs(){
     // Move forward
     if (glfwGetKey(window, 'W') == GLFW_PRESS){
	  posY += 1.0f * speed;
	  if(posY > height-scaleV) posY = height-scaleV;
     }
     // Move backward
     if (glfwGetKey(window, 'S') == GLFW_PRESS){
	  posY -= 1.0f * speed;
	  if(posY < 0.0f+scaleV) posY = 0.0f+scaleV;
     }
     // Strafe right
     if (glfwGetKey(window, 'D') == GLFW_PRESS){
	  posX += 1.0f * speed;
	  if(posX > width-scaleV) posX = width-scaleV;
     }
     // Strafe left
     if (glfwGetKey(window, 'A') == GLFW_PRESS){
	  posX -= 1.0f * speed;
	  if(posX < 0.0f+scaleV) posX = 0.0f+scaleV;
     }
     
     if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			BulletList.push_back(new Bullet());						//弾幕の作成
			BulletList[Bpoint]->setParameter(posX, posY, 0.0f, 10.0f);		//弾幕にパラメータをセット
			BulletList[Bpoint]->activate();							//弾幕をアクティブにする

			Bpoint++;
     }


     TranslationMatrix = glm::translate(glm::mat4(), glm::vec3(posX,posY,1.0f));
     ModelMatrix = TranslationMatrix * ScalingMatrix * OriginalVector;

}
