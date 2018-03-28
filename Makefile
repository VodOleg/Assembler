	
Assembler : Assembler.o utils.o list.o DataStructures.o Parse_tools.o first_iteration.o second_iteration.o
		gcc -g -ansi -Wall -pedantic Assembler.o utils.o list.o Parse_tools.o DataStructures.o first_iteration.o second_iteration.o -o Assembler

Assembler.o : Assembler.c
		gcc -c -ansi -g -Wall -pedantic Assembler.c -o Assembler.o

utils.o : utils.c
		gcc -c -ansi -g -Wall -pedantic utils.c -o utils.o

list.o : list.c
		gcc -c  -ansi -g -Wall -pedantic list.c -o list.o

DataStructures.o : DataStructures.c
		gcc -c -ansi -g -Wall -pedantic DataStructures.c -o DataStructures.o

Parse_tools.o : Parse_tools.c
		gcc -c -ansi -g -Wall -pedantic Parse_tools.c -o Parse_tools.o

first_iteration.o : first_iteration.c
		gcc -c -ansi -g -Wall -pedantic first_iteration.c -o first_iteration.o

second_iteration.o : second_iteration.c
		gcc -c -ansi -g -Wall -pedantic second_iteration.c -o second_iteration.o