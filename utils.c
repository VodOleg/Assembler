#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include "map.h"
#include "list.h"
#include "DataStructures.h"
#include "Parse_tools.h"
#include "stdlib.h"

/*Implementation of a custom atoi function from the book*/
/*the function recieves array of chars (string) and casts the value into an int */
int my_atoi(char s[])
{
    int i, n, sign;

    for (i = 0; isspace(s[i]); i++)
        ;

    sign = (s[i] == '-') ? -1 : 1;
    if (s[i] == '+' || s[i] == '-')
        i++;
    for (n = 0; isdigit(s[i]); i++)
        n = 10 * n + (s[i] - '0');

    return sign * n;
}



/*count array of chars length*/
char length(char arr[])
{
    int i = 0;
    while (arr[i] != EOF && arr[i] != '\0')
    {
        i++;
    }
    return i;
}

/*  check if input char c is a number*/
int isNumber(int c)
{
    if (c <= '9' && c >= '0')
        return 1;
    return 0;
}
/*removes any white space in a string*/
void RemoveSpaces(char *source)
{
    char *i = source;
    char *j = source;
    while (*j != 0)
    {
        *i = *j++;
        if (*i != ' ' && *i != COUNT_NINE && *i != (COUNT_NINE+COUNT_TWO))
            i++;
    }
    *i = 0;
}
/*counts whitespaces in a string*/
int countWhiteSpaces(char input[])
{
    int i = 0, counter = 0;
    while (input[i] != EOF && input[i] != '\0')
    {
        if (isWhiteSpace(input[i]))
            counter++;
        i++;
    }
    return counter;
}
/*checks if a string has 2 consecutive commas in a raw*/
int hasConsecutiveCommas(char input[])
{
    int i = 0,has=0;
    if(input[0]==';') return has; /*skip comments*/
    while (input[i] != EOF && input[i] != '\0')
    {
        if( input[i] == '\"') {
           i++;
           while(input[i] != '\"' && input[i++] != '\0');
        }
        if (input[i]=='\n') input[i]='\0'; /*remove newline char from end of string*/
        if (i > 0 && (input[i] == ',') && (input[i - 1] == ','))
            has= 1;
        i++;
    }
    return has;
}



/*printing the args in array of strings*/
void printargs(char **in, int numargs)
{
    int i = 0;
    for (i = 0; i < numargs; i++)
        printf("%s\n", in[i]);
}



/* private function for printing array of integers */
void printintarr(int* arr){
    int i=0;
    while(i<COUNT_TEN){
        printf("%d",arr[i++]);
    }
    printf("\n");
}

/*boolean function to check if array of integers of size ten is empty, has only nulls*/
int isEmpty_binary(int* arr){
    int i=0;
    while(i<COUNT_TEN){
        if(arr[i++]) return 0;
    }
    return 1;
}


/* count the number of arguments in a give array of arrays (counts till NULL) */
int argsLength(char** arg){
    int countArgs=0;

    while(*arg++){
        countArgs++;
    }
    return countArgs;
}

/*function that replaces in a given string the char_now by char_later*/
void replaceChar(char ch_now, char ch_later, char* s){
    int i =0;
    for(i=0;s[i]!='\0';i++){
        if(s[i]==ch_now){
            s[i]=ch_later;
        }
    }
}

/*variadic function to stringify unknown number of strings into the destination string*/
void appendStr(char* dest,unsigned int count,  ...){
    va_list list;
    int i;
    char* temp=NULL;
    va_start(list,count);
    for(i=0;i<count;i++){
        temp =va_arg(list,char*);
        if(!temp) return;
        strcat(dest,temp);
    }
    va_end(list);
}


/* a simple function that returns the last char in a given string*/
char lastChar(char* in){
    char trash;
    while(*in){
        trash =*in;
        in++;
    }
    return trash;
}

/*boolean function that checks if a given string is a valid number that could be casted to int*/
int isInt(char* param){
    int i;
    for(i =0 ; i < length(param); i++){
         if ((!isNumber(param[i])  && param[i] != '-' && param[i] != '+') )
        {
            return 0;
        }
        if (i > 0 && (param[i] == '-' || param[i] == '+'))
            return 0;
        }
        return 1;
}

/*checks if the string can be a label*/
int isLabel(char* label){
    char trash;
    while(*label){
        if ( isNumber(*label) || (*label >= 'A' && *label <= 'Z') || (*label >= 'a' && *label <= 'z' ) || (*label==':' && *(label+1)=='\0')) trash = *label++;
        else return 0;
    }
    trash = trash -1;
    return 1;
}

/* a simple function that returns numeric value for a given operand, to determine its proccessing */
int operandType(char* input){
    int ret=-1;
    size_t in_size = length(input);
    if (input[0]=='\"' && input[in_size-1]=='\"')                                                                          ret= COUNT_TEN;      /*string*/
    else if(in_size>COUNT_THREE && input[in_size-COUNT_TWO]=='.' && (input[in_size-1]=='1' || input[in_size-1]=='2' ))     ret= access;         /*operand struct*/
    else if( isInt(input) )                                                                                                ret= COUNT_EIGHT;   /*plain number*/
    else if( input[0]=='#' && isInt(input+1))                                                                              ret= imidiate;       /*operand number*/
    else if( input[0]=='r' && my_atoi(input+1)>-1 && my_atoi(input+1)<COUNT_EIGHT )                                        ret= regist;        /*operand register*/
    else if( isLabel(input))                                                                                               ret= direct;         /*operand label*/
    return ret;
}

/* private function to print binary array of size 10 */
void printBinary(void* details){
  printintarr(details);
}

/*concatane a string from array of strings*/
char* concatString(char** args){
    char* output = calloc (MAX_LEN, sizeof(output));
    while(*args){
        strcat(output," ");
        strcat(output,*args++);
    }

    return output;
}

/* function that scans an int number and checks if its contains digit */
int containDigit(int src, int digit){
    
    while(src!=0){
        if (src % COUNT_TEN == digit) return 1;
        src /= COUNT_TEN;
    }
    return 0;
}

/* function that encodes an opcode */
void encode(int* binary,int id, int src, int dist, int ARE){
    int j=COUNT_THREE,temp=id;
        while(j>=0){
            binary[j--]=temp % COUNT_TWO;
            temp/=COUNT_TWO;
        }
        j=COUNT_FOUR;
        binary[j]=(src>1);
        binary[++j]=src%COUNT_TWO;
        binary[++j]=(dist>1);
        binary[++j]=dist%COUNT_TWO;
        binary[++j]=(ARE>1);
        binary[++j]=ARE%COUNT_TWO;
    
}

/*create array of integers that creates the binary value of ascii value of char*/
int* char_to_binary(char chr){
    int i;
    int c_val=chr;
    int* arr = calloc(COUNT_TEN,sizeof(int));
    
    for(i=COUNT_NINE;i>=0;i--){
        arr[i]=c_val % COUNT_TWO;
        c_val /= COUNT_TWO;
    }
return arr;
}

/* function that receives array of binary and converts the value temp to this binary representation in the array*/
void int_to_binary(int* arr,int temp, int size){
int i;
if(temp>=0){
    /* for positive value run the algorithm */
        for(i=size-1;i>=0;i--){
            arr[i]= temp%COUNT_TWO;
            temp /= COUNT_TWO;
        }
    } else{
        /*for negative value, run the algorithm for its positive value*/
        temp = temp * (-1);
        for(i=size-1;i>=0;i--){
            arr[i]= temp%COUNT_TWO;
            temp /= COUNT_TWO;
        }
        /* toggle every bit  */
        for(i=0;i<size;i++){
            arr[i]=!arr[i];
        }
        /* run from the end till find first zero, toggle it and exit */
        for(i=size-1;i>=0;i--){
            if(!arr[i]) {
                arr[i]=!arr[i];
                break;
            }
        }
        /* swap all previous ONES to ZEROS*/
        while(i<size-1 && arr[++i]){
            arr[i]=!arr[i];
        }
    }
}

/* function that allocates memory for binary array of size 10 and converts the value of string into the binary */
int* num_to_binary(char* num){
    int temp = my_atoi(num);
    int* arr = calloc(COUNT_TEN,sizeof(int));
    int_to_binary(arr,temp,COUNT_TEN);

    return arr;
}

/*debuggind purpses, to be removed*/
void printstring(void* details){
    char* str = ((char*)details);
    printf("%s\n",str);
}

/*encodes the binary code to have 8 bits of value, and 2 bits for ARE*/
/*0 = static(00), 1 = external(01) , 2 = relocatable (10) */
void updateAddress(int* arr, int val, int ARE){
    int i=9;
    if(ARE==1){
        arr[i]=1;
    }else {
        int_to_binary(arr,val,COUNT_EIGHT);
        if(ARE) arr[COUNT_EIGHT]=1;
    }
}

/* operation that uses labels must be appended with ':' so it can be compared in Label List */
void fixOp(char* dest, char*src){
    while(*src && *src!='.'){
        (*dest++)=*src++;
    }
    *dest++=':';
    *dest='\0';
}

/*simple function to recieve the wierd 32 char for a given value*/
char getSymbol(int num){
    char* symbols = "!@#$%^&*<>abcdefghijklmnopqrstuv";
return symbols[num];
}

/* converts an array of binnary code of size 10, to its wierd32 base representation */
char* converter(int* binary){
    char* string = calloc(COUNT_EIGHT, sizeof(char));
    int i =0;
    string[i++]= getSymbol((binary[0]*COUNT_FOUR*COUNT_FOUR + binary[1]*COUNT_EIGHT + binary[COUNT_TWO]*COUNT_FOUR + binary[COUNT_THREE]*COUNT_TWO+binary[COUNT_FOUR]*1)); 
    string[i++]= getSymbol((binary[COUNT_FIVE]*COUNT_FOUR*COUNT_FOUR + binary[COUNT_SIX]*COUNT_EIGHT + binary[COUNT_SEVEN]*COUNT_FOUR + binary[COUNT_EIGHT]*COUNT_TWO+binary[COUNT_NINE]*1));
    string[i]='\0';

    return string;
}

/* converts int to its wierd 32 value, returns string*/
char* int_to_32(int num){
    char* string; 
    int i = 0;
    int holder = num;

    while(num){
        num/=(COUNT_FOUR*COUNT_FOUR*COUNT_TWO);
        i++;
    }
    string = calloc(i+1,sizeof(char));
    i=i-1;
    while (i>=0){
        string[i--]=getSymbol(holder %(COUNT_FOUR*COUNT_FOUR*COUNT_TWO));
        holder/=(COUNT_FOUR*COUNT_FOUR*COUNT_TWO);
    }
    
return (string);
}

/* checks if a give binary code is a representation of an extern label */
int extern_binary(void* binary){
    int* bin = binary;
    int i=0;
    for(i=0; i<COUNT_NINE; i++){
        if (bin[i]) return 0;
    }
    if(!bin[i]) return 0;
    return 1;
}

/* converts an unsigned value of int to a string of the value (revert atoi) */
char* uint_to_string(unsigned int num){
    int temp = num, i = 0;
    char* string = NULL;
    if(!temp) i++;
    while(temp){
        temp/=COUNT_TEN;
        i++;
    }
    string = calloc(i+1,sizeof(char));
    temp=num;
    i=i-1;
    if(!num) string[0]='0';
    while(temp){
        string[i]=(temp%COUNT_TEN)+'0';
        temp/=COUNT_TEN;
        i--;
    }
return string;
}