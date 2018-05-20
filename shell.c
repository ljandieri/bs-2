#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h> // gibt " Cannot find 'wait.h' " bei mir

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
        printf("initLoop: memory allocation failed for entryDir\n"); //Todo: debug-message
    }
    loop->entryDir->dirName = ".";
    return loop; 
}

char* concat(char* string1, char* string2) {
    int l1 = strlen(string1);
    int l2 = strlen(string2);
    char* result = malloc(l1+l2+1); // Maybe use realloc(result,l1+l2+1) instead?
    if (result==NULL){printf("Concat: memory allocation failed\n");} //Todo: check for errors in malloc here
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

            // tmp = tmp->nextDir; oder?

            dirs = concat(dirs, "/");
        }
    }
    return dirs;
}

Loop * changeDir(char * newDir) // not sure this should return type Loop *
{
    char * newDirCopy = newDir;
    char * tmp = ""; 
    int startDirCheck = 0;
    Loop * newLoop = initLoop();
    newLoop->entryDir->dirName = "";
    Dir * tempDir = newLoop->entryDir;

    while (newDirCopy!=NULL)
    {
        if (newDirCopy[0]=='/')
        {
            if (startDirCheck++ == 0)
            {
                newLoop->entryDir->dirName = tmp;
            }
            else
            {
                tempDir->nextDir = (Dir*)malloc(sizeof(Dir));
                tempDir->nextDir->dirName = tmp;
                tempDir = tempDir->nextDir;
            }
            tmp = "";
        }
        else
        {
            concat(tmp,newDirCopy[0]);
        }
        newDirCopy++;
        // does there have to be '/' at the very end of path when using cd command?
    }
}

int runProg(Loop* loop) {

    return 0;
}

char* readInput(void) {
    char* input = NULL;
    size_t size = 0;
    ssize_t length = getline(&input, &size, stdin);
    if (length == -1) {
        printf("readInput: failed to read user input"); //Todo: do something
    }
    return input;
}

int handleInput(char* input, Loop* loop) {
    if (strncmp(input, "exit", 4) == 0) {
        loop->status = 1;
        return 0;
    } else if (strncmp(input, "run", 3) == 0) {
        runProg(loop);
    } else if (strncmp(input, "cd", 2) == 0){
        cd(input,loop);
    }
    return 1;
}

int cd(char * newDirPath, Loop * loop)
{
    newDirPath += 3; // slide pointer to where the path begins in input
    if (newDirPath == NULL)
    {
        printf("cd: directory not specified");
    }
    else 
    {
        loop = changeDir(newDirPath); // set loop to new loop?
    }
    return 0;
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
