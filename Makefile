RM 	= rm 
CC 	= gcc

default:
	$(CC) -c -fPIC  hook.c 
	$(CC) -shared hook.o -o libhook.so -ldl
	$(CC) -c -fPIC  raceguard.c 
	$(CC) -shared raceguard.o -o libraceguard.so -ldl
	$(CC) -o test1 test1.c
	$(CC) -o race1 race1.c -lpthread

clean:
	$(RM) libhook.so hook.o test1 race1 libraceguard.so raceguard.o
	$(RM) race??????
