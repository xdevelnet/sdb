#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdbool.h>
#include <signal.h>
#include <poll.h>
#include <string.h>
#include <iso646.h>
#include <errno.h>
#include "../libsdb/libsdb.h"
#include "util.h"
#include "rodata.h"
#include "dumb_http.h"

#define BACKLOG_SIZE 10

static inline int prepare_inet_listener(void) {
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		perror(SOCKET_FAILURE_ERRMSG);
		exit(EXIT_FAILURE);
	}
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0) {
		perror(NULL);
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in bind_addr = {
		.sin_addr.s_addr = INADDR_ANY,
		.sin_family = AF_INET,
		.sin_port = htonsed_port
	};

	if (bind(fd, (struct sockaddr *) &bind_addr, sizeof(bind_addr)) < 0) {
		perror(BIND_FAILURE_ERRMSG);
		close(fd);
		exit(EXIT_FAILURE);
	}

	if (listen(fd, BACKLOG_SIZE) < 0) {
		perror(LISTEN_FAILURE_ERRMSG);
		close(fd);
		exit(EXIT_FAILURE);
	}

	return fd;
}

char *readbuffer;

static inline void prepare_readbuffer() {
	readbuffer = calloc(INET_READ_CHUNK_SIZE, sizeof(char));
	if (readbuffer == NULL) {
		write(STDERR_FILENO, memory_allocation_failure, strizeof(memory_allocation_failure));
		exit(EXIT_FAILURE);
	}
}

static inline void proceed_netinet_request(int fd, void *context) {
	struct pollfd pollfds = { .fd = fd, .events = POLLIN };

	int pollret = poll(&pollfds, 1, POLL_TIMEOUT); // 1 is number of descriptors we gonna poll()

	if (pollret <= 0) return;
	// What could possibly happens here? Let's see:
	// EFAULT - only if programmer made a mistake
	// EINTR - possible signals, which may interrupt poll system call, are already handled,
	//         and we gonna gracefully close() and exit() below.
	// EINVAL - practically impossible.
	// ENOMEM - I should handle this, but I'll do that later. TODO!

	ssize_t bytes_got = read(fd, readbuffer, INET_READ_CHUNK_SIZE);
	http_processing(fd, bytes_got, readbuffer, context);
}

int main() {
	handle_posix_signals();
	int netfd = prepare_inet_listener();
	prepare_readbuffer();

	int datafd;
	sdb_tune(INET_READ_CHUNK_SIZE);
	sdb_dbo *db = sdb_open(SDB_FILENO, NULL);
	if (db == NULL) return EXIT_FAILURE;

	while (1) {
		datafd = accept(netfd, NULL, NULL);
		if (datafd < 0) {
			if (we_need_to_stop == 0) perror(ACCEPT_FAILURE_ERRMSG);
			break;
		} else {
			proceed_netinet_request(datafd, db);
			shutdown(datafd, SHUT_RDWR);
			close(datafd);
		}
		if (we_need_to_stop) break;
	}

	free(readbuffer);
	sdb_close(db);
	close(netfd);
	return EXIT_SUCCESS;
}
