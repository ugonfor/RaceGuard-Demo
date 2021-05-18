#define _GNU_SOURCE
#include<stdio.h>
#include<dlfcn.h>
#include<sys/types.h>
#include<string.h>
#include<unistd.h>

#define MAX 0x100

extern char * __progname;

char cache[MAX] = {0};

// mktemp
static char *(*hook_mktemp)(char *__template) = NULL;
static int (*hook_mkstemp)(char *__template) = NULL;
static char *(*hook_mkdtemp)(char *__template) = NULL;

// probe
static int (*hook_stat)(int ver, const char *__restrict__ __file, struct stat *__restrict__ __buf) = NULL;
static int (*hook_lstat)(int ver, const char *__restrict__ __file, struct stat *__restrict__ __buf) = NULL;

// create
static int (*hook_open)(const char *pathname, int flags, mode_t mode) = NULL;
static int (*hook_creat)(const char *pathname, mode_t mode) = NULL;
static int (*hook_openat)(int dirfd, const char *pathname, int flags, mode_t mode) = NULL;

#define MAX 0x100

void sleep_log(int n){
    printf("\n[!] sleep(%d)\n",n);
    sleep(n);
    return ;
}

void push(char* name){
    printf("[+] cache = %s\n", name);
    strncpy(cache, name, MAX);
}

int check(char* name){
    printf("[?] cache check (%s == %s)\n", cache, name);
    if (strncmp(name, cache, MAX) == 0) return 1;
    else return 0;
}


void clear(){
    printf("[-] cache clear\n");
    memset(cache, 0, MAX);
}

int raceguard(char* pathname){
    printf("=======raceguard START=======\n");
    int c;
    if (check(pathname)){
        c = access(pathname, 0);
        if (c == -1) {
            printf("[O] %s not exist\n", cache);
            clear();
            printf("=======raceguard DONE=======\n");
            return 0;
        }
        else {
            printf("[X] %s exist\n", cache);
            puts("===== race attack occurred! =====");
            puts("========= exit process ==========");
            sleep(1);
            _exit(-1);
        }
    }
}

char* mktemp(char *__template){
    if (hook_mktemp == NULL) hook_mktemp    = dlsym(RTLD_NEXT, "mktemp");
    printf("[!] mktemp(%s) = ", __template);
    char* hook_ret                          = hook_mktemp(__template);
    printf("%s\n", hook_ret);
    return hook_ret;
}

int mkstemp(char *__template){
    if (hook_mkstemp == NULL) hook_mkstemp    = dlsym(RTLD_NEXT, "mkstemp");
    printf("[!] mkstemp(%s) = ", __template);
    int hook_ret                          = hook_mkstemp(__template);
    printf("%d\n", hook_ret);
    return hook_ret;
}

char* mkdtemp(char *__template){
    if (hook_mkdtemp == NULL) hook_mkdtemp    = dlsym(RTLD_NEXT, "mkdtemp");
    printf("[!] mkdtemp(%s) = ", __template);
    char* hook_ret                          = hook_mkdtemp(__template);
    printf("%s\n", hook_ret);
    return hook_ret;
}

int __xstat(int ver, const char *pathname, struct stat *statbuf){
    if (hook_stat == NULL) hook_stat    = dlsym(RTLD_NEXT, "__xstat");
    printf("[!] stat(%s, struct stat statbuf) = ", pathname);
    int hook_ret                          = hook_stat(ver, pathname, statbuf);
    printf("%d\n", hook_ret);

    // if not exist -> push
    if (hook_ret == -1) push(pathname);

    return hook_ret;
};

int __lxstat(int ver, const char *pathname, struct stat *statbuf){
    if (hook_lstat == NULL) hook_lstat    = dlsym(RTLD_NEXT, "__lxstat");
    printf("[!] lstat(%s, struct stat statbuf) = ", pathname);
    int hook_ret                          = hook_lstat(ver, pathname, statbuf);
    printf("%d\n", hook_ret);

    // if not exist -> push
    if (hook_ret == -1) push(pathname);

    return hook_ret;
};

int open(const char * path, int flags, mode_t mode)
{
	if (hook_open 	== NULL) hook_open = dlsym(RTLD_NEXT, "open");
    printf("[!] open(%s, %d, %u) = ", path, flags, mode);

    // sleep for easy attack
    sleep_log(10);
    
    // raceguard
    raceguard(path);

	int hook_ret 	= hook_open(path,flags,mode);
    printf("%d\n", hook_ret);
	return hook_ret;
}

int openat(int dirfd, const char *pathname, int flags, mode_t mode)
{
	if (hook_openat	== NULL) hook_openat = dlsym(RTLD_NEXT, "openat");
    printf("[!] openat(%d, %s, %d, %u) = ", dirfd, pathname, flags, mode);

    // sleep for easy attack
    sleep_log(10);
    
    // raceguard
    raceguard(pathname);

	int hook_ret 	= hook_openat(dirfd, pathname ,flags,mode);
    printf("%d\n", hook_ret);
	return hook_ret;
}

int creat(const char *pathname, mode_t mode)
{
	if (hook_creat	== NULL) hook_creat = dlsym(RTLD_NEXT, "creat");
    printf("[!] creat(%s, %u) = ", pathname, mode);

    // sleep for easy attack
    sleep_log(10);
    
    // raceguard
    raceguard(pathname);

	int hook_ret 	= hook_creat(pathname,mode);
    printf("%d\n", hook_ret);
	return hook_ret;
}

void __attribute__ ((constructor)) before_load(void)
{
    if (hook_mktemp == NULL) hook_mktemp    = dlsym(RTLD_NEXT, "mktemp");
    if (hook_mkstemp == NULL) hook_mkstemp  = dlsym(RTLD_NEXT, "mkstemp");
    if (hook_stat == NULL) hook_stat        = dlsym(RTLD_NEXT, "__xstat");
    if (hook_lstat == NULL) hook_lstat      = dlsym(RTLD_NEXT, "__lxstat");
    if (hook_open == NULL) hook_open        = dlsym(RTLD_NEXT, "open");
    if (hook_creat == NULL) hook_creat      = dlsym(RTLD_NEXT, "creat");
    if (hook_openat == NULL) hook_openat    = dlsym(RTLD_NEXT, "openat");
}

