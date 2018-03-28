#define MAX_LEN 81

typedef struct{
    char* name;                         /*the name the label was written in the as code*/
    int address;                         /*saves the address where this label is saved*/
    int line;                            /* saves the line where this label was defined in the assembly code*/
    unsigned short isExtern;            /*boolean value for the label that indicates if its External-1 or its declared in the scope of this code-0*/
    unsigned short isOperation;         /*boolean value for the label that indicates if its operation-1 or assignment-0*/
    unsigned short isEntry;
}Label;

typedef struct{
    unsigned short line;                /*which line in the assembly code this line is*/
    unsigned short operandsNum;         /*number of operands ni that line*/
    char** operands;                    /*array of string which stores the operands*/
}AssemblyLine;

typedef struct{
    int line;                           /*in which line the error occured*/
    char* message;                      /*the error message to be displayed to the user*/
}Error;


/*constructor for a new line struct*/
void* newLabel(char* label, int address, int line, unsigned short isExtern, unsigned short isOperation);

/*constructor for a new line struct*/
void* newLine(unsigned short line,unsigned short operationCount, char** operation);

/*initialize an array of unsigned ints to the size of the binary code */
void* BinaryCode(unsigned int codeSize);

/*a function to set a binary code to a cell in the array*/
void setCodeLine(unsigned int index, unsigned int value, unsigned int* code);

/* function that cleans all arguments in a give array of arguments */
void freeArgs(char **args);

/*private function to free allocated space of each array of arguments per line*/
void freeAssembly(void *details);

/*private print function of a AssemblyLine struct*/
void printAssembly(void *details);

/* creates new array of strings and copies the contents from src to dest */
void copyArrayOfString(char** dest, char** src);

/*function that appends new Errors to a list of global errors*/
Error* logError(unsigned int line, char* message);

/* private destroyer for struct of type Error */
void freeError(void *details);

/*private function to print struct of type Error*/
void printErrors(void *error);

/* function that supposed to release the details inside the List struct*/
void freebin(void* details);

/* a private function to print a single struct of type Label */
void printLabels(void* details);

/* a function that compares if the label pointed by details has the same name as the desired string pointed by item */
/* returns the pointer to the label if it is equal or null if it is not*/
void* findLabel(void* details, void* item);