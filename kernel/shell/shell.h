#pragma once

typedef struct
{
    char * commands;
} script_t;

int total_command_count = 20;
char *commands[] = {
    "cd",
    "Switch directory",
    "echo",
    "Print to the screen",
    "info",
    "Get system info",
    "time",
    "Get the time",
    "date",
    "Get the date",
    "ls",
    "List folders and files",
    "exit",
    "Shutdown",
    "shutdown",
    "Shutdown",
    "quit",
    "Shutdown",
    "help",
    "Get list of commands"
};

bool check_name(char * name, char * check_against);
void run_command(char * command);
void execute_script();
void shell();
