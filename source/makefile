barragegame: main.o shader.o controls.o Bullet.o Enemy.o
	g++ --std=c++14 -o barragegame main.o shader.o controls.o Bullet.o Enemy.o -lGL -lGLU -lglfw -lXrandr -lXi -lXxf86vm -lX11 -lrt -lm -lpthread -lGLEW -ldl

main.o: main.cpp
	g++ --std=c++14 -c main.cpp 

shader.o: shader.cpp shader.hpp
	g++ --std=c++14 -c shader.cpp

controls.o: controls.cpp
	g++ --std=c++14 -c controls.cpp

Bullet.o: Bullet.cpp
	g++ --std=c++14 -c Bullet.cpp

Enemy.o: Enemy.cpp
	g++ --std=c++14 -c Enemy.cpp

clean:
	rm main.o shader.o controls.o Bullet.o Enemy.o
	

