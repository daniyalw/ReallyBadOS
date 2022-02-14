#pragma once

int get_slash_in_path(char *path);
char *parse_name_parent(char *path, char *out);
char *parse_current_name(char *path, char *out);
char *format_folder(char *path);
char *format_file(char *path);
