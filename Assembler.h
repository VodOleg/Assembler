/* creates new item of type error, and pushes it to the global errors List*/
void logErr(int line, char* message);

/* increase global variable IC or DC by 1*/
void inc_IC_DC(int IC_DC);

/* get the value of global variable IC or DC*/
int get_IC_DC(int IC_DC);

/* the function that manages the synchornous tasks for a single file of assembly.as code */
void manager(FILE *file, char* fileName);

/* function that creates a 3 new output files for a give assembled binary code */
void makeOutputs(List * bin_code, List * bin_data, List * Labels, char* fileName, List* extern_Labels);
