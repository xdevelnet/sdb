all:
	cp ../libsdb/libsdb.so ./ ; cc --std=c99 main.c -o main -Wall -Werror -Wno-unused-result -O2 -l:libsdb.so -Wl,-rpath '-Wl,$$ORIGIN'
clean:
	rm main libsdb.so
