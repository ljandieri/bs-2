#include <stdio.h> 
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct Dir {
	char* dirName;
	struct Dir* nextDir;
} Dir;

typedef struct Loop {
	Dir* entryDir;
	int status;
	pid_t children[8];
} Loop;

Loop* initLoop(){
	Loop* loop = (Loop*)malloc(sizeof(Loop));
	loop->status = 0;
	
	loop->entryDir = (Dir*)malloc(sizeof(Dir));
	if (loop->entryDir == NULL) {
		//Todo: debug-message
	}
	loop->entryDir->dirName = ".";
	
	return loop;
}

char* concat(char* string1, char* string2) {
    char* result = malloc(strlen(string1)+strlen(string2)+1);
    //Todo: check for errors in malloc here
    strcpy(result, string1);
    strcat(result, string2);
    return result;
}

char* buildCurrentDir(Dir* startDir) {
	char* dirs = "";
	if (startDir != NULL) {
		dirs = startDir->dirName;
		dirs = concat(dirs, "/");
		Dir* tmp = startDir->nextDir;
		while(tmp != NULL) {
			dirs = concat(dirs, tmp->dirName);
			dirs = concat(dirs, "/");
		}
	}
	return dirs;
}

int runProg(Loop* loop) {
	
	return 0;
}

char* readInput(void) {
	char* input = NULL;
	size_t size = 0;
	ssize_t length = getline(&input, &size, stdin);
	if (length == -1) {
		//Todo: do something
	}
	return input;
}

int handleInput(char* input, Loop* loop) {
	if (strncmp(input, "exit", 4) == 0) {
		loop->status = 1;
		return 0;
	} else if (strncmp(input, "run", 3) == 0) {
		runProg(loop);
	}
	return 1;
}

int loop(void) {
	
	Loop* loop = initLoop();
	
	char* input;
	int inputSuccess;
	while (loop->status == 0) {
		printf("%s", buildCurrentDir(loop->entryDir));
		printf(">");
		input = readInput();
		inputSuccess = handleInput(input, loop);
		if (inputSuccess == 1) {
			//Todo: do something
		}
	}
	
	return 0;
}

int main(void) {
	
	loop();
	
	return 0;
}
