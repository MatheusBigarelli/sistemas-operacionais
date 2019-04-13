default:
	debug

all:
	make debug
	make task1
	make task2
	make task3

debug: pingpong.c
	gcc -Wall -Wextra -DDEBUG -DTEST -g -o debug pingpong.c


task1: pingpong.c pingpong-tasks1.c
	gcc -Wall -Wextra -DDEBUG -g -o pingpong-tasks1 pingpong.c pingpong-tasks1.c


task2: pingpong.c pingpong-tasks2.c
	gcc -Wall -Wextra -DDEBUG -g -o pingpong-tasks2 pingpong-tasks2.c pingpong.c


task3: pingpong.c pingpong-tasks3.c
	gcc -Wall -Wextra -DDEBUG -g -o pingpong-tasks3 pingpong-tasks3.c pingpong.c
