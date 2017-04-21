#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <dlfcn.h>

#define RESTOFPASSWD "::::::\n"
#define TMPNAME "/tmp/lmspoof-"
#define ETCPASSWD "/etc/passwd"

FILE *(*fopen_in)(const char *path, const char *modes);
char *passwd_file;

void __attribute__ ((constructor)) init(void) {
    FILE *fp;
    int namelen;
    char *passwd_data;
    char *username;

    // find username
    username = getenv("LOGNAME");
    namelen = strlen(username);

    // create empty passwd entry using username
    passwd_data = (char*) malloc(namelen + sizeof(RESTOFPASSWD));
    strcpy(passwd_data, username);
    strcat(passwd_data, RESTOFPASSWD);

    // generate fake passwd filename
    passwd_file = (char*) malloc(namelen + sizeof(TMPNAME));
    strcpy(passwd_file, TMPNAME);
    strcat(passwd_file, username);

    // install the symbolic relocation
    fopen_in = dlsym(RTLD_NEXT, "fopen");

    // create temporary passwd file with correct username
    fp = fopen_in(passwd_file, "w+");
    fwrite(passwd_data, strlen(passwd_data), sizeof(char), fp);
    fclose(fp);
}

FILE *fopen(const char *path, const char *modes) {
    if ((strlen(path) == sizeof(ETCPASSWD)) && (!strncmp(ETCPASSWD, path, strlen(path)))) {
        return fopen_in(passwd_file, modes);
    } else
        return fopen_in(path, modes);
}


