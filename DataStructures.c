#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "list.h"
#include "Parse_tools.h"
#include "DataStructures.h"


/*constructor for a new label struct*/
void* newLabel(char* label, int address, int line, unsigned short isExtern, unsigned short isOperation){
    Label * new = NULL;
    new = calloc(1, sizeof(Label));
    new->name=label;
    new->address = address;
    new->isExtern = isExtern;
    new->isOperation = isOperation;
    new->isEntry=0;
    new->line=line;
    return new;
}

/*constructor for a new line struct*/
/*this struct requires a constructor because it uses dynamic field (operands)*/
void* newLine(unsigned short line,unsigned short operationCount, char** operation){
    AssemblyLine *new = NULL;
    new= calloc(1,sizeof(AssemblyLine));
    new->line = line;
    new->operandsNum = operationCount;
    new->operands = operation;

    return new;
}

/*initialize an array of unsigned ints to the size of the binary code */
/*this function can only be called after the first iteration.*/
void* BinaryCode(unsigned int codeSize){
    unsigned int* code;
    code = calloc(codeSize+1, sizeof(code));
    return code;
}

/*a function to set a binary code to a cell in the array*/
void setCodeLine(unsigned int index, unsigned int value, unsigned int* code){
    code[index]=value;
}

/* function that cleans all arguments in a give array of arguments */
void freeArgs(char **args){

    while(*args){
        free(*args++);
    }
    free(*args);
}

/*private function to free allocated space of each array of arguments per line*/
void freeAssembly(void *details){
        if( details != NULL){
            freeArgs((*(AssemblyLine *)details).operands);
            free((*(AssemblyLine *)details).operands);
        } else {
            printf("Error occured while trying to free details that are pointing to NULL\n");
        }
return;
}

/*private print function of a AssemblyLine struct*/
void printAssembly(void *details)
{
    int i = 0;
    printf("line = %d  ,  args count = %d\n", (*(AssemblyLine *)details).line, (*(AssemblyLine *)details).operandsNum);
    while (i < (*(AssemblyLine *)details).operandsNum)
    {   
        printf("%s_", (*(AssemblyLine *)details).operands[i]);
        i++;
    }
    printf("\n");
}

/* creates new array of strings and copies the contents from src to dest */
void copyArrayOfString(char** dest, char** src){
    int sizeSrc = 0 ;
    while(*src){
         dest[sizeSrc] = calloc (length(*src), sizeof(*src));
         strcpy(dest[sizeSrc],*src++);
         sizeSrc++;   
    }
}

/*function that appends new Errors to a list of global errors*/
Error* logError(unsigned int line, char* message){
    Error *err=NULL;
    err = malloc(sizeof(Error));
    err->line = line;
    err->message = calloc(MAX_LEN,sizeof((err->message)));
    strcpy(err->message, message);
    return err;
}

/* private destroyer for struct of type Error */
void freeError(void *details){
    if( details != NULL){
            free((*(Error *)details).message);
        } else {
            printf("Error occured while trying to print details that are pointing to NULL\n");
        }
}
    
/*private function to print struct of type Error*/
void printErrors(void *error)
{
    int line = (*(Error *)error).line;
    if (line == -1)
    {
    }
    else if (line == -2)
    {
        printf("Assembler  - %s\n", (*(Error *)error).message);
    }
    else
    {
        printf("Error in : %s [LINE %d]\n\n", (*(Error *)error).message, line);
    }
}

/* function that supposed to release the details inside the List struct*/
void freebin(void* details){
    if( details != NULL){
           /* Do nothing, since the destroyList function already frees those addresses */
        } else {
            printf("Error occured while trying to free details that are pointing to NULL\n");
        }
    }

/* a private function to print a single struct of type Label */
void printLabels(void* details){
    Label temp = (*(Label *)details);
    printf("%s  at  %d in line %d  %s  %s  %s\n",temp.name, temp.address ,temp.line, ( (temp.isExtern)? ("Extern"): ("Internal") ), ( (temp.isOperation)? ("Action"): ("Assignment") ) , ( (temp.isEntry)? ("Entry"): ("not Entry") )  );
      
}

/* a function that compares if the label pointed by details has the same name as the desired string pointed by item */
/* returns the pointer to the label if it is equal or null if it is not*/
void* findLabel(void* details, void* item){
    Label temp = (*(Label *)details);
    if (!strcmp(temp.name, item)) {
        return details;
    }
return NULL;
}