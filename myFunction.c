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
        perror("-myShell: unlink");
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

void readFile(char **args) {
 if (args[1] == NULL) {
        printf("Usage: read <filename>\n");
        return;
    }

    // Combine arguments into a single path
    char path[1024] = "";
    for (int i = 1; args[i] != NULL; i++) {
        strcat(path, args[i]);
        strcat(path, " ");
    }

    // Remove trailing whitespace
    path[strlen(path) - 1] = '\0';

    // Remove quotes if present
    if (path[0] == '"' && path[strlen(path) - 1] == '"') {
        memmove(path, path + 1, strlen(path));
        path[strlen(path) - 1] = '\0';
    }

    FILE *file = fopen(path, "r");
    if (file == NULL) {
        perror("Error");
        return;
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }

    fclose(file);
}


void move(char **args) {
    if (args[1] == NULL || args[2] == NULL) {
        fprintf(stderr, "Usage: move <source_file> <destination_path>\n");
        return;
    }

    char *sourceFile = args[1];
    char *destinationPath = args[2];

    // Concatenate source file path
    char *sourcePath = strdup(sourceFile);
    for (int i = 2; args[i] != NULL; ++i) {
        sourcePath = realloc(sourcePath, strlen(sourcePath) + strlen(args[i]) + 2); // +2 for space and null terminator
        strcat(sourcePath, " ");
        strcat(sourcePath, args[i]);
    }

    // Concatenate destination path
    char *destination = strdup(destinationPath);
    for (int i = 3; args[i] != NULL; ++i) {
        destination = realloc(destination, strlen(destination) + strlen(args[i]) + 2); // +2 for space and null terminator
        strcat(destination, " ");
        strcat(destination, args[i]);
    }

    // Move file
    int status = rename(sourcePath, destination);

    if (status != 0) {
        fprintf(stderr, "Error: Failed to move file '%s' to '%s'.\n", sourcePath, destination);
    } else {
        printf("File '%s' moved to '%s' successfully.\n", sourcePath, destination);
    }

    free(sourcePath);
    free(destination);
}

void wordCount(char **args) {
    if (args[1] == NULL || args[2] == NULL) {
        fprintf(stderr, "Usage: wc <-l/-w> <filename>\n");
        return;
    }

    char *option = args[1];
    char *filename = args[2];
    char *path;

    if (filename[0] == '\"') {
        // Concatenate all substrings enclosed by quotes
        path = strdup(filename + 1); // Skip the opening quote
        for (int i = 3; args[i] != NULL; i++) {
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
        path = strdup(filename);
    }

    // Check if the last character is a closing quote
    if (path[strlen(path) - 1] == '\"') {
        // Remove the closing quote
        path[strlen(path) - 1] = '\0';
    }

    FILE *file = fopen(path, "r");

    if (file == NULL) {
        fprintf(stderr, "Error: Unable to open file '%s'\n", path);
        free(path);
        return;
    }

    if (strcmp(option, "-l") == 0) {
        // Count the number of lines
        int lineCount = 0;
        char line[1024]; // Assuming maximum line length of 1024 characters
        while (fgets(line, sizeof(line), file) != NULL) {
            lineCount++;
        }
        // Print the number of lines without appending the filename
        printf("Number of lines: %d\n", lineCount);
    
    } else if (strcmp(option, "-w") == 0) {
        // Count the number of words
        int wordCount = 0;
        int c;
        int lastChar = '\n'; // Previous character
        while ((c = fgetc(file)) != EOF) {
            if ((c == ' ' || c == '\n' || c == '\t') && lastChar != ' ' && lastChar != '\n' && lastChar != '\t') {
                wordCount++;
            }
            lastChar = c;
        }
        // If the file doesn't end with a whitespace, we need to increment word count by 1
        if (lastChar != ' ' && lastChar != '\n' && lastChar != '\t') {
            wordCount++;
        }
        printf("Number of words: %d\n", wordCount);
    } else {
        fprintf(stderr, "Error: Invalid option. Please use -l or -w.\n");
    }

    fclose(file);
    free(path);
}

void echoppend(char **args) {
    // Ensure there are at least 4 arguments (echoppend, string, ">>", path)
    if (args[0] == NULL || args[1] == NULL || args[2] == NULL || args[3] == NULL) {
        fprintf(stderr, "Invalid arguments\n");
        return;
    }

    // Open file in append mode (creates file if it doesn't exist)
    FILE *file = fopen(args[3], "a");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file\n");
        return;
    }

    // Write the string to the file
    fprintf(file, "%s\n", args[1]);

    // Close the file
    fclose(file);

    printf("String appended to file successfully.\n");
}