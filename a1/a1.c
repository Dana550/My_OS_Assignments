#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>

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
            }
        }
    }
    closedir(dir);

}


void parse(char* path)
{
	int f=open(path,O_RDONLY);
	if(f==-1)
	{
		printf("ERROR\ncould not open file\n");
		return;
	}
	
	lseek(f,-2,SEEK_END);
	char magic[3];
	read(f,magic,2);
	magic[2]='\0';
	if(strcmp(magic,"HP")!=0)
	{
		printf("ERROR\nwrong magic\n");
		return;
	}
	
	lseek(f,-4,SEEK_END);
	
	short int header_size;
	read(f,&header_size,2);
	lseek(f,-header_size,SEEK_END);
	
	unsigned char version;
	read(f,&version,1);
	if(version<120 || version>220)
	{
		printf("ERROR\nwrong version\n");
		return;
	}
	
	unsigned char no_of_sections;
	read(f,&no_of_sections,1);
	if(no_of_sections<7 || no_of_sections>20)
	{
		printf("ERROR\nwrong sect_nr\n");
		return;
	}
	
	short int sect_type;
	for(int i=0;i<no_of_sections;i++)
	{	
		lseek(f,20,SEEK_CUR);
		read(f,&sect_type,2);
		if(sect_type!=12 && sect_type!=61 && sect_type!=49 && sect_type!=56 && sect_type!=63)
		{
			printf("ERROR\nwrong sect_types\n");
			return;
		}
		lseek(f,8,SEEK_CUR);
	}
	
	lseek(f,-header_size+2,SEEK_END);
	printf("SUCCESS\n");
	printf("version=%d\nnr_sections=%d\n",version,no_of_sections);
	for(int i=0;i<no_of_sections;i++)
	{	
		char sect_name[21];
		int sect_offset;
		int sect_size;
		read(f,sect_name,20);
		sect_name[20]='\0';
		read(f,&sect_type,2);
		read(f,&sect_offset,4);
		read(f,&sect_size,4);
		printf("section%d: %s %d %d\n",i+1,sect_name,sect_type,sect_size);
		
	}
	
	close(f);
}

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
		for (int i = 2; i < argc; i++)
            	{
                    if (strncmp(argv[i], "path=", 5) == 0)
                    {
                        path = argv[i] + 5;
                    }

           	 }
           	 parse(path);
           	
	
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

