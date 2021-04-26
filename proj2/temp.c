#include <stdio.h>

void change(int* Register){
    for (int i = 0; i < 31; i++)
    {
        Register[i]=i;
    }

}

int main(){

    int Register[31]={0};
    int (*R)[31];
    R=&Register;
    change(Register);
    for (int i=0; i<31; i++){
        printf("%d\n",Register[i]);
    }

    return 0;
}