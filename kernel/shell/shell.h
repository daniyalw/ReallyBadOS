#pragma once

char last_command[100];

bool check_name(char * name, char * check_against);
int run_command(char * command);
int execute_script(char *text);
void shell();
