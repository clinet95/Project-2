// vms stands for Virtual Memory System

#include <stdio.h>
#include <stdlib.h>

//An access is every line item in the trace file.
//It is basically the memory address being requested as well as the type of memory access it is. Read/Write
struct Access{
	int bitString[32];
	char type;
	};
	
//Prototype functions
void loadBitString(char input[], struct Access* access);
void loadAccesses(char* file);

//personal debug functions
void printAccessBitString(struct Access* access);


int main(char traceFile[], int frames, char alg[], char mode[]){

	loadAccesses(traceFile);
	
	char input[] = "190aff38"; //created a test address
	struct Access* access = malloc(sizeof(struct Access)); //create and allocate memory for access
	loadBitString(input, access); //convert hexidecimal address into binary
	printAccessBitString(access); //print access' bit string
	return 0;
}

void loadBitString(char input[], struct Access* access){
	int i = 0;
	printf("%s\n", input);
	for(i; i < 8; i++){
		switch (input[i]){
			case '0': //0000
				access->bitString[i * 4 + 3] = 0;
				access->bitString[i * 4 + 2] = 0;
				access->bitString[i * 4 + 1] = 0;
				access->bitString[i * 4]	 = 0;
				break;
			case '1': //0001
				access->bitString[i * 4 + 3] = 1;
				access->bitString[i * 4 + 2] = 0;
				access->bitString[i * 4 + 1] = 0;
				access->bitString[i * 4] 	 = 0;			
				break;
			case '2': //0010
				access->bitString[i * 4 + 3] = 0;
				access->bitString[i * 4 + 2] = 1;
				access->bitString[i * 4 + 1] = 0;
				access->bitString[i * 4] 	 = 0;			
				break;
			case '3': //0011
				access->bitString[i * 4 + 3] = 1;
				access->bitString[i * 4 + 2] = 1;
				access->bitString[i * 4 + 1] = 0;
				access->bitString[i * 4] 	 = 0;			
				break;
			case '4': //0100
				access->bitString[i * 4 + 3] = 0;
				access->bitString[i * 4 + 2] = 0;
				access->bitString[i * 4 + 1] = 1;
				access->bitString[i * 4] 	 = 0;			
				break;
			case '5': //0101
				access->bitString[i * 4 + 3] = 1;
				access->bitString[i * 4 + 2] = 0;
				access->bitString[i * 4 + 1] = 1;
				access->bitString[i * 4] 	 = 0;			
				break;
			case '6': //0110
				access->bitString[i * 4 + 3] = 0;
				access->bitString[i * 4 + 2] = 1;
				access->bitString[i * 4 + 1] = 1;
				access->bitString[i * 4] 	 = 0;			
				break;
			case '7': //0111
				access->bitString[i * 4 + 3] = 1;
				access->bitString[i * 4 + 2] = 1;
				access->bitString[i * 4 + 1] = 1;
				access->bitString[i * 4] 	 = 0;			
				break;
			case'8': //1000
				access->bitString[i * 4 + 3] = 0;
				access->bitString[i * 4 + 2] = 0;
				access->bitString[i * 4 + 1] = 0;
				access->bitString[i * 4] 	 = 1;			
				break;
			case '9': //1001
				access->bitString[i * 4 + 3] = 1;
				access->bitString[i * 4 + 2] = 0;
				access->bitString[i * 4 + 1] = 0;
				access->bitString[i * 4] 	 = 1;			
				break;
			case 'a': //1010
			case 'A':
				access->bitString[i * 4 + 3] = 0;
				access->bitString[i * 4 + 2] = 1;
				access->bitString[i * 4 + 1] = 0;
				access->bitString[i * 4] 	 = 1;
				break;
			case 'b': //1011
			case 'B':
				access->bitString[i * 4 + 3] = 1;
				access->bitString[i * 4 + 2] = 1;
				access->bitString[i * 4 + 1] = 0;
				access->bitString[i * 4] 	 = 1;			
				break;
			case 'c': //1100
			case 'C':
				access->bitString[i * 4 + 3] = 0;
				access->bitString[i * 4 + 2] = 0;
				access->bitString[i * 4 + 1] = 1;
				access->bitString[i * 4] 	 = 1;			
				break;
			case 'd': //1101
			case 'D':
				access->bitString[i * 4 + 3] = 1;
				access->bitString[i * 4 + 2] = 0;
				access->bitString[i * 4 + 1] = 1;
				access->bitString[i * 4] 	 = 1;			
				break;
			case 'e': //1110
			case 'E':
				access->bitString[i * 4 + 3] = 0;
				access->bitString[i * 4 + 2] = 1;
				access->bitString[i * 4 + 1] = 1;
				access->bitString[i * 4] 	 = 1;			
				break;
			case 'f': //1111
			case 'F':
				access->bitString[i * 4 + 3] = 1;
				access->bitString[i * 4 + 2] = 1;
				access->bitString[i * 4 + 1] = 1;
				access->bitString[i * 4] 	 = 1;			
				break;
			default:
				access->bitString[i * 4 + 3] = -1;
				access->bitString[i * 4 + 2] = -1;
				access->bitString[i * 4 + 1] = -1;
				access->bitString[i * 4] 	 = -1;		
		}	
	}
}
void loadAccesses(char* input){
	FILE* file;
	file = fopen(input, "r");
	printf("%s", *input);
	if (file == NULL){
		printf("Error opening file.\n");
		exit(1);
	}
	//while(){
		char addr[8];
		char rw;
		fscanf(file,"%x %c",&addr,&rw);	
		printf("%x %c\n", addr, rw);
	//}
}

//personal debug functions
void printAccessBitString(struct Access* access){
	printf("%d%d%d%d %d%d%d%d %d%d%d%d %d%d%d%d %d%d%d%d %d%d%d%d %d%d%d%d %d%d%d%d \n",
			access->bitString[0], access->bitString[1], access->bitString[2], access->bitString[3],
			access->bitString[4], access->bitString[5], access->bitString[6], access->bitString[7],
			access->bitString[8], access->bitString[9], access->bitString[10], access->bitString[11],
			access->bitString[12], access->bitString[13], access->bitString[14], access->bitString[15],
			access->bitString[16], access->bitString[17], access->bitString[18], access->bitString[19],
			access->bitString[20], access->bitString[21], access->bitString[22], access->bitString[23],
			access->bitString[24], access->bitString[25], access->bitString[26], access->bitString[27],
			access->bitString[28], access->bitString[29], access->bitString[30], access->bitString[31]);
}