#include "myFunction.h"

char *getInputFromUser() {
    char ch;
    int size = 1;
    int index = 0;
    char *str = (char *)malloc(size * sizeof(char));
    if (str == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    
    while ((ch = getchar()) != '\n' && ch != EOF) {
        *(str + index) = ch;
        index++;
        if (index >= size - 1) { // Check if more memory is needed
            size *= 2; // Double the size
            str = (char *)realloc(str, size * sizeof(char));
            if (str == NULL) {
                fprintf(stderr, "Memory reallocation failed\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    *(str + index) = '\0'; 

    return str;
}

char **splitArgument(char *str) {
    char **arguments;
    int size = 2;
    int index = 0;
    char *subStr = strtok(str, " ");

    arguments = (char **)malloc(size * sizeof(char *));
    if (arguments == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    while (subStr != NULL) {
        arguments[index] = strdup(subStr); // Duplicate the substring
        if (arguments[index] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }

        index++;

        if (index >= size - 1) { // Check if more memory is needed
            size *= 2; // Double the size
            arguments = (char **)realloc(arguments, size * sizeof(char *));
            if (arguments == NULL) {
                fprintf(stderr, "Memory reallocation failed\n");
                exit(EXIT_FAILURE);
            }
        }

        subStr = strtok(NULL, " ");
    }

    arguments[index] = NULL; // Null-terminate the array

    return arguments;
}

void getLocation() {
    char location[BUFF_SIZE];
    char hostname[BUFF_SIZE];
    char *username;

    if (gethostname(hostname, BUFF_SIZE) == -1) {
        printf("Error getting hostname.\n");
        return;
    }

    if (getcwd(location, BUFF_SIZE) == NULL) {
        printf("Error getting current directory.\n");
        return;
    }

    username = getlogin(); // get username

    if (username != NULL) {
        green();
        bold();
        printf("%s@%s", username, hostname); // print username and computer name
        reset();
        printf(":");
        blue();
        bold();
        printf("~%s", location); // print current path
        reset();
        printf("$ ");
    } else {
        printf("Error getting username.\n");
    }
}

void logout(char *input) {
    // Trim leading and trailing spaces from input string
    while (*input && isspace(*input))
        input++;
    int len = strlen(input);
    while (len > 0 && isspace(input[len - 1]))
        input[--len] = '\0';

    // Check if the input string starts with "exit " or matches "exit" exactly
    if (strncmp(input, "exit ", 5) == 0 || strcmp(input, "exit") == 0) {
        puts("log out");
        free(input); // Free memory if input was dynamically allocated
        exit(EXIT_SUCCESS);
    } else {
        free(input); // Free memory if input was dynamically allocated
        return;
    }
}

void echo(char **arg)
{
    arg++; 
    while (*arg != NULL) {
        printf("%s ", *arg);
        arg++;
    }
    printf("\n");
}

void cd(char **args) {
    char *path;
    int i = 1;
    if (args[i][0] == '\"') {
        // Concatenate all substrings enclosed by quotes
        path = strdup(args[i] + 1); // Skip the opening quote
        while (args[++i] != NULL) {
            if (args[i][strlen(args[i]) - 1] == '\"') {
                // Remove the double quotes from the end of the concatenated path
                args[i][strlen(args[i]) - 1] = '\0';
                strcat(path, " ");
                strcat(path, args[i]);
                break;
            }
            strcat(path, " ");
            strcat(path, args[i]);
        }
    } else {
        // If the argument doesn't start with a double quote, use it as is
        path = strdup(args[i]);
    }

    // Check if the last character is a closing quote
    if (path[strlen(path) - 1] == '\"') {
        // Remove the closing quote
        path[strlen(path) - 1] = '\0';
    }

    if (chdir(path) != 0)
        printf("-myShell: cd: %s: No such file or directory\n", path);
    free(path);
}

void cp(char **args) {
    char ch;
    FILE *src, *des;
    char *source = NULL, *destination = NULL;
    int i = 1;

    // Concatenate source path
    if (args[i][0] == '\"') {
        source = strdup(args[i] + 1); // Skip the opening quote
        while (args[++i] != NULL) {
            if (args[i][strlen(args[i]) - 1] == '\"') {
                args[i][strlen(args[i]) - 1] = '\0'; // Remove the closing quote
                strcat(source, " ");
                strcat(source, args[i]);
                break;
            }
            strcat(source, " ");
            strcat(source, args[i]);
        }
    } else {
        source = strdup(args[i]);
    }

    // Concatenate destination path
    i++;
    if (args[i] == NULL) {
        fprintf(stderr, "Error: Destination path not provided.\n");
        free(source);
        return;
    }

    if (args[i][0] == '\"') {
        destination = strdup(args[i] + 1); // Skip the opening quote
        while (args[++i] != NULL) {
            if (args[i][strlen(args[i]) - 1] == '\"') {
                args[i][strlen(args[i]) - 1] = '\0'; // Remove the closing quote
                strcat(destination, " ");
                strcat(destination, args[i]);
                break;
            }
            strcat(destination, " ");
            strcat(destination, args[i]);
        }
    } else {
        destination = strdup(args[i]);
    }

    // Open source file
    if ((src = fopen(source, "rb")) == NULL) {
        fprintf(stderr, "Error opening source file: %s\n", source);
        free(source);
        free(destination);
        return;
    }

    // Open destination file
    if ((des = fopen(destination, "wb")) == NULL) {
        fprintf(stderr, "Error opening destination file: %s\n", destination);
        fclose(src); // Close source file before returning
        free(source);
        free(destination);
        return;
    }

    // Copy content
    while ((ch = fgetc(src)) != EOF) {
        fputc(ch, des);
    }

    // Close files
    fclose(src);
    fclose(des);
    free(source);
    free(destination);
}

void get_dir()
{
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir("./")) == NULL)
    {
        /* could not open directory */
        perror("");
        return;
    }
    /* print all the files and directories within directory */
    while ((ent = readdir(dir)) != NULL)
    {
        printf("%s ", ent->d_name);
    }
    puts("");
    closedir(dir);
}

void delete(char **arg) {
    char *path = NULL;
    int i = 1;

    // Check if the argument is provided
    if (arg[1] == NULL) {
        fprintf(stderr, "Error: Missing argument.\n");
        return;
    }

    // Concatenate path
    if (arg[i][0] == '\"') {
        // Concatenate all substrings enclosed by quotes
        path = strdup(arg[i] + 1); // Skip the opening quote
        while (arg[++i] != NULL) {
            if (arg[i][strlen(arg[i]) - 1] == '\"') {
                // Remove the double quotes from the end of the concatenated path
                arg[i][strlen(arg[i]) - 1] = '\0';
                strcat(path, " ");
                strcat(path, arg[i]);
                break;
            }
            strcat(path, " ");
            strcat(path, arg[i]);
        }
    } else {
        // If the argument doesn't start with a double quote, use it as is
        path = strdup(arg[i]);
    }

    // Check if the last character is a comma
    if (path[strlen(path) - 1] == ',') {
        // Remove the comma
        path[strlen(path) - 1] = '\0';
    }

    // Delete file
    if (unlink(path) == 0) {
        printf("File deleted successfully: %s\n", path);
    } else {
        printf("-myShell: unlink: %s: No such file or directory\n", path);
    }

    free(path);
}

void systemCall(char **arg)
{
    pid_t pid = fork();
    if (pid == -1)
    {
        printf("fork err\n");
        return;
    }
    if (pid == 0 && execvp(arg[0], arg) == -1)
        exit(1);
}
void mypipe(char **argv1, char **argv2)
{
    int fildes[2];
    if (fork() == 0)
    {
        pipe(fildes);
        if (fork() == 0)
        {
            /* first component of command line */
            close(STDOUT_FILENO);
            dup(fildes[1]);
            close(fildes[1]);
            close(fildes[0]);
            /* stdout now goes to pipe */
            /* child process does command */
            execvp(argv1[0], argv1);
        }
        /* 2nd command component of command line */
        close(STDIN_FILENO);
        dup(fildes[0]);
        close(fildes[0]);
        close(fildes[1]);
        /* standard input now comes from pipe */
        execvp(argv2[0], argv2);
    }
}