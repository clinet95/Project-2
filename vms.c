// vms stands for Virtual Memory System

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//An access is every line item in the trace file.
//It is basically the memory address being requested as well as the type of memory access it is. Read/Write
struct Access{
	int bitString[32];
	char type;
	struct Access* nextAccess;
	};
	
//Prototype functions
void loadBitString(char input[], struct Access* access);
char* decimalToBinary(int input);
void loadAccesses(char input[], struct Access* a);
int* stringToIntArray(char input[], int size);
//personal debug functions
void printAccessBitString(struct Access* access);
void printAccesses(struct Access* access); //prints provides access bitstring as well as the accesses linked


int main(int argc, char* argv[]){
	
	struct Access* accessHead = malloc(sizeof(struct Access)); //create and allocate memory for access

	loadAccesses(argv[1], accessHead); //reads in every trace from trace file and creates structs for them. Then, creates a linked list out of them
	printAccesses(accessHead); //prints all accesses starting from the head

	return 0;
}

//converts decimal to binary
char* decimalToBinary(int input){
   int c, d, count;
   char *output;
   
   count = 0;
   output = (char*) malloc(32 + 1);//chars are one byte, plus additional byte for null char @ end
   
   if (output == NULL)
      exit(1);
     
   for (c = 31; c >= 0; c--){
      d = input >> c;
     
      if (d & 1){
         *(output + count) = 1 + '0';
      } else{
         *(output + count) = 0 + '0';
	  }
      count++;
   }
   *(output + count) = '\0';
   
   return  output;
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
void loadAccesses(char input[], struct Access* a){
	FILE* file;
	file = fopen(input, "r");
	if (file == NULL){
		printf("Error opening file.\n");
		exit(1);
	}
	//used to keep track of previous access pointer
	struct Access* prevAccess = malloc(sizeof(struct Access)); //create and allocate memory for access

	int lines = 0;
	while(!feof(file)){ //loop through each line of the file until every line has been read
		int addr;
		char rw;
		fscanf(file,"%x %c", &addr,&rw);
		if (lines ==0){ //create head node
			int i = 0;
			for(i; i < 32; i++){
				a->bitString[i] = decimalToBinary(addr)[i] - '0'; //convert each char value into decimal and store in headAccess
			}
			a->nextAccess = NULL;
			a->type = rw;
			prevAccess = a;
		} else { //adding additional nodes
			struct Access* access = malloc(sizeof(struct Access)); //create and allocate memory for access
			int i = 0;
			for(i; i < 32; i++){
				access->bitString[i] = decimalToBinary(addr)[i] - '0'; //convert each char value into decimal and store in headAccess
			}
			prevAccess->nextAccess = access; //set  previous node's "nextaccess" value to current
			access->nextAccess = NULL;
			access->type = rw;
			prevAccess  = access; //now that this node is done, set it as previous so that the next iteration can reference it
		}
		lines++;
	}
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

void printAccesses(struct Access* access){
	struct Access* currAccess = access;
	while(currAccess->nextAccess != NULL){
		printAccessBitString(currAccess);
		currAccess = currAccess->nextAccess;
	}
}