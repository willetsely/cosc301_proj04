#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <arpa/inet.h>

#include "network.h"
#include "datastructures.h"

// global variable; can't be avoided because
// of asynchronous signal interaction
int still_running = TRUE;
void signal_handler(int sig) {
    still_running = FALSE;
}
request_t *head;
request_t *tail;
pthread_mutex_t queue_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t log_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t queue_cond = PTHREAD_COND_INITIALIZER;
int queue_cnt = 0;

void usage(const char *progname) {
    fprintf(stderr, "usage: %s [-p port] [-t numthreads]\n", progname);
    fprintf(stderr, "\tport number defaults to 3000 if not specified.\n");
    fprintf(stderr, "\tnumber of threads is 1 by default.\n");
    exit(0);
}


void edit_filepath(char *path, int size)
{
	if (path[0] != '/')
		return;
	char buffer[size];
	int i;
	for (i = 0; i < size + 1; i++)
		buffer[i] = path[i + 1];
	*path = buffer;
	return;
} 


void worker(void)
{
    while(1)
    {
        pthread_mutex_lock(&queue_lock);
        while(queue_cnt == 0){
            pthread_cond_wait(&queue_cond, &queue_lock);
        }
        //save info from the tail to local stack
        const int sock = tail->sock;
        const int port = tail->port;
        const char *ip = tail->ip_add;

        tail = request_t_remove(); //free the old tail, reassign tail to new tail
        queue_cnt--;
        pthread_mutex_unlock(&queue_lock);
        
		char filepath[1024];		
		if (!getrequest(sock, &filepath, 1024))
			edit_filepath(&filepath, strlen(filepath));
		else
		{
			fprintf(stderr, "failed to get request from socket %d\n", sock);
			return;
		}
        
        struct stat statinfo;
        int success_code;
        int totalsize; //totalsize used for the log

        if(stat(filepath, statinfo) == 0) //if stat succeeds i.e. file exists
        {
            success_code = 200;
            int filesize = statinfo->st_size;

            char filesizestr[10];   //cast filesize to string (char[])
            sprintf(filesizestr, "%d", filesize)

            senddata(sock, (HTTP_200,filesize), strlen(HTTP_200,filesizestr));
            totalsize = strlen(HTTP_200) + strlen(filesizestr);

            int file_desc = open(filepath, O_RDONLY); //returns the file descriptor
            if(file_desc == -1) //open failed
            {
                fprintf(stderr, "failed to open file %s\n", filepath);
                return;
            }
            char *read_buffer[filesize];
            if(read(file_desc, read_buffer, filesize) == -1) //read failed
            {
                fprintf(stderr, "failed to read file %s\n", filepath);
                return;
            }
            senddata(sock, read_buffer, filesize);
            totalsize += filesize;
        }   
        else //file doens't exist
        {
            success_code = 404;
            senddata(sock, HTTP_404, strlen(HTTP_404));
            totalsize = strlen(HTTP_404);
        }
		
		time_t now = time(NULL);
		char *time = ctime(&now);		

		pthread_mutex_lock(&log_lock);
		FILE *weblog = fopen("weblog.txt", "a");
		fprintf(weblog, "%s:%d %s \"GET /%s\" %s %d\n", ip, port, time, filepath, success_code, totalsize); 
		pthread_mutex_unlock(&log_lock);
		
    }
}


	

void runserver(int numthreads, unsigned short serverport) {
    //////////////////////////////////////////////////

    // create your pool of threads here

    pthread_t threads[numthreads];
    int i = 0;
    for(;i < numthreads; i++)
        pthread_create(&(threads[i]), NULL, worker, NULL);
    //////////////////////////////////////////////////

    int main_socket = prepare_server_socket(serverport);
    if (main_socket < 0) {
        exit(-1);
    }
    signal(SIGINT, signal_handler);

    struct sockaddr_in client_address;
    socklen_t addr_len;

    fprintf(stderr, "Server listening on port %d.  Going into request loop.\n", serverport);
    while (still_running) {
        struct pollfd pfd = {main_socket, POLLIN};
        int prv = poll(&pfd, 1, 10000);

        if (prv == 0) {
            continue;
        } else if (prv < 0) {
            PRINT_ERROR("poll");
            still_running = FALSE;
            continue;
        }
        
        addr_len = sizeof(client_address);
        memset(&client_address, 0, addr_len);

        int new_sock = accept(main_socket, (struct sockaddr *)&client_address, &addr_len);
        if (new_sock > 0) {
            
            time_t now = time(NULL);
            fprintf(stderr, "Got connection from %s:%d at %s\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port), ctime(&now));
 
           ////////////////////////////////////////////////////////
           /* You got a new connection.  Hand the connection off
            * to one of the threads in the pool to process the
            * request.
            *
            * Don't forget to close the socket (in the worker thread)
            * when you're done.
            */
           ////////////////////////////////////////////////////////
            pthread_mutex_lock(&queue_lock);
            queue_cnt++;
			char *address = inet_ntoa(client_address.sin_addr);
			int port = ntohs(client_address.sin_port);
            head = request_t_insert(new_sock, address, port);
            pthread_cond_signal(&queue_cond);
            pthread_mutex_unlock(&queue_lock);
        }
    }
    fprintf(stderr, "Server shutting down.\n");
        
    close(main_socket);
}


int main(int argc, char **argv) {
    unsigned short port = 3000;
    int num_threads = 1;

    int c;
    while (-1 != (c = getopt(argc, argv, "hp:t:"))) {
        switch(c) {
            case 'p':
                port = atoi(optarg);
                if (port < 1024) {
                    usage(argv[0]);
                }
                break;

            case 't':
                num_threads = atoi(optarg);
                if (num_threads < 1) {
                    usage(argv[0]);
                }
                break;
            case 'h':
            default:
                usage(argv[0]);
                break;
        }
    }

    runserver(num_threads, port);
    
    fprintf(stderr, "Server done.\n");
    exit(0);
}
