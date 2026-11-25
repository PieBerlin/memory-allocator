/* memory-allocator.c */
#include "memory-allocator.h"

extern heap *memspace;

header *findblock_(header *hdr,Word allocation ,Word n){
    bool ok;
    void *mem;
    header *hdr_;
    Word n_;

    if ((n + allocation) > Maxwords - 2)
    {
        reterr(ErrNoMem);
    }
    ok = false;
    ok = (!(hdr->w)) ? true : 
    (!(hdr->allocated) && (hdr->w >= allocation)) ? true
        : false;

    if (ok){
        return hdr;

    }else{

        mem =  $v hdr + (hdr->w * 4) + 4 ;
        hdr_ = $h mem;
        n_ = n + hdr->w;

        return findblock_(hdr_, allocation, n_);
    }

    reterr(ErrUnknown);
}
void *mkalloc(Word words,header *hdr){
    void *ret;
    int32 bytesin;
    Word wordsin;

    bytesin = ($v hdr - memspace);
    wordsin = ((bytesin) / 4 + 1);

    if (words > (Maxwords - wordsin)){
        reterr(ErrNoMem);
    }

    hdr->w = words;
    hdr->allocated = true;
    ret = ($v hdr) + 4;

    return ret;
}

void *alloc(int32 bytes){
    /* Check if bytes is a multiple of 4 to allocate the exact bytes
    Else allocate one which wont fill completely eg 7 / 4 = 1
    then add 1 byte = 2 
    2 * 4 = 8 bytes 
    now 7 can fit in 8 bytes  */
    Word words;
    header *hdr;
    void *mem;


    words = (!(bytes % 4)) ? bytes / 4 : (bytes / 4) + 1;

    mem = $v memspace;
    hdr = $h findblock(words);
    if (!hdr)
        return $v 0;

    if (words > Maxwords){
        reterr(ErrNoMem);
    }
    mem = mkalloc(words, hdr);
    if (!mem)
        return $v 0;
    return mem;
 
    return $v 0;
}
void show_(header *hdr){
    header *p;
    void *mem;
    int n;
    for (n = 1, p = hdr; p->w; mem = $v p + ((p->w + 1) * 4), p = mem,n++)
    {
        printf("Alloc %d = %d %s words\n", n, p->w, (p->allocated) ? "allocated" : "free");
    }
}

void free_(void  *mem){
    /* Either this gets us back to the header to dealocate the following bytes */
    // header *hdr = $h mem - 1; // go back i Word(4 bytes) - to the start of the header
    header *hdr = $v mem - 4; // go back 4 bytes - to the start of header

    hdr->w = 0;
    hdr->allocated = false;
}

int main(int argc , char *arg[]){

    int8 *p,*p2,*p3, *p4;

    printf("Memspace = 0x%x\n",$i memspace); 

    p = alloc(7);
    printf("Alloc p 0x%x\n", $i p);
    
    header *hdr;

    hdr = findblock(500);
    if (!p){
        printf("Error %d\n", errno);
        return -1;
    }
    
    printf("Block = 0x%x\n",$i hdr);

    p2 = alloc(2000);
    printf("Alloc p2 0x%x\n", $i p2);

    p3 = alloc(1);
    printf("Alloc p3 0x%x\n", $i p3);
    show();

    free_($h p3);
    printf("_______________\n");
    show();

    /* P4 should contain the free space previously held by p3 */
    p4 = alloc(1);
    printf("Alloc p4 0x%x\n", $i p4);
    show();

    return 0;
}