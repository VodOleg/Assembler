#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include "list.h"
#include "DataStructures.h"
#include "Assembler.h"
#include "map.h"
#include "first_iteration.h"

/*the function that performs second iteration duties*/
/*mostly updating the binary code, for the apropriate label addresses*/
int second_iteration(List* assembly,char* fileName,List* bincode, List* Label_Tabel, void* extern_Labels){
    AssemblyLine * Line = NULL;
    int i=0, operand_counter = 0, ic_counter = 0;
    int size = listLength(assembly);
    int* binary=NULL;
    List* node=NULL;
    List* lab = NULL;
    Label* label=NULL;
    char fixedLabel[MAX_LEN];
    char *temp_string= NULL;
    char errorMsg[MAX_LEN + MAX_LEN]=" ";
    int  errorFlag=0;  

    /*check if the code did not exceed the maximum code size of 10 bit storage*/
    if((get_IC_DC(IC) + get_IC_DC(DC)) > (POSITIVE_TEN_BIT+POSITIVE_TEN_BIT)){
        appendStr(errorMsg,COUNT_TWO,fileName,"\n The whole binary code exceeded 10 bit length (1023 words of data), next address requires 11 bit");
        logErr(FileNotFound,errorMsg);
        errorFlag++;
        strcpy(errorMsg,"");
    } 
    for( i=0; i<size; i++){
        Line = (get(assembly,i))->details;
        /* skip empty lines */
        if( OPERAND[0]=='\0' || OPERAND[0]==';') continue;
        /* skip processing labels */
        if(isLabel(OPERAND) && lastChar(OPERAND)==':') operand_counter++;

        /* work only on action command line, not assign ones */
        if (  get_cmd_id(OPERAND) < ACTION_COUNT){
            operand_counter++; 
            ic_counter++;
            
            /*iterate threw the operands in a line of code*/
            while( Line->operandsNum - operand_counter ){ 
                    
                switch(OP_ADDRESSING){
                    /* if the operand is of addressing type LABEL (direct) */
                    case direct: 
                        /* get the apropriate binary code from the bincode list*/
                        node = get(bincode, ic_counter);
                        binary = node->details;
                        
                        /*fix the label to not contain ':' */
                        strcpy(fixedLabel," ");
                        fixOp(fixedLabel,OPERAND);

                        /*search for the label in the labels list and assign it to lab variable*/
                        lab = searchList(Label_Tabel,findLabel,fixedLabel);

                        /*check if lab isnt NULL or the details in it arent NULL*/
                        if(lab && lab->details){
                            label = lab->details;
                            if(label->isOperation){ 
                                /*if the label is operation append with apropriate  ARE*/
                                updateAddress(binary,label->address,COUNT_TWO);
                            } 
                            else if( label->isExtern){
                                /*if the label is extern append with apropriate  ARE, and update list of extern labels*/
                                updateAddress(binary, label->address,1);
                                push(extern_Labels, newLabel(label->name,ic_counter,Line->line,1,1));

                            }else{
                                /*if the label is defined in code append with apropriate (relocatable)  ARE*/
                                updateAddress(binary, (label->address)+get_IC_DC(IC), COUNT_TWO);
                            }
                        }else{
                            ERROR("usage of undefined label", OPERAND);
                        }
                        ic_counter++;
                    break;
                    case access:
                        /* address of label*/
                        /* get the apropriate binary code from the bincode list*/
                        node = get(bincode, ic_counter);
                        binary = node->details;

                        /*fix the label to not contain ':' */
                        strcpy(fixedLabel," ");
                        fixOp(fixedLabel,OPERAND);

                        /*search for the label in the labels list and assign it to lab variable*/
                        lab = searchList(Label_Tabel,findLabel,fixedLabel);

                        /*check if lab isnt NULL or the details in it arent NULL*/
                        if(lab && lab->details){
                            label = lab->details;
                            if(label->isOperation){
                                /*if the label is operation append with apropriate  ARE*/
                                updateAddress(binary,label->address,access);
                            } 
                            else if( label->isExtern){
                                /*if the label is extern append with apropriate  ARE, and update list of extern labels*/
                                updateAddress(binary, label->address,1);
                                push(extern_Labels, newLabel(label->name,ic_counter,Line->line,1,1));
                            }else{
                                /*if the label is defined in code append with apropriate (relocatable)  ARE*/
                                updateAddress(binary, (label->address)+get_IC_DC(IC), access);
                            }
                        }
                        else{
                            /* if the label was not found in label list, log error*/
                            ERROR("usage of undefined label",OPERAND);
                        }
                        ic_counter++;
                        /* value of struct index (1 or 2)*/
                        /* its a struct so we must update also the values */
                        node = get(bincode, ic_counter);
                        binary = node->details;
                        updateAddress(binary,my_atoi(OPERAND +length((OPERAND))-1 ), 0);
                        ic_counter++;

                    break;
                    case regist:
                    if((operand_counter == (Line->operandsNum-1)) && operandType(Line->operands[operand_counter-1])==regist){
                        /*if the operand is register we dont do anything because we already did in iteration one*/
                    } else {
                        /* if the operand is register but the operand before it wasnt, we just keep track of ic counter*/
                        ic_counter++;
                    }
                    break;
                    default:
                    ic_counter++;
                    break;
                }
                
                operand_counter++;
            }
            
        }
        operand_counter = 0;
    }
return errorFlag;
}


/*a routine that updates the table of labels, after the first iteration whenever*/
/* the update is for entry Labels*/
int updateEntry(void* Labels, void* entries, char* fileName){
    List * entry_Labels = entries;
    List * Label_Table = Labels;
    Label * temp = NULL;
    void* point = NULL;
    int errors = 0;
    char errorMsg[MAX_LEN + MAX_LEN]=" ";

        /* loop threw list of entry labels*/
        while(entry_Labels){
            /* search for labels that are flagged as entry and save it to point*/
            point = searchList(Label_Table,findLabel,entry_Labels->details);

            if ( point == NULL ) {
                /*if the search returned null, Log error, because we had definition of entry label but the label was not defined */
                strcpy(errorMsg," ");
                appendStr(errorMsg,COUNT,fileName,"\n\"",entry_Labels->details,"\" - Defined to be an Entry Label but is not defined in the code.\n");
                logErr(FileNotFound,errorMsg);
                errors++;
                entry_Labels = entry_Labels->next;
                continue;
            }
            if(((List*)point)->details == NULL){
                entry_Labels = entry_Labels->next;
                continue;
            }
            temp =  ((List*)point)->details;
            temp->isEntry= 1;
            entry_Labels = entry_Labels->next;
        }
    return errors;
}

/* function that checks if a label was defined more then once in a code */
int label_validation(List* Label_Table, char* fileName){
    int size = listLength(Label_Table);
    int i=0,j=0, error=0;
    char message[MAX_LEN+MAX_LEN]="";
    Label* temp_label=NULL;
    Label* temp_compare=NULL;
    char* line_holder=NULL;

    /*run from the end of list of labels*/
    for(i=size-1;i>=0;i--){
        temp_label=(get(Label_Table,i))->details;
        /*for each label found, look if it defined somewhere earlier*/
        for(j=0;j<i;j++){
            if (i==j) continue;
            temp_compare=(get(Label_Table,j))->details;
            if (!strcmp(temp_label->name, temp_compare->name) ){
                /*if a label defined twice log error*/
                strcpy(message,"");
                line_holder = uint_to_string(temp_compare->line);
                appendStr(message,COUNT_SIX,fileName,"\nMultiple definition of Label \"", temp_label->name ,"\". First defined at Line [", line_holder ,"].");
                logErr(temp_label->line,message);
                error++;
                free(line_holder);
                break;
            }
        }
    }
    /* return the number of multiply defined labels*/
    return error;
}