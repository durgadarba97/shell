# edit: setprompt.o shell.o
# 	gcc -o setprompt setprompt.o
# 	gcc -o shell shell.o
# setprompt.o : setprompt.c
# shell.o : shell.c

# clean:
# 	rm setprompt
# 	rm shell

default: shell
shell: shell.o 
	gcc -o shell shell.o
shell.o: shell.c
	gcc -c shell.c