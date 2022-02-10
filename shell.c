#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>
#include <ctype.h>
#include <sys/types.h>
#include <unistd.h>

const int MAX_SIZE = 100;
const int MAX_HISTORY = 50;

/*
 * Description:
 *      Reads input from user, splitting each word into it's own c-string.
 * 
 * Input:
 *      int *words
 *          - int pointer passed by reference to keep count of how many words
 *            were given in user input
 * Return:
 *      char**
 *          - 2D Array of characters used to act as a 1D array of strings for 
 *            user input
 */
char** reads(int *words) {

    // Allocating memory for input and return
    char* text = (char *)malloc(512 * sizeof(char));
    char** parameters = (char **)malloc(2 * sizeof(char *));

    // Getting input from user and separating the first word into a c-string
    scanf("%[^\n]s", text);
    getchar();

    // fgets(text, 1000, stdin);
    char* token = strtok(text, " ");

    // Allocating memory for the first word
    parameters[0] = (char *)malloc(sizeof(token));

    // Copying first word into the parameters array
    strcpy(parameters[0], token);

    // Priming separation for while loop    
    token = strtok(NULL, " ");
    *words = 1;

    // NOTE: MAY NEED TO CODE A CATCH FOR MISINPUT

    // Separating the rest of the user input into individual c-strings 
    while(token != NULL)
    {
        // Allocating memory for next word
        parameters = realloc(parameters, ((*words+1) * sizeof(char **)));
        parameters[*words] = (char *)malloc(sizeof(token));

        // Copying next word into parameters
        strcpy(parameters[*words], token);

        // Increasing word counter
        *words = *words + 1;

        token = strtok(NULL, " ");
    }

    // Allocating memory for NULL value to end parameters
    parameters = realloc(parameters, ((*words+1) * sizeof(char **)));
    parameters[*words] = NULL;

    // Freeing memory
    free(text);
    free(token);

    return parameters;
}

// void empty_history(char** history){
//     for (int i = 0; i < sizeof(history); i++){
//         history[i] = "\0";
//     }
// }

// void history(char** commands, char* parameter){
//     if (parameter[0] == '-' && tolower(parameter[1]) == 'c' && parameter[2] == ' '){
//         empty_history(&commands);
//         return;
//     }
//     for (int i = 0; i < sizeof(commands); i++){
//         if(commands[i] == "\0")
//             return;
//         else{
//             printf("%i,. %s", i, commands[i]);
//         }
//     }
// }

int main() {
    char* command;
    char** parameters;
    char* test = "/usr/bin/ls";
    // char command_history[MAX_HISTORY][MAX_SIZE];
    // empty_history(command_history);
    
    // char *env[]={"PATH=/usr/bin/",NULL};

    while(true) {
        printf(">:");
        int words = 0;
        parameters = reads(&words);

        strcpy(command, "/usr/bin/");
        strcat(command, parameters[0]);
        // command[strlen(command)] = '\0';

        if(strcasecmp("exit", parameters[0]) == 0 || strcasecmp("quit", parameters[0]) == 0) {
            return 0;
        }

        // if(strcasecmp("history", command) == 0){
        //     history(&command_history, parameters);
        // }

        if(fork() != 0) {
            waitpid(-1, NULL, 0);
        } else {
            execvp(command, parameters);
        }

        for (int i = 0; i < words; i++){
            free(parameters[i]);
        }

    }
    free(parameters);
    free(command);
    free(test);
    return 0;
}