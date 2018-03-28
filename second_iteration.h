/*the function that performs second iteration duties*/
/*mostly updating the binary code, for the apropriate label addresses*/
int second_iteration(List* assembly,char* fileName,List* bincode, List* Label_Tabel, void* extern_Labels);

/*a routine that updates the table of labels, after the first iteration whenever*/
/* the update is for entry Labels*/
int updateEntry(void* Labels, void* entries, char* fileName);

/* function that checks if a label was defined more then once in a code */
int label_validation(List* Label_Table,char* fileName);