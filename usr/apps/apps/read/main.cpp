#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (argc >= 2)
    {
        if (strcmp(argv[1], "-h") == 0)
        {
            printf("Help: read <file-name>\n");
            printf("Options: \n"
                    " -h help\n"
                    " -v version info");
            return 0;
        }
        else if (strcmp(argv[1], "-v") == 0)
        {
            printf("Version: 0.2");
            return 0;
        }
    }

    if (argc < 2)
    {
        printf("Help: read <file-name>\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");

    if (file == NULL)
    {
        printf("File is null.\n");
    }
    else
    {
        char *buf = (char *)malloc(100);
        memset(buf, 0, 100);

        printf("Name: %s\n", file->node->name);
        printf("Size: %d bytes\n", file->node->size);

        int ret = fread(buf, file->node->size, 1, file);

        if (ret) {
            printf("Could not read from file %s\n", file->node->path);
            fclose(file);
            return 0;
        }

        bool issp = true;

        if (!buf)
            strcpy(buf, "(null)");

        for (int z = 0; z < strlen(buf); z++)
            if (buf[z] != ' ')
                issp = false;
        printf("Read: %s\n", (char *)(issp ? "(space)" : buf));

        free(buf);
    }

    fclose(file);

    return 0;
}
