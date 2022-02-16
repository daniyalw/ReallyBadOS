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

        if (!file->node.size && file->read)
        {
            char *out = file->read(out);

            printf("Read: %s\n", out);
        }
        else
        {
            printf("Size: %d bytes\n", file->node.size);
            printf("Contents: \n%s", file->node.contents);
        }
    }

    fclose(file);

    return 0;
}
