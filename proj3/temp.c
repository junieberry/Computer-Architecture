#include<stdio.h>

int a=0;
int b(){
    a=3;
    return 1;
}

int main(){
    if(b()==1){
        printf("%d",a);
    }

    return 0;
}