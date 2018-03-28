/*Maman 14, Assembler, Final Project!*/
/*Wrote by:                                 */
/*Oleg Vodsky, 317465599                    */
/*Semester alef, 2018                       */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include "list.h"
#include "Parse_tools.h"
#include "DataStructures.h"
#include "map.h"
#include "first_iteration.h"
#include "second_iteration.h"
#include "Assembler.h"



/*global variables to keep track of global error list and current IC and DC*/ 
List *errorList=NULL;
int IC_ = INIT_ADDRESS;
int DC_ = 0;

/*public function to push new errors from all around the project*/
void logErr(int line,char* message){
    push(&errorList, logError(line,message));
}

/*public setter to increment IN or DC*/
void inc_IC_DC(int IC_DC){
    (IC_DC) ? DC_++ : IC_++;
}

/*public getter for IC or DC global vars*/
int get_IC_DC(int IC_DC){
    if(IC_DC) return DC_;
    return IC_;
}



int main(int argc, char *argv[]){
   
    FILE* fp;
    int i=0;
    char msg[MAX_LEN]="";
    char fileName[MAX_LEN]="";

    /* loop threw args to load the files */
    for(i = 1; i<argc ; i++){
        strcpy(fileName,"");
        appendStr(fileName, COUNT_TWO, argv[i], ".as");

        if ( (fp = fopen(fileName,"r"))==NULL){
            strcpy(msg,"");
            appendStr(msg,COUNT_TWO,"couldn't open file: ",fileName);
            logErr(FileNotFound, msg);
        } else {
            /*if the file found, and everything is ok.. start the manager */
            manager(fp,argv[i]);
        }
    }

   /*if there are errors, print them and destroy the allocated List*/
   if(errorList){
        printList(errorList,printErrors);
        Destroy_List(&errorList,freeError);
    }
    
   return 0;
}



/* the function that manages the synchornous tasks for a single file of assembly.as code */
void manager(FILE *file, char* fileName){
    
    List * AssemblyProg = NULL;
    List * bin_code = NULL;
    List * Label_Table = NULL;
    List * bin_data = NULL;
    List * entry_Labels =NULL;
    List * extern_Labels = NULL;
    
    void* pointer_extern_labels = &extern_Labels;
    void* pointer_bincode = &bin_code;
    void* pointer_label_table = &Label_Table;
    void* pointer_bindata = &bin_data;
    void* pointer_Entry_Labels = &entry_Labels;
    int errors = 0;

    /*create list of assembly lines from the assembly code in the file*/
    AssemblyProg = Append_parsed_line(file, AssemblyProg, fileName, &errors);

    /*start the first routine, if there are any errors return how many*/
    errors += routine_one(AssemblyProg,fileName, pointer_bincode, pointer_label_table, pointer_bindata, pointer_Entry_Labels);

    /*update entry list, if there errors save them too*/
    errors += updateEntry(Label_Table,entry_Labels, fileName);

    /*check for duplicate labels, return number of errors*/
    errors += label_validation(Label_Table, fileName);

    /*if there were no errors so far start continue */
    if(!errors){
        
        /* itarete again over the list of assembly lines*/
        errors += second_iteration(AssemblyProg,fileName,bin_code, Label_Table, pointer_extern_labels);

        /* if there were no errors in second iteration and before that, create apropriate output files.*/
        if(!errors) makeOutputs(bin_code,bin_data,Label_Table,fileName, extern_Labels); 

    }
    
    /* free all memory that was used by the lists in heap */
    if(extern_Labels)        Destroy_List(&extern_Labels, freebin);
    if(entry_Labels)         Destroy_List(&entry_Labels, freebin);
    if(Label_Table)    Destroy_List(&Label_Table, freebin);
    if(bin_code)       Destroy_List(&bin_code, freebin);
    if(bin_data)       Destroy_List(&bin_data, freebin);
    if(AssemblyProg)   Destroy_List(&AssemblyProg,freeAssembly);
    
    /*initialize global variables IC and DC for next file iteration.*/
    IC_=INIT_ADDRESS;
    DC_=0;
}






void makeOutputs(List * bin_code, List * bin_data, List * Labels, char* fileName, List* extern_Labels){

    FILE* fp=NULL;
    int size=0, i=0, flag=0;
    char* base32=NULL;
    char file_name[MAX_LEN]="";
    char* address_holder=NULL;
    List* node = NULL;
    

        size = listLength(bin_code);
        strcpy(file_name, fileName);
        strcat(file_name,".ob");
        /* start writting the .ob file*/
        fp = fopen(file_name,"w");

        /* append number of code addresses */
        address_holder = int_to_32(size);
        fprintf(fp," %s ",address_holder);
        free(address_holder);

        /* append number of data addresses */
        address_holder=int_to_32(listLength(bin_data));
        fprintf(fp,"%s\n",address_holder);
        free(address_holder);

        /* append addresses and the binary code */
        while(i<size){
            base32=converter(((get(bin_code,i))->details));
            address_holder=int_to_32(INIT_ADDRESS+i);
            fprintf(fp,"%s  %s\n",address_holder,base32);
            free(base32);
            free(address_holder);
            i++;
        }
        IC_=size;
        size = listLength(bin_data);
        i=0;

        /* append the addresses and the binary data */
        while(i<size){
            base32=converter(((get(bin_data,i))->details));
            address_holder=int_to_32(IC_+INIT_ADDRESS+i);
            fprintf(fp,"%s  %s\n",address_holder, base32);
            free(base32);
            free(address_holder);
            i++;
        }
        fclose(fp);

        /* make entry output */
        node=Labels;
            strcpy(file_name, fileName);
            strcat(file_name,".ent");
            while(node){
                if ((*(Label*)node->details).isEntry) {  
                    /*open the file only if found at least one entry */
                    if (!flag) fp = fopen(file_name,"w");

                    address_holder = int_to_32( ((*(Label*)node->details).isOperation) ? ((*(Label*)node->details).address) : ((*(Label*)node->details).address + IC_ +INIT_ADDRESS)  );
                    flag++;
                    fprintf(fp,"%s  %s\n", (*(Label*)node->details).name , address_holder);
                    free(address_holder);
                }
                node=node->next;
            }
            /* close the file only if there was an entry */
            if(flag) fclose(fp);
            flag=0;
        /* make extern output */
        node=extern_Labels;
        strcpy(file_name, fileName);
        strcat(file_name,".ext");
                while(node){
                        if(!flag) fp = fopen(file_name,"w");
                        flag++;
                        base32 = int_to_32((*(Label*)node->details).address +INIT_ADDRESS);
                        replaceChar(':','\0',(*(Label*)node->details).name);
                        fprintf(fp,"%s  %s\n", (*(Label*)node->details).name , base32);
                        free(base32);
                    node=node->next;
                }
        if(flag) fclose(fp);
        flag=0;
        
}
