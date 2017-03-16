#include <unistd.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <iso646.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define CT_HTONS(x) (((x >> 8) & 0x00FF) | ((x << 8) & 0xFF00))
#define CT_HTONL(x) (((x >> 24) & 0x000000FF) | ((x >> 8) & 0x0000FF00) | ((x << 8) & 0x00FF0000) | ((x << 24) & 0xFF000000))

#define BUFFER_SIZE 65536 * 700
#define PORT 3003
#define REQUESTS 1000000

size_t call_cnt = 0;

const char call[] = "GET /rdata.jpg HTTP/1.1\r\n\r\n";

int main() {
	char *buffer = calloc(BUFFER_SIZE, sizeof(char));
	if (buffer == NULL) {
		perror(NULL);
		return EXIT_FAILURE;
	}

	const struct sockaddr_in connaddr = {.sin_family = AF_INET, .sin_port = CT_HTONS(PORT), .sin_addr.s_addr = CT_HTONL(INADDR_LOOPBACK)};

	while (1) {
		int fd = socket(AF_INET, SOCK_STREAM, 0);
		if (fd < 0) {
			perror(NULL);
			return EXIT_FAILURE;
		}
		int ret = connect(fd, (const struct sockaddr *) &connaddr, sizeof(struct sockaddr_in));
		if (ret < 0) {
			perror(NULL);
			return EXIT_FAILURE;
		}
		ssize_t got = write(fd, call, sizeof(call) -1);
		if (got > 0 and read(fd, buffer, BUFFER_SIZE) > 0) call_cnt++;
		close(fd);
		if (call_cnt > 1000000 - 1) break;
	}
	return EXIT_SUCCESS;
}
