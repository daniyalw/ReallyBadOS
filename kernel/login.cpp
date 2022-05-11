#include <kernel/kernel.h>
#include <filesystem/disk.h>
#include <stdio.h>

namespace auth {

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
            printf("Successfully logged in!\n");
            return;
        }
    } else {
        printf("Uh oh! There seems to be no username of %s.\n", username);
        printf("Error code: 0x%x\n", funame);
        printf("Please try again.\n");
        login();
    }
}

void signup() {
    char *username;
    char *password;
    char *check;

    printf("New username: ");
    username = scanf();
    printf("New password: ");
    password = scanf();
    printf("Re-enter password: ");
    check = scanf();

    rbfs::init();

    if (strcmp(password, check) == 0) {
        char uname_auth[100];
        memset(uname_auth, 0, 100);
        sprintf(uname_auth, "/users/%s/auth", username);

        auto file = rbfs::open(uname_auth);

        if (file) {
            free(password);
            free(check);

            printf("Uh oh! There already seems to be user %s.\n", username);

            free(username);
            signup();
        }

        char uname_auth_f[100];
        memset(uname_auth_f, 0, 100);
        sprintf(uname_auth_f, "/users/%s", username);

        auto u = strdup(uname_auth);

        rbfs::create_folder(uname_auth_f);
        int ret = rbfs::create_file(u, password);
        free(u);

        if (ret) {
            printf("Uh oh! Failed to create account.\n");
            free(check);
            free(username);
            free(password);
            signup();
        }

        user = username;

        free(check);
        free(password);

        printf("Successfully signed up!\n");
        return;
    } else {
        free(username);
        free(password);
        free(check);

        printf("Uh oh! Passwords do not match.\n");
        signup();
    }
}

void change_pswd() {
    char *npswd;
    char *check;
    char *username;

    printf("Username: ");
    username = scanf();
    printf("New password: ");
    npswd = scanf();
    printf("Re-enter new password: ");
    check = scanf();

    rbfs::init();

    if (strcmp(npswd, check) == 0) {
        char uname_auth[100];
        memset(uname_auth, 0, 100);
        sprintf(uname_auth, "/users/%s/auth", username);

        auto file = rbfs::open(uname_auth);

        if (!file) {
            free(npswd);
            free(check);

            printf("Uh oh! Failed to find user '%s'.\n", username);

            free(username);
            change_pswd();
        }

        rbfs::write(npswd, 0, 100, file);

        free(npswd);
        free(check);

        user = username;

        printf("Changed password!\n");
        return;
    } else {
        free(username);
        free(npswd);
        free(check);

        printf("Uh oh! Passwords do not match.\n");
        change_pswd();
    }
}

}
