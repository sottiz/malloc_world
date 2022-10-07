mallocWorld:  main.o
	gcc -o mallocWorld main.o

main.o : main.c
	gcc -o main.o -W -Wall -Wextra -Werror -c main.c

clean :
	rm -rf *.o *.zip

mrproper : clean
	rm mallocWorld

zip : 
	zip -r mallocWorld.zip .