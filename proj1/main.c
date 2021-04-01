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

void Rinst(char* inst){
    int rs=uint(inst+6,5);
    int rt=uint(inst+11,5);
    int rd=uint(inst+16,5);
    int sa=uint(inst+21,5);

    if (!strncmp(inst+26,"100000",6)){
        printf(" add $%d, $%d, $%d", rd,rs,rt);

    }
    else if (!strncmp(inst+26,"100001",6)){
        printf(" addu $%d, $%d, $%d", rd,rs,rt);

    }
    else if (!strncmp(inst+26,"100100",6)){
        printf(" and $%d, $%d, $%d", rd,rs,rt);

    }
    else if (!strncmp(inst+26,"011010",6)){
        printf(" div $%d, $%d",rs,rt);

    }
    else if (!strncmp(inst+26,"011011",6)){
        printf(" divu $%d, $%d",rs,rt);

    }
    else if (!strncmp(inst+26,"001001",6)){
        printf(" jalr $%d, $%d", rd,rs);

    }
    else if (!strncmp(inst+26,"001000",6)){
        printf(" jr $%d",rs);

    }
    else if (!strncmp(inst+26,"010000",6)){
        printf(" mfhi $%d", rd);

    }
    else if (!strncmp(inst+26,"010010",6)){
        printf(" mflo $%d", rd);

    }
    else if (!strncmp(inst+26,"010001",6)){
        printf(" mthi $%d", rs);

    }
    else if (!strncmp(inst+26,"010011",6)){
        printf(" mtlo $%d", rs);

    }
    else if (!strncmp(inst+26,"011000",6)){
        printf(" mult $%d, $%d",rs,rt);

    }
    else if (!strncmp(inst+26,"011001",6)){
        printf(" multu $%d, $%d",rs,rt);

    }
    else if (!strncmp(inst+26,"100111",6)){
        printf(" nor $%d, $%d, $%d", rd,rs,rt);

    }
    else if (!strncmp(inst+26,"100101",6)){
        printf(" or $%d, $%d, $%d", rd,rs,rt);

    }
    else if (!strncmp(inst+26,"000000",6)){
        printf(" sll $%d, $%d, %d", rd,rt,sa);

    }
    else if (!strncmp(inst+26,"000100",6)){
        printf(" sllv $%d, $%d, $%d", rd,rt,rs);

    }
    else if (!strncmp(inst+26,"101010",6)){
        printf(" slt $%d, $%d, $%d", rd,rs,rt);

    }
    else if (!strncmp(inst+26,"101011",6)){
        printf(" sltu $%d, $%d, $%d", rd,rs,rt);

    }
    else if (!strncmp(inst+26,"000011",6)){
        printf(" sra $%d, $%d, %d", rd,rt,sa);

    }
    else if (!strncmp(inst+26,"000111",6)){
        printf(" srav $%d, $%d, $%d", rd,rt,rs);

    }
    else if (!strncmp(inst+26,"000010",6)){
        printf(" srl $%d, $%d, %d", rd,rt,sa);

    }
    else if (!strncmp(inst+26,"000110",6)){
        printf(" srlv $%d, $%d, $%d", rd,rt,rs);

    }
    else if (!strncmp(inst+26,"100010",6)){
        printf(" sub $%d, $%d, $%d", rd,rs,rt);

    }
    else if (!strncmp(inst+26,"100011",6)){
        printf(" subu $%d, $%d, $%d", rd,rs,rt);

    }
    else if (!strncmp(inst+26,"001100",6)){
        printf(" syscall");

    }
    else if (!strncmp(inst+26,"100110",6)){
        printf(" xor $%d, $%d, $%d", rd,rs,rt);

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
    else if (!strncmp(inst,"001001",6)){
        printf(" addiu $%d, $%d, %d",rt,rs,immediate);

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
    else if (!strncmp(inst,"001011",6)){
        printf(" sltiu $%d, $%d, %d",rt,rs,immediate);

    }
    else if (!strncmp(inst,"101001",6)){
        printf(" sh $%d, %d($%d)",rt,immediate,rs);

    }
    else if (!strncmp(inst,"101011",6)){
        printf(" sw $%d, %d($%d)",rt,immediate,rs);

    }
    else if (!strncmp(inst,"001110",6)){
        printf(" xori $%d, $%d, %d",rt,rs,immediate);

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
    else if (!strncmp(inst,"000011",6)){
        printf(" jal %d",address);

    }
    else{
        printf("unknown instruction");
    }
}




void instruction(char* inst){
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
        if(fread((Word*)&word, sizeof(word), 1, fp)==0)break;
        
         // little endian을 Big endian으로 바꿔주기
        for (i=0; i<2; i++){
            temp=word.inst_arr[i];
            word.inst_arr[i]=word.inst_arr[3-i];
            word.inst_arr[3-i]=temp;
        }
        printf("\ninst %2d : %08x ",n,word.inst_num);
        //2진수를 문자열로 저장
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
        instruction(inst);
        n++;
    }
    fclose(fp);
    return 0;
}

