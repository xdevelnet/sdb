#ifndef SDB_UTIL_H
#define SDB_UTIL_H

#define strizeof(i) sizeof(i)-1

int we_need_to_stop = 0;

void term_handler(int i) {
	we_need_to_stop++;
}

static inline void handle_posix_signals() {
	signal(SIGTERM, term_handler);
	signal(SIGINT, term_handler);
}

#endif //SDB_UTIL_H
