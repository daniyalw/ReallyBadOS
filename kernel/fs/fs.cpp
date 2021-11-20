#pragma once
#include "fs.h"
#include "../sys/log/log.cpp"

File send_null_file()
{
    File f;
    f.name = "NULL";
    f.data = "NULL";
    return f;
}

bool is_not_null_file(char * fname)
{
    if (fname == "NULL" || fname == "" || fname == nullptr)
        return false;

    return true;
}

File open_file(char * fname)
{
    if (!is_not_null_file(fname))
        return send_null_file();

    // loop through files
    for (int z = 0; z < file_limit; z++)
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
    if (!is_not_null_file(fname))
        return send_null_file();

    // loop through files
    for (int z = 0; z < file_limit; z++)
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
    if (!is_not_null_file(fname))
        return;

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
    if (!is_not_null_file(fname))
        return;

    // check if there is file with same name
    File f = kopen_file(fname);

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

void delete_file(char * fname)
{
    if (!is_not_null_file(fname))
        return;

    File file = open_file(fname);

    File f;
    f.name = "NULL";

    if (file.name != "NULL" && file.permission == P3)
    {
        fs.files[file.id] = f;
    }
}

void kdelete_file(char * fname)
{
    if (!is_not_null_file(fname))
        return;

    File file = kopen_file(fname);

    File f;
    f.name = "NULL";

    if (file.name != "NULL")
    {
        fs.files[file.id] = f;
    }
}

void create_file(char * fname, char * fdata)
{
    if (!is_not_null_file(fname))
        return;

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

void create_file(char * fname)
{
    if (!is_not_null_file(fname))
        return;

    // check if there is a file by the same name
    File f = open_file(fname);

    // if there is, return
    if (f.name != "NULL")
        return;

    // by here it is determined there is no such file
    f.name = fname;
    f.data = "";
    f.id = fs.file_number;
    f.permission = P3;
    fs.files[fs.file_number] = f;
    fs.file_number++;
}

void kcreate_file(char * fname, char * fdata)
{
    if (!is_not_null_file(fname))
        return;

    // check if there is a file by the same name
    File f = kopen_file(fname);

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

void kcreate_file(char * fname)
{
    if (!is_not_null_file(fname))
        return;

    // check if there is a file by the same name
    File f = kopen_file(fname);

    // if there is, return
    if (f.name != "NULL")
        return;

    // by here it is determined there is no such file
    f.name = fname;
    f.data = "";
    f.id = fs.file_number;
    f.permission = P0;
    fs.files[fs.file_number] = f;
    fs.file_number++;
}

void rename_file(char * fname, char * newname)
{
    if (!is_not_null_file(fname))
        return;

    File f = open_file(fname);

    if (f.name == "NULL" || f.permission != P3)
        return;

    f.name = newname;

    fs.files[f.id] = f;
}

void krename_file(char * fname, char * newname)
{
    if (!is_not_null_file(fname))
        return;

    File f = kopen_file(fname);

    if (f.name == "NULL")
        return;

    f.name = newname;

    fs.files[f.id] = f;
}

void kappend_file(char * fname, char * newdata)
{
    if (!is_not_null_file(fname))
        return;

    File f = kopen_file(fname);

    if (f.name == "NULL")
        return;

    char * nd = append(f.data, newdata);
    write_file(fname, nd);
}

void append_file(char * fname, char * newdata)
{
    if (!is_not_null_file(fname))
        return;

    File f = open_file(fname);

    if (f.name == "NULL" || f.permission != P3)
        return;

    char *nd = append(f.data, newdata);
    int nd_size = 0;

    write_file(fname, nd);
}

void init_fs()
{
    system_log("File system initialized.\n");
    for (int z = 0; z < file_limit; z++)
    {
        fs.files[z].name = "NULL";
    }
}

int ls_file()
{
    int count = 0;

    for (int z = 0; z < file_limit; z++)
    {
        if (fs.files[z].name != "NULL")
            count++;
    }

    return count;
}
