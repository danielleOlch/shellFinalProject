#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/stat.h>

#define BUFF_SIZE 256
#define blue() printf("\033[0;34m")
#define reset() printf("\033[0m")
#define green() printf("\033[0;32m")
#define bold() printf("\033[1m")
#define boldOff() printf("\e[m")

/*
 * Function: getInputFromUser
 * --------------------------
 * Reads input from the user until a newline character is encountered or EOF is reached,
 * dynamically allocating memory to store the input string.
 * 
 * Returns:
 * - A pointer to the dynamically allocated string containing the user input.
 * - NULL if memory allocation fails.
 * 
 * Memory allocation failures are handled by printing an error message to stderr and exiting
 * the program with EXIT_FAILURE status.
 */
char *getInputFromUser();

/*
 * Function: splitArgument
 * ------------------------
 * Splits a string into individual arguments based on the specified delimiter (whitespace by default),
 * dynamically allocating memory to store the arguments.
 * 
 * Parameters:
 * - str: A pointer to the input string to be split.
 * 
 * Returns:
 * - A pointer to the dynamically allocated array of strings containing the arguments.
 * - NULL if memory allocation fails.
 * 
 * Memory allocation failures are handled by printing an error message to stderr and exiting
 * the program with EXIT_FAILURE status.
 */
char **splitArgument(char *);

/*
 * Function: getLocation
 * ----------------------
 * Retrieves the current location information, including username, hostname, and current path,
 * and prints it in a formatted manner.
 * 
 * This function uses system calls to get the hostname, current working directory, and username.
 * It then formats the information to display the username and hostname in green and bold,
 * followed by a colon, the current path in blue and bold, and finally, a dollar sign ($) prompt.
 * 
 * The function also handles error cases by printing appropriate error messages to stderr if
 * any of the system calls fail.
 */
void getLocation();

/*
 * Function: logout
 * ----------------
 * Logs out the user from the shell session.
 * 
 * Frees the memory allocated for the input string, prints "log out" to indicate the logout action,
 * and exits the program with EXIT_SUCCESS status.
 * 
 * Parameters:
 * - input: A pointer to the string containing user input, typically used for freeing memory.
 * 
 * Note:
 * Memory allocated for the input string should have been dynamically allocated using functions
 * like malloc(), calloc(), or realloc(). If the input was not dynamically allocated, calling
 * free(input) would result in undefined behavior.
 */
void logout(char *);

/*
 * Function: echo
 * --------------
 * Prints the arguments passed to the echo command, excluding the command itself.
 * 
 * Parameters:
 * - arg: A pointer to the array of strings containing the arguments.
 * 
 * The function starts printing arguments from the second one onwards, skipping the command itself.
 * Each argument is printed followed by a space, and a newline character is printed at the end.
 */
void echo(char **);

/*
 * Function: cd
 * ------------
 * Changes the current working directory to the one specified in the argument.
 * 
 * Parameters:
 * - arg: A pointer to the array of strings containing the command and its arguments.
 * 
 * If no directory is specified as an argument, an error message is printed.
 * If the specified directory doesn't exist or if permission is denied, an appropriate error message is printed.
 * If the directory change is successful, a confirmation message with the new directory is printed.
 * 
 * Note:
 * - The first argument (arg[0]) is assumed to be the command itself (e.g., "cd").
 * - The second argument (arg[1]) should be the directory to change to.
 */
void cd(char **);
void cp(char **);
void get_dir();
void delete(char **);
void systemCall(char **);
void mypipe(char **,char **);