#include <stdio.h>
#include <string.h>
#include "list.h"
#include "DataStructures.h"
#include "map.h"
#include "utils.h"
#include "stdlib.h"
#include "Assembler.h"
#include "Parse_tools.h"




/* function that creates array of strings tokenized by space */
char **parseStr(char *str, char token)
{
    unsigned short argNum = countSeperators(str) + 1;
    char **args;
    int i, j = 0;
    int stringFlag=0;
    args = calloc(argNum +1, sizeof(*args));
    for (i = 0; i < argNum; i++)
    {
        args[i] = calloc(MAX_LEN ,(sizeof(char)));
            while ( *str!=0 && *str != token && !isWhiteSpace(*str) )
            {
                if(*str == '\"') stringFlag=!stringFlag;
                while(*str && stringFlag){
                    args[i][j++] = *str++;
                    if(*str == '\"') stringFlag=!stringFlag;
                }
                args[i][j++] = *str++;
            }
            
        j = *str++;
        j = 0;
    }
        args[i]=NULL; /* closing argument of null for easy iteration threw args */
    return args;
}

/*nullifies whitespace in the beginning of a string and the end of it*/
/*the function returns new string (allocated) without the spaces*/
char* trimWhiteSpace(char *source)
{

    int i;
    int j = 0;
    int inLeng = length(source);
    char *temp = calloc(MAX_LEN , sizeof(*temp));

    /*ignore the first white spaces in the string*/
    while (isWhiteSpace(*source))
    {
        i = *source++;
    }

    /*copy the chars from source to temp and skip consecutive white spaces*/
    for (i = 0; i < inLeng; i++, j++)
    {
        if (i > 0 && isWhiteSpace(temp[i - 1]) && isWhiteSpace(source[j]))
        {
            i--;
            continue;
        }
        temp[i] = source[j];
    }

    /*remove the last consecutive whitespaces from the temp string*/
    for (i = length(temp); i >= 0; i--)
    {
        if (isWhiteSpace(temp[i]) || temp[i] == 0)
        {
            temp[i] = 0;
        }
        else
        {
            break;
        }
    }

    return temp;
}


/*count the number of seperating white spaces in a given string*/
int countSeperators(char *str)
{
    int counter = 0, trash = 0;
    int whiteSpaceFlag = 0, stringFlag=0;
    while (*str)
    {
        if (*str == '\"')  stringFlag=!stringFlag;
        if (!stringFlag){
            if (whiteSpaceFlag == 0 && isWhiteSpace(*str))
            {
                counter++;
                whiteSpaceFlag = 1;
                /*move forward*/
            }
            else if (!isWhiteSpace(*str))
            {
                whiteSpaceFlag = 0;
            }
        }
        trash = *str++;
    }
    counter = counter + trash - trash;

    return counter;
}



/*check if a given char is a white space*/
int isWhiteSpace(int c)
{
    if (c == ' ' || c == COUNT_NINE || c == (COUNT_FIVE+COUNT_FIVE))
        return 1;
    return 0;
}

/*function that checks number of errors for a missplaced comma, if the function found erroneous position*/
/*the function will return POSITIVE_TEN_BIT value, else the func returns the number of commas*/
int invalid_comma(char* input){
    int result =0;
    int i =0;
    if(!input[0]) return 0;
    if (input[0] == ';') return 0; /* dont flag commented out lines */
    if (input[0] == ',') result=POSITIVE_TEN_BIT; /* comma before label is error*/

    for (i=0;i<length(input) && !isWhiteSpace(input[i]) ; i++);/*loop till pass the first token */
    if(i && i+1<length(input) && (input[i+1]==',' || input[i-1]==',' ) ) result=POSITIVE_TEN_BIT; /* found comma before label/command*/
    
    /* count commas */
    for (i=0;i<length(input) ; i++){
        if( input[i] == ',') result++;
    }
    
    /*if input ends with comma its an error*/
    if(input[length(input)-1] && input[length(input)-1]== ',') result=POSITIVE_TEN_BIT;

    return result;
}

/*replacing all commas in a string with a space, unless its in a block of string*/
void replaceCommas(char* in){
    int i =0;
    if(!in[0]) return;
    if (in[0] == ';') return; /* dont flag commented out lines */
    for (i=0;i<length(in);i++){
       if( in[i] == '\"') {
           i++;
           while(in[i] != '\"' && in[i++] != '\0');
       }
       if (in[i] == ',') in[i] = ' ';
       
    }
}


/*counts the occurance of a given char in a given string*/
int countToken(char *string, char token)
{
    int counter = 0, trash = 0;
    while (*string)
    {
        if ((trash = *string++) == token || isWhiteSpace(trash))
            counter++;
    }
    return counter;
}


/*function that reads the file, and appends its contexts into a given List of AssemblyCode struct*/
List* Append_parsed_line(FILE *file,List* AssemblyProg, char* fileName, int* error){
    int line = 1;
    char temp[MAX_LEN*MAX_LEN]= " " ;
    char errorMsg[MAX_LEN + MAX_LEN]=" ";
    char *sent=NULL;
    char** args=NULL;

        /*while block of creating breaking assembly code into AssemblyLine struct*/
        while ( fgets (temp,MAX_LEN*MAX_LEN, file)!=NULL){
            /*check if length of temp is no larger then MAX_LEN*/
            if(length(temp)>MAX_LEN){
                strcpy(errorMsg," ");
                appendStr(errorMsg,COUNT,fileName,"\n\"",temp,"\" - Line of code exceeded 80 characters.");
                logErr(line,errorMsg);
                *error = (*error) +1;
                continue;
            }else if(length(temp)==0){printf("error!\n");}
            /*save a new copy of the string without excessive spaces*/
            sent = trimWhiteSpace(temp);
            /*check if there are consecutive commas in a line */
            if(hasConsecutiveCommas(sent)) {
                strcpy(errorMsg," ");
                appendStr(errorMsg,COUNT,fileName,"\n\"",temp,"\" - Consecutive commas.");
                logErr(line,errorMsg);
                *error = (*error) +1;
            }
            /* check if there are invalid commas */
            if(invalid_comma(sent)>=POSITIVE_TEN_BIT){
                strcpy(errorMsg," ");
                appendStr(errorMsg,COUNT,fileName,"\n\"",temp,"\" - Missplaced comma.");
                logErr(line,errorMsg);
                *error = (*error) +1;
            }
            /*replace all commas with whitespace for easier parssing*/
            replaceCommas(sent);
            strcpy(temp,sent);
            free(sent);
            sent = trimWhiteSpace(temp);
            /*save each word in the sentance in seperate array*/
            args = parseStr(sent, ' ');
            free(sent);
            /*create new struct of type AssemblyLine and push it to the given List*/
            push(&AssemblyProg, newLine(line++ ,argsLength(args) , args ));
        }
        fclose(file);
    return AssemblyProg;
}

