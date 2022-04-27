#pragma once

char **history;
int hist_count = 0;

void hist_add(char *cmd);
char *hist_get(int pos);
void hist_traverse();
