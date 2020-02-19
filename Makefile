./bin/demo: ./obj/demo.o ./obj/sudoku.o
	gcc ./obj/demo.o ./obj/sudoku.o -o ./bin/demo -lm
	cp ./bin/demo run

./obj/demo.o: ./src/demo.c
	gcc -c ./src/demo.c -o ./obj/demo.o -g

./obj/sudoku.o: ./src/sudoku.c
	gcc -c ./src/sudoku.c -o ./obj/sudoku.o -g

clear: 
	rm ./obj/*.o

