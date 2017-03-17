# sdb
sdb is single threaded [key-value database](https://en.wikipedia.org/wiki/Key-value_database) with RESTful access for low load POSIX-compatiable systems. This program is designed to be simple as possible.

RESTful access:

|       Function        |      HTTP method       |
|-----------------------|------------------------|
|   Add record   |         POST           |
|   Update record   |         PUT            |
|    Select record     |         GET            |
|    Remove record    |        DELETE          |
|Check record existence|         HEAD           |

You are allowed to use visible ASCII characters as key.

# Building and usage

sdb is dependent on libsdb. So, you need to download and compile it first
```bash
git clone https://github.com/xdevelnet/libsdb.git
cd libsdb
make
```
Now got libsdb.so file. You may wish to place copy somewhere at system libraries directory.
```bash
cd -
git clone https://github.com/xdevelnet/sdb.git
cd sdb
make
```
You are ready to launch program with ./main

# Errata & notes

1. Currently, percent-encoding (also known as URL encoding) isn't supported. Maybe i'll do that some day.
2. This program is designed to use in your own machine, or, at least, your own LAN. Therefore, there is no security checks (so, in theory, your can receive any file through GET request from remote machine) or SSL support.
3. sdb is single threaded program. It will not respond to any request immediatly if it's already busy with other one.
4. If client is connected, but didn't send any data for 1 second, it will be disconnected immediatly. You can change poll timeout by modifying program source (that's really easy task BTW).
