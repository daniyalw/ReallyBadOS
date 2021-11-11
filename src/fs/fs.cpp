#pragma once
#include "../include/string.cpp"
#include "fs.h"

file_t * create_file(char * name, char * data)
{
	for (int z = 0; z < file_count; z++)
		if (files[z]->name == name)
			// here the name is the same so return
			return 0;

	file_t * file;
	file->name = name;
	file->id = file_count;
	file_count++;
	file->permission_level = R3;
	file->contents = data;
	save_file(file);
	return file;
}

file_t * kcreate_file(char * name, char * data)
{
	for (int z = 0; z < file_count; z++)
		if (files[z]->name == name)
			// here the name is the same so return
			return 0;

	file_t * file;
	file->name = name;
	file->id = file_count;
	file_count++;
	file->permission_level = R0;
	file->contents = data;
	save_file(file);
	return file;
}

void rename_file(char * name)
{
	file_t * file = get_file_from_name(name);

	if (file->permission_level < 3)
		return;
		
	file->name = name;
}

void krename_file(char * name)
{
	file_t * file = get_file_from_name(name);

	file->name = name;
}

void save_file(file_t * file)
{
	files[file->id] = file;
}

file_t * get_file_from_name(char * name)
{
	file_t * file;

	for (int z = 0; z < file_count; z++)
	{
		if (files[z]->name == name)
			return files[z];
	}
}

file_t * open_file(char * name)
{
	file_t * file = get_file_from_name(name);

	if (file->permission_level < 3)
	{
		file_t * f;
		f->name = "NULL";
		f->contents = "NULL";
		f->id = 1000000;
		f->permission_level = 3;
		return f;
	}

	return file;
}

file_t * kopen_file(char * name)
{
	file_t * file = get_file_from_name(name);
	return file;
}

void delete_file(char * name)
{
	file_t * file = get_file_from_name(name);

	if (file->permission_level < 3)
	{
		return;
	}

	files[file->id] = 0;
}

void kdelete_file(char * name)
{
	file_t * file = get_file_from_name(name);

	files[file->id] = 0;
}

void fs_init()
{
	for (int z = 0; z < 128; z++)
		files[z] = 0;
}
