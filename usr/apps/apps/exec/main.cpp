// this app not completely working
#include <stdio.h>
#include <file.h>

void display_help()
{
    printf("Help: ");

    printf("exec <file-name>.r");
}

int main(int argc, char const *argv[])
{
    if (argc <= 1)
    {
        display_help();
        return 1;
    }

    FILE *file = fopen((char *)argv[1], "r");

    if (file == NULL)
    {
        printf("Cannot open file.\n");
        return 1;
    }

    int res = fexec(file->node->contents);

    fclose(file);

    return res;
}
