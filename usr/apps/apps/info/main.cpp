#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <path>\n", argv[0]);
        return 1;
    }

    char *path = argv[1];

    auto file = fopen(path, "r");

    if (!file) {
        perror("fopen");
        return 1;
    }

    auto node = file->node;

    /* path */
    printf("Path: %s\n", node->path);

    /* ownership */
    printf("ID: %d\n", node->id);
    printf("Parent ID: %d\n",  node->parent_id);

    /* time */
    auto time = node->ctime;

    char * weekdays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    char * months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

    printf("Creation time: %s, %s %d, ", weekdays[time.wd-1], months[time.m-1], time.d);

    if (time.min < 10)
        printf("%d:0%d %s\n", time.h, time.min, (char *)(time.pm ? "PM" : "AM"));
    else
        printf("%d:%d %s\n", time.h, time.min, (char *)(time.pm ? "PM" : "AM"));

    /* tags */
    int count = node->tag_count;
    printf("Tags: ");

    if (count == 0) {
        printf("(none)\n");
    }

    for (int z = 0; z < count; z++) {
        char tag[100];
        memset(tag, 0, 100);
        get_tag(tag, node->tags[z]);

        printf(tag);

        if (z != count - 1) {
            printf(", ");
        }
    }

    /* Size */
    printf("Size: %d\n", node->get_size(node));

    fclose(file);

    return 0;
}
