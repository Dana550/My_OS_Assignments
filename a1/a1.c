#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

void list(char* path, int filter_size, char* name_filter){
    DIR* dir = opendir(path);
    if (dir == NULL) {
        printf("ERROR\ninvalid directory path\n");
        exit(1);
    }
    struct dirent* entry = NULL;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                char subdir_path[300];
                snprintf(subdir_path, sizeof(subdir_path), "%s/%s", path, entry->d_name);
                if (strstr(entry->d_name, name_filter) == entry->d_name) {
                    printf("%s/%s\n", path, entry->d_name);
                }
            }
        } else if (entry->d_type == DT_REG) {
            if (filter_size == -1 || entry->d_reclen < filter_size) {
                if (strstr(entry->d_name, name_filter) == entry->d_name) {
                    printf("%s/%s\n", path, entry->d_name);
                }
            }
        }
    }
    closedir(dir);
    printf("SUCCESS\n");
}

int main(int argc, char** argv){
    char* path = NULL;
    int filter_size = -1;
    char name_filter[256] = "";

    if (argc >= 2) { //are enough arguments
        if (strcmp(argv[1], "variant") == 0) {
            printf("45790\n");
            return 0;
        }

        for (int i = 1; i < argc; i++) { 
            if (strcmp(argv[i], "list") == 0) { //the argument calls list function
                i++;
                if (i >= argc) {
                    printf("ERROR\nmissing path argument\n");
                    return 1;
                }
                
                if (strncmp(argv[i], "path=", 5) == 0) { //the argument starts with "path="
                //the path will be the characters after 5
                    path = argv[i] + 5; 
                }
                if (path == NULL) {
                    printf("ERROR\nmissing path argument\n");
                    return 1;
                }
            }

    	    list(path, filter_size, name_filter);
     }
    
    } else {
        printf("ERROR\ntoo few arguments\n");
        return 1;
    }

 

    return 0;
}
