#include <stdio.h>

typedef struct R{
    unsigned int opcode:6;
    unsigned int rs:5;
    unsigned int rt:5;
    unsigned int rd:5;
    unsigned int shamt:5;
    unsigned int funct:6;
} R;

typedef struct I{
    unsigned int opcode:6;
    unsigned int rs:5;
    unsigned int rt:5;
    unsigned int immediate:16;
} I;


int main(){
    int a=4;
    R r=(R)4;

    printf("%d %d",r.opcode,r.funct);
}