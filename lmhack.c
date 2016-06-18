#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <dlfcn.h>

#include "params.h"


extern FILE *fopen (const char *, const char *);
FILE *fopen(const char *path, const char *modes) {
    FILE *fp;
    char passwd_file[30] = PATH_TO_FAKE;
    char passwd_data[] = FAKE_USERDATA;

    FILE *(*fopen_in)(const char *path, const char *modes) = NULL;

    //printf("%s hooked\n", __func__);
    
    if (!fopen_in) {
        //printf("lmhack.so...first time\n");
        fopen_in = dlsym(RTLD_NEXT, "fopen");
    }

    if ((strlen(path) >= 11) && (!strncmp("/etc/passwd", path, strlen(path)))) {
        // create temporary passwd file with correct username
        fp = fopen_in(passwd_file, "w+");
        fwrite(passwd_data, strlen(passwd_data), sizeof(char), fp);
        fclose(fp);
        return fopen_in(passwd_file, modes);
    } else
        return fopen_in(path, modes);
}


