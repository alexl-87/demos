#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/epoll.h>
#include <signal.h>

const int port=5050;
const int buffer_size=4096;
const int filename_len=128;
const int num_of_events=100;
const int max_message=300;

int loop = 1;

typedef struct _response
{
	char* buffer;
	char* filename;
	unsigned int index;
	unsigned int size;

} response;

response* buffer_manager;

void ERR(int n, const char* msg);
void check_not_null(void* p, const char* msg);

void init_buffer_manager();
int message_has_ended(int fd);
int socket_init();
int epoll_init(int socket_fd);
void read_request(int fd, int epoll_fd);
void handle_response(FILE* file, int* fd, int* file_len, char* file_content);
void get_file_name(char* buffer, char* filename);
int accept_request(int socket_fd, int epoll_fd);
int get_file_len(FILE* file);

void SIGINT_handler(int signal) {
	printf("\nSignal %d accepted\nNothing to do with this yet\n", signal);
	loop = 0;
}

int main(int argc, char const *argv[])
{
	init_buffer_manager();
	if(signal(SIGINT, SIGINT_handler) == SIG_ERR) {
		printf("can't catch SIGINT\n");
		exit(1);
	}

	int socket_fd=socket_init();
	int epoll_fd=epoll_init(socket_fd);

	while(loop) {
		struct epoll_event events_q[num_of_events];
		int events_counter = epoll_wait(epoll_fd, events_q, num_of_events, -1);
		ERR(events_counter, "Failed epoll_wait()");
		for (int i=0;i<events_counter;i++) {
			int fd = events_q[i].data.fd;
			int events=events_q[i].events;
			if(fd == socket_fd) {	
				accept_request(socket_fd, epoll_fd);
			} else {
				if(events & EPOLLIN) {
					read_request(fd, epoll_fd);
				}

				if(events & EPOLLOUT)
				{
					const char* filename=buffer_manager[i].filename;
					FILE* file = fopen(filename, "r");

					if (file)
					{
						int file_len = get_file_len(file);
						file_content = malloc(file_len);
						buffer_manager[fd] = (response*) malloc(sizeof(response));
						if (!file_content || !buffer_manager[fd])
						{
							printf("malloc error\n");
							free(file_content);
							free(buffer_manager[fd]);
						}
						else
						{
							buffer_manager[fd]->m_index = 0;
							buffer_manager[fd]->m_buffer = file_content;
							buffer_manager[fd]->m_buffsize = file_len;
							handle_response(file, &fd, &file_len, file_content);
						}
						
					}

					struct epoll_event delete_event;
					delete_event.data.fd = fd;
					ERR(epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &delete_event),  "Failed epoll_ctl()");
				}
			}
		}
	}

	ERR(close(socket_fd), "Failed close()");
	ERR(close(epoll_fd), "Failed close()");

	return 0;
}

void ERR(int errcode, const char* msg)
{
	if (errcode==-1) {
		perror(NULL);
		fprintf(stderr, "%s\n", msg);
		exit(1);
	}
}

void check_not_null(void* p, const char* msg) {
	if(p==NULL) {
		fprintf(stderr, "%s\n", msg);
		exit(1);
	}
}

int socket_init()
{
	int socket_fd;
	ERR(socket_fd = socket(AF_INET, SOCK_STREAM, 0), "Failed socket()");
	struct sockaddr_in serv_addr;
	bzero((void*) serv_addr, sizeof(struct sockaddr_in));
	serv_addr->sin_family = AF_INET;
	serv_addr->sin_addr.s_addr= htonl(INADDR_ANY);
	serv_addr->sin_port = htons(port);

	ERR(bind(socket_fd, (struct sockaddr*) serv_addr, sizeof(*serv_addr)), "Failed bind()");

	ERR(listen(socket_fd, 5), "Failed listen()");
	return socket_fd;
}

int epoll_init(int socket_fd, struct epoll_event* event)
{
	int epoll_fd = epoll_create1(0);
	ERR(epoll_fd, "Failed epoll_create1()");
	struct epoll_event event;
	event->events = EPOLLIN;
	event->data.fd = socket_fd;
	ERR(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, event), "Failed epoll_ctl()");
	return epoll_fd;
}

const char* eom="\r\n\r\n";
const int eom_len=4;

int message_has_ended(int fd) {
	const char* buffer=buffer_manager[fd].buffer;
	const int index=buffer_manager[fd].index;
	if(size<eom_len) {
		return 0;
	} else {
		return memcmp(eom, buffer+(index-eom_len), eom_len)==0;
	}
}

void read_request(int fd, int epoll_fd) {
	ERR(read(
		fd,
		buffer_manager[fd].buffer+index,
		buffer_manager[fd].size-buffer_manager[fd].index,
	), "Failed read()");
	if(message_has_ended(fd)) {
		struct epoll_event updated_event;
		updated_event.events = EPOLLOUT;
		updated_event.data.fd =fd;
		ERR(epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &updated_event),  "Failed epoll_ctl()");
	}
}

void handle_response(FILE* file, int* fd, int* file_len, char* file_content)
{
	int retval = fread((void*)file_content, 1, *file_len, file);//change to read()
	ERR(retval, "Failed fread()");
	retval = write(*fd, file_content, strlen(file_content));
	ERR(retval, "Faied write()");
	free(file_content);
	fclose(file);
}

int accept_request(int socket_fd, int epoll_fd)
{
	struct sockaddr_in client_addr;
	unsigned int len = sizeof(client_addr);
	int accept_fd = accept(socket_fd, (struct sockaddr*) client_addr, &len);
	ERR(accept_fd, "Failed accept()");
	struct epoll_event new_event;
	new_event.events = EPOLLIN;
	new_event.data.fd = accept_fd;
	ERR(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, accept_fd, &new_event),  "Failed epoll_ctl()");
	buffer_manager[accept_fd].index=0;
	buffer_manager[accept_fd].size=max_message;
	return accept_fd;
}

void get_file_name(char* buffer, char* filename)
{
int i = 0, j = 0;
	while(buffer[i++] != '/'){}

	while(buffer[i] != ' ')
	{
		filename[j] = buffer[i];
		++i;
		++j;
	}
	filename[j] = '\0';
}

int get_file_len(FILE* file)
{
	fseek(file, 0, SEEK_END);
	int retval = ftell(file);
	fseek(file, 0, SEEK_SET);
	return retval;
}

void init_buffer_manager() {
	buffer_manager=(response*)malloc(sizeof(response)*num_of_events);
	check_not_null(buffer_manager, "malloc error");

	for(int i=0;i<num_of_events;i++) {

		buffer_manager[i].buffer=(char*)malloc(max_message);
		check_not_null(buffer_manager[i].buffer, "alloc");
		buffer_manager[i].index=0;
		buffer_manager[i].size=max_message;
		buffer_manager[i].filename=NULL;
	}
}