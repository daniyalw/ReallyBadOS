#include <string.h>
#include <stdio.h>
#include <filesystem.h>

void display_help()
{
    printf("Help: mkdir <new_dir> <existing_dir>");
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        display_help();
        return 1;
    }

    int result = mkdir(argv[1], argv[2]);

    if (result == 0)
    {
        printf("Directory successfully created!\n");
    }
    else
    {
        printf("Failed to create directory: %s\n", argv[1]);
    }

    return result;
}
