#ifndef SDB_RODATA_H
#define SDB_RODATA_H

// common settings

#define APPLICATION_PORT 3003
const uint16_t htonsed_port = CT_HTONS(APPLICATION_PORT);

// error strings

#define SOCKET_FAILURE_ERRMSG "Failed to create socket"
#define BIND_FAILURE_ERRMSG "Failed to bind"
#define LISTEN_FAILURE_ERRMSG "Failed to listen"
#define ACCEPT_FAILURE_ERRMSG "Failed to accept incoming connection"

const char memory_allocation_failure[] = "Sorry, but we're unable to allocate enough memory. Exiting...";

// limits

#define POLL_TIMEOUT 1000
#define MAX_HTTP_HEADER_SIZE 4096 // nginx default
#define MAX_KEY_SIZE 255
#define MAX_VALUE_SIZE 65536
#define REDUNDANT_SIZE 5 // why? Dunno!
#define INET_READ_CHUNK_SIZE (MAX_HTTP_HEADER_SIZE + MAX_KEY_SIZE + MAX_VALUE_SIZE + REDUNDANT_SIZE)

// HTTP stuff

const char method_post_str[] = "POST /";
const char method_put_str[] = "PUT /";
const char method_get_str[] = "GET /";
const char method_delete_str[] = "DELETE /";
const char method_head_str[] = "HEAD /";

#define HTTP_CONNECTION_CLOSE "Connection: close\r\n"
#define HTTP_SERVER_SIGN "Server: sdb\r\n"
#define HTTP_HEADER_CONTENT_TYPE "Content-Type: "
#define HTTP_VALUE_FORM_URLENCODED "application/x-www-form-urlencoded"
#define HTTP_VALUE_FORM_TEXT_PLAIN "text/plain"
#define HTTP_HALF_SEPARATOR "\r\n"
#define HTTP_STD_RESPONSE HTTP_SERVER_SIGN HTTP_CONNECTION_CLOSE HTTP_HEADER_CONTENT_TYPE \
HTTP_VALUE_FORM_TEXT_PLAIN "; charset=utf-8\r\n\r\n"

#define HTTP_OK "HTTP/1.1 200 OK\r\n"
#define HTTP_HEADER_OK HTTP_OK HTTP_STD_RESPONSE
#define HTTP_HEADER_CREATED "HTTP/1.1 201 Created\r\n" HTTP_STD_RESPONSE
#define HTTP_HEADER_BAD_REQUEST "HTTP/1.1 400 Bad Request\r\n" HTTP_STD_RESPONSE
#define HTTP_HEADER_NOT_FOUND "HTTP/1.1 404 Not Found\r\n" HTTP_STD_RESPONSE
#define HTTP_HEADER_INTERNAL_ERR "HTTP/1.1 500 Internal Server Error\r\n" HTTP_STD_RESPONSE
#define HTTP_HEADER_REQUEST_ENT_TOO_LARGE "HTTP/1.1 413 Request Entity Too Large\r\n" HTTP_STD_RESPONSE
//#define HTTP_HEADER_PAYLOAD_TOO_LARGE "HTTP /1.1 413 Payload Too Large\r\n" HTTP_STD_RESPONSE // Should I use this one?
#define HTTP_HEADER_UNPROC_ENTRY "HTTP/1.1 422 Unprocessable Entity\r\n" HTTP_STD_RESPONSE
#define HTTP_OK_BIN HTTP_OK HTTP_CONNECTION_CLOSE HTTP_SERVER_SIGN HTTP_HALF_SEPARATOR

const char http_response_ok[] = HTTP_HEADER_OK "Done.";
const char http_response_ok_bin[] = HTTP_OK_BIN;
const char http_response_created[] = HTTP_HEADER_CREATED "Record has been created.";
const char http_response_bad_request[] = HTTP_HEADER_BAD_REQUEST "Sorry, but you sent a request that can't be processed.";
const char http_response_not_found[] = HTTP_HEADER_NOT_FOUND "Unfortunately, we did not find requested content.";
const char http_response_internal_server_error[] = HTTP_HEADER_INTERNAL_ERR "Whops! Weird bad thing happens!";
const char http_response_request_ent_too_large[] = HTTP_HEADER_REQUEST_ENT_TOO_LARGE "Request is too large!";
const char http_response_unproc_entry[] = HTTP_HEADER_UNPROC_ENTRY "Already exist.";
const char http_response_content_separator[] = HTTP_HALF_SEPARATOR HTTP_HALF_SEPARATOR;


#endif //SDB_RODATA_H
