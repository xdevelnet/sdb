#ifndef SDB_RODATA_H
#define SDB_RODATA_H

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
#define INET_READ_CHUNK_SIZE MAX_HTTP_HEADER_SIZE + MAX_KEY_SIZE + MAX_VALUE_SIZE


#endif //SDB_RODATA_H
