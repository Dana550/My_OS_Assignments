#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

void list(char* path,int filter_size, char* name_filter,int recursive){
    DIR* dir = opendir(path);
    if (dir == NULL) {
        printf("ERROR\ninvalid directory path\n");
        exit(1);
    }
    struct dirent* entry = NULL;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {//if in director
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) { //if NOT curent or parent 
                char subdir_path[300];
                snprintf(subdir_path, sizeof(subdir_path), "%s/%s", path, entry->d_name);
                if (strstr(entry->d_name, name_filter) == entry->d_name) {
                    printf("%s/%s\n", path, entry->d_name);
                }
               if(recursive){
               	    list(path,filter_size,name_filter,recursive);
               }
            }
            
        }
         else if (entry->d_type == DT_REG) { //if in regular
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

void list_rec(char* path, int filter_size,char* name_filter){
    DIR* dir = opendir(path);
    char subdir_path[300];
    struct stat stat_buf;
    if (dir == NULL) {
        printf("ERROR\ninvalid directory path\n");
        exit(1);
    }
    struct dirent* entry = NULL;
    while(((entry=readdir(dir))!=NULL)){
    	//checking curent and parent
    	if(strcmp(entry->d_name,".")!=0||strcmp(entry->d_name,"..")!=0){
                snprintf(subdir_path, sizeof(subdir_path), "%s/%s", path, entry->d_name);
                if(stat(subdir_path,&stat_buf==-1)){
                	printf("ERROR\nfailed to get information\n");
                }
                if(S_ISDIR(stat_buf.st_mode)){
                	list_rec(subdir_path,filter_size,name_filter);
                }else if(S_ISREG(stat_buf.st_mode)){
                	if(filter_size>-1||strncmp(entry->d_name,name_filter,strlen(name_filter))!=0){
                		continue;
                	}
                }else{
                	continue;
                }
                
    	}
    	printf("%s/%s\n",path,entry->d_name);
    }
    
    closedir(dir);
    printf("SUCCESS\n");


}

int main(int argc, char** argv){
    char* path = NULL;
    int recursive = 0;
    int filter_size = -1;
    char name_filter[256] = "";

    if (argc >= 2) {
        if (strcmp(argv[1], "variant") == 0) {
            printf("45790\n");
            return 0;
        }

        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "list") == 0) {
                i++;
                if (i >= argc) {
                    printf("ERROR\nmissing path argument\n");
                    return 1;
                }
                if (strncmp(argv[i], "path=", 5) == 0) {
                    path = argv[i] + 5;
                }
                if (path == NULL) {
                    printf("ERROR\nmissing path argument\n");
                    return 1;
                }
            }
            if (strncmp(argv[i], "recursive",9) == 0) {
                recursive = 1;
            }
            if (strncmp(argv[i], "size_smaller=",13) == 0) {
                filter_size = atoi(argv[i] + strlen("size_smaller="));
            }
            if (strncmp(argv[i], "name_starts_with=",17) == 0) {
                strcpy(name_filter, argv[i] + strlen("name_starts_with="));
            }
        }
         	list(path,filter_size, name_filter,recursive);
	
    } else {
        printf("ERROR\ntoo few arguments\n");
        return 1;
    }

   
    return 0;
}
