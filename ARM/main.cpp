



#include<iostream>
#include<cmath>

using namespace std;
void simulate (unsigned short);



char C, V, Z, N;

unsigned char Memory[1024];
unsigned int Regs[16] ;

#define PC Regs[15]
#define LR Regs[14]
#define SP Regs[13]

int main()
{
    
    
    FILE *fp;
    unsigned short inst_word;
    
    fp = fopen("/Users/shahdsherif/Downloads/Archive(1)/sum.f.s.bin","rb");
    
    if(NULL==fp) {
        printf("Cannot open the file\n");
        exit(0);
    }
    
    while(fread(&inst_word, 2,1, fp))
    {
        //printf("%08x\t%04x\t", Regs[15], inst_word);
        simulate(inst_word);
        Regs[15] += 2;
    }
    fclose(fp);
    cout << Regs[0];
    
    return 0;
}
void simulate (unsigned short meow)
{
    unsigned short hi = meow >> 13;
    switch (hi)
    {
        case 0:
        {
            if ( ((meow & 0x1800) >> 11) < 3)  // Format 1
            {
                int offset = static_cast<int>((meow & 0x07C0) >> 6);
                int Rd = static_cast<int>(meow & 0x0007);
                int Rs = static_cast<int>((meow & 0x0038) >> 3);
                switch ((meow & 0x1800) >> 11)  // Op code bits = meow & (0001 1000 0000 0000)
                {
                    case 0: // LSL
                        printf("LSL R%d, R%d, #%d", Rd, Rs, offset);
                        cout << endl;
                        Regs[Rd] = Regs[Rs] << offset;
                        break;
                    case 1: // LSR
                        printf("LSR R%d, R%d, #%d", Rd, Rs, offset);
                        cout << endl;
                        Regs[Rd] = Regs[Rs] >> offset;
                        break;
                    case 2: // ASR
                        printf("ASR R%d, R%d, #%d", Rd, Rs, offset);
                        cout << endl;
                        Regs[Rd] = Regs[Rs] / (offset >= 0 ? pow(2, offset) : 1);
                        break;
                }
            }
            else    // Format 2 (Add/Sub)
            {
                int RnOffset = static_cast<int>((meow & 0x01C0) >> 6);
                int Rd = static_cast<int>(meow & 0x0007);
                int Rs = static_cast<int>((meow & 0x0038) >> 3);
                bool iFlag = static_cast<bool>((meow & 0x0400) >> 10), op = static_cast<bool>((meow & 0x0200) >> 9);
                if (iFlag)  // Immediate operand
                {
                    if (op) // Sub
                    {
                        printf("SUB R%d, R%d, #%d", Rd, Rs, RnOffset);
                        cout << endl;
                        Regs[Rd] = Regs[Rs] - RnOffset;
                    }
                    else    // Add
                    {
                        printf("ADD R%d, R%d, #%d", Rd, Rs, RnOffset);
                        cout << endl;
                        Regs[Rd] = Regs[Rs] + RnOffset;
                    }
                }
                else    // Reg operand
                {
                    if (op) // Sub
                    {
                        printf("SUB R%d, R%d, R%d", Rd, Rs, RnOffset);
                        cout << endl;
                        Regs[Rd] = Regs[Rs] - Regs[RnOffset];
                    }
                    else    // Add
                    {
                        printf("ADD R%d, R%d, R%d", Rd, Rs, RnOffset);cout << endl;
                        Regs[Rd] = Regs[Rs] + Regs[RnOffset];
                    }
                }
            }
            
        }
            break;
        case 1:
        { //format 3
            int offset = (int)(meow & 0b0000000011111111);
            int Rd = (int) ((meow >> 8) & 0b0000000000000111);
            switch ((meow >> 11) & 0b0000000000000011) { //the 2 op code bits
                case 0:
                {cout << "MOV R" << Rd << ", #" << offset << endl;
                    Regs[Rd] = offset;
                    //PC++?
                    
                }
                    break;
                case 1:
                {cout << "CMP R" << Rd << ", #" << offset << endl;
                    if (Regs[Rd] - offset == 0)
                        Z ='1';
                    else
                        Z = '0';
                    
                    //Pc?
                    
                }
                    break;
                case 2:
                {cout << "ADD R" << Rd << ", #" << offset << endl;
                    Regs[Rd]+= offset;
                    //Pc?
                }
                    break;
                case 3:
                {cout << "SUB R" << Rd << ", #" << offset << endl;
                    
                    Regs[Rd] -=offset;
                    //PC?
                }
                    break;
                    
            }
        }
            break;
        case 2:
            
            switch (meow >> 11 & 0b0000000000000011 ) { //now we have the two bits after the 3 bits that tell us we're in case 2.
                case 0:
                    if ((meow >> 10 & 0b0000000000000001)== 0) { //format 4
                        int Rd = (int) meow & 0b0000000000000111;
                        int Rs = (int) meow >>3 &0b0000000000000111;
                        switch (meow >> 6 & 0b0000000000001111) {
                                
                            case 0:
                            {
                                cout << "AND R" << Rd << ", R" << Rs << endl;
                                Regs[Rd] &= Regs[Rs];
                                //PC?
                                
                            }
                                break;
                            case 1:
                            { cout << "EOR R" << Rd << ", R" << Rs << endl;
                                Regs[Rd] ^=Regs[Rs];
                                //Pc?
                            }
                                break;
                            case 2:
                            {cout << "LSL R" << Rd << ", R" << Rs<< endl;
                                Regs[Rd] <<=Regs[Rs];
                                //Pc?
                                
                            }
                                break;
                            case 3:
                            {cout << "LSR R" << Rd << ", R" << Rs<< endl;
                                Regs[Rd] >>= Regs[Rs];
                                //PC?
                            }
                                break;
                            case 4:
                            {cout << "ASR R" << Rd << ", R" << Rs<< endl;
                                
                                //how am I supposed ot do an arithmatic shift?
                            }
                                break;
                            case 5:
                            {cout << "ADC R" << Rd << ", R" << Rs<< endl;
                                //how am I supposed to get the carry bit?
                            }
                                break;
                            case 6:
                            { cout << "SBC R" << Rd << ", R" << Rs<< endl;
                                //same, how am I supposed to get the carry bit?
                            }
                                break;
                            case 7:
                            {  cout << "ROR R" << Rd << ", R" << Rs<< endl;
                                //how should i rotate?
                            }
                                break;
                            case 8:
                            {cout << "TST R" << Rd << ", R" << Rs<< endl;
                                //shoud i really update the condition flags?
                                
                            }
                                break;
                            case 9:
                            {cout << "NEG R" << Rd << ", R" << Rs<< endl;
                                
                                Regs[Rd] = -1 * Regs[Rs];
                                //Pc?
                                
                            }
                                break;
                            case 10:
                            {cout << "CMP R" << Rd << ", R" << Rs<< endl;
                                //should i cmp? Rd-Rs?
                            }
                                break;
                            case 11:
                            { cout << "CMN R" << Rd << ", R" << Rs<< endl;
                                //cmp Rs-Rd?
                            }
                                break;
                            case 12:
                            {cout << "ORR R" << Rd << ", R" << Rs<< endl;
                                Regs[Rd] |= Regs[Rs];
                                //Pc
                            }
                                break;
                            case 13:
                            {cout << "MUL R" << Rd << ", R" << Rs<< endl;
                                Regs[Rd] *= Regs[Rs];
                                //PC
                            }
                                break;
                            case 14:
                            {cout << "BIC R" << Rd << ", R" << Rs<< endl;
                                Regs[Rd] = Regs[Rd] & (~Regs[Rs]);
                                //Pc?
                            }
                                break;
                            case 15:
                            {cout << "MVN R" << Rd << ", R" << Rs<< endl;
                                Regs[Rd] = ~Regs[Rs];
                            }
                                break;
                                
                        }
                    }
                    else
                    {   //format 5
                        int Rd,Hd, Rs, Hs;
                        Rd  = (int) meow & 0b0000000000000111;
                        Hd = Rd + 8;
                        Rs = (int) meow >> 3 &0b0000000000000111;
                        Hs = Rs +8;
                        
                        switch (meow >> 6 & 0b0000000000001111) {
                                
                                
                            case 1:
                            {cout << "ADD R" << Rd << ", R" << Hs << endl;
                                
                                Regs[Rd] += Regs[Hs];
                                
                            } break; //rd, hs
                            case 2:
                            {cout << "ADD R" << Hd << ", R" << Rs <<endl;
                                
                                Regs[Hd] += Regs[Rs];
                            } break; //hd, rs
                            case 3:
                            {cout << "Add R" << Hd <<", R" << Hs << endl;
                                Regs[Hd] += Regs[Hs];
                                
                            }break; //Hd, Hs
                            case 5://says i shouldln't update for format 5
                                cout << "CMP R" << Rd << ", R" << Hs << endl; //Rd, Hs
                            case 6:
                                cout << "CMP R" << Hd << ", R" << Rs << endl;// CMP Hd, Rs
                                break;
                            case 7:
                                cout << "CMP R" << Hd << ", R" << Hs << endl;//CMP Hd, Hs
                                break;
                            case 9:
                            {cout << "MOV R" << Rd << ", R" << Hs << endl;
                                Regs[Rd] = Regs[Hs];
                                
                            }//MOV Rd, Hs
                                break;
                            case 10:
                            {cout << "MOV R" << Hd << ", R" << Rs << endl;
                                Regs[Hd] = Regs[Rs];
                                
                            }//Mod Hd, Rs
                                break;
                            case 11:
                            {cout << "MOV R" << Hd << ", R" << Hs << endl;
                                Regs[Hd] = Regs[Hs];
                            }//Mod Hd, Hs
                                break;
                            case 12:
                            {cout << "BX R"<< Rs <<endl;
                                Regs[15] = Regs[Rs];
                            }
                                break;
                            case 13:
                            { cout << "Bx R" << Hs << endl;
                                if (Hs == 15)
                                    if (Regs[15] % 4 !=0) {
                                        cout << "you are branching to the PC, from a non word aligned address, therefore the execution will be unpredictable" << endl;
                                        Regs[15]= (Regs[15] & 0b01111111111111111111111111111111) + 4;
                                    }
                                    else Regs[15]= (Regs[15] & 0b01111111111111111111111111111111) + 4;
                                
                            }
                                break;
                            default:
                                cout << "undefined";
                                break;
                                break;
                                
                        }
                    }
                    break;
                    
                case 1: //format 6
                {
                    int imm = meow & 0b0000000011111111;
                    imm = imm<<2;
                    int Rd = (int) meow >> 8 & 0b0000000000000111;
                    
                    cout << "LDR R" << Rd << ", [PC, #" << imm << "]" << endl;
                    Regs[Rd] = Memory[imm+Regs[15]];
                    
                }
                    
                    break;
                default:
                    if ((meow >> 9 & 0b0000000000000001) == 0)
                    { // format 7
                        int Rd, Ro, Rb;
                        Rd = meow & 0b0000000000000111;
                        Rb = meow >> 3 &0b0000000000000111;
                        Ro = meow >> 6 &0b0000000000000111;
                        switch (meow >> 10 & 0b0000000000000011) {
                            case 0:
                            {cout << "STR R" << Rd << ", [R" << Rb << ", R" << Ro << "]" << endl;
                                Memory[Regs[Rb] + Regs[Ro]] = Regs[Rd];
                            }
                                break;
                            case 1:
                            {cout << "STRB R" << Rd << ", [R" << Rb << ", R" << Ro << "]" << endl;
                                Memory[Regs[Rb] + Regs[Ro]] = Regs[Rd];
                                
                            }
                                break;
                            case 2:
                            {cout << "LDR R" << Rd << ", [R" << Rb << ", R" << Ro << "]" << endl;
                                Regs[Rd] = Memory[Regs[Rb] + Regs[Ro]];
                                
                            }
                                break;
                            case 3:
                            {cout << "LDRB R" << Rd << ", [R" << Rb << ", R" << Ro << "]" << endl;
                                Regs[Rd] = Memory[Regs[Rb] + Regs[Ro]];
                            }
                                break;
                                
                        }
                    }
                    else //format 8
                    {
                        int Rd, Ro, Rb;
                        Rd = meow & 0b0000000000000111;
                        Rb = meow >> 3 &0b0000000000000111;
                        Ro = meow >> 6 &0b0000000000000111;
                        switch (meow >> 10 & 0b0000000000000011) {
                            case 0:
                                cout << "STRH R" << Rd << ", [R" << Rb << ", R" << Ro << "]" << endl;
                                break;
                            case 1:
                                cout << "LDSB R" << Rd << ", [R" << Rb << ", R" << Ro << "]" << endl;
                                break;
                            case 2:
                                cout << "LDRH R" << Rd << ", [R" << Rb << ", R" << Ro << "]" << endl;
                                break;
                            case 3:
                                cout << "LDSH R" << Rd << ", [R" << Rb << ", R" << Ro << "]" << endl;
                                break;
                                
                        }
                        
                    }
                    
                    break;
            }
            break;
            
            
            
            
            
            
            
        case 3:
        { //format 9 need switch on BL
            int imm, Rd, Rb;
            imm = (int) meow >> 6 &0b0000000000011111;
            int imm_b_zero= imm << 2;
            Rd = meow & 0b0000000000000111;
            Rb = meow >> 3 & 0b0000000000000111;
            switch (meow >> 11 & 0b0000000000000011){ //Switching on BL
                case 0:
                    cout << "STR R" << Rd << ", [R" << Rb << ", #" << imm_b_zero << "]"<< endl;
                    break;
                case 1:
                    cout << "LDR R" << Rd << ", [R" << Rb << ", #" << imm_b_zero << "]"<< endl;
                    break;
                case 2:
                    cout << "STRB R" << Rd << ", [R" << Rb << ", #" << imm << "]"<< endl;
                    break;
                case 3:
                    cout << "LDRB R" << Rd << ", [R" << Rb << ", #" << imm << "]"<< endl;
                    break;
                    
            }
            break;
            
        }
            ////////////////AYA'S PART/////////////////////////AYA'S PART /////////////////////////AYA'S PART//////////////////////////////
            ///////////HAS A PROBLEM//////////////////////////HAS A PROBLEM ///////////////////////////HAS A PROBLEM///////////////////////
            ////////REGISTER LIST////////////////////REGISTER LIST/////////////////////REGISTER LIST///////////////////////////////////////
            /////////////////DO NOT SET CONDITIONS CODES //////////////////////////////DO NOT SET CONDITIONS CODES/////////////////////////
            ///////////////AYA'S PART///////////////////AYA'S PART/////////////////////////////AYA'S PART///////////AYA'S PART/////////////
            /////////////////////////////CASE 5//////////////////////////CASE 5/////////////////////////////CASE 5/////////////////////////
            
        case 5:
            //bit12: used as an indicator 1 for me to indicate in which case I am in
            // to get indicator1: step1: shift meow 12 to the right. step2: anding it with one to get the first bit from the left.
            int indicator1= (int)((meow>>12)&0b0000000000000001);
            
            switch (indicator1)
        {
                //********************FORMAT 12*********************FORMAT 12**********************FORMAT 12***********************
            case 0://Format 12: Load address
            {
                int SPP;
                int Rd;
                int Word8;
                
                SPP= (int)((meow>>11)&0b0000000000000001);// Source: 0 -> PC  ,  1 -> SP
                Rd= (int)((meow>>8)&0b0000000000000111); //Destination Register
                Word8=(int)(meow &0b0000000011111111); // 8-bit unsigned constant
                
                int Imm;
                
                Imm=(int)((Word8<<2)&0b0000001111111111);// changing from 8 bit to 10 bit immediate value by addig two zeros in the right of the Word8
                //step1: shift Word 8 to the left by 2
                //step2: anding the new shifted int with 1023 (ten ones at the right in the binary), to get the first 10 bits form the right after being shifted by 2
                
                
                //(Imm ready now)
                
                
                //The addition depend on the value of the SPP
                // if SPP=0 then, Rd=Imm+PC;
                // if SPP=1 then, Rd=Imm+SP;
                
                switch(SPP)
                {
                    case 0:
                    {
                        cout << "ADD R" << Rd << ", [PC, #" << Imm << "]" << endl;
                        Regs[Rd] = Memory[Imm+PC];
                        
                    }
                        break;
                        
                    case 1:
                    {
                        cout << "ADD " << Rd << ", [SP, #" << Imm << "]" << endl;
                        //Regs[Rd] = Memory[Imm+SP];
                    }
                        break;
                }
                
                
                
            }
                break;
                //end of Format 12
                //*********************END OF FORMAT 12****************END OF FORMAT 12****************************************
                //*********************END OF FORMAT 12****************END OF FORMAT 12****************************************
                //*********************END OF FORMAT 12****************END OF FORMAT 12****************************************
                
                
                
                
                //*****************FORMAT 13 AND 14****************FORMAT 13 AND 14*******************FORMAT 13 AND 14*************
            case 1:
            {
                int indicator2; // indicator2 is the bit number 10
                //I use this bit to differate between fomat 13 and format 14
                // if indicator2 (bit10) =0. then, I am in format 13.
                // if indicator2 (bit10)= 1. then, I am in format 14.
                
                indicator2=(int)((meow>>10)& 0b0000000000000001);
                //step1: shift meow right by 10
                //step2: bit wise anding with 0b0000000000000001, to get the first fit from the right after being shifted.
                
                switch(indicator2)
                {
                    case 0:
                    {
                        
                        //Format 13: Add offset to the stack pointer
                        //***************FORMAT 13***********************FORMAT 13*************************FORMAT 13***********
                        
                        int S;  // Sign flag  (S=0 -> offset is positive , S=1 ->offset is negtive)
                        int SWord7;// 7-bit immediate value
                        
                        S=(int)((meow>>7) & 0b0000000000000001);
                        SWord7=(int)(meow & 0b0000000001111111);
                        
                        //SWord7 is 7-bit immediate value
                        //Step 1: I will transform those 7 bits to be 9 bits by shifting the number to the left by 2
                        //then, take the first 9 bits from the right
                        //Imm will be the result of this step (I mean step 1)
                        int Imm;
                        Imm=(int)((SWord7<<2) & 0b0000000111111111);
                        
                        
                        // if S=0 -> Imm is positive
                        // if S=1 -> Imm is negtive
                        switch(S)
                        {
                            case 0:
                            {
                                //Imm is positive in this case.
                                //that means that I will get the Imm as it is without changing anything on its value.
                                
                                cout << "ADD " << "SP" << ", #" << Imm <<endl;
                                SP = Memory[Imm+SP]; //add #Imm to the stack pointer
                                
                            }
                                break;
                                
                            case 1:
                            {
                                //Imm is negtive in this case.
                                //that means that I have to get its 2's complement first.
                                //HOW??
                                //Step1: get the 1's complement of the Imm first.
                                Imm=(int)(~Imm);
                                //Step2: add 1 to the Imm
                                Imm=(int)(Imm+0b0000000000000001);
                                
                                //After getting the 2's complement of the Imm, I can complete normally my code.
                                cout << "ADD " << "SP" << ", #" << Imm <<endl;
                                //SP=577;??????????
                                // SP = Memory[SP+Imm]; //add #Imm after getting its 2's complement to the stack pointer
                            }
                                break;
                                
                        }
                        
                        
                    }
                        break;
                        //end of Format 13
                        //*********************END OF FORMAT 13****************END OF FORMAT 13**************************************
                        
                        
                        
                    case 1:
                    {
                        //Format 14:push/pop registers
                        //***************FORMAT 14***********************FORMAT 14*************************FORMAT 14***********
                        
                        int Rlist; //Register List
                        Rlist=(int)(meow & 0b0000000011111111);
                        
                        
                        //###############################################################################################
                        int count;//count number of ones in the Rlist, to know how many registers I will work with after that.
                        count=0;
                        
                        int spare[8];//I open an array called spare of size 8 , because the number of bits in the Rlist is 8.
                        //Here, I fill the array from spare[0] to spare[7] with a value in that location of the bit.
                        // location of bit -> i.
                        for(int i=0; i<=7; i++){
                            spare[i]=(int)((Rlist>>i)&(0b0000000000000001));
                            //cout<<spare[i]<<" ";
                        }
                        
                        
                        //Then, I will go through the all array, (spare array) that I just did.
                        //if there is 1 in the location of i in the array spare, I will increment the value of count. Otherwise, i will do nothing.
                        for(int i=0; i<=7; i++){
                            if(spare[i]==0b0000000000000001) count++;} //???????????????
                        
                        
                        //################################################################################################
                        
                        int R;// PC/LR bit (0-> do not store LR/ load PC   ,    1-> Store LR/ Load PC)
                        R=(int)((meow>>8)&0b0000000000000001);
                        
                        if(R==0b0000000000000001) //means that I will gonna to store LR/ or load PC.
                            //that means the number of registers that I will gonna deal with will be incremented by one, which
                            //is PC or LR
                        {
                            count=count+1;
                        }
                        
                        //###############################################################################################
                        
                        int L; // Load/Store Bit , to know whether I will load from the memory or store in the memory
                        //if L=0-> Store to memory.
                        //if L=1-> Load from memory.
                        L=(int)((meow>>11)&0b0000000000000001);
                        
                        //###############################################################################################
                        int stackaddress;
                        int copySP;
                        
                        switch(L) //??????????????????stack
                        //??????????? I do not save workspace at the start of the subroutine and I do not return address
                        //??????????? Are those two important?
                        {
                                //#############################################START CASE 0 IN FORMAT 14:PUSH ONTO THE STACK
                                //#############################################START CASE 0 IN FORMAT 14:PUSH ONTO THE STACK
                            case 0: //L=0 -> store to memory (push registers)
                            {
                                if(R==0b0000000000000000)//push registers specified in Rlist onto the stack
                                {if(count>0) cout<<"PUSH "<<"{";
                                    for(int i=0; i<=7; i++){
                                        if(spare[i]==0b0000000000000001){
                                            //if that happen, I will push the register i onto the stack
                                            //??????????
                                            //I will get the value of the SP in sth. called stackaddress.
                                            stackaddress=SP;
                                            //I will save space in the memory
                                            //count->number of registers I wanna to load in the memory
                                            //each register contains instruction word of 16 bits (2 bytes) ??????
                                            //for each register I will save 2 places in the memory for them
                                            stackaddress= stackaddress-(count*2);
                                            //????????????? how i will fill the stack from top to bottom or from bottom to top ot it does not matter?
                                            //The stack is always assumed to be full decending
                                            //for store and push, I will decrement first.
                                            //update stack pointer
                                            SP=stackaddress;
                                            //save original value of SP in storeSP
                                            copySP=stackaddress;
                                            
                                            if(i<=6){//load/push register number i onto the stack
                                                Memory[copySP]= (int)(Regs[i] & 0b0000000011111111);//LEAST SIGNIFICANT 8 BITS
                                                Memory[copySP+1]=(int)((Regs[i]>>8) & 0b0000000011111111);//MOST SIGINIFICANT 8 BITS????????
                                                //increment stackaddress by 2 //??????? increment stack pointer by 2 or what??
                                                stackaddress=stackaddress+2;
                                                cout<<"R"<< i <<",";}
                                            
                                            
                                            else{//load/push register number i onto the stack
                                                Memory[copySP]= (int)(Regs[i] & 0b0000000011111111);//LEAST SIGNIFICANT 8 BITS
                                                Memory[copySP+1]=(int)((Regs[i]>>8) & 0b0000000011111111);//MOST SIGINIFICANT 8
                                                //increment stackaddress by 2 //??????? increment stack pointer by 2 or what??
                                                stackaddress=stackaddress+2;
                                                cout<<"R"<<i<<"}"<<endl;}}}}
                                
                                //####################ELSE OF CASE 0 IN FORMAT 14:PUSH LIST AND LR ONTO STACK##################
                                //####################ELSE OF CASE 0 IN FORMAT 14:PUSH LIST AND LR ONTO STACK##################
                                
                                else
                                    // that means that R=1 -> push the link register and the registers specified in the Rlist onto the stack
                                {
                                    if(count>0) cout<<"PUSH "<<"{";
                                    //??????? Is the order of puhing things matters or not?
                                    //??????? I mean have I push the link register first or it doesnot matter ????
                                    
                                    for(int i=0; i<=7; i++){
                                        if(spare[i]==0b0000000000000001){
                                            //if that happen, I will push the register i in the stack.
                                            
                                            //push register number i in the stack.
                                            
                                            //??????????
                                            //I will get the value of the SP in sth. called stackaddress.
                                            stackaddress=SP;
                                            
                                            //I will save space in the memory
                                            //count->number of registers I wanna to load in the memory
                                            //each register contains instruction word of 16 bits (2 bytes) ??????
                                            //for each register I will save 2 places in the memory for them
                                            stackaddress= stackaddress-(count*2);
                                            
                                            //????????????? how i will fill the stack from top to bottom or from bottom to top ot it does not matter?
                                            //The stack is always assumed to be full decending
                                            //for store and push, I will decrement first.
                                            
                                            //update stack pointer
                                            SP=stackaddress;
                                            copySP=SP;
                                            
                                            if(i<=6){//load/push register number i onto the stack
                                                Memory[copySP]= (int)(Regs[i] & 0b0000000011111111);//LEAST SIGNIFICANT 8 BITS
                                                Memory[copySP+1]=(int)((Regs[i]>>8) & 0b0000000011111111);//MOST SIGINIFICANT 8
                                                copySP=copySP+1;
                                                //increment stackaddress by 2 //??????? increment stack pointer by 2 or what??
                                                stackaddress=stackaddress+2;
                                                cout<<"R"<<i<<",";}
                                            
                                            if(i==7){//load/push link register onto the stack
                                                Memory[copySP]= (int)(LR & 0b0000000011111111);//LEAST SIGNIFICANT 8 BITS
                                                Memory[copySP+1]=(int)((LR>>8) & 0b0000000011111111);//MOST SIGINIFICANT 8
                                                copySP=copySP+1;
                                                //increment stackaddress by 2 //??????? increment stack pointer by 2 or what??
                                                stackaddress=stackaddress+2;
                                                cout<<",LR "<<"}"<<endl;}
                                        }}}
                                
                            }break;
                                
                                //#############################################START CASE 1 IN FORMAT 14:POP FROM THE STACK
                                //#############################################START CASE 1 IN FORMAT 14:POP FROM THE STACK
                            case 1://L=1 ->load from memory (pop registers)
                            {
                                //for load and pop instructions, the stack pointer in the full descending is incremented after
                                
                                
                                if(R==0b0000000000000000){
                                    int copy1,copy2 ;
                                    copy1=SP; //least
                                    copy2=SP+1; //most
                                    //POP REGISTERS SPECIFIED IN RLIST FROM THE STACK
                                    //pop registers specified in Rlist from the stack
                                    if(count>0) cout<<"POP "<<"{";
                                    for(int i=0; i<=7; i++){
                                        if(spare[i]==0b0000000000000001){
                                            //if that happen, I will pop the register i in the stack.
                                            //pop register number i from the stack.
                                            //????????????? how i will fill the stack from top to bottom or from bottom to top ot it does not matter?
                                            //The stack is always assumed to be full decending
                                            //for load and pop instructions, the stack pointer in the full descending is incremented after
                                            
                                            if(i<=6){
                                                //pop register number i (16 bits) from the stack ??check plz??
                                                int least, most;
                                                least=(int)((Memory[copy1])& 0b0000000011111111);
                                                most=(int)((Memory[copy2]<<8) & 0b1111111100000000);
                                                Regs[i]= most | least;
                                                copy1=copy1+2;
                                                copy2=copy2+2;
                                                cout<<"R"<<i<<",";}
                                            
                                            if(i==7){
                                                //pop register number i (16 bits) from the stack ??check plz??
                                                int least, most;
                                                least=(int)((Memory[copy1])& 0b0000000011111111);
                                                most=(int)((Memory[copy2]<<8) & 0b1111111100000000);
                                                PC= most | least;
                                                copy1=copy1+2;
                                                copy2=copy2+2;
                                                cout<<"R"<<i<<"}";}}}
                                    
                                    //update stack pointer by incrementing it(after finishing the loop)
                                    SP=SP+(count*2);}
                                
                                //$$$$$$$$$$$$$$$$$$$CLOSE  THE POPING FROM THR STACK THE SPECFIED REGISTERS$$$$$$$$$$$$$$$$$$
                                
                                else{
                                    //for load and pop instructions, the stack pointer in the full descending is incremented after
                                    int copy1,copy2 ;
                                    copy1=SP; //least
                                    copy2=SP+1; //most
                                    
                                    //pop registers specified in Rlist and PC from the stack
                                    if(count>0) cout<<"POP "<<"{";
                                    
                                    for(int i=0; i<=7; i++){
                                        if(spare[i]==0b0000000000000001)
                                        {
                                            //if that happen, I will pop the register i in the stack.
                                            //pop register number i from the stack.
                                            //????????????? how i will fill the stack from top to bottom or from bottom to top ot it does not matter?
                                            //The stack is always assumed to be full decending
                                            //for load and pop instructions, the stack pointer in the full descending is incremented afterctions, the stack pointer in the full descending is incremented after
                                            
                                            if(i<=6){
                                                //pop register number i (16 bits) from the stack ??check plz??
                                                int least, most;
                                                least=(int)((Memory[copy1])& 0b0000000011111111);
                                                most=(int)((Memory[copy2]<<8) & 0b1111111100000000);
                                                Regs[i]= most | least;
                                                copy1=copy1+2;
                                                copy2=copy2+2;
                                                cout<<"R"<<i<<",";}
                                            
                                            if(i==7){
                                                //pop register number i (16 bits) from the stack ??check plz??
                                                int least, most;
                                                least=(int)((Memory[copy1])& 0b0000000011111111);
                                                most=(int)((Memory[copy2]<<8) & 0b1111111100000000);
                                                PC= most | least;
                                                copy1=copy1+2;
                                                copy2=copy2+2;
                                                cout<<",PC"<<"}"<<endl;}}}
                                    
                                    //update stack pointer by incrementing it(after finishing the loop)
                                    SP=SP+(count*2);}
                                //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
                            }
                                break;
                                
                        }
                        
                    }break;
                }}break;
                
                
                
                ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                ///////Start The Second Part of My Part Whose hi=6//////////////Start The Second Part of My Part Whose hi=6////////////////////
                ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            case 6:
            {
                int checkformat=(int)((meow>>12)&0b0000000000000001);
                //if checkformat=0. Then, I am in format 15:Multiple load/store.
                //if checkformat=1. Then, I am between formats 16 and 17. I still need one more check to determine which format from those twos.
                switch(checkformat)
                {
                    case 0: //format 15: multiplt load/store
                    {
                        int L=(int)((meow>>11)&0b0000000000000001);//load/store bit
                        int Rb=(int)((meow>>8)&0b0000000000000111);//Base Register ?????only 3 bits
                        int Rlist=(int)(meow&0b0000000011111111);//Register List
                        int countM;//count number of ones in the Rlist, to know how many registers I will work with after that.
                        countM=0;
                        int spareM[8];//I open an array called spare of size 8 , because the number of bits in the Rlist is 8.
                        //Here, I fill the array from spare[0] to spare[7] with a value in that location of the bit.
                        // location of bit -> i.
                        for(int i=0; i<=7; i++){
                            spareM[i]=(int)((Rlist>>i)&(0b0000000000000001));}
                        //Then, I will go through the all array, (spare array) that I just did.
                        //if there is 1 in the location of i in the array spare, I will increment the value of count. Otherwise, i will do nothing.
                        for(int i=0; i<=7; i++){ if(spareM[i]==0b0000000000000001) countM++;} //???????????????
                        
                        int baseaddress;
                        int copyb;
                        
                        if(L==0b0000000000000000)//store specified registers in Rlist in the memory
                        {if(countM>0) cout<<"STMIA R"<<Rb<<"!, { ";
                            for(int i=0; i<=7; i++){
                                if(spareM[i]==0b0000000000000001){
                                    //if that happen, I will store the register i in the memory
                                    //??????????
                                    //I will get the value in the Regs[Rb] in sth. called baseaddress.
                                    baseaddress=Regs[Rb];
                                    //I will save space in the memory
                                    //count->number of registers I wanna to load/store in the memory
                                    //each register contains instruction word of 16 bits (2 bytes) ??????
                                    //for each register I will save 2 places in the memory for them
                                    baseaddress= baseaddress-(countM*2);
                                    //????????????? how i will fill the memory from top to bottom or from bottom to top ot it does not matter?
                                    //The memory is always assumed to be full decending
                                    //for store and push, I will decrement first.
                                    //update Regs[Rb] containing actual base address
                                    Regs[Rb]=baseaddress;
                                    //save original value of baseaddress in copyb
                                    copyb=baseaddress;
                                    
                                    if(i<=6){//load/push register number i onto the stack
                                        Memory[copyb]= (int)(Regs[i] & 0b0000000011111111);//LEAST SIGNIFICANT 8 BITS
                                        Memory[copyb+1]=(int)((Regs[i]>>8) & 0b0000000011111111);//MOST SIGINIFICANT 8 BITS????????
                                        //increment baseaddress by 2 //??????? increment baseaddress by 2 or what??
                                        baseaddress=baseaddress+2;
                                        cout<<"R"<<i<<",";}
                                    
                                    
                                    if(i==7){//load/push register number i in the memory
                                        Memory[copyb]= (int)(Regs[i] & 0b0000000011111111);//LEAST SIGNIFICANT 8 BITS
                                        Memory[copyb+1]=(int)((Regs[i]>>8) & 0b0000000011111111);//MOST SIGINIFICANT 8
                                        //increment baseaddress by 2 //??????? increment baseaddress by 2 or what??
                                        baseaddress=baseaddress+2;
                                        cout<<"R"<<i<<"}"<<endl;}}}}
                        
                        else{int copy1,copy2 ;
                            copy1=Regs[Rb]; //least
                            copy2=Regs[Rb]+1; //most
                            //load from memory to REGISTERS SPECIFIED IN RLIST
                            if(countM>0) cout<<"LDMIA R"<<Regs[Rb]<<"!,{ ";
                            for(int i=0; i<=7; i++){
                                if(spareM[i]==0b0000000000000001){
                                    //if that happen, I will load from memory to register i
                                    //The memory is always assumed to be full decending
                                    //for load , the base address (Regs[Rb]) is incremented after
                                    
                                    if(i<=6){
                                        //load from memory to register number i (16 bits)  ??check plz??
                                        int least, most;
                                        least=(int)((Memory[copy1])& 0b0000000011111111);
                                        most=(int)((Memory[copy2]<<8) & 0b1111111100000000);
                                        Regs[i]= most | least;
                                        copy1=copy1+2;
                                        copy2=copy2+2;
                                        cout<<"R"<<i<<",";}
                                    
                                    if(i==7){
                                        //load from memory to register number i (16 bits) ??check plz??
                                        int least, most;
                                        least=(int)((Memory[copy1])& 0b0000000011111111);
                                        most=(int)((Memory[copy2]<<8) & 0b1111111100000000);
                                        PC= most | least;
                                        copy1=copy1+2;
                                        copy2=copy2+2;
                                        cout<<"R"<<i<<"}";}}}
                            
                            //update base address in the Regs[Rb] by incrementing it(after finishing the loop)
                            Regs[Rb]=Regs[Rb]+(countM*2);}
                    }break;
                        
                        
                        //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                    case 1:// format 16 and 17. Need one more check to determine which one of those two formats.
                    {
                        
                    }break;
                }
            }
                break;
                //////////////////END OF PART 2 OF MINE///////////////////END OF PART 2 OF MINE/////////////////END OF PART 2 OF MINE//////////
        case 4:
        {
            if (!((meow >> 12) & 0x0001))  // Format 10
            {
                bool L = static_cast<bool>((meow >> 11) & 0x0001);  // Load == 1 / Store == 0
                int offset = static_cast<int>((meow & 0x07C0) >> 6);
                int Rd = static_cast<int>((meow & 0x0007));
                int Rb = static_cast<int>((meow & 0x0038) >> 3);
                int address = Regs[Rb] + offset * 2 ;
                if (L)  // Load
                {
                    printf("LDRH R%d, [R%d, #%d]", Rd, Rb, offset);
                    Regs[Rd] = (Memory[address] & 0x00FF);
                }
                else    // Store
                {
                    printf("STRH R%d, [R%d, #%d]", Rd, Rb, offset);
                    Memory[address] = (Regs[Rd] & 0x00FF);
                    
                    
                }
            }
            else    // Format 11
            {
                bool L = static_cast<bool>((meow >> 11) & 0x0001);  // Load == 1 / Store == 0
                int imm = static_cast<int>(meow & 0x00FF);
                int Rd = static_cast<int>((meow & 0x0700) >> 8);  // Load == 1 / Store == 0
                int address = Regs[7] + imm * 4;
                if (L)  // Load
                {
                    printf("LDR R%d, [R7, #%d]", Rd, imm);
                    Regs[Rd] = Memory[address];
                    
                }
                else    // Store
                {
                    printf("STR R%d, [R7, #%d]", Rd, imm);
                    Memory[address] = Regs[Rd];
                }
            }
        }
            break;
        case 7:
        {
            bool op = static_cast<bool>((meow >> 12) & 0x0001);
            bool h = static_cast<bool>((meow >> 11) & 0x0001);
            int offset = static_cast<int>((meow & 0x07FF));
            if (op) // Format 18
            {
                Regs[15] += (offset - 4) << 1;
            }
            else    // Format 19
            {
                if (h)  // High offset
                {
                    
                }
                else    // Low offset
                {
                    
                }
            }
        }
            break;
    }
    
    }}
