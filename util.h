#ifndef SDB_UTIL_H
#define SDB_UTIL_H

// CHEATER! Gotcha!

#define strizeof(i) sizeof(i)-1
#define CT_HTONS(x) (((x >> 8) & 0x00FF) | ((x << 8) & 0xFF00)) // Don't mention "byteswap.h", please. Ever. Again.
#define CT_HTONL(x) (((x >> 24) & 0x000000FF) | ((x >> 8) & 0x0000FF00) | ((x << 8) & 0x00FF0000) | ((x << 24) & 0xFF000000))

// signal needs

int we_need_to_stop = 0;

void term_handler(int i) {
	we_need_to_stop++;
}

static inline void handle_posix_signals() {
	signal(SIGTERM, term_handler);
	signal(SIGINT, term_handler);
}

// common

static bool validate_key(const char *key) {
	if (key == NULL) return false;
	while (*key != '\0') {
		if (*key < '!' or *key > '~') return false;
		key++;
	}
	return true;
}

#endif //SDB_UTIL_H
