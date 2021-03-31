#include <stdio.h>
#include <string.h>

int sint(char* inst, int n){
    int sum=0;
    if (inst[0]==48){
        for (int i=0;i<n; i++){
        sum+=(inst[i]-48)*(1<<(n-1-i));
        }
    }
    else{
        for (int i=1;i<n; i++){
        sum+=(inst[i]-48)*(1<<(n-1-i));
        }
        sum=sum-(1<<(n-1));
    }
    return sum;
}

int main(){
    char inst[]="0100000";
    printf("%d",sint(inst,7));
    return 0;
}