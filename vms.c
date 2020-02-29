// vms stands for Virtual Memory System
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//An access is every line item in the trace file.
//It is basically the memory address being requested as well as the type of memory access it is. Read/Write
struct Access{
	int bitString[32];
	int ramIndex;
	int hexVal;
	int isDirty;
	char type;
	struct Access* nextAccess;
	};
	
struct fifoQueue{
	int key;
	struct Access* front,* end;
};	
//GLOBAL VARIABLES
int numOfFrames;
int* RAM;
char replacementMethod;
int debugMode;
int fifoIndex;
struct fifoQueue* fifoQueue;
int eventsInTrace;
int diskReads;
int diskWrites;
	
//Prototype functions
void loadBitString(char input[], struct Access* access);
char* decimalToBinary(int input);
void loadAccesses(char input[], struct Access* a);
int* stringToIntArray(char input[], int size);
int getPageNum(struct Access* a);
void initializeSimulator(char frames[], char mode[], char debug[]);
void simulate(struct Access* accessHead);
void insertPage(int pageNum, int frameNum, struct Access* trace);
struct Access* getAccess(int frameNum, struct Access* accessHead);

//page replacement policies here
int randomPageRemoval();//returns an integer which will be the index of the page's location in RAM
int fifoPageRemoval(); //returns an integer which will be the index of the page's location in RAM

//personal debug functions
void printAccessBitString(struct Access* access);
void printAccesses(struct Access* access); //prints provides access bitstring as well as the accesses linked


int main(int argc, char* argv[]){
	
	struct Access* accessHead = malloc(sizeof(struct Access)); //create and allocate memory for access
	
	initializeSimulator(argv[2], argv[3], argv[4]);
	loadAccesses(argv[1], accessHead); //reads in every trace from trace file and creates structs for them. Then, creates a linked list out of them
	simulate(accessHead);
	
	
	printf("\nTotal memory frames: %d\n", numOfFrames);
	printf("Events in trace: %d\n", eventsInTrace);
	printf("Total disk reads: %d\n", diskReads);
	printf("Total disk writes: %d\n", diskWrites);
	//put debug stuff here
	if (debugMode){

	}

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
			eventsInTrace = -1;
			int i = 0;
			for(i; i < 32; i++){
				a->bitString[i] = decimalToBinary(addr)[i] - '0'; //convert each char value into decimal and store in headAccess
			}
			a->isDirty = 0;
			a->hexVal = addr;
			a->nextAccess = NULL;
			a->type = rw;
			a->ramIndex = -1;
			prevAccess = a;
		} else { //adding additional nodes
			struct Access* access = malloc(sizeof(struct Access)); //create and allocate memory for access
			int i = 0;
			for(i; i < 32; i++){
				access->bitString[i] = decimalToBinary(addr)[i] - '0'; //convert each char value into decimal and store in headAccess
			}
			prevAccess->nextAccess = access; //set  previous node's "nextaccess" value to current
			access->nextAccess = NULL;
			a->isDirty = 0;
			access->hexVal = addr;
			access->type = rw;
			access->ramIndex = -1;
			prevAccess  = access; //now that this node is done, set it as previous so that the next iteration can reference it
		}
		lines++;
		eventsInTrace++;
	}
	
	if(debugMode){
		printf("Successfully loaded accesses from trace file.\n");
	}
}
int getPageNum(struct Access* a){
	int i, sum, j, bit, pow;
	bit = 19;
	sum = 0;//initialize
	// loop through the first 20msb of  access and convert each bit into it's decimalrepresentation
	//Then, add it to the sum
	for(i = 0; i < 20; i++){ 
		pow = 1;
		//calculate 2 to the power of i
			for (j = 0; j < bit; j++){
				pow *= 2;
			}
			sum += a->bitString[i] * pow;
			bit--;
	}
	return sum;
}
void initializeSimulator(char frames[], char mode[], char debug[]){
	//initialize global variables
	eventsInTrace = 0;
	diskReads = 0;
	diskWrites = 0;
	
	//allocate array used to act as RAM
	numOfFrames = atoi(frames);
	RAM = (int*)malloc(numOfFrames * sizeof(int));
	
	//initialze fifo queue
	fifoQueue = (struct fifoQueue*)malloc(sizeof(struct fifoQueue)); 
    fifoQueue->front = fifoQueue->end = NULL; 
	
	//Fill RAM with -1 to know when RAM is full or not
	int i;
	for(i = 0; i < numOfFrames; i++){
		RAM[i] = -1;
	}
	
	//set replacementMethod.
	//f = FIFO, r = random, v = vms, lru = l;
	if(strcmp(mode, "rdm") == 0){
		replacementMethod = 'r';
	} else if (strcmp(mode, "lru") == 0){
		replacementMethod = 'l';
	} else if (strcmp(mode, "fifo") == 0){
		replacementMethod = 'f';
	} else if (strcmp(mode, "vms") == 0){
		replacementMethod = 'v';
	} else {
		printf("Page fault detected. No valid page replacement method was input. Program terminating.\n");
		exit(1);
	}
	
	//set debugMode
	//1 = debug mode on,  0 = debug mode off
	if(strcmp(debug, "debug") == 0){
		debugMode = 1;
	} else {
		debugMode = 0;
	}
	
	if(debugMode){
		printf("Initializing main memory.\n");
		printf("Successfully allocated %d frames in memory.\n", numOfFrames);
		printf("Page replacement method: %s\n", mode);
	}
}
void simulate(struct Access* accessHead){

	if (debugMode){
		printf("Begginning simulation.\n\n");
	}
	int RAMHits = 0, RAMMisses = 0; 
	struct Access* trace = malloc(sizeof(struct Access*));
	trace = accessHead;
	
	/*trace->ramIndex = accessHead->ramIndex;
	trace->type = accessHead->type;
	trace->hexVal = accessHead->hexVal;
	trace->isDirty = accessHead->isDirty;
	int i;
	for(i = 0; i < 32; i++){
		trace->bitString[i] = accessHead->bitString[i];
	}
	*/
	int tick = 0;
	//loop through every trace loaded
	while(trace->nextAccess != NULL){
		tick++;
		
		if(debugMode){
			//if(tick % 1000 == 0)
				//printf("%d\n", tick);
			switch (tick){
			case 100000:
					printf("10%%\n");
				break;
			case 200000:
					printf("20%%\n");
				break;
			case 300000:
					printf("30%%\n");
				break;
			case 400000:
					printf("40%%\n");
				break;
			case 500000:
					printf("50%%\n");
				break;
			case 600000:
					printf("60%%\n");
				break;
			case 700000:
					printf("70%%\n");
				break;
			case 800000:
					printf("80%%\n");
				break;
			case 900000:
					printf("90%%\n");
				break;
			case 1000000:
					printf("100%%\n");
				break;
			}
		}
		int i;
		int pageFound = 0;
		//iterate through RAM trying to find current trace's page
		for(i = 0; i < numOfFrames; i++){
			if(RAM[i] == getPageNum(trace)){
				pageFound = 1;
			}
		}
		
		if(pageFound){ // current trace's page was found on RAM
			RAMHits++;
		} else { //current trace's page was not found on RAM
			RAMMisses++;
			
			//try to find empty frame
			int emptyFrameFound = 0;
			for(i = 0; i < numOfFrames; i++){
				if(RAM[i] == -1){ //check if there is an empty frame in RAM
					insertPage(getPageNum(trace), i, trace); //add current trace's page to RAM
					emptyFrameFound = 1; //set the flag saying that an empty frame was found(no page fault necessary)
					break;
				}
			}
			
			
			//if there was no empty frame found
			if(!emptyFrameFound){
				int newFrame;
				//remove frame
				switch(replacementMethod){
					case 'r':
						newFrame = randomPageRemoval(); //generate frame to place trace in
						//printf("Trying to removeframe: %d\n", newFrame);
						//check if frame about to be removed is dirty
						if(getAccess(newFrame, accessHead)->isDirty){
							getAccess(newFrame, accessHead)->isDirty = 0;
							diskWrites += 1;
						}
						insertPage(getPageNum(trace), newFrame, trace); //place page in frame
						break;
					case 'f':
						newFrame = fifoPageRemoval();
						//check if frame about to be removed is dirty
						if(getAccess(newFrame, accessHead)->isDirty){
							getAccess(newFrame, accessHead)->isDirty = 0;
							diskWrites += 1;
						}
						
						insertPage(getPageNum(trace), newFrame, trace);
						break;
					case 'l':
					
						break;
					case'v':
					
						break;
				}
			}
			
		}
		
		
		
		
		trace = trace->nextAccess;//increment tonext trace
	}
	
	diskReads = RAMMisses;
	if(debugMode){
		printf("RAM Hits:\t%d\n", RAMHits);
		printf("RAM Misses:\t%d\n", RAMMisses);
	}
	 
	
}
void insertPage(int pageNum, int frameNum, struct Access* trace){
	RAM[frameNum] = pageNum;
	trace->ramIndex = frameNum;
	//printf("Trace %x  being put in frame %d\n", trace->hexVal, trace->ramIndex);
	if(trace->type == 'W'){
		trace->isDirty = 1;
	}
	
	if(replacementMethod == 'f'){
		//add trace to fifoqueue
		struct Access* a = (struct Access*)malloc(sizeof(struct Access));
		a->ramIndex = trace->ramIndex;
		a->type = trace->type;
		a->hexVal = trace->hexVal;
		a->isDirty = 0;
		int i;
		for(i = 0; i < 32; i++){
			a->bitString[i] = trace->bitString[i];
		}
			// If queue is empty, then new node is front and rear both 
		if (fifoQueue->end == NULL) { 
			fifoQueue->front = fifoQueue->end = a; 
		} else {
			// Add the new node at the end of queue and change rear 
			fifoQueue->end->nextAccess = a; 
			fifoQueue->end = a; 
		}
	}
}
struct Access* getAccess(int frameNum, struct Access* accessHead){
	
	struct Access* currAccess = accessHead;
	while(currAccess->nextAccess  != NULL){
		if(currAccess->ramIndex == frameNum){
			return currAccess;
		}
		currAccess = currAccess->nextAccess;
	}
	struct Access* nullAccess = malloc(sizeof(struct Access*));
	nullAccess->ramIndex = -1;
	return nullAccess;
}

int randomPageRemoval(){
	return rand() % (numOfFrames);
}
int fifoPageRemoval(){
	
	
	// Store previous front and move front one node ahead 
    struct Access* temp = fifoQueue->front; 
  
    fifoQueue->front = fifoQueue->front->nextAccess; 
  
    // If front becomes NULL, then change rear also as NULL 
    if (fifoQueue->front == NULL) 
        fifoQueue->end = NULL; 
	int returnVal = temp->ramIndex;
	if(debugMode){
		//printf("Evicting %X from index %d.\n", temp->hexVal, temp->ramIndex);
	}
	
    free(temp); 
	return returnVal;
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
	
	int i;
	for(i = 0; i < numOfFrames; i++){
		printf("%x\n", getAccess(i, access)->hexVal);
	}
	/*
	struct Access* currAccess = access;
	while(currAccess->nextAccess != NULL){
		printAccessBitString(currAccess);
		currAccess = currAccess->nextAccess;
	}
	 * */
}


