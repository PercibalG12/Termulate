#include <stdio.h>
#include <string.h>
#include <dirent.h>

#ifdef _WIN32
#include <direct.h>
#define getcwd _getcwd
#else
#include <unistd.h>
#endif

#include "commands.h"

#define MAX_PATH_LENGTH 1024
#define MAX_INPUT_LENGTH 1024

typedef enum
{
    CMD_UNKNOWN,
    CMD_EMPTY,
    CMD_LS,
    CMD_HELP,
    CMD_PWD,
    CMD_RENAME
} CommandType;

static void showPrompt(void (*appendOutput)(const char *))
{
    char cwd[MAX_PATH_LENGTH];
    char prompt[MAX_PATH_LENGTH + 5];

    if (getcwd(cwd, sizeof(cwd)) != NULL)
        snprintf(prompt, sizeof(prompt), "\n%s> ", cwd);
    else
        snprintf(prompt, sizeof(prompt), "\nTermulate> ");

    appendOutput(prompt);
}

static void listDirectory(void (*appendOutput)(const char *))
{
    DIR *dir = opendir(".");

    if (dir == NULL)
    {
        appendOutput("Unable to read directory.\n");
        return;
    }

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL)
    {
        appendOutput(entry->d_name);
        appendOutput("\n");
    }

    closedir(dir);
}

static void showCurrentDirectory(void (*appendOutput)(const char *))
{
    char cwd[MAX_PATH_LENGTH];

    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        appendOutput(cwd);
        appendOutput("\n");
    }
    else
    {
        appendOutput("Unable to get current directory.\n");
    }
}

static void renameFile(const char *oldName, const char *newName, void (*appendOutput)(const char *))
{
    if (oldName == NULL || newName == NULL)
    {
        appendOutput("Usage: mv oldname newname\n");
        appendOutput("   or: ren oldname newname\n");
        return;
    }

    if (rename(oldName, newName) == 0)
    {
        appendOutput("File renamed/moved successfully.\n");
    }
    else
    {
        appendOutput("Rename/move operation failed.\n");
    }
}

static void showHelp(void (*appendOutput)(const char *))
{
    appendOutput("Available commands:\n");
    appendOutput("--------------------------------------\n");
    appendOutput("ls / dir              List files\n");
    appendOutput("help                  Show help menu\n");
    appendOutput("pwd / cd              Print current directory\n");
    appendOutput("mv old new            Rename or move a file\n");
    appendOutput("ren old new           Rename or move a file\n");
    appendOutput("clear                 Clear screen\n");
    appendOutput("exit                  Close Termulate\n");
    appendOutput("--------------------------------------\n");
}

static CommandType getCommandType(const char *command)
{
    if (command == NULL || strcmp(command, "") == 0)
        return CMD_EMPTY;

    if (strcmp(command, "ls") == 0 || strcmp(command, "dir") == 0)
        return CMD_LS;

    if (strcmp(command, "help") == 0)
        return CMD_HELP;

    if (strcmp(command, "pwd") == 0 || strcmp(command, "cd") == 0)
        return CMD_PWD;

    if (strcmp(command, "mv") == 0 || strcmp(command, "ren") == 0)
        return CMD_RENAME;

    return CMD_UNKNOWN;
}

void processCommand(const char *input, void (*appendOutput)(const char *))
{
    char copy[MAX_INPUT_LENGTH];

    strncpy(copy, input, sizeof(copy) - 1);
    copy[sizeof(copy) - 1] = '\0';

    char *command = strtok(copy, " ");
    char *arg1 = strtok(NULL, " ");
    char *arg2 = strtok(NULL, " ");

    switch (getCommandType(command))
    {
        case CMD_EMPTY:
            break;

        case CMD_LS:
            listDirectory(appendOutput);
            break;

        case CMD_HELP:
            showHelp(appendOutput);
            break;

        case CMD_PWD:
            showCurrentDirectory(appendOutput);
            break;

        case CMD_RENAME:
            renameFile(arg1, arg2, appendOutput);
            break;

        case CMD_UNKNOWN:
        default:
            appendOutput("Command not recognized: ");
            appendOutput(input);
            appendOutput("\n");
            break;
    }

    showPrompt(appendOutput);
}