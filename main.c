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
#include <pwd.h>
#include <grp.h>
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
            printf("%*s%s", numspace, "", dirptr->d_name);
            struct stat directory;
            stat(path, &directory);
            printf("%2ld", directory.st_nlink);  
            printf(" %s %s ", getpwuid(directory.st_uid)->pw_name, getgrgid(directory.st_gid)->gr_name);
            printf("d");
            printf((directory.st_mode & S_IRUSR) ? "r" : "-");
            printf((directory.st_mode & S_IWUSR) ? "w" : "-");
            printf((directory.st_mode & S_IXUSR) ? "x" : "-");
            printf((directory.st_mode & S_IRGRP) ? "r" : "-");
            printf((directory.st_mode & S_IWGRP) ? "w" : "-");
            printf((directory.st_mode & S_IXGRP) ? "x" : "-");
            printf((directory.st_mode & S_IROTH) ? "r" : "-");
            printf((directory.st_mode & S_IWOTH) ? "w" : "-");
            printf((directory.st_mode & S_IXOTH) ? "x" : "-");
            printf("\n");
            listdir(path, numspace + 2, size);
        } else {
            struct stat file;
            // printf("%s\n", name);
            char path[512];
            snprintf(path, sizeof(path), "%s/%s", name, dirptr->d_name);
            stat(path, &file);
            // if(errno) {
            //     printf("error: %s\n",strerror(errno));
            // }
            // printf("\t%s  —file\n", dirptr->d_name);
            // printf("%ld\n", file.st_size);
            printf("%*s%s", numspace, "", dirptr->d_name);
            printf("%2ld", file.st_nlink);  
            printf(" %s %s ", getpwuid(file.st_uid)->pw_name, getgrgid(file.st_gid)->gr_name);
            printf("-");
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
            printf(" | file size: %d bytes", fileSize);
            size += fileSize;
            // int fileSizeGB = fileSize / 1073741824;
            // fileSize %= 1073741824;
            // int fileSizeMB = fileSize / 1048576;
            // fileSize %= 1048576;
            // int fileSizeKB = fileSize / 1024;
            // fileSize %= 1024;
            // int fileSizeB = fileSize;
            // printf(" | file size: %d GB, %d MB, %d KB, %d B", fileSizeGB, fileSizeMB, fileSizeKB, fileSizeB);
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
    char dir[20];
    DIR * dum = opendir(dir);
    struct stat sb;
    printf("Please input the directory to scan: ");
    fgets(dir, sizeof(dir), stdin);
    
    int i;
    for(i = 0; i < sizeof(dir); i++) {
        if(dir[i] == '\n') {
            dir[i] = '\0';
        }
    }
    while(!(stat(dir, &sb) == 0 && S_ISDIR(sb.st_mode))) {
        printf("Cannot find given directory %s ;(... please try again.\n", dir);
        printf("Please input the directory to scan: ");
        fgets(dir, sizeof(dir), stdin);
        for(i = 0; i < sizeof(dir); i++) {
            if(dir[i] == '\n') {
                dir[i] = '\0';
            }
        }   
    }
    int fileSize = listdir(dir, 0, 0);
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