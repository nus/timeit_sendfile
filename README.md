Timeit sendfile(2)
====

For comparing performance of `sendfile(2)` and `user-land sendfile`.
`user-land sendfile` means that copying a file by using `read(2)` function and `write(2)`.

Build
----

```
$ make
```

Exection
----

```
$ time ./a.out -s

real	0m9.900s
user	0m0.004s
sys	0m2.268s


$ time ./a.out -u

real	0m16.220s
user	0m0.032s
sys	0m3.044s

```
