#include <stdio.h>
#include <string.h>


typedef union Word{
    int inst_num; 
    unsigned char inst_arr[4];
} Word;

int uint(char* inst, int n){
    int sum=0;
    for (int i=0;i<n; i++){
        sum+=(inst[i]-48)*(1<<(n-1-i));
    }
    return sum;
}

void Rinst(char* inst){

}


void instruction(char* inst){
    if (!strncmp(inst,"000000",6)){
        printf("\nR instruction");
    }
    else if (!strncmp(inst,"00001",5)){
        printf("\nJ instruction");
    }
    else if (strncmp(inst,"0100",4)){
        printf("\nI instruction");
    }
    else{
        printf("unknown instruction");
    }
};





int main(int argc, char**argv){
    Word word;
    unsigned char temp;
    char inst[32];
    char opcode[7]={'\0'};
    int i,j,n=0;
    FILE*fp=fopen(argv[1],"rb");

    if(fp==NULL){ //스트림 오류 처리
        printf("스트림 생성시 오류 발생");
        return 1;
    }

    while(!feof(fp)){
        //instruction 하나만큼 읽어오기
        fread((Word*)&word, sizeof(word), 1, fp);
        
         // little endian을 Big endian으로 바꿔주기
        for (i=0; i<2; i++){
            temp=word.inst_arr[i];
            word.inst_arr[i]=word.inst_arr[3-i];
            word.inst_arr[3-i]=temp;
        }
        printf("\ninst %2d : %08x\n",n,word.inst_num);
        //2진수를 문자열로 저장
        if (word.inst_num>=0){
            for (int i=31; i>-1; i--){
                inst[i]=(word.inst_num%2==0)?'0':'1';
                word.inst_num=word.inst_num/2;
            }
        }
        else{
            word.inst_num=2147483647-word.inst_num;
            for (int i=31; i>0; i--){
                inst[i]=(word.inst_num%2==0)?'0':'1';
                word.inst_num=word.inst_num/2;
            }
            inst[0]='1';
        }

        for (int i=0; i<6; i++){
            printf("%c",inst[i]);
        }
        instruction(inst);
        n++;
    }
    fclose(fp);
    return 0;
}

