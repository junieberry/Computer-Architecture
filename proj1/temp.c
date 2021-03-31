#include <stdio.h>
#include <string.h>

int uint(char* inst, int n){
    int sum=0;
    for (int i=0;i<n; i++){
        sum+=(inst[i]-48)*(1<<(n-1-i));
    }
    return sum;
}

int main(){
    char inst[]="1011";
    printf("%d",uint(inst+1,2));
    return 0;
}