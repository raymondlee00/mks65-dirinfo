#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <dirent.h>
#define _GNU_SOURCE

long listdir(char *name, int numspace, long size) {
    struct dirent *dirptr;
    DIR * dummy = opendir(name);
    if (!dummy) {
        printf("Cannot find given directory %s ;(\n", name);
        return 0L;
    }
    // printf("Directories:\n");
    while((dirptr = readdir(dummy)) != NULL) {
        if(dirptr->d_type == DT_DIR && strcmp(dirptr->d_name, ".git") != 0) { //ignore .git dirs
            // while((dirptr = readdir(dirptr)) != NULL) {
            //     printf("\t%s\n", dirptr->d_name);
            // }
            char path[512];
            if (strcmp(dirptr->d_name, ".") == 0 || strcmp(dirptr->d_name, "..") == 0)
                continue;
            snprintf(path, sizeof(path), "%s/%s", name, dirptr->d_name);
            printf("%*s[%s]\n", numspace, "", dirptr->d_name);
            listdir(path, numspace + 2, size);
        } else {
            struct stat file;
            stat(name, &file);
            // if(errno) {
            //     printf("error: %s\n",strerror(errno));
            // }
            // printf("\t%s  —file\n", dirptr->d_name);
            // printf("%ld\n", file.st_size);
            printf("%*s-%s | file permissions: ", numspace, "", dirptr->d_name);
            printf((file.st_mode & S_IRUSR) ? "r" : "-");
            printf((file.st_mode & S_IWUSR) ? "w" : "-");
            printf((file.st_mode & S_IXUSR) ? "x" : "-");
            printf((file.st_mode & S_IRGRP) ? "r" : "-");
            printf((file.st_mode & S_IWGRP) ? "w" : "-");
            printf((file.st_mode & S_IXGRP) ? "x" : "-");
            printf((file.st_mode & S_IROTH) ? "r" : "-");
            printf((file.st_mode & S_IWOTH) ? "w" : "-");
            printf((file.st_mode & S_IXOTH) ? "x" : "-");
            int fileSize = file.st_size;
            printf(" | %d bytes | path: %s", fileSize, name);
            size += fileSize;
            int fileSizeGB = fileSize / 1073741824;
            fileSize %= 1073741824;
            int fileSizeMB = fileSize / 1048576;
            fileSize %= 1048576;
            int fileSizeKB = fileSize / 1024;
            fileSize %= 1024;
            int fileSizeB = fileSize;
            printf(" | file size: %d GB, %d MB, %d KB, %d B", fileSizeGB, fileSizeMB, fileSizeKB, fileSizeB);
            printf(" | time of last access: %s", ctime(&file.st_atime));
            // if(errno){
            //     printf("errno: %d error: %s\n", errno, strerror(errno));
            // }
            printf("\n");
        }
    }
    // if((dirptr = readdir(dummy)) == NULL){
    //     printf("Total Directory Size: %ld\n", size);
    // }
    closedir(dummy);
    return size;
}

int main() {
    int fileSize = listdir(".", 0, 0);
    int fileSizeGB = fileSize / 1073741824;
    fileSize %= 1073741824;
    int fileSizeMB = fileSize / 1048576;
    fileSize %= 1048576;
    int fileSizeKB = fileSize / 1024;
    fileSize %= 1024;
    int fileSizeB = fileSize;
    printf("Total Directory Size: %d GB, %d MB, %d KB, %d B\n", fileSizeGB, fileSizeMB, fileSizeKB, fileSizeB);
    return 0;
}