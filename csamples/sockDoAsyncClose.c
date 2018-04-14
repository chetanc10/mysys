
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <errno.h>
#include <utmp.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <execinfo.h>
#include <signal.h>
#include <printf.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/prctl.h>

#include "h.h"

#define __rte_unused __attribute__((__unused__))

#define __rte_aligned(a) __attribute__((__aligned__(a)))

static volatile int alive = 1;

int welcomeSocket, newSocket;

void *killer (void *arg)
{
	char ch;
REDO:
	printf ("enter y to test..");
	scanf (" %c", &ch);
	if (ch == 'y') {
		printf ("Doned!\n");
		shutdown (welcomeSocket, SHUT_RDWR);
		close (welcomeSocket);
		perror ("closing welcomeSocket: ");
		welcomeSocket = -1;
		return NULL;
	}
	else goto REDO;
}

int main (int argc, char **argv)
{
	struct sockaddr_in serverAddr;
	struct sockaddr_storage serverStorage;
	socklen_t addr_size;
	pthread_t killer_tid;

	/*---- Create the socket. The three arguments are: ----*/
	/* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
	welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);

	/*---- Configure settings of the server address struct ----*/
	/* Address family = Internet */
	serverAddr.sin_family = AF_INET;
	/* Set port number, using htons function to use proper byte order */
	serverAddr.sin_port = htons(7891);
	/* Set IP address to localhost */
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	/* Set all bits of the padding field to 0 */
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

	/*---- Bind the address struct to the socket ----*/
	bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

	/*---- Listen on the socket, with 5 max connection requests queued ----*/
	if(listen(welcomeSocket,5)==0)
		printf("Listening\n");
	else
		printf("Error\n");

	pthread_create (&killer_tid, NULL, killer, NULL);

ACCEPT_MODE:
	/*---- Accept call creates a new socket for the incoming connection ----*/
	addr_size = sizeof serverStorage;
	newSocket = accept(welcomeSocket, (struct sockaddr *) &serverStorage, &addr_size);
	/*newSocket = accept(4, (struct sockaddr *) &serverStorage, &addr_size);*/

	printf ("waiting for my child to die..\n");
	pthread_join (killer_tid, NULL);

	return 0;
}

