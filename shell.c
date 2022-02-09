#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <wait.h>
#include <ctype.h>

const int MAX_SIZE = 100;
const int MAX_HISTORY = 50;

char** read() {
    char text[100];
    char (*parameters)[100];
    scanf("%[^\n]s", text);
    char* token = strtok(text, " ");
    int i = 0;
    while(&token[i] != "\0") {
        parameters[0][i] = token[i];
        i++;
    }
    
    token = strtok(NULL, "\0");
    i = 1;
    int j = 0;
    while(&token[i] != "\0")
    {
        parameters[i][j] = token[j];
        i++; j++;
    }
    printf("%s",parameters);
    return parameters;
}

void empty_history(char** history){
    for (int i = 0; i < sizeof(history); i++){
        history[i] = "\0";
    }
}

void history(char** commands, char* parameter){
    if (parameter[0] == '-' && tolower(parameter[1]) == 'c' && parameter[2] == ' '){
        empty_history(&commands);
        return;
    }
    for (int i = 0; i < sizeof(commands); i++){
        if(commands[i] == "\0")
            return;
        else{
            printf("%i,. %s", i, commands[i]);
        }
    }
}

int main() {
    char* command[MAX_SIZE], * parameters[MAX_SIZE];
    char command_history[MAX_HISTORY][MAX_SIZE];
    empty_history(command_history);
    
    char *env[]={"PATH=/usr/bin/",NULL};

    while(true) {
        printf(">:");
        read(command, parameters);

        if(strcasecmp("exit", command) == 0 || strcasecmp("quit", command) == 0) {
            return 0;
        }

        if(strcasecmp("history", command) == 0){
            history(&command_history, parameters);
        }

        if(fork() != 0) {
            waitpid(-1, NULL, 0);
        } else {
            execve(command, parameters, 0);
        }
    }
    return 0;
}