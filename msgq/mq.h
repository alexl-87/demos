#ifndef MQ_H
#define MQ_H

#define MSGQ_WRITER 840
#define MSGQ_READER 841
#define MESSAGE_SIZE 4096

#pragma pack(push, 1)
struct mq_reg
{
	const char* data;
	int size;
};
#pragma pack(pop)

#endif