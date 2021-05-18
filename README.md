# RaceGuard-Demo

## Explanation

paper review https://www.usenix.org/legacy/events/sec01/full_papers/cowanbeattie/cowanbeattie.pdf

[Usenix Security][2001][RaceGuard: Kernel Protection From Temporary File Race Vulnerabilities]

This code is unofficial implementation to "understand concept" of RaceGuard.

It doesn't work the same as described in the paper. 

In paper, RaceGuard is the kernel level solution for defensing Race condition Attack.

However, I make this demo code through hooking some codes since this code is for understanding Raceguard's Concept Easily.

---

## Make
```shell
$ make
```

## how to use

### Test
```shell
$ export LD_PRELOAD=$PWD/libhook.so
$ ./test1
[!] mktemp(./raceXXXXXX) = ./raceevQY82
[!] open(./raceevQY82, 65, 511) = 
[!] sleep(10)
3                                         # this is output of open(~~~) =
```

### RaceGuard Off

#### terminal 1

```shell
$ echo "don't touch" > donttouch
$ cat donttouch 
don't touch

$ ln -s donttouch ./racewfpiwF      # between sleep

$ cat donttouch                     # after terminal 2 done
This is text
```

#### terminal 2
```shell
$ export LD_PRELOAD=$PWD/libhook.so
$ ./test1
[!] mktemp(./raceXXXXXX) = ./racewfpiwF
[!] open(./racewfpiwF, 65, 511) = 
[!] sleep(10)                       # ln -s donttouch ./racewfpiwF is executed on terminal 1
3
```
race attack is success

### RaceGuard on

#### terminal 1

```shell
$ echo "don't touch" > donttouch
$ cat donttouch 
don't touch

$ ln -s donttouch ./raceJEgNeQ      # between sleep

$ cat donttouch                     # after terminal 2 done
don't touch
```

#### terminal 2
```shell
$ export LD_PRELOAD=$PWD/libraceguard.so
$ ./test1
[!] mktemp(./raceXXXXXX) = ./raceJEgNeQ
[!] stat(./raceJEgNeQ, struct stat statbuf) = -1
[+] cache = ./raceJEgNeQ
[!] open(./raceJEgNeQ, 65, 511) = 
[!] sleep(10)                                        # ln -s donttouch ./raceJEgNeQ is executed on terminal 1
=======raceguard START=======
[?] cache check (./raceJEgNeQ == ./raceJEgNeQ)
[X] ./raceJEgNeQ exist
===== race attack occurred! =====
========= exit process ==========
```
race attack is defensed
