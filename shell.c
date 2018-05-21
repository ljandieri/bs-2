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

            tmp = tmp->nextDir;

            dirs = concat(dirs, "/");
        }
    }
    return dirs;
}

void changeDir(char * newDir, Loop * loop) 
{
    char * newDirCopy = newDir;
    char * tmp = "";
    char * tmp2 = "";
    int startDirCheck = 0;
    loop->entryDir->dirName = "";
    Dir * tempDir = loop->entryDir;
    while (newDirCopy!=NULL)
    {
        if (newDirCopy[0]=='/')
        {
            if (startDirCheck++ == 0)
            {
                loop->entryDir->dirName = tmp;
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
	    tmp2[0] = newDirCopy[0];
        concat(tmp,tmp2);
        }
        newDirCopy++;
        // does there have to be '/' at the very end of path when using cd command?
    }
}

int cd(char * newDirPath, Loop * loop)
{
    newDirPath += 3; // slide pointer to where the path begins in input
    if (newDirPath == NULL)
    {
        printf("cd: directory not specified"); 
        return 1;
    } else if (strncmp(newDirPath,"..",2)==0){ // remove the last ->nextDir a.k.a last folder in directory
        Dir * tempDir = loop->entryDir;
            while (tempDir->nextDir->nextDir!=NULL)
            {
                tempDir = tempDir->nextDir;
            }
            tempDir->nextDir = NULL;
            return 1; 
    } else {
        changeDir(newDirPath,loop); 
    }
    return 0;
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

char ** parseInput(char * input)
{
    char ** args; // args[0] is the name of program, args[1-...] are arguments
    char * temp = "";
    int counter = 1;
    args = (char**) malloc(sizeof(char*));
    // store text until the first whitespace (program name)   into temp
    while (input[0]!=' ') 
    {   
        concat(temp,&input[0]);
        input++;
    }
    args[0] = (char*) malloc(sizeof(char) *strlen(temp));
    args[0] = temp;
    input++;
    //
    strcpy(temp,"");
    // get following arguments
    while (input!=NULL)
    {
        if (input[0]==' ' || input[0] == '\0') // if there's a whitespace or null character (end of input string)
        {
            args[counter] = (char*) malloc(sizeof(char) * strlen(temp));
	        args = (char**) realloc(args, sizeof(char*) * ++counter); // allocate an extra pointer in args for new argument 
                                                                     // increment counter
            strcpy(args[counter],temp);
            strcpy(temp,"");
        }
        else
        {
            temp+= input[0];
        }
        input ++;
    }

    return args;
}

int handleInput(char* input, Loop* loop) {
    if (strncmp(input, "exit", 4) == 0) {
        loop->status = 1;
        return 0;
    } else if (strncmp(input, "run", 3) == 0) {
        runProg(loop);
    } else if (strncmp(input, "cd", 2) == 0){
        cd(input,loop);
    } else { //parse input here

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
