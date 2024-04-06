#include "myShell.h"
#include "myFunction.h"

int main()
{
    welcome();
    while (1)
    {
        int piping = 0;
        getLocation();
        char *input = getInputFromUser();
        if (strcmp(input, "exit") == 0 || strncmp(input, "exit ", 5) == 0)
            logout(input);
        char **arguments = splitArgument(input);
        if (strcmp(input, "echo") == 0)
            echo(arguments);
        else if (strcmp(input, "cd") == 0)
            cd(arguments);
        else if (strcmp(input, "cp") == 0)
            cp(arguments);
        else if (strcmp(input, "mv") == 0)
            move(arguments);
        else if (strcmp(input, "delete") == 0)
            delete (arguments);
        else if (strcmp(input, "dir") == 0)
            get_dir();
        else if (strcmp(input, "read") == 0)
            readFile (arguments);
        else if (strcmp(input, "wc") == 0 && strcmp(arguments[1], "-w") == 0 && arguments[2] != NULL)
            wordCount(arguments);
        else if (strcmp(input, "append") == 0 && strstr(input, ">>"))
            echoppend(arguments);
        else if (piping)
        {
            char *argv1[] = {arguments[0], NULL};
            char *argv2[] = {arguments[2], arguments[3], NULL};
            mypipe(argv1, argv2);
            wait(NULL);
        }
        else
        {
            systemCall(arguments);
            wait(NULL);
        }
        free(arguments);
        free(input);
    }
    return 1;
}
void welcome()
{
    puts("#####################################  |");
    puts("#####################################  |   h!");
    puts("####                  ###############  |   welcome to my shell project!");
    puts("####                    #############  |");
    puts("####    ############     ############  |");
    puts("####    #############     ###########  |   this is my github:");
    puts("####    ##############     ##########  |   https://github.com/danielleOlch");
    puts("####    ###############    ##########  |");
    puts("####    ################    #########  |");
    puts("####    ################    #########  |   enjoy!");
    puts("####    ################    #########  |");
    puts("####    ################    #########  |");
    puts("####    ################    #########  |");
    puts("####    ################    #########  |"); 
    puts("####    ###############    ##########  |");
    puts("####    ##############     ##########  |");
    puts("####    #############     ###########  |");
    puts("####    ############     ############  |");
    puts("####                    #############  |");
    puts("####                  ###############  |");
    puts("#####################################  |");
    puts("#####################################  |");
}