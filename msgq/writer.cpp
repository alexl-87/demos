#include "mq.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
using namespace std;

int main(int argc, char** argv)
{
	int fd = open(argv[1], O_RDWR);
	if(fd == -1)
	{
		cerr << "error file name" << endl;
		return 1;
	}
	cout << "Enter message" << endl;
	string message;
	cin >> message;

	struct mq_reg new_reg;
	new_reg.data = message.c_str();
	int len = message.length();
	new_reg.size = (len<MESSAGE_SIZE)?len:MESSAGE_SIZE;

	int ret = ioctl(fd, MSGQ_WRITER, &new_reg);
	
	cout<< "Return value from get ioctl: " << ret << endl;

	if(close(fd) < 0)
	{
		cerr << "error closing file" << endl;
		return 1;        
	}
     
	return 0;
}