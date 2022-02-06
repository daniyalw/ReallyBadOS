#pragma once

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
int run_command(char * command);
int execute_script(char *text);
void shell();
