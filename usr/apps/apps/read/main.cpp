#include <stdio.h>
#include <file.h>

int main(int argc, char **argv)
{
    if (argc <= 1)
    {
        printf("Help: read <file-name>\n");
        return 1;
    }

    FILE *file = fopen(argv[1]);

    if (file == NULL)
    {
        printf("File is null.\n");
    }
    else
    {
        printf("Name: %s\n", file->node->name);
        printf("Size: %d bytes\n", file->node->size);
        printf("Read: ");
        printf(fread("", 0, file->node->size, file));
    }

    fclose(file);

    return 0;
}
