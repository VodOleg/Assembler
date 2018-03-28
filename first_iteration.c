#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "map.h"
#include "utils.h"
#include "list.h"
#include "Parse_tools.h"
#include "DataStructures.h"
#include "first_iteration.h"
#include "Assembler.h"

command ops_Table[OPS_C]= 
{
    {"mov", 0, 3210, 321,2},
    {"cmp", 1, 3210, 3210, 2},
    {"add", 2, 3210, 321,2},
    {"sub", 3, 3210, 321,2},
    {"not", 4, 9, 321,1},
    {"clr", 5, 9, 321,1},
    {"lea", 6, 21, 321,2},
    {"inc", 7, 9, 321,1},
    {"dec", 8, 9, 321,1},
    {"jmp", 9, 9, 321,1},
    {"bne", 10, 9, 321,1},
    {"red", 11, 9, 321,1},
    {"prn", 12, 9, 3210,1},
    {"jsr", 13, 9, 321,1},
    {"rts", 14, 9, 9,0},
    {"stop", 15, 9, 9,0},
    {".data", 16, 9, 9, 999},
    {".string", 17,9, 9, 1},
    {".struct", 18, 9, 9, 2},
    {".entry", 19, 0, 0, 1},
    {".extern", 20, 0,0, 1}
};


/*function that performs the tasks of the first routine*/
int routine_one(List* assembly,char* fileName,void* bincode, void* Label_Tabel, void* bindata, void* entry_Labels){
    AssemblyLine * Line = NULL;
    int i = 0, id = 0, operand_counter = 0, numeric_value =0;
    char *argument, *temp_string;
    char errorMsg[MAX_LEN + MAX_LEN]=" ";
    int labelFlag =0, errorFlag=0;  
    int* binary;
    int size = listLength(assembly);
    

    for(i = 0; i<size; i++){
        Line = (get(assembly,i))->details;    /*save the current node into local variable*/
        argument =  OPERAND; 
        /*ignore empty lines and comments*/
        if(argument[operand_counter]=='\0' || argument[operand_counter]==';') continue;
        if(lastChar(argument)==':') {
            if(isLabel(argument) && Line->operandsNum>1){    
                labelFlag=1;
                argument = Line->operands[++operand_counter];
                /*TODO: continous analyze of the string from operands[1], push the Label into List*/
            } else if(isLabel(argument)){
                ERROR(" Line of code contains only Label ",OPERAND);
            }else {
                appendStr(errorMsg,COUNT_FIVE,fileName,"\n\"",Line->operands[0],Line->operands[1],"...\" - Label can contain only small and big latters or numbers.");
                logErr(Line->line,errorMsg);
                continue;
            }
        }
        if ( (id =get_cmd_id(argument)) < ACTION_COUNT ){
            
                if(ops_Table[id].operands_count!=((Line->operandsNum)-labelFlag-1)  ){
                    ERROR("Inapropriate number of operands for", ops_Table[id].cmd);
                } else{
                    operand_counter++;
                    
                    if (ops_Table[id].operands_count==1 ){
                        /*processing command with 1 legal operand */
                        if (operandType(OPERAND)<0){
                            ERROR("undefined operand", OPERAND);
                        }
                        else if( containDigit(ops_Table[id].dist_addressing, operandType(OPERAND) )  ){
                            /*create and push the command binary code*/
                            binary = calloc(COUNT_TEN,sizeof(int));
                            encode(binary,id,0,operandType(OPERAND),0);
                            push(bincode,binary);
                            if (labelFlag) {PUSH_LABEL(IC,1);}
                            inc_IC_DC(IC);
                            
                            if( operandType(OPERAND) != access){
                                /*create 1 more array*/
                                PUSH_EMPTY
                                if(operandType(OPERAND) != direct){
                                   if (operandType(OPERAND) == regist){
                                       /* operand is a register of 1 operand operation, so it is destination type */
                                        numeric_value = NUMERIC;
                                   } else {
                                       /* number is raw number */
                                       numeric_value = NUMERIC;
                                       if(numeric_value>POSITIVE_EIGH_BIT || numeric_value<NEGATIVE_EIGHT_BIT ){
                                           ERROR("imidiate number out of 8 bit range [-128,127]", OPERAND);
                                       }
                                   }
                                    updateAddress(binary, numeric_value ,0);
                                    
                                }
                            }else{
                                /*create 2 more arrays*/
                                PUSH_EMPTY
                                PUSH_EMPTY
                            }
                            }
                        
                        else{
                            ERROR("Inapropriate destination argument addressing for",Line->operands[operand_counter-1]);
                        }
                    }
                    else if(ops_Table[id].operands_count == COUNT_TWO ){
                        /*proccessing command with 2 legal operands*/
                        /*---- FIRST OPERAND PROCCESSING -----*/
                         if (operandType(OPERAND)<0){
                            ERROR("undefined operand", OPERAND);
                        }
                        else if( containDigit(ops_Table[id].src_addressing, operandType(OPERAND) )  ){

                            binary = calloc(COUNT_TEN,sizeof(int));
                            encode(binary,id,operandType(OPERAND),operandType(Line->operands[operand_counter+1]),0);
                            push(bincode,binary);
                            if (labelFlag) {PUSH_LABEL(IC,1);}
                            inc_IC_DC(IC);

                            if( operandType(OPERAND) != access){
                                /*create 1 more array*/
                                PUSH_EMPTY
                                if(operandType(OPERAND) != direct){
                                  if  (operandType(OPERAND) == regist){
                                       /* operand is a register of 1 operand operation, so it is destination type */
                                    numeric_value = NUMERIC *COUNT_FOUR*COUNT_FOUR;
                                   } else {
                                       /* number is raw number */
                                       numeric_value = NUMERIC;
                                       if(numeric_value>POSITIVE_EIGH_BIT || numeric_value<NEGATIVE_EIGHT_BIT ){
                                           ERROR("imidiate number out of 8 bit range [-128,127]", OPERAND);
                                       }
                                   }
                                     updateAddress(binary,numeric_value ,0);
                                }
                            }else{
                                /*create 2 more arrays*/
                                PUSH_EMPTY
                                PUSH_EMPTY
                            }
                            }
                        
                        else{
                            ERROR("Inapropriate source argument addressing for",Line->operands[operand_counter-1]);
                        }
                        /*---- SECOND OPERAND PROCCESSING ----*/
                        operand_counter++;
                        if (operandType(OPERAND)<0){
                            ERROR("undefined operand", OPERAND);
                        }
                        else if( containDigit(ops_Table[id].dist_addressing, operandType(OPERAND) )  ){
                            if( operandType(OPERAND) < access ){
                                /*create 1 more array*/
                                PUSH_EMPTY
                                if(operandType(OPERAND) != direct){
                                       /* number is raw number */
                                       numeric_value = NUMERIC;
                                       if(numeric_value>POSITIVE_EIGH_BIT || numeric_value<NEGATIVE_EIGHT_BIT ){
                                           ERROR("imidiate number out of 8 bit range [-128,127]", OPERAND);
                                       }
                                   
                                     updateAddress(binary,numeric_value ,0);
                                }
                            }else if(operandType(OPERAND) == access){
                                /*create 2 more arrays*/
                                PUSH_EMPTY
                                PUSH_EMPTY
                            }else{
                                if(operandType(Line->operands[operand_counter-1]) != regist){
                                    PUSH_EMPTY
                                    updateAddress(binary, NUMERIC ,0);
                                } else {
                                    /*do nothing, 2 registeres can be stored in same array*/
                                    updateAddress(binary, my_atoi((Line->operands[operand_counter-1])+1)*COUNT_FOUR*COUNT_FOUR + NUMERIC ,0);
                                }
                            }
                            
                        }
                        else{
                            ERROR("Inapropriate destination argument addressing for",Line->operands[operand_counter-COUNT_TWO]);
                        }


                    }
                    else {
                        binary = calloc(COUNT_TEN,sizeof(int));
                        encode(binary,id,0,0,0);
                        push(bincode,binary);
                        if (labelFlag) {PUSH_LABEL(IC,1);}
                        inc_IC_DC(IC);
                    }


                }
        } else if ( id < ASSIGN) {
            if (ops_Table[id].operands_count == ((Line->operandsNum)-labelFlag-1) ){
                operand_counter++;
                if(id==STRING){
                    /*string code goes here*/
                    if (operandType(OPERAND) == COUNT_TEN){
                        if (labelFlag) {PUSH_LABEL(DC,0);}
                        pushString(OPERAND, bindata);
                    }else{
                        ERROR("inapropriate string decleration", argument);
                    }

                }
                else if(id==STRUCT){
                    /*struct code goes here*/
                    if (isInt(OPERAND) && operandType(Line->operands[operand_counter+1])==COUNT_TEN ){
                        if (labelFlag) {PUSH_LABEL(DC,0);}
                        push(bindata, num_to_binary(OPERAND));
                        inc_IC_DC(DC);
                        pushString(Line->operands[operand_counter+1], bindata);
                    } else{
                        ERROR("Inapropriate operand for struct. 1st must be int, 2nd must be string", argument );
                    }
                }
                else if(id==ENTRY){
                    /*entry code goes here*/
                    if( isLabel(OPERAND)){
                    temp_string = calloc(length(OPERAND+1),sizeof(char*) );
                    strcpy(temp_string, OPERAND);
                    strcat(temp_string,":\0");
                    push(((List**)entry_Labels), temp_string);
                    temp_string=NULL;
                    }else{
                        ERROR("Looks like invalid LABEL name :", OPERAND);
                    }
                }
                else if(id==EXTERN){
                    /*extern code goes here*/
                    if( isLabel(OPERAND)){
                        strcat(OPERAND,":");
                        push(((List**)Label_Tabel), newLabel(OPERAND, 0,Line->line, 1,0));
                    }else{
                        ERROR("Looks like invalid LABEL name :", OPERAND);
                    }
                }
            } 
            else if(id == DATA && ((Line->operandsNum)-labelFlag-1)>0 ) {
                /* .data code goes here */
                operand_counter++;
                if (labelFlag) {PUSH_LABEL(DC,0);}
                while(operand_counter<Line->operandsNum){
                    if(isInt(OPERAND)){
                        if(my_atoi(OPERAND)>POSITIVE_TEN_BIT || my_atoi(OPERAND)<NEGATIVE_TEN_BIT) {
                            ERROR("data number out of 10 bit range [-512,511]", OPERAND);
                        }
                      push(bindata, num_to_binary(Line->operands[operand_counter++]));
                      inc_IC_DC(DC);
                    }else{
                        ERROR("Inapropriate operand, is not valid number",Line->operands[operand_counter++]);
                    }
                }

            }
            else{
                ERROR("inapropriate number of arguments for:",argument);
            }
                /*this line should be parsed and saved into Data array*/
                /*also if its entry, should write this into seperate file if it has Label optionally can warn*/
                /*also if its extern should save adress as 000*/
                
        } else {
            if(argument){
            ERROR("invalid command",argument);
            } else {
                printf(" Error: Empty argument at %s, [LINE=%d]\n", fileName, Line->line);
            }
        }
        
        labelFlag=0;
        operand_counter = 0;
    }

    return errorFlag;
}

/*returns the id of the command*/
int get_cmd_id(char* argument){
    int i=0;
    if(!argument){  return POSITIVE_EIGH_BIT;}
    for(i=0;i<OPS_C;i++){
      if  (!strcmp(argument,ops_Table[i].cmd)) return i;
    }
    return ++i;
}

/*for every char in string, create binary array and push it to bin data, push extra word of null*/
void pushString(char* str,void* bindata ){
    int *binary = NULL;
    char* temp = str+1;
    int i=0;
    int size = length(str);
    binary = calloc(COUNT_TEN, sizeof(int));
    for(i=0;i<size-COUNT_TWO;i++){
        push( bindata , char_to_binary(temp[i]));
        inc_IC_DC(DC);
    }
    push(bindata, binary);
    inc_IC_DC(DC);
}

