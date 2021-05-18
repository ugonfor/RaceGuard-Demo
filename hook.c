#define _GNU_SOURCE
#include<stdio.h>
#include<dlfcn.h>
#include<sys/types.h>
#include<string.h>
#include<unistd.h>

extern char * __progname;

// mktemp
static char *(*hook_mktemp)(char *__template) = NULL;
static int (*hook_mkstemp)(char *__template) = NULL;
static char *(*hook_mkdtemp)(char *__template) = NULL;

// probe
static int (*hook_stat)(const char *__restrict__ __file, struct stat *__restrict__ __buf) = NULL;
static int (*hook_lstat)(const char *__restrict__ __file, struct stat *__restrict__ __buf) = NULL;

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

int stat(const char *pathname, struct stat *statbuf){
    if (hook_stat == NULL) hook_stat    = dlsym(RTLD_NEXT, "stat");
    printf("[!] stat(%s, struct stat statbuf) = ", pathname);
    int hook_ret                          = hook_stat(pathname, statbuf);
    printf("%d\n", hook_ret);
    return hook_ret;
};

int lstat(const char *pathname, struct stat *statbuf){
    if (hook_lstat == NULL) hook_lstat    = dlsym(RTLD_NEXT, "stat");
    printf("[!] lstat(%s, struct stat statbuf) = ", pathname);
    int hook_ret                          = hook_lstat(pathname, statbuf);
    printf("%d\n", hook_ret);
    return hook_ret;
};

int open(const char * path, int flags, mode_t mode)
{
	if (hook_open 	== NULL) hook_open = dlsym(RTLD_NEXT, "open");
    printf("[!] open(%s, %d, %u) = ", path, flags, mode);
    
    sleep_log(10);

	int hook_ret 	= hook_open(path,flags,mode);
    printf("%d\n", hook_ret);
	return hook_ret;
}

int openat(int dirfd, const char *pathname, int flags, mode_t mode)
{
	if (hook_openat	== NULL) hook_openat = dlsym(RTLD_NEXT, "openat");
    printf("[!] openat(%d, %s, %d, %u) = ", dirfd, pathname, flags, mode);

    sleep_log(10);

	int hook_ret 	= hook_openat(dirfd, pathname ,flags,mode);
    printf("%d\n", hook_ret);
	return hook_ret;
}

int creat(const char *pathname, mode_t mode)
{
	if (hook_creat	== NULL) hook_creat = dlsym(RTLD_NEXT, "creat");
    printf("[!] creat(%s, %u) = ", pathname, mode);

    sleep_log(10);
    
	int hook_ret 	= hook_creat(pathname,mode);
    printf("%d\n", hook_ret);
	return hook_ret;
}

void __attribute__ ((constructor)) before_load(void)
{
    if (hook_mktemp == NULL) hook_mktemp    = dlsym(RTLD_NEXT, "mktemp");
    if (hook_mkstemp == NULL) hook_mkstemp  = dlsym(RTLD_NEXT, "mkstemp");
    if (hook_stat == NULL) hook_stat        = dlsym(RTLD_NEXT, "stat");
    if (hook_lstat == NULL) hook_lstat      = dlsym(RTLD_NEXT, "lstat");
    if (hook_open == NULL) hook_open        = dlsym(RTLD_NEXT, "open");
    if (hook_creat == NULL) hook_creat      = dlsym(RTLD_NEXT, "creat");
    if (hook_openat == NULL) hook_openat    = dlsym(RTLD_NEXT, "openat");
}

