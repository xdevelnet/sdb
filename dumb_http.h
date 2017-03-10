#ifndef SDB_DUMB_HTTP_H
#define SDB_DUMB_HTTP_H

static void ok(int fd) {
	write(fd, http_response_ok, strizeof(http_response_ok));
}

static void ok_bin(int fd) {
	write(fd, http_response_ok_bin, strizeof(http_response_ok_bin));
}

static void created(int fd) {
	write(fd, http_response_created, strizeof(http_response_created));
}

static void bad_request(int fd) {
	write(fd, http_response_bad_request, strizeof(http_response_bad_request));
}

static void too_large(int fd) {
	write(fd, http_response_request_ent_too_large, strizeof(http_response_request_ent_too_large));
}

static void internal_server_error(int fd) {
	write(fd, http_response_internal_server_error, strizeof(http_response_internal_server_error));
}

static void unproc_entry(int fd) {
	write(fd, http_response_unproc_entry, strizeof(http_response_unproc_entry));
}

static void not_found(int fd) {
	write(fd, http_response_not_found, strizeof(http_response_not_found));
}

static void http_processing(int fd, ssize_t bbytes_got, char *http_income, void *context) {
	if (bbytes_got < 0) {
		perror(NULL);
		return;
	}
	if (bbytes_got == INET_READ_CHUNK_SIZE) {
		return too_large(fd);
	}
	size_t bytes_got = (size_t) bbytes_got;
	http_income[bytes_got] = '\0'; // just because I don't care.

	char *fly;
	char *key;
	char *value;
	const char *result_value;
	sdb_dbo *db = context;

	// I know that I can split methods below to separate functions. But I didn't. Because it's really easy & handy to
	// do something like "return bad_request(fd);" and hang up. Keep it simply silly!

	if (strncmp(method_post_str, http_income, strizeof(method_post_str)) == 0) {
		//  ______     ______        _______.___________.
		// |   _  \   /  __  \      /       |           |
		// |  |_)  | |  |  |  |    |   (----`---|  |----`
		// |   ___/  |  |  |  |     \   \       |  |
		// |  |      |  `--'  | .----)   |      |  |
		// | _|       \______/  |_______/       |__|

		fly = strstr(http_income + strizeof(method_post_str), HTTP_HEADER_CONTENT_TYPE);
		if (fly == NULL) return bad_request(fd);
		fly = fly + strizeof(HTTP_HEADER_CONTENT_TYPE);
		if (memcmp(fly, HTTP_VALUE_FORM_URLENCODED, strizeof(HTTP_VALUE_FORM_URLENCODED)) != 0) return bad_request(fd);
		fly = strstr(fly, http_response_content_separator); // move away from headers (and two lines below)
		if (fly == NULL) return bad_request(fd);
		fly += strizeof(http_response_content_separator);
		key = fly; // we're expecting usual post request like key=value.
		// but the thing is, that we need only one key-value pair (for now). Current position is (or may be) key.
		fly = strchr(fly, '='); // find separator.
		if (fly == NULL or fly == key) return bad_request(fd); // obvious validation
		*fly = '\0'; // now key is null-terminated string.
		if (validate_key(key) == false) return bad_request(fd);
		fly++;
		if (*fly == '\0') return bad_request(fd);
		value = fly;
		if (bytes_got - (value - http_income) > MAX_VALUE_SIZE) return too_large(fd);
		if (sdb_insert(db, key, value) == false) {
			if (enomem_flag) return internal_server_error(fd);
			return unproc_entry(fd);
		}
		return created(fd);
	}

	if (strncmp(method_put_str, http_income, strizeof(method_put_str)) == 0) {
		// .______    __    __  .___________.
		// |   _  \  |  |  |  | |           |
		// |  |_)  | |  |  |  | `---|  |----`
		// |   ___/  |  |  |  |     |  |
		// |  |      |  `--'  |     |  |
		// | _|       \______/      |__|

		key = http_income + strizeof(method_put_str);
		fly = strchr(key, ' ');
		if (fly == NULL) return bad_request(fd);
		*fly = '\0';
		if (validate_key(key) == false) return bad_request(fd);
		fly = strstr(fly + 1, http_response_content_separator);
		if (fly == NULL) return bad_request(fd);
		fly += strizeof(http_response_content_separator);
		if (bytes_got - (fly - http_income) == 0) value = ""; else value = fly;
		if (bytes_got - (fly - http_income) > MAX_VALUE_SIZE) return too_large(fd);
		if (sdb_update(db, key, value) == false) {
			if (enomem_flag) return internal_server_error(fd);
			return not_found(fd);
		}

		return ok(fd);
	}

	if (strncmp(method_get_str, http_income, strizeof(method_get_str)) == 0) {
		//   _______  _______ .___________.
		//  /  _____||   ____||           |
		// |  |  __  |  |__   `---|  |----`
		// |  | |_ | |   __|      |  |
		// |  |__| | |  |____     |  |
		//  \______| |_______|    |__|

		key = http_income + strizeof(method_get_str);
		fly = strchr(key, ' ');
		if (fly == NULL) return bad_request(fd);
		*fly = '\0';
		if (validate_key(key) == false) return bad_request(fd);
		fly = strstr(fly + 1, http_response_content_separator);
		if (fly == NULL) {
				fprintf(stderr, "can't find http header request separator!\n");
				return bad_request(fd);
			};

		result_value = sdb_select(db, key);
		if (result_value == NULL) {
			if (enomem_flag) return internal_server_error(fd);
			return not_found(fd);
		}
		ok_bin(fd);
		write(fd, result_value, (size_t) read_size_hook);

		return;
	}

	if (strncmp(method_delete_str, http_income, strizeof(method_delete_str)) == 0) {
		//  _______   _______  __       _______ .___________. _______
		// |       \ |   ____||  |     |   ____||           ||   ____|
		// |  .--.  ||  |__   |  |     |  |__   `---|  |----`|  |__
		// |  |  |  ||   __|  |  |     |   __|      |  |     |   __|
		// |  '--'  ||  |____ |  `----.|  |____     |  |     |  |____
		// |_______/ |_______||_______||_______|    |__|     |_______|

		key = http_income + strizeof(method_delete_str);
		fly = strchr(key, ' ');
		if (fly == NULL) return bad_request(fprintf(stderr, "1\n") * 0 + fd);
		*fly = '\0';
		if (validate_key(key) == false) return bad_request(fprintf(stderr, "2\n") * 0 + fd);
		fly = strstr(fly + 1, http_response_content_separator);
		if (fly == NULL) return bad_request(fprintf(stderr, "3\n") * 0 + fd);

		if (sdb_delete(db, key) == false) {
			if (enomem_flag) return internal_server_error(fd);
			return not_found(fd);
		}

		return ok(fd);
	}

	if (strncmp(method_head_str, http_income, strizeof(method_head_str)) == 0) {
		//  __    __   _______     ___       _______
		// |  |  |  | |   ____|   /   \     |       |
		// |  |__|  | |  |__     /  ^  \    |  .--.  |
		// |   __   | |   __|   /  /_\  \   |  |  |  |
		// |  |  |  | |  |____ /  _____  \  |  '--'  |
		// |__|  |__| |_______/__/     \__\ |_______/

		key = http_income + strizeof(method_head_str);
		fly = strchr(key, ' ');
		if (fly == NULL) return bad_request(fd);
		*fly = '\0';
		if (validate_key(key) == false) return bad_request(fd);
		fly = strstr(fly + 1, http_response_content_separator);
		if (fly == NULL) return bad_request(fd);

		if (sdb_exist(db, key) == false) {
			if (enomem_flag) return internal_server_error(fd);
			return not_found(fd);
		}

		return ok(fd);
	}

	return bad_request(fd);
}

#endif //SDB_DUMB_HTTP_H
