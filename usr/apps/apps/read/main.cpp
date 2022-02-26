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

    if (file->null || file == NULL)
    {
        printf("File is null.\n");
    }
    else
    {
        printf("Name: %s\n", file->name);
        printf("Size: %d bytes\n", file->node.size);
        printf("Read: ");
        printf(file->read(""));
    }

    fclose(file);

    return 0;
}
