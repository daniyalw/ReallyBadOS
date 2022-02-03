#pragma once

void copy_file(char *orig_folder, char *orig_name, char *to_folder);
void permanent_delete_file(char *file_c);
void delete_folder(char *foldername);
void rename_file(char *folder_name, char *name, char *newname);
void rename_folder(char *foldername, char *newname);
void delete_file(char *folder_c, char *file_c);
void move_file(char *old_folder, char *old_name, char *to_folder, char *to_name);
