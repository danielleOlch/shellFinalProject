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
    *(str + index) = '\0'; // Null-terminate the string

    return str;
}
char **splitArgument(char *str) {
    char *subStr;
    int size = 2;
    int index = 0;
    subStr = strtok(str, " ");
    char **arguments = (char **)malloc(size * sizeof(char *));
    if (arguments == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    arguments[index] = subStr;
    while ((subStr = strtok(NULL, " ")) != NULL) {
        index++;
        if (index >= size - 1) { // Check if more memory is needed
            size *= 2; // Double the size
            arguments = (char **)realloc(arguments, size * sizeof(char *));
            if (arguments == NULL) {
                fprintf(stderr, "Memory reallocation failed\n");
                exit(EXIT_FAILURE);
            }
        }
        arguments[index] = subStr;
    }
    arguments[index + 1] = NULL; // Null-terminate the array

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
        bold();
        green();
        printf("%s@%s", username, hostname); // print username and computer name
        reset();
        printf(":");
        bold();
        blue();
        printf("~%s", location); // print current path
        reset();
        printf("$ ");
    } else {
        printf("Error getting username.\n");
    }
}

void logout(char *input)
{
    free(input);
    puts("log out");
    exit(EXIT_SUCCESS);
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


void cd(char **arg)
{
    if (strncmp(arg[1], "\"", 1) != 0 && arg[2] != NULL)
        puts("-myShell: cd: too many arguments");
    else if (strncmp(arg[1], "\"", 1) == 0)
    {
        // input =  cd "OneDrive - Ariel University"\0
        // [cd, "OneDrive, - , Ariel, University", NULL]
        if (chdir(arg[1]) != 0)
            printf("-myShell: cd: %s: No such file or directory\n", arg[1]);
    }
    if (chdir(arg[1]) != 0)
        printf("-myShell: cd: %s: No such file or directory\n", arg[1]);
}
void cp(char **arguments)
{
    char ch;
    FILE *src, *des;
    if ((src = fopen(arguments[1], "r")) == NULL)
    {
        puts("Erorr");
        return;
    }
    if ((des = fopen(arguments[2], "w")) == NULL)
    {
        puts("Erorr");
        fclose(src);
        return;
    }

    while ((ch = fgetc(src)) != EOF)
    {
        fputc(ch, des);
    }
    fclose(src);
    fclose(des);
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
void delete(char **arg)
{
    if (unlink(arg[1]) != 0)
        printf("-myShell: unlink: %s: No such file or directory\n", arg[1]);
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