#include <stdio.h>
#include <stdlib.h>

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
        char buf[100];
        memset(buf, 0, 100);

        printf("Name: %s\n", file->node->name);
        printf("Size: %d bytes\n", file->node->size);

        fread(buf, 0, file->node->size, file);
        printf("Read: ");
        printf(buf);
    }

    fclose(file);

    return 0;
}
