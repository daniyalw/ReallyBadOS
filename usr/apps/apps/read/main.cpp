#include <stdio.h>
#include <file.h>

int main(int argc, char **argv)
{
    if (argc <= 1)
    {
        printf("Help: test <file-name>\n");
        return 1;
    }
    
    FILE *file = fopen(argv[1]);

    if (file->null)
    {
        printf("File is null.\n");
    }
    else
    {
        printf("Name: %s\n", file->name);
        printf("Size: %d\n", file->size);
        printf("Contents: %s", file->contents);
    }

    fclose(file);

    return 0;
}
