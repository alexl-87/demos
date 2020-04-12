#include "DirWalk.h"
#include <dirent.h>
#include <string.h>

#define FILE 8
#define DIRECTORY 4

typedef struct dirent dirent;

DirWalkStatus getFiles(char* dirPath, onFileFoundedAction userFunc)
{
	DIR *d;
	dirent *dir;
	char nextPath[512];
	unsigned char* MD5;

	if(!dirPath)return Dir_ParamE;
	d = opendir(dirPath);

	if(!d) return dirE;

	while((dir = readdir(d)))
	{
		if(dir->d_name[0] != '.' && dir->d_type == FILE)
		{
			strcpy(nextPath, dirPath);
			strcat(nextPath, "/");
			strcat(nextPath, dir->d_name);
			userFunc(nextPath);
		}

		else if(dir->d_name[0] != '.' && dir->d_type == DIRECTORY)
		{
			strcpy(nextPath, dirPath);
			strcat(nextPath, "/");
			strcat(nextPath, dir->d_name);
			getFiles(nextPath, userFunc);
		}
	}

	closedir(d);
	return Dir_OK;
}