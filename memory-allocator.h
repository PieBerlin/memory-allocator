/*memory-allocator.h*/
#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <pieutils.h>

typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned int int32;
typedef unsigned long long int int64;
typedef unsigned _BitInt(128) int128;
typedef void heap;
typedef int32 Word;

#define packed __attribute__((__packed__))
#define unused __attribute__((__unused__))
#define Maxwords ((1024*1024*1014/4)-1)

#define ErrNoMem 1
#define ErrUnknown 2

typedef struct packed s_header{
    Word w : 30; // Stores the number of bytes allocated
    bool allocated : 1; // keep infor if the bytes are allocated ot not
    bool unused reserved : 1; // reserved for future use
}header;



// for typecasting
#define $8 (int8*)
#define $c (char *)
#define $16 (int16)
#define $32 (int32)
#define $64 (int64)
#define $i (int)

#define $128 (int128)
#define $v (void *)
#define $h (header *)

#define reterr(x) \
    errno = (x);  \
    return $v 0;

#define findblock(x) findblock_((header*)memspace,(x),0)
#define show() show_($h memspace)

void free_(void *);
void show_(header *);
header *findblock_(header *, Word, Word);
void *mkalloc(Word, header *);
void *alloc(int32);
int main(int, char **);
