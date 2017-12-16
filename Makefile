CC=g++
CFLAGS=-Wall -pedantic `sdl2-config --cflags` `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -g -lm -std=c++11
TARGETS=main.o Graphic.o Ball.o Input.o Desk.o queue.o check_collision.o Player.o Button.o Threads.o

compile: $(TARGETS)
	$(CC) $(TARGETS) -o pong $(CFLAGS) -lpthread
run: compile
	./pong &

main.o: main.cpp Classes.h Desk.o
	$(CC) $(CFLAGS) -c main.cpp -lpthread

Graphic.o: Graphic.cpp Classes.h
	$(CC) $(CFLAGS) -c Graphic.cpp 

Ball.o: Ball.cpp Classes.h
	$(CC) $(CFLAGS) -c Ball.cpp

Input.o: Input.cpp Classes.h
	$(CC) $(CFLAGS) -c Input.cpp

Desk.o: Desk.cpp Classes.h
	$(CC) $(CFLAGS) -c Desk.cpp

queue.o: queue.cpp Classes.h
	$(CC) $(CFLAGS) -c queue.cpp -lpthread

Player.o: Player.cpp Classes.h
	$(CC) $(CFLAGS) -c Player.cpp -lpthread

Button.o: Button.cpp Classes.h
	$(CC) $(CFLAGS) -c Button.cpp -lpthread

Threads.o: Threads.cpp Classes.h
	$(CC) $(CFLAGS) -c Threads.cpp -lpthread





clean:
	rm -f *.o pong


