CC=g++
CFLAGS= -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual
all: stack.exe 

stack.exe: main.cpp stack.cpp 
	   	   $(CC) main.cpp stack.cpp -o stack.exe $(CFLAGS)
		   
clean:
	   del *.o *.exe