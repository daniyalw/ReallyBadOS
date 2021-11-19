#pragma once
#include "fs.h"

File open_file(char * fname)
{
    // loop through files
    for (int z = 0; z < sizeof(fs.files); z++)
    {
        if ((fs.files[z].name == fname) && (fs.files[z].permission == P3))
        {
            return fs.files[z];
        }
    }

    // in case there's no file like what is being requested
    File f;
    f.name = "NULL";
    f.data = "NULL";
    f.id = 0;
    f.permission = P3;
    return f;
}

File kopen_file(char * fname)
{
    // loop through files
    for (int z = 0; z < sizeof(fs.files); z++)
    {
        if (fs.files[z].name == fname)
        {
            return fs.files[z];
        }
    }

    // in case there's no file like what is being requested
    File f;
    f.name = "NULL";
    f.data = "NULL";
    f.id = 0;
    f.permission = P3;
    return f;
}

void write_file(char * fname, char * newdata)
{
    // check if there is file with same name
    File f = open_file(fname);

    // there is file by same name
    if (f.name != "NULL")
    {
        if (f.permission == P3)
        {
            f.data = newdata;
            fs.files[f.id] = f;
        }
    }
    // we simply create a new file if there is no file
    else
    {
        create_file(fname, newdata);
    }
}

void kwrite_file(char * fname, char * newdata)
{
    // check if there is file with same name
    File f = open_file(fname);

    // there is file by same name
    if (f.name != "NULL")
    {
        f.data = newdata;
        fs.files[f.id] = f;
    }
    // we simply create a new file if there is no file
    else
    {
        create_file(fname, newdata);
    }
}

void create_file(char * fname, char * fdata)
{
    // check if there is a file by the same name
    File f = open_file(fname);

    // if there is, return
    if (f.name != "NULL")
        return;

    // by here it is determined there is no such file
    f.name = fname;
    f.data = fdata;
    f.id = fs.file_number;
    f.permission = P3;
    fs.files[fs.file_number] = f;
    fs.file_number++;
}

void kcreate_file(char * fname, char * fdata)
{
    // check if there is a file by the same name
    File f = open_file(fname);

    // if there is, return
    if (f.name != "NULL")
        return;

    // by here it is determined there is no such file
    f.name = fname;
    f.data = fdata;
    f.id = fs.file_number;
    f.permission = P0;
    fs.files[fs.file_number] = f;
    fs.file_number++;
}

void rename_file(char * fname, char * newname)
{
    File f = open_file(fname);

    if (f.name == "NULL" || f.permission != P3)
        return;

    f.name = newname;

    fs.files[f.id] = f;
}

void krename_file(char * fname, char * newname)
{
    File f = open_file(fname);

    if (f.name == "NULL")
        return;

    f.name = newname;

    fs.files[f.id] = f;
}

void kappend_file(char * fname, char * newdata)
{
    File f = open_file(fname);

    if (f.name == "NULL")
        return;

    char * nd = append(f.data, newdata);
    write_file(fname, nd);
}

void append_file(char * fname, char * newdata)
{
    File f = open_file(fname);

    if (f.name == "NULL" || f.permission != P3)
        return;

    char *nd = append(f.data, newdata);
    int nd_size = 0;

    write_file(fname, nd);
}

void log(char * data)
{
    kwrite_file(".log", data);
}

void log_start()
{
    kcreate_file(".log", "");
}
