/* Names: Reily Stanford and Riki Tejeda
 * Date: 2/10/22
 * Filename: shell.c
 * Description: Program acts as a shell and uses MOST? bash commands like ls, 
 *  cat, cp, emacs, history, etc.
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <wait.h>
#include <ctype.h>
#include <sys/types.h>
#include <unistd.h>

const int MAX_HISTORY = 50;

/*
 * Description:
 *      Adds the given command to the history.
 * 
 * Input:
 *      char** history
 *          - array of c-strings holding the current
 *            history of entered commands.
 *      char* command
 *          - c-string holding the command to be
 *            added to the history.
 * Return:
 *      char**
 *          - 2D Array of characters used to act as a 1D array of strings
 *            holding all of the entered commands.
 */
char** add_history(char** history, char* command){
    // Loop until NULL is found to insert the command into history[i] and
    // returning.
    for(int i = 0; i < MAX_HISTORY; i++) {
        if(history[i] == NULL){
            history[i] = command;
            return history;
        }
    }

    // Pushing each element at history[i] over by one index
    for(int i = 0; i < MAX_HISTORY - 1; i++){
        strcpy(history[i], history[i+1]);
    }

    // Add the command to the last index of history, which makes it the newest
    // command
    strcpy(history[MAX_HISTORY-1], command);
    return history;
}

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
    char* text = malloc(512 * sizeof(char));
    char** parameters = malloc(2 * sizeof(char*));

    // Getting input from user and separating the first word into a c-string
    scanf("%[^\n]s", text);
    getchar();
    char* token = strtok(text, " ");

    // Allocating memory for the first word
    parameters[0] = (char *)malloc(sizeof(token));

    // Copying first word into the parameters array
    strcpy(parameters[0], token);

    // Priming separation for while loop    
    token = strtok(NULL, " ");
    *words = 1;

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

/*
 * Description:
 *      Clears the given history.
 * 
 * Input:
 *      char** history
 *          - Array of c-strings used to hold the history.
 * 
 * Result:
 *      The given history is cleared.
 */
void empty_history(char** history){
    // Empty the history by freeing the memory at each index
    for (int i = 0; i < MAX_HISTORY; i++){
        free(history[i]);
    }
}

/*
 * Description:
 *      Gets the parameters for a desired command.
 * 
 * Input:
 *      char** history
 *          - Array of c-strings used to hold the history
 *      int index
 *          - The index to be accessed in history
 * Return:
 *      char**
 *          - The parameters for the requested function
 */
char** access_history(char** history, int index){
    // Allocating memory for return
    char** parameters = malloc(2 * sizeof(char*));

    // Grabbing the first word in history and inserting as a c-string in token
    char* token = strtok(history[index], " ");
    int words = 1;

    // Allocating memory for the first word
    parameters[0] = (char *)malloc(sizeof(token));

    // Copying first word into the parameters array
    strcpy(parameters[0], token);

    // Priming separation for while loop    
    token = strtok(NULL, " ");

    // Separating the rest of the history's words into individual c-strings and
    // inserting into parameters
    while(token != NULL)
    {
        // Allocating memory for next word
        parameters = realloc(parameters, ((words+1) * sizeof(char **)));
        parameters[words] = (char *)malloc(sizeof(token));

        // Copying next word into parameters
        strcpy(parameters[words], token);

        // Increasing word counter
        words = words + 1;

        token = strtok(NULL, " ");
    }

    // Allocating memory for NULL value to end parameters
    parameters = realloc(parameters, ((words+1) * sizeof(char **)));
    parameters[words] = NULL;

    // Freeing memory
    free(token);

    return parameters;
}

/*
 * Description:
 *      Runs a shell that allows for any command from
 *      /usr/bin/ to be used. 
 */
int main() {

    // Creating a variable to hold the previously entered commands
    char** command_history;
    command_history = malloc(MAX_HISTORY * sizeof(char*));
    
    // Creating a command a parameters variable that is needed for execvp
    char command[256];
    char** parameters;

    // Loops until user enters "exit", "quit", or the user issues an interrupt
    while(true) {
        printf(">: ");

        // Counter to see how many words are given by the user
        int words = 0;

        // Holds a pointer of c-strings given by user input
        parameters = reads(&words);

        // Allocating memory for input that will be used a helper to keep track
        // of the user's commands
        char* input = malloc(256 * sizeof(char));

        // Grab the first word in the command
        strcpy(input, parameters[0]);

        // Loop will only run if there are other words following the first word
        // so that history can get the full command
        for (int i = 1; i < words; i++){
            strcat(input, " ");
            strcat(input, parameters[i]);
        }

        // Add input to the history, which is held in command_history
        command_history = add_history(command_history, input);

        // Catch quit or exit to leave the program and free all the memory
        // before returning
        if(strcasecmp("exit", parameters[0]) == 0 || strcasecmp("quit", parameters[0]) == 0){
            for (int i = 0; i < words; i++)
                free(parameters[i]);
            free(parameters);
            for (int i = 0; i < MAX_HISTORY; i++)
                free(command_history[i]);
            free(command_history);
            return 0;
        }

        // If history is given by the user the following is executed
        if(strcasecmp("history", parameters[0]) == 0){
            // Prints all of the user's commands up to 50 if "history" is only
            // inputted
            if(parameters[1] == NULL){
                for(int i = 0; i < MAX_HISTORY; i++){
                    if(command_history[i] != NULL){
                        printf("%d. %s\n", i, command_history[i]);
                    }
                }
            }
            // Clears the history and allocates new memory for the next list
            // of commands
            else if(strcasecmp(parameters[1], "-c") == 0){
                empty_history(command_history);
                command_history = malloc(MAX_HISTORY * sizeof(char*));
            }
            // Convert the user's input into integers and makes sure it is
            // within the bounds of the command list
            else if (atoi(parameters[1]) >= 0 && atoi(parameters[1]) < 49) {
                // Makes sure that user can't call a command that doesn't exist
                // because the command list hasn't reached that index yet
                if (command_history[atoi(parameters[1])] == NULL){
                    printf("Offset is not valid.\n");
                } 
                // If the offset matches a command in the command list, then
                // parameters will hold the command at the given offset
                else {
                    parameters = access_history(command_history, atoi(parameters[1]));
                }
            }
            // User did not provide the right syntax of history
            else {
                printf("Invalid use of history.\n Please use:\n\thistory\n\thistory -c\n\thistory [index]\n");
            }
        }

        // Need the path so that the program knows where the bash commands are from
        strcpy(command, "/usr/bin/");
        // Concatenate the path to what the user inputted
        strcat(command, parameters[0]);

        if(fork() != 0) 
            waitpid(-1, NULL, 0);
        // Execute the command
        else 
            execvp(command, parameters);
    }
    return 0;
}