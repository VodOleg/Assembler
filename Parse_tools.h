
/* function that creates array of strings tokenized by token */
char** parseStr(char *str, char token);

/*check if a given char is a white space*/
int isWhiteSpace(int c);

/*function that checks number of errors for a missplaced comma, if the function found erroneous position*/
/*the function will return POSITIVE_TEN_BIT value, else the func returns the number of commas*/
int invalid_comma(char* input);

/*replacing all commas in a string with a space, unless its in a block of string*/
void replaceCommas(char* in);

/*counts the occurance of a given char in a given string*/
int countToken(char* string, char token);

/*nullifies whitespace in the beginning of a string and the end of it*/
/*the function returns new string (allocated) without the spaces*/
char* trimWhiteSpace(char* source);

/*count the number of seperating white spaces in a given string*/
int countSeperators(char *str);

/*function that reads the file, and appends its contexts into a given List of AssemblyCode struct*/
List* Append_parsed_line(FILE *file,List* AssemblyProg, char* fileName, int* error);