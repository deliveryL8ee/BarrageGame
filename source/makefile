barragegame: main.o shader.o controls.o MyBullet.o Enemy.o EnemyBullet.o
	g++ --std=c++14 -o barragegame main.o shader.o controls.o MyBullet.o Enemy.o EnemyBullet.o -lGL -lGLU -lglfw -lXrandr -lXi -lXxf86vm -lX11 -lrt -lm -lpthread -lGLEW -ldl

main.o: main.cpp
	g++ --std=c++14 -Wall -c main.cpp 

shader.o: shader.cpp shader.hpp
	g++ --std=c++14 -Wall -c shader.cpp

controls.o: controls.cpp
	g++ --std=c++14 -Wall -c controls.cpp

MyBullet.o: MyBullet.cpp
	g++ --std=c++14 -Wall -c MyBullet.cpp

Enemy.o: Enemy.cpp
	g++ --std=c++14 -Wall -c Enemy.cpp

EnemyBullet.o: EnemyBullet.cpp
	g++ --std=c++14 -Wall -c EnemyBullet.cpp

clean:
	rm main.o shader.o controls.o MyBullet.o Enemy.o EnemyBullet.o
	

