#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Arithmetic/logical: add, sub, and, or, slt, 
// Arithmetic/logical with immediate: addi, andi, ori, lui, slti
// Memory access: lw, sw, lh, lhu, sh, lb, lbu, sb
// Control transfer: beq, bne, j
// Shift instructions: sll, srl

// lb,lh : sign-extension
// lbu, lhu : zero-extension

int Uint(char* inst, int n){
    int sum=0;
    for (int i=0;i<n; i++){
        sum+=(inst[i]-48)*(1<<(n-1-i));
    }
    return sum;
}

int Sint(char* inst, int n){
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

typedef struct block{
    unsigned int V;
    unsigned int D;
    unsigned int Tag;
    int Data[16];
}block;

typedef struct line{
    int LRU;
    block set[2];
}line;

int HIT=0;
int MISS=0;
int PC=0x00000000;
int Reg[32]={0, };
int Inst[0x00010000/4];
int Data[0x00010000/4];
line Cache[8];

int CacheLoad(int address){
    int tag=address>>9;
    int index=(address>>6)%(1<<3);
    int block=(address)%(1<<6);

    // printf("%x = tag : %x, index : %x, block : %x\n",address, tag, index, block/4);

    //Cache Hit
    for(int i=0;i<2;i++){
        if ((Cache[index].set[i].Tag==tag)&(Cache[index].set[i].V==1)){
            HIT++;
            Cache[index].LRU=1-i;
            return Cache[index].set[i].Data[block/4];
        }
    }

    //Cache Miss
    MISS++;
    int LRU=Cache[index].LRU;
    
    //Dirty Bit
    if(Cache[index].set[LRU].D==1){
        for (int i=0; i<16; i++){
            Data[(((address/4)>>4)<<4)+i]=Cache[index].set[LRU].Data[i];
        }
    }

    Cache[index].set[LRU].Tag=tag;
    for (int i=0; i<16; i++){
        Cache[index].set[LRU].Data[i]=Data[(((address/4)>>4)<<4)+i];
    }
    Cache[index].set[LRU].V=1;
    Cache[index].LRU=1-LRU;
    
    return Cache[index].set[LRU].Data[block/4];
}

int CacheStore(int address, int fix){
    int tag=address>>9;
    int index=(address>>6)%(1<<3);
    int block=(address)%(1<<6);

    // printf("%x = tag : %x, index : %x, block : %x\n",address, tag, index, block/4);
    
    //Cache Hit
    for(int i=0;i<2;i++){
        if ((Cache[index].set[i].Tag==tag)&(Cache[index].set[i].V==1)){
            HIT++;
            Cache[index].LRU=1-i;
            Cache[index].set[i].D=1;
            Cache[index].set[i].Data[block/4]=fix;
            return 0;
        }
    }

    //Cache Miss
    MISS++;
    int LRU=Cache[index].LRU;
    if(Cache[index].set[LRU].D==1){
        for (int i=0; i<16; i++){
            Data[(((address/4)>>4)<<4)+i]=Cache[index].set[LRU].Data[i];
        }
    }

    //Data fix and store
    Data[address/4]=fix;
    Cache[index].set[LRU].Tag=tag;
    for (int i=0; i<16; i++){
        Cache[index].set[LRU].Data[i]=Data[(((address/4)>>4)<<4)+i];
    }
    Cache[index].set[LRU].V=1;
    Cache[index].set[LRU].D=1;

    Cache[index].LRU=1-LRU;
    return 0;
}

int Rinst(char* inst){
    int rs=Uint(inst+6,5);
    int rt=Uint(inst+11,5);
    int rd=Uint(inst+16,5);
    int sa=Uint(inst+21,5);

    if (!strncmp(inst+26,"100000",6)){
        Reg[rd]=Reg[rs]+Reg[rt];
        // printf(" add $%d, $%d, $%d\n", rd,rs,rt);
    }

    else if (!strncmp(inst+26,"100001",6)){
        Reg[rd]=Reg[rs]+Reg[rt];
        // printf(" addu $%d, $%d, $%d", rd,rs,rt);

    }

    else if (!strncmp(inst+26,"100100",6)){
        Reg[rd]=Reg[rs]&Reg[rt];
        // printf(" and $%d, $%d, $%d\n", rd,rs,rt);

    }

    else if (!strncmp(inst+26,"100101",6)){
        Reg[rd]=Reg[rs]|Reg[rt];
        // printf(" or $%d, $%d, $%d\n", rd,rs,rt);

    }

    else if (!strncmp(inst+26,"000000",6)){
        Reg[rd]=Reg[rt]<<sa;
        // printf(" sll $%d, $%d, %d", rd,rt,sa);

    }


    else if (!strncmp(inst+26,"101010",6)){
        Reg[rd]=(Reg[rs]<Reg[rt])? 1:0;
        // printf(" slt $%d, $%d, $%d\n", rd,rs,rt);

    }

    else if (!strncmp(inst+26,"101011",6)){
        Reg[rd]=(Reg[rs]<Reg[rt])? 1:0;
        // printf(" sltu $%d, $%d, $%d\n", rd,rs,rt);

    }

    else if (!strncmp(inst+26,"000010",6)){
        Reg[rd]=(unsigned)Reg[rt]>>sa;
        // printf(" srl $%d, $%d, %d\n", rd,rt,sa);

    }

    else if (!strncmp(inst+26,"100010",6)){
        Reg[rd]=Reg[rs]-Reg[rt];
        // printf(" sub $%d, $%d, $%d\n", rd,rs,rt);

    }

    else if (!strncmp(inst+26,"001000",6)){
        PC=(Reg[rs]/4)-1;
        printf(" jr $%d",rs);
    }

    else{
        return 1;
    }
    return 0;
}

int Iinst(char* inst){
    int rs=Uint(inst+6,5);
    int rt=Uint(inst+11,5);
    short immediate=Sint(inst+16, 16);

    if (!strncmp(inst,"001000",6)){
        Reg[rt]=Reg[rs]+immediate;
        // printf(" addi $%d, $%d, %d\n",rt,rs,immediate);
    }

    else if (!strncmp(inst,"001001",6)){
        Reg[rt]=Reg[rs]+immediate;
        // printf(" addiu $%d, $%d, %d\n",rt,rs,immediate);

    }

    else if (!strncmp(inst,"001100",6)){
        Reg[rt]=Reg[rs]&(unsigned short)immediate;
        // printf(" andi $%d, $%d, %d\n",rt,rs,immediate);

    }
    else if (!strncmp(inst,"000100",6)){
        if (Reg[rs]==Reg[rt]){
            PC+=immediate;
        }
        // printf(" beq $%d, $%d, %d\n",rs,rt,immediate);

    }
    else if (!strncmp(inst,"000101",6)){
        if(Reg[rs]!=Reg[rt]){
            PC+=immediate;
        }
        // printf(" bne $%d, $%d, %d\n",rs,rt,immediate);
    }

    else if (!strncmp(inst,"100011",6)){
        Reg[rt]=CacheLoad(immediate+(Reg[rs]-0x10000000));
        // printf(" lw $%d, %d($%d)\n",rt,immediate,rs);

    }

    else if (!strncmp(inst,"100000",6)){
        Word word;
        word.inst_num=CacheLoad(immediate+(Reg[rs]-0x10000000));
        char a=word.inst_arr[3-(immediate+(Reg[rs]-0x10000000)%4)];

        Reg[rt]=a;

        // printf(" lb $%d, %d($%d)\n",rt, immediate, rs);

    }
    else if (!strncmp(inst,"100100",6)){
        Word word;
        // word.inst_num=Data[(immediate+(Reg[rs]-0x10000000))/4];
        word.inst_num=CacheLoad(immediate+(Reg[rs]-0x10000000));
        unsigned char a=word.inst_arr[3-(immediate+(Reg[rs]-0x10000000)%4)];
        Reg[rt]=a;

        // printf(" lbu $%d, %d($%d)\n",rt,immediate,rs);
        

    }
    else if (!strncmp(inst,"100001",6)){
        Word word;
        word.inst_num=CacheLoad(immediate+(Reg[rs]-0x10000000));
        
        if(((immediate+(Reg[rs]-0x10000000))%4)==0){
            short temp=word.inst_num;
            Reg[rt]=temp>>16;
        }
        else{
            word.inst_num=word.inst_num<<16;
            Reg[rt]=word.inst_num>>16;
        }
        
        // printf(" lh $%d, %d($%d)\n",rt,immediate,rs);

    }
    else if (!strncmp(inst,"100101",6)){
        Word word;
        word.inst_num=CacheLoad(immediate+(Reg[rs]-0x10000000));

        if(((immediate+(Reg[rs]-0x10000000))%4)==0){
            Reg[rt]=(unsigned short)word.inst_num>>16;
        }
        else{
            Reg[rt]=(unsigned short)word.inst_num&0x0000ffff;
        }    
        // printf(" lhu $%d, %d($%d)\n",rt,immediate,rs);

    }
    else if (!strncmp(inst,"001111",6)){
        Reg[rt]=immediate<<16;
        // printf(" lui $%d, %d\n",rt,immediate);

    }
    
    else if (!strncmp(inst,"001101",6)){
        Reg[rt]=Reg[rs]|(unsigned short)immediate;
        // printf(" ori $%d, $%d, %d\n",rt,rs,immediate);

    }
    
    else if (!strncmp(inst,"001010",6)){
        Reg[rt]=(Reg[rs]<immediate)?1:0;
        // printf(" slti $%d, $%d, %d\n",rt,rs,immediate);

    }

    
    else if (!strncmp(inst,"101011",6)){
        CacheStore(immediate+(Reg[rs]-0x10000000),Reg[rt]);
        // Data[(immediate+(Reg[rs]-0x10000000)))/4]=Reg[rt];
        // printf(" sw $%d, %d($%d)\n",rt,immediate,rs);
    }
    else if (!strncmp(inst,"101001",6)){
        int address=(immediate+(Reg[rs]-0x10000000));
        Word word1;
        Word word2;
        int tag=address>>9;
        int index=(address>>6)%(1<<3);
        int block=(address)%(1<<6);

        for(int i=0;i<2;i++){
        if ((Cache[index].set[i].Tag==tag)&(Cache[index].set[i].V==1)){
            word1.inst_num=Cache[index].set[i].Data[block/4];
             word2.inst_num=Reg[rt];

            word1.inst_arr[address%4]=word2.inst_arr[address%4];
            word1.inst_arr[address%4]=word2.inst_arr[address%4];
            // printf("%x..\n\n",word1.inst_num);

            CacheStore(address,word1.inst_num);
            return 0;
        }
    }
        word1.inst_num=Data[address/4];
        word2.inst_num=Reg[rt];

        word1.inst_arr[address%4]=word2.inst_arr[address%4];
        word1.inst_arr[address%4]=word2.inst_arr[address%4];
        // printf("%x..\n\n",word1.inst_num);

        CacheStore(address,word1.inst_num);
        // Data[(immediate+(Reg[rs]-0x10000000))/4]=word1.inst_num;

        // printf(" sh $%d, %d($%d)\n",rt,immediate,rs);
    }
    else if (!strncmp(inst,"101000",6)){
        int address=immediate+(Reg[rs]-0x10000000);
        Word word;
        int tag=address>>9;
        int index=(address>>6)%(1<<3);
        int block=(address)%(1<<6);

        for(int i=0;i<2;i++){
        if ((Cache[index].set[i].Tag==tag)&(Cache[index].set[i].V==1)){
            word.inst_num=Cache[index].set[i].Data[block/4];
                
            word.inst_arr[address%4]=Reg[rt];

            CacheStore(immediate+(Reg[rs]-0x10000000),word.inst_num);   
            return 0;
        }}

        word.inst_num=Data[address/4];
        word.inst_arr[address%4]=Reg[rt];

        CacheStore(immediate+(Reg[rs]-0x10000000),word.inst_num);
        // Data[(immediate+(Reg[rs]-0x10000000))/4]=word.inst_num;
        // printf(" sb $%d, %d($%d)\n",rt,immediate,rs);
        
    }
    else {
        return 1;
    }
    return 0;
}

int Jinst(char* inst){
    int address=Uint(inst+6,26);
    if (!strncmp(inst,"000010",6)){
        PC=address;
        printf(" j %d\n",address);
    }
    else if (!strncmp(inst,"000011",6)){
        Reg[31]=PC+1;
        PC=address;
        printf(" jal %d",address);
    }
    else{
        return 1;
    }
    return 0;
}


int implement(char* inst){
    if (!strncmp(inst,"000000",6)){
        PC++;
        return Rinst(inst);
    }
    else if (!strncmp(inst,"00001",5)){
        return Jinst(inst);
    }
    else if (strncmp(inst,"0100",4)){
        PC++;
        return Iinst(inst);
    }
    else{
        return 0;
    }
};

//레지스터 출력 함수
void print_register(){
    for (int i=0; i<32; i++){
        printf("$%d: 0x%08x\n",i,Reg[i]);
    }
    printf("PC: 0x%08x\n",PC*4);

}

// 메모리 출력 함수
void print_mem(unsigned int address, int num){
    for (int i=0; i<num; i++){
        printf("0x%08x\n",CacheLoad(address+i*4));
    }
}

// instruction 출력함수?
void print_instruction(unsigned int address){
    for (int i=0; i<4; i++){
        printf("0x%08x\n",Inst[address/4+i]);
    }
}



int main(int argc, char*argv[]){
    Word word;
    char inst[32];
    unsigned char temp;
    int i,j,n=0;

    // 매개 변수들 정리
    FILE*fp=fopen(argv[1],"rb"); //입력 파일
    int N=atoi(argv[2]); //instruction 개수
    char* option=argv[3]; //print option

    // 초기화
    for (int i=0; i< 0x10000/4; i++){
        Inst[i]=0xFFFFFFFF;
        Data[i]=0xFFFFFFFF;
    }


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
        Inst[n]=word.inst_num;
        n++;
    }
    fclose(fp);

    // Your program simulates each instruction one-by-one, up to N instructions.
    for (j=0; j<N; j++){
        //2진수를 문자열로 저장
        word.inst_num=Inst[PC];
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
        if(implement(inst)==1){
            printf("unknown instruction\n");
            j++;
            break;
        }
    }

    printf("Instructions: %d\n",j);
    printf("Hits: %d\n",HIT);
    printf("Misses: %d\n",MISS);
    printf("Total: %d\n", HIT+MISS);



    if(!strncmp(argv[3],"reg",3)){
        print_register();  
    }
    else if(!strncmp(argv[3],"mem",3)){
        char* ad=argv[4];
        unsigned int address=0;
        for(i=0; i<5; i++){
            // printf("%d = %d\n",ad[9-i]-48, 4*i);
            // printf("%x\n",(ad[9-i]-48)<<(4*i));
            if(ad[9-i]<58){
                address+=(ad[9-i]-48)<<(4*i);
            }
            else{
                address+=(ad[9-i]-87)<<(4*i);
            }
            
        }
        if (ad[2]=='1'){
            int num=atoi(argv[5]);
            printf("%d\n",num);
            print_mem(address,num);
        }
        else{
            print_instruction(address);
        }
    }
    else{
        return 0;
    }
    
    return 0;
}