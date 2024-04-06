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
 * 
 * example:
 * danielle@danielle-VirtualBox:~/home/danielle/Desktop/finalProject$ exit
    log out
    danielle@danielle-VirtualBox:~/Desktop/finalProject$ 
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
 * 
 * example:
 * danielle@danielle-VirtualBox:~/home/danielle/Desktop/finalProject$ echo hello world
    hello world 
    danielle@danielle-VirtualBox:~/home/danielle/Desktop/finalProject$ echo hi!
    hi! 
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
 * 
 * examples:
 * danielle@danielle-VirtualBox:~/home/danielle/Desktop/finalProject$ cd ..
    danielle@danielle-VirtualBox:~/home/danielle/Desktop$ cd "testing 1"
    danielle@danielle-VirtualBox:~/home/danielle/Desktop/testing 1$ cd ..
    danielle@danielle-VirtualBox:~/home/danielle/Desktop$ cd test 
    danielle@danielle-VirtualBox:~/home/danielle/Desktop/test$ 
 */
void cd(char **);

/*
 * Function: cp
 * ------------
 * Copies a file from a source path to a destination path.
 * 
 * Parameters:
 * - args: A pointer to the array of strings containing the command and its arguments.
 * 
 * If the source path or destination path is not provided, an error message is printed.
 * If the source file cannot be opened or if permission is denied, an error message is printed.
 * If the destination file cannot be created or if permission is denied, an error message is printed.
 * If the copying process is successful, a confirmation message is printed.
 * 
 * Note:
 * - The first argument (args[0]) is assumed to be the command itself (e.g., "cp").
 * - The second argument (args[1]) should be the source file path.
 * - The third argument (args[2]) should be the destination file path.
 * 
 * example:
 * danielle@danielle-VirtualBox:~/home/danielle/Desktop/testing 1$ cp file.txt fileCopy.txt
 * danielle@danielle-VirtualBox:~/home/danielle/Desktop/testing 1$ cp "file 1.txt" "file 1 copy.txt"
 */
void cp(char **);
/*
 * Function: get_dir
 * -----------------
 * Prints the names of all files and directories in the current directory.
 * 
 * This function does not take any arguments.
 * 
 * If the current directory cannot be opened or if permission is denied, an error message is printed.
 * Otherwise, it prints the names of all files and directories within the directory.
 * 
 * example:
 * danielle@danielle-VirtualBox:~/home/danielle/Desktop/testing 1$ dir
    fileCopy.txt file 1 copy.txt file.txt . file 1.txt file 2.txt .. 
 */
void get_dir();

/*
 * Function: delete
 * ----------------
 * Deletes a file specified by the given path.
 * 
 * Parameters:
 * - arg: A pointer to the array of strings containing the command and its arguments.
 * 
 * If the path to the file is not provided, an error message is printed.
 * If the file is successfully deleted, a confirmation message is printed.
 * If the file does not exist or cannot be deleted, an appropriate error message is printed.
 * 
 * example:
 * danielle@danielle-VirtualBox:~/home/danielle/Desktop/testing 1$ delete fileCopy.txt
    File deleted successfully: fileCopy.txt
    danielle@danielle-VirtualBox:~/home/danielle/Desktop/testing 1$ delete "file 1 copy.txt"
    File deleted successfully: file 1 copy.txt
 */
void delete(char **);
void systemCall(char **);
void mypipe(char **,char **);
void move(char **arguments);
/*
 * Function: readFile
 * ------------------
 * Reads and prints the content of a file specified by the given path.
 * 
 * Parameters:
 * - args: A pointer to the array of strings containing the command and its arguments.
 * 
 * If the filename is not provided, an error message is printed.
 * If the file cannot be opened or if permission is denied, an error message is printed.
 * Otherwise, the content of the file is printed to the standard output.
 */
void readFile(char **arguments);

/*
 * Function: wordCount
 * -------------------
 * Counts either the number of lines or words in a file specified by the given path.
 * 
 * Parameters:
 * - args: A pointer to the array of strings containing the command and its arguments.
 * 
 * Usage: wc <-l/-w> <filename>
 * -l: Count the number of lines in the file.
 * -w: Count the number of words in the file.
 * 
 * If the filename is not provided or the option is missing, an error message is printed.
 * If the file cannot be opened or if permission is denied, an error message is printed.
 * Otherwise, it counts and prints the number of lines or words based on the option specified.
 * 
 * example:
 * danielle@danielle-VirtualBox:~/home/danielle/Desktop/testing 1$ read "file 1.txt"
    hello world
    danielle

    34567
 */
void wordCount(char **arguments);

void echoppend(char **args);