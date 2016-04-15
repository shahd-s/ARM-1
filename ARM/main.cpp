@@ -1,15 +1,513 @@
#include<iostream>
#include<cmath>

using namespace std;
void simulate (unsigned short);
int Regs[16] ;char C, V, Z;
unsigned char Memory[1024];
int main()
{
    
    
    FILE *fp;
    unsigned short inst_word;
    
    fp = fopen("put the extension here for the bin file","rb");
    
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
                        Regs[Rd] = Regs[Rs] << offset;
                        break;
                    case 1: // LSR
                        printf("LSR R%d, R%d, #%d", Rd, Rs, offset);
                        Regs[Rd] = Regs[Rs] >> offset;
                        break;
                    case 2: // ASR
                        printf("ASR R%d, R%d, #%d", Rd, Rs, offset);
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
                        Regs[Rd] = Regs[Rs] - RnOffset;
                    }
                    else    // Add
                    {
                        printf("ADD R%d, R%d, #%d", Rd, Rs, RnOffset);
                        Regs[Rd] = Regs[Rs] + RnOffset;
                    }
                }
                else    // Reg operand
                {
                    if (op) // Sub
                    {
                        printf("SUB R%d, R%d, R%d", Rd, Rs, RnOffset);
                        Regs[Rd] = Regs[Rs] - Regs[RnOffset];
                    }
                    else    // Add
                    {
                        printf("ADD R%d, R%d, R%d", Rd, Rs, RnOffset);
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
    
}

//
//  main.cpp
//  ARM
//case 4:
//    //
//    break;
//
//  Created by Mohamed Shahawy on 4/15/16.
//  Copyright © 2016 Mohamed Shahawy. All rights reserved.
//case 5:
//    //
//    break;
//case 7:
//    break;
//case 8:
//    break;
//
//case 9:
//    break;
//
//    }
