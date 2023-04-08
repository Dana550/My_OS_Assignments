#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>


void list(char* path,int filter_size, char* name_filter,int recursive)
{

    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[512];
    struct stat statbuf;

    dir = opendir(path);
    if(dir == NULL)
    {
        perror("Could not open directory");
        return;
    }
    while((entry = readdir(dir)) != NULL)
    {
        if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
            if(lstat(fullPath, &statbuf) == 0)
            {
                //if ( S_ISREG(statbuf.st_mode)  || S_ISDIR(statbuf.st_mode))
                //{

                if(filter_size == -1 && strcmp(name_filter,"")==0)
                {
                    printf("%s\n", fullPath);
                }
                else
                {
                	if(filter_size!=-1)
                	{
                		if(S_ISREG(statbuf.st_mode) && filter_size>statbuf.st_size)
                		{
                			if(strcmp(name_filter,"")==0)
                			{
                				printf("%s\n", fullPath);
                			}
                			else
                			{
                				char* p=strstr(entry->d_name,name_filter);
                				if(p!=NULL && strlen(p)==strlen(entry->d_name))
                					printf("%s\n", fullPath);
                			}
                		}
                	}
                	else
                	{
                		char* p=strstr(entry->d_name,name_filter);
                			if(p!=NULL && strlen(p)==strlen(entry->d_name))
                				printf("%s\n", fullPath);
                	}
                }
                
                if(S_ISDIR(statbuf.st_mode)&&recursive==1)
                {
                    list(fullPath,filter_size, name_filter,recursive);
                }

                //}
                //printf("%s\n", fullPath);

            }
        }
    }
    closedir(dir);

}

/*
void list_rec(char* path, int filter_size,char* name_filter)
{
    DIR* dir = opendir(path);
    struct stat statbuf;
    if (dir == NULL)
    {
        printf("ERROR\ninvalid directory path\n");
        exit(1);
    }
    struct dirent* entry = NULL;
    while(((entry=readdir(dir))!=NULL))
    {
        //checking curent and parent
        if(strcmp(entry->d_name,".")!=0||strcmp(entry->d_name,"..")!=0)
        {
            char subdir_path[512];
            printf("%s/%s\n",path,entry->d_name);
            snprintf(subdir_path,512, "%s/%s\n", path, entry->d_name);
            //if(stat(subdir_path,&stat_buf==-1)){
            //printf("ERROR\nfailed to get information\n");
            // }
            if(lstat(subdir_path, &statbuf) == 0)
            {
                printf("%s\n",subdir_path);
                if(S_ISDIR(statbuf.st_mode))
                {
                    list_rec(subdir_path,filter_size,name_filter);
                }
            }
        }
        //printf("%s/%s\n",path,entry->d_name);
        //snprintf(subdir_path, sizeof(subdir_path), "%s/%s\n", path, entry->d_name);
    }

    closedir(dir);
    //printf("SUCCESS\n");


}
*/

int main(int argc, char** argv)
{
    char* path =NULL ;
    int recursive = 0;
    int filter_size = -1;
    char name_filter[256] = "";

    if (argc >= 2)
    {
        if (strcmp(argv[1], "variant") == 0)
        {
            printf("45790\n");
            return 0;
        }
        if(strcmp(argv[1], "list")==0)
    	{
            for (int i = 2; i < argc; i++)
            {
                    if (strncmp(argv[i], "path=", 5) == 0)
                    {
                        path = argv[i] + 5;
                        //printf("AICII 22");
                    }

                    if (strncmp(argv[i], "recursive",9) == 0)
                    {
                        recursive = 1;

                    }
                    if (strncmp(argv[i], "size_smaller=",13) == 0)
                    {
                        filter_size = atoi(argv[i] + strlen("size_smaller="));
                    }
                    if (strncmp(argv[i], "name_starts_with=",17) == 0)
                    {
                        strcpy(name_filter, argv[i] + strlen("name_starts_with="));
                    }
            }
            printf("SUCCESS\n");
            list(path,filter_size, name_filter,recursive);
       	}
	if(strcmp(argv[1], "parse")==0)
	{
	
	}
	if(strcmp(argv[1], "extract")==0)
	{
	
	}
	if(strcmp(argv[1], "findall")==0)
	{
	
	}


    }
    else
    {
        printf("ERROR\ntoo few arguments\n");
        return 1;
    }


    return 0;
}

