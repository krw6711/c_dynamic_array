#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
// #include <signal.h>
#include <stdbool.h>

typedef struct{
    int* array;
    unsigned int length;
    int filled;
    int cursor;
} DArray; // Dynamic Array

// void clean_mem(int sig);

int* new_int_array(size_t len){
    int *raw_mem = calloc(len, sizeof(int));
    if(raw_mem == NULL){
        printf("out of memory");
        return NULL;
    }
    return raw_mem;
}

DArray new_array_info(int *array, unsigned int len){
    return (DArray){
        .array = array,
        .length = len,
        .filled = 0,
        .cursor = 0
    };
}

DArray new_darray(size_t len){
    return new_array_info(new_int_array(len), (int)len);
}

int expand(DArray *buf, size_t len)
{
    if(len == 0){
        printf("can not increase by nothing");
        return 1;
    }
    size_t new_size = ((size_t)buf->length + len) * sizeof(int);
    int* new_ptr = realloc(buf->array, new_size);
    if(new_ptr ==  NULL){
        printf("cant expand, oout of memory");
        return 1;
    }
    buf->array = new_ptr;
    buf->length+= (int)len;
    return 0;
}

int collapse(DArray *buf, size_t len)
{
    if(len == 0){
        printf("can not decrease by nothing");
        return 1;
    }
    size_t new_size = ((size_t)buf->length - len) * sizeof(int);
    int* new_ptr = realloc(buf->array, new_size);
    if(new_ptr ==  NULL){
        printf("cant expand, oout of memory");
        return 1;
    }
    buf->array = new_ptr;
    buf->length-= (int)len;
    buf->filled-= (int)len;
    buf->cursor-= (int)len;
    return 0;
}

int d_insert(DArray *buf, int item){
    if(buf->length == buf->filled){
        if(expand(buf, 1)){
            printf("can not insert, no space");
            return 1;
        }
    }
    buf->array[buf->cursor] = item;
    buf->cursor++;
    buf->filled++;
    return 0;
}

void d_remove(DArray *buf, int index) // 2
{
    // [10 ,20, (30), 40]
    //  0   1    2    3
    for(int i = index; i < (buf->filled - 1); i++){
        // index 2 = index 3
        buf->array[i] = buf->array[i + 1];
    }
    // decrease the size to delete index 3
    collapse(buf, 1);
}

int d_index_of(DArray *buf, int item)
{
    for (int i = 0; i < buf->filled; i++) {
        if(buf->array[i] == item) return i;
    }
    return -1;
}

void print_darray(DArray *buf){
    for(int i = 0; i < buf->filled; i++){
        printf("%d\n", buf->array[i]);
    }
}

void free_darray(DArray *buf)
{
    free(buf->array);
    buf->array = NULL;
}

int main(int argc, char *argv[]){
    // signal(SIGINT, clean_mem);

    DArray numbers = new_darray(3);
    d_insert(&numbers, 10); // 0
    d_insert(&numbers, 20); // 1
    d_insert(&numbers, 30); // 2
    d_insert(&numbers, 40);
    d_insert(&numbers, 50);
    d_insert(&numbers, 60);
    d_remove(&numbers, 2);  // no 30
    print_darray(&numbers);
    printf("%d\n", d_index_of(&numbers, 20)); // 1
    printf("%d\n", d_index_of(&numbers, 600)); // -1
    free_darray(&numbers);

    return 0;
}

// void clean_mem(int sig) {
//     const char msg[] = "\nCleaning up and exiting...\n";
//     write(STDOUT_FILENO, msg, sizeof(msg) - 1); 
//     exit(0); 
// }