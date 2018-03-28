
/*function that performs the tasks of the first routine*/
int routine_one(List* assembly, char* fileName, void* bincode, void* Label_Tabel, void* bindata, void* entry_Labels);

/* function that returns the command id if there is one or just number>100 if there is none */
int get_cmd_id(char* argument);

/*for every char in string, create binary array and push it to bin data, push extra word of null*/
void pushString(char* str,void* bindata );
