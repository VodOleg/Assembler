#define dprint(expr) printf("\n---------------------DEBUG---------------------\n" #expr " = %d\n-----------------------------------------------\n",expr)
#define sprint(expr) printf("\n::"#expr":: %s\n",expr)

/*Implementation of a custom atoi function from the book*/
/*the function recieves array of chars (string) and casts the value into an int */
int my_atoi(char s[]);

/*count array of chars length*/
char length(char arr[]);

/*  check if input char c is a number*/
int isNumber(int c);

/*removes any white space in a string*/
void RemoveSpaces(char* source);

/*counts whitespaces in a string*/
int countWhiteSpaces(char input[]);

/*checks if a string has 2 consecutive commas in a raw*/
int hasConsecutiveCommas(char input[]);

/*printing the args in array of strings*/
void printargs(char ** in, int numargs);

/* private function for printing array of integers */
void printintarr(int* arr);

/*boolean function to check if array of integers of size ten is empty, has only nulls*/
int isEmpty_binary(int* arr);

/* count the number of arguments in a give array of arrays (counts till NULL) */
int argsLength(char** arg);

/*function that replaces in a given string the char_now by char_later*/
void replaceChar(char ch_now, char ch_later, char* string);

/*variadic function to stringify unknown number of strings into the destination string*/
void appendStr(char* dest, unsigned int count, ...);

/* a simple function that returns the last char in a given string*/
char lastChar(char* in);

/*boolean function that checks if a given string is a valid number that could be casted to int*/
int isInt(char* param);

/*checks if the string can be a label*/
int isLabel(char* label);

/* a simple function that returns numeric value for a given operand, to determine its proccessing */
int operandType(char* input);

/* private function to print binary array of size 10 */
void printBinary(int * bincode);

/*concatane a string from array of strings*/
char* concatString(char** args);

/* function that scans an int number and checks if its contains digit */
int containDigit(int src, int digit);

/* function that encodes an opcode */
void encode(int* binary,int id, int src, int dist, int ARE);

/*create array of integers that creates the binary value of ascii value of char*/
int* char_to_binary(char chr);

/* function that receives array of binary and converts the value temp to this binary representation in the array*/
void int_to_binary(int* arr,int temp, int size);

/* function that allocates memory for binary array of size 10 and converts the value of string into the binary */
int* num_to_binary(char* num);

/*print String, debugging purpses*/
void printstring(void* details);

/*encodes the binary code to have 8 bits of value, and 2 bits for ARE*/
/*0 = static(00), 1 = external(01) , 2 = relocatable (10) */
void updateAddress(int* arr, int val, int ARE);

/* operation that uses labels must be appended with ':' so it can be compared in Label List */
void fixOp(char* dest, char*src);

/*simple function to recieve the wierd 32 char for a given value*/
char getSymbol(int num);

/* converts an array of binnary code of size 10, to its wierd32 base representation */
char* converter(int* binary );

/* converts int to its wierd 32 value, returns string*/
char* int_to_32(int size);

/* checks if a give binary code is a representation of an extern label */
int extern_binary(void* binary);

/* converts an unsigned value of int to a string of the value (revert atoi) */
char* uint_to_string(unsigned int num);