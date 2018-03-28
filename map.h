#define OPS_C 21
#define MAX_LEN 81

enum type {comment=-1, empty=0, assign = 1, action = 2};  /* enum for error management Logger*/
enum addressing {imidiate, direct, access, regist};         /* addressing values */
enum range { POSITIVE_EIGH_BIT = 127, NEGATIVE_EIGHT_BIT = -128, POSITIVE_TEN_BIT = 511, NEGATIVE_TEN_BIT = -512};
typedef struct {
    char* cmd;
    int id;                     /*numeric value of the item- decimal opcode*/
    int src_addressing;        /*which operands allowed for first operand (addressing) */
    int dist_addressing;       /*which operands allowed for second operand (addressing) */
    int operands_count;
}command;

typedef struct operand_map {
    char* item;             /*the context of the operand*/
    int addressing_type;    /*operand addressing type*/
}opernad;


#define FileNotFound -2
#define INIT_ADDRESS 100

/*Macro to log an error*/
#define ERROR(message, item  ) \
                temp_string= concatString (Line->operands); \
                strcpy(errorMsg,"");  errorFlag++;          \
                appendStr(errorMsg,COUNT+2,fileName,"\n\"",temp_string,"\" - "message" \"",item,"\".");   free(temp_string);      logErr(Line->line,errorMsg);    

/*Macro to push empty array of integers into the binary code*/                           
#define PUSH_EMPTY binary = calloc(10,sizeof(int)); push( (bincode),binary);  inc_IC_DC(IC); 

/*Macro to get the numeric value of a given operand of type string */
#define NUMERIC my_atoi((Line->operands[operand_counter])+1)

/*Macro to get the addressing value of a single operand */
#define OP_ADDRESSING operandType(Line->operands[operand_counter])

/*Macro to get the operand as a string*/
#define OPERAND (Line->operands[operand_counter])

/*Macro to push a Label to the list of labels*/
#define PUSH_LABEL(IC,is_OP) push(((List**)Label_Tabel), newLabel(Line->operands[0], get_IC_DC(IC),Line->line, 0,is_OP)); 


enum IC_DC {IC,DC};
enum assignment {DATA=16, STRING, STRUCT, ENTRY, EXTERN};   /* enum to manage and recognize between assignment operation and erroneous commands */
enum count_c {COUNT_ZERO,COUNT_ONE,COUNT_TWO,COUNT_THREE,COUNT_FOUR,COUNT_FIVE, COUNT_SIX, COUNT_SEVEN, COUNT_EIGHT, COUNT_NINE, COUNT_TEN};

#define COUNT 4
#define ACTION_COUNT 16
#define ASSIGN 21


