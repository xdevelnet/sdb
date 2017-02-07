#ifndef SDB_DUMB_HTTP_H
#define SDB_DUMB_HTTP_H

#include <fcntl.h>

const char method_post_str[] = "POST";
const char method_put_str[] = "PUT";
const char method_get_str[] = "GET /";
const char method_delete_str[] = "DELETE";
const char method_head_str[] = "HEAD";

#define HTTP_STD_RESPONSE "Server: sdb\r\n"\
	"Connection: close\r\n"\
	"Content-Type: text/plain; charset=utf-8\r\n\r\n"

#define HTTP_HEADER_BAD_REQUEST "HTTP/1.1 400 Bad Request\r\n" HTTP_STD_RESPONSE
#define HTTP_HEADER_NOT_FOUND "HTTP/1.1 404 Not Found\r\n" HTTP_STD_RESPONSE
#define HTTP_HEADER_INTERNAL_ERR "HTTP/1.1 500 Internal Server Error" HTTP_STD_RESPONSE

const char http_response_bad_request[] = HTTP_HEADER_BAD_REQUEST "Sorry, but you sent a request that can't be processed.";
const char http_response_not_found[] = HTTP_HEADER_NOT_FOUND "Unfortunately, we did not find requested content.";
const char http_response_internal_server_error[] = HTTP_HEADER_INTERNAL_ERR "Whops! Weird bad thing happens!";
const char http_response_content_separator[] = "\r\n\r\n";

void bad_request(int fd) {
	write(fd, http_response_bad_request, strizeof(http_response_bad_request));
}

size_t dramatic_posix_laziness(ssize_t no) {
	if (no < 0) return 0;
	return (size_t) no;
}

static void http_processing(int fd, ssize_t bytes_got, char *str) {
	if (bytes_got < 0) {
		perror(NULL);
		return;
	}

	if (strncmp(method_post_str, str, strizeof(method_post_str)) == 0) {

		// TODO

	} else if (strncmp(method_put_str, str, strizeof(method_put_str)) == 0) {

		// TODO

	} else if (strncmp(method_get_str, str, strizeof(method_get_str)) == 0) {

		char *filename = str += strizeof(method_get_str);
		char *fly = strchr(filename, ' ');
		*fly = '\0';
		fly = strstr(fly + 1, http_response_content_separator);
		if (fly == NULL) return bad_request(fd);
		int filefd = open(filename, O_RDONLY);
		if (filefd < 0 and errno == ENOENT) {
			write(fd, http_response_not_found, strizeof(http_response_not_found));
			return;
		}
		if (filefd < 0) {
			write(fd, http_response_internal_server_error, strizeof(http_response_internal_server_error));
			return;
		}

		char fbuffer[4096];
		while (write(fd, fbuffer, dramatic_posix_laziness(read(filefd, fbuffer, sizeof(fbuffer)))) > 0) ; // OMG! Should I be proud now?
		return;

	} else if (strncmp(method_delete_str, str, strizeof(method_delete_str)) == 0) {

		// TODO

	} else if (strncmp(method_head_str, str, strizeof(method_head_str)) == 0) {

		// TODO

	} else {
		bad_request(fd);
	}
}

#endif //SDB_DUMB_HTTP_H
