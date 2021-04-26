#include <stdio.h>
#include <string.h>

// Arithmetic/logical: add, sub, and, or, slt, 
// Arithmetic/logical with immediate: addi, andi, ori, lui, slti
// Memory access: lw, sw, lh, lhu, sh, lb, lbu, sb
// Control transfer: beq, bne, j
// Shift instructions: sll, srl\

// lb,lh : sign-extension
// lbu, lhu : zero-extension

int uint(char* inst, int n){
    int sum=0;
    for (int i=0;i<n; i++){
        sum+=(inst[i]-48)*(1<<(n-1-i));
    }
    return sum;
}

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

typedef union Word{
    int inst_num; 
    unsigned char inst_arr[4];
} Word;

void Rinst(char* inst){
    int rs=uint(inst+6,5);
    int rt=uint(inst+11,5);
    int rd=uint(inst+16,5);
    int sa=uint(inst+21,5);

    if (!strncmp(inst+26,"100000",6)){
        printf(" add $%d, $%d, $%d", rd,rs,rt);

    }

    else if (!strncmp(inst+26,"100100",6)){
        printf(" and $%d, $%d, $%d", rd,rs,rt);

    }

    else if (!strncmp(inst+26,"100101",6)){
        printf(" or $%d, $%d, $%d", rd,rs,rt);

    }
    else if (!strncmp(inst+26,"000000",6)){
        printf(" sll $%d, $%d, %d", rd,rt,sa);

    }

    else if (!strncmp(inst+26,"101010",6)){
        printf(" slt $%d, $%d, $%d", rd,rs,rt);

    }

    else if (!strncmp(inst+26,"000010",6)){
        printf(" srl $%d, $%d, %d", rd,rt,sa);

    }

    else if (!strncmp(inst+26,"100010",6)){
        printf(" sub $%d, $%d, $%d", rd,rs,rt);

    }

    else{
        printf("unknown instruction");
    }
}

void Iinst(char* inst){
    int rs=uint(inst+6,5);
    int rt=uint(inst+11,5);
    int immediate=sint(inst+16, 16);

    if (!strncmp(inst,"001000",6)){
        printf(" addi $%d, $%d, %d",rt,rs,immediate);

    }

    else if (!strncmp(inst,"001100",6)){
        printf(" andi $%d, $%d, %d",rt,rs,immediate);

    }
    else if (!strncmp(inst,"000100",6)){
        printf(" beq $%d, $%d, %d",rs,rt,immediate);

    }
    else if (!strncmp(inst,"000101",6)){
        printf(" bne $%d, $%d, %d",rs,rt,immediate);

    }
    else if (!strncmp(inst,"100000",6)){
        printf(" lb $%d, %d($%d)",rt, immediate, rs);

    }
    else if (!strncmp(inst,"100100",6)){
        printf(" lbu $%d, %d($%d)",rt,immediate,rs);

    }
    else if (!strncmp(inst,"100001",6)){
        printf(" lh $%d, %d($%d)",rt,immediate,rs);

    }
    else if (!strncmp(inst,"100101",6)){
        printf(" lhu $%d, %d($%d)",rt,immediate,rs);

    }
    else if (!strncmp(inst,"001111",6)){
        printf(" lui $%d, %d",rt,immediate);

    }
    else if (!strncmp(inst,"100011",6)){
        printf(" lw $%d, %d($%d)",rt,immediate,rs);

    }
    else if (!strncmp(inst,"001101",6)){
        printf(" ori $%d, $%d, %d",rt,rs,immediate);

    }
    else if (!strncmp(inst,"101000",6)){
        printf(" sb $%d, %d($%d)",rt,immediate,rs);

    }
    else if (!strncmp(inst,"001010",6)){
        printf(" slti $%d, $%d, %d",rt,rs,immediate);

    }
    else if (!strncmp(inst,"101001",6)){
        printf(" sh $%d, %d($%d)",rt,immediate,rs);

    }
    else if (!strncmp(inst,"101011",6)){
        printf(" sw $%d, %d($%d)",rt,immediate,rs);

    }
    else {
        printf("unknown instruction");
    }
}

void Jinst(char* inst){
    int address=uint(inst+6,26);
    if (!strncmp(inst,"000010",6)){
        printf(" j %d",address);

    }
    else{
        printf("unknown instruction");
    }
}


void implement(char* inst){
    if (!strncmp(inst,"000000",6)){
        Rinst(inst);
    }
    else if (!strncmp(inst,"00001",5)){
        Jinst(inst);
    }
    else if (strncmp(inst,"0100",4)){
        Iinst(inst);
    }
    else{
        printf("unknown instruction");
    }
};

void print_register(int* Register, int PC){
    for (int i=0; i<31; i++){
        printf("$%d: %x\n",i,Register[i]);
    }
    printf("PC: %x",PC);

}

int main(int argc, char*argv[]){

    int PC=0x00000000;
    int Register[31]={0};
    int Instruction[0x00010000]={0xFF};
    int Data[0x00010000]={0xFF};
    int *pc=&PC;


    Word word;
    char inst[32];
    unsigned char temp;
    int i,j,n=0;

    // 매개 변수들 정리
    FILE*fp=fopen(argv[1],"rb"); //입력 파일
    int N=*argv[2]-48; //instruction 개수
    char* option=argv[3]; //print option


    if(fp==NULL){ //스트림 오류 처리
        printf("스트림 생성시 오류 발생");
        return 1;
    }
 
    // Your program reads the file and load the binary instructions to address
    // 0x00000000 of the instruction memory. 
    while(!feof(fp)){
        //instruction 하나만큼 읽어오기
        if(fread((Word*)&word, sizeof(word), 1, fp)==0)break;
        
         // little endian을 Big endian으로 바꿔주기
        for (i=0; i<2; i++){
            temp=word.inst_arr[i];
            word.inst_arr[i]=word.inst_arr[3-i];
            word.inst_arr[3-i]=temp;
        }
        Instruction[n]=word.inst_num;
        n++;
    }
    fclose(fp);

    // Your program simulates each instruction one-by-one, up to N instructions.
    for (j=0; j<N; j++){
        //2진수를 문자열로 저장
        word.inst_num=Instruction[PC];
        if (word.inst_num>=0){
            for (int i=31; i>-1; i--){
                inst[i]=(word.inst_num%2==0)?'0':'1';
                word.inst_num=word.inst_num/2;
            }
        }
        else{
            word.inst_num=2147483648-word.inst_num;
            for (int i=31; i>0; i--){
                inst[i]=(word.inst_num%2==0)?'0':'1';
                word.inst_num=word.inst_num/2;
            }
            inst[0]='1';
        }
        implement(inst);
    }

    print_register(Register,PC);

    return 0;
}