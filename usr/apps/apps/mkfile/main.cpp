#include <filesystem.h>
#include <stdio.h>

void display_help()
{
    printf("mkfile <new_file_name> <folder_name>");
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        display_help();
        return 1;
    }

    int res = Filesystem::file(argv[1], argv[2]);

    if (!res)
    {
        text::write("Failed to create file %s!", argv[1]);
    }
    else
    {
        text::write("Successfully created file %s!", argv[1]);
    }

    return res;
}
