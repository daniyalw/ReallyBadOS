#include <kernel/kernel.h>
#include <filesystem/disk.h>
#include <stdio.h>

void login() {
    int fpswd = 0x900;
    int fuid = 0x800;
    int funame = 0x700;
    char *username;
    char *password;

    printf("Username: ");
    username = scanf();
    printf("Password: ");
    password = scanf();

    rbfs::init();

    char uname_auth[100];
    memset(uname_auth, 0, 100);
    sprintf(uname_auth, "/users/%s/auth", username);

    printf("UNAME: %s\n", uname_auth);

    auto file = rbfs::open(uname_auth);

    if (file) {
        char pswd[100];
        memset(pswd, 0, 100);

        int ret = rbfs::read(pswd, 0, 100, file);

        if (ret) {
            printf("Uh oh! Failed to read password file.\n");
            printf("Error code: 0x%x\n", fpswd);
            printf("Please try again.\n");
            login();
        }

        if (strcmp(password, pswd) == 0) {
            user = username;
            free(password);

            char uid[5];
            char uid_auth[100];
            memset(uid_auth, 0, 100);
            memset(uid, 0, 5);
            sprintf(uid_auth, "/users/%s/uid", username);

            auto ufile = rbfs::open(uid_auth);

            if (!ufile) {
                printf("Uh oh! Failed to find user ID.\n");
                printf("Error code: 0x%x\n", fuid);
                printf("Please try again.\n");
                login();
            }

            int r = rbfs::read(uid, 0, 5, ufile);

            if (r) {
                printf("Uh oh! Failed to find user ID.\n");
                printf("Error code: 0x%x\n", fuid);
                login();
            }

            current_login = atoi(uid);

            printf("Successfully logged in!\n");
        }
    } else {
        printf("Uh oh! There seems to be no username of %s.\n", username);
        printf("Error code: 0x%x\n", funame);
        printf("Please try again.\n");
        login();
    }
}
