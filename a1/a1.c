#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

void list(char* path, int recursive, int filter_size, char** name_filter){
    DIR* dir=opendir(path);
    if(dir==NULL){
        fprintf(stderr,"ERROR\ninvalid path\n");
        exit(1);
    }
    struct dirent* entry=NULL;
    while((entry=readdir(dir))!=NULL){
        char file_path[300];
        snprintf(file_path, sizeof(file_path),"%s%s", path, entry->d_name);
    }
    closedir(dir);

}

int main(int argc, char **argv){

    char path[1024];
    int recursive=0;
    int filter_size=-1;
    char name_filter[256]="";

    if(argc >= 2){
        if(strcmp(argv[1], "variant") == 0){
            printf("45790\n");
            return 0;
        }

        for(int i=1;i<argc;i++){
            if(strcmp(argv[i],"list")==0){
                i++;
                if(i>=argc){
                    fprintf(stderr,"ERROR\nmissing path argument\n");
                    return 1;
                }
                strcpy(path, argv[i]);
            }
            if(strcmp(argv[i],"recursive")==0){
                recursive=1;
            }
            if(strstr(argv[i],"size smaller=")==argv[i]){
                filter_size=atoi(argv[i]+strlen("size smaller="));
            }
            if(strstr(argv[i],"name starts with=")==argv[i]){
                strcpy(name_filter, argv[i]+strlen("name starts with="));
            }
        }
    }else{
        fprintf(stderr, "ERROR\ntoo many arguments");
        return 1;
    }

    list(path, recursive, filter_size, name_filter);

    return 0;
}

