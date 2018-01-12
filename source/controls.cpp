
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

float playerLife = 5.0f;


//change pattern of Enemy movement
void getFrag(){
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
     
     if (glfwGetKey(window, 'P') == GLFW_PRESS && glfwGetKey(window, 'P') == GLFW_RELEASE){
		EnemyGroup.push_back(new Enemy());							//敵の作成
		EnemyGroup[EnemyGroup.size()-1]->setParameter(e_posX, e_posY, 5.0f, 5.0f, 0.0f, 1.0f, EnemyGroup.size()-1, frag);		//敵にパラメータをセット
	}

     if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		BulletList.push_back(new MyBullet());							//弾幕の作成
		BulletList[BulletList.size()-1]->setParameter(posX, posY+scaleVec, 0.0f, 15.0f, BulletList.size()-1);		//弾幕にパラメータをセット
		BulletList.push_back(new MyBullet());
		BulletList[BulletList.size()-1]->setParameter(posX, posY+scaleVec, 2.0f, 15.0f, BulletList.size()-1);
		BulletList.push_back(new MyBullet());
		BulletList[BulletList.size()-1]->setParameter(posX, posY+scaleVec, -2.0f, 15.0f, BulletList.size()-1);

		ModelMatrixVector.push_back(glm::mat4());
		ModelMatrixVector.push_back(glm::mat4());
		ModelMatrixVector.push_back(glm::mat4());
     }

     if (glfwGetKey(window, 'O') == GLFW_PRESS) {
		float ex, ey;
		for(auto enemy : EnemyGroup){
			enemy->getPosition(&ex, &ey);

			for(float i = 0.0; i < 2.0; i += 0.2){
				e_BulletList.push_back(new EnemyBullet());							//弾幕の作成
				e_BulletList[e_BulletList.size()-1]->setParameter(ex, ey, 5*cos(i*M_PI), 5*sin(i*M_PI), e_BulletList.size()-1);		//弾幕にパラメータをセット

				e_ModelMatrixVector.push_back(glm::mat4());
			}
		}
     }


     TranslationMatrix = glm::translate(glm::mat4(), glm::vec3(posX,posY,1.0f));
     ModelMatrix = TranslationMatrix * ScalingMatrix * OriginalVector;

}

//当たり判定
/*
 * 引数は左から円1の半径、円2の半径、円1のx座標、円2のx座標、円1のy座標、円2のy座標
 * rlengthよりもお互いの円の中心座標の距離が短ければ当たっているという判定
 */

bool isCollide(float r1, float r2, float x1, float x2, float y1, float y2){
	float rlength = r1 + r2;		//半径同士を足し合わせた値
	float xlength = x1 - x2;
	float ylength = y1 - y2;

	if(rlength*rlength >= xlength*xlength + ylength*ylength) return true;
	else return false;
}

void CollisionAll(){
	float pbx, pby, ex, ey, ebx, eby;
	for(auto bullet : BulletList){
		for(auto enemy : EnemyGroup){
			bullet->getPosition(&pbx, &pby);
			enemy->getPosition(&ex, &ey);
			if(isCollide(scaleVec, 20.0f, pbx, ex, pby, ey)) enemy->damege();	//自機弾幕と敵の当たり判定
		}
	}
	for(auto bullet : e_BulletList){
		bullet->getPosition(&ebx, &eby);
		if(isCollide(scaleVec, 20.0f, posX, ebx, posY, eby)) playerLife -= 0.01f;	//自機と敵弾幕の当たり判定

		if(playerLife <= 0.0f){
			std::cout << std::endl;
			std::cout << "score is ... " << score << std::endl;
			exit(0);
		}
	}
}
