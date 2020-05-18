#include "mq.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
using namespace std;


int main(int argc, char const *argv[])
{
	int fd = open(argv[1], O_RDWR);
	if(fd == -1)
	{
		cerr << "error file name" << endl;
		return 1;
	}

	char* message = (char*)malloc(MESSAGE_SIZE);

	int ret = ioctl(fd, MSGQ_READER, message);
	cout<< "Ret val = " << ret << endl
		<<"Message: " << message << endl;


	return 0;
}