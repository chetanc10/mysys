
/**
 * Compilation: gcc ex_mqx.c -lrt
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>

/*#define MQNAME "/tmp/mqxtest"*/
#define MQNAME "/mqxtest"
#define MQMESG "Hello there!"

static mqd_t serverUp (void) {
	int rc;
	mqd_t svrHndl;
	struct mq_attr mqAttr;

	printf ("Bringing up server.\n");
	rc = mq_unlink (MQNAME);
	if (rc < 0) {
		printf ("   Warning %d (%s) on server mq_unlink.\n",
				errno, strerror (errno));
	}

	memset (&mqAttr, 0, sizeof (struct mq_attr));
	mqAttr.mq_maxmsg = 8192;
	mqAttr.mq_msgsize = 4;
	/*svrHndl = mq_open (MQNAME, O_RDWR|O_CREAT);*/
	svrHndl = mq_open (MQNAME, O_RDWR|O_CREAT, S_IWUSR|S_IRUSR, &mqAttr);
	if (svrHndl < 0) {
		printf ("   Error %d (%s) on server mq_open.\n",
				errno, strerror (errno));
		exit (1);
	}
	printf ("   Server opened mqd_t of %d.\n", svrHndl);
	return svrHndl;
}

static void serverReceive (mqd_t svrHndl) {
	int rc;
	printf ("Server receiving on mqd_t %d.\n", svrHndl);
	unsigned int msgN = 0;
	rc = mq_receive (svrHndl, (char *)&msgN, sizeof (msgN), NULL);
	if (rc < 0) {
		printf ("   Error %d (%s) on server mq_receive.\n",
				errno, strerror (errno));
		exit (1);
	}
	/*printf ("   Received [%s].\n", buffer);*/
	printf ("   Received [%d].\n", msgN);
}

static void clientSend (void) {
	mqd_t cliHndl;
	int rc;
	unsigned int msgN = 1219;
	printf ("Client sending.\n");
	cliHndl = mq_open (MQNAME, O_RDWR);
	if (cliHndl < 0) {
		printf ("   Error %d (%s) on client mq_open.\n",
				errno, strerror (errno));
		exit (1);
	}
	printf ("   Client opened mqd_t of %d.\n", cliHndl);

	/*rc = mq_send (cliHndl, MQMESG, sizeof (MQMESG), 1);*/
	rc = mq_send (cliHndl, (char *)&msgN, sizeof (unsigned int), 1);
	if (rc < 0) {
		printf ("   Error %d (%s) on client mq_send.\n",
				errno, strerror (errno));
		exit (1);
	}

	mq_close (cliHndl);
}

int main (void) {
	mqd_t svrHndl;

	svrHndl = serverUp ();
	clientSend ();
	serverReceive (svrHndl);

	printf ("Bringing down server with mqd_t %d.\n", svrHndl);
	mq_close (svrHndl);

	return 0;
}
