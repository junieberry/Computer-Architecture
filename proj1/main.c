#include <stdio.h>

typedef union Word{
    int inst_num; 
    unsigned char inst_arr[4];
} Word;

int main(int argc, char**argv){
    Word word;
    unsigned char temp;   
    int i,n=0;
    FILE*fp=fopen(argv[1],"rb");

    if(fp==NULL){
        printf("스트림 생성시 오류 발생");
        return 1;
    }

    while(!feof(fp)){
        fread((Word*)&word, sizeof(word), 1, fp);
        for (i=0; i<2; i++){
            temp=word.inst_arr[i];
            word.inst_arr[i]=word.inst_arr[3-i];
            word.inst_arr[3-i]=temp;
        }
        printf("inst %2d : %08x\n",n,word.inst_num);
        n++;
    }
    
    fclose(fp);

    return 0;
}