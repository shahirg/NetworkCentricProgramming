#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

int main(int argc, char* argv[]){
	// min number of for argc = 3
	// exe <filename> <substringstring>
	if(argc <3) {
		perror("Not enough input arguements\n");
		exit(1);
	}
	//check flag
	int flag = strcmp(argv[1], "--systemcalls") == 0 ? 1:0;

	//min argc value if systemcall is 4 because you need file and 
	if(flag && argc < 4) {
		perror("Not enough input arguements\n");
		exit(1);
	}

	//printf("%d",argc);
	//number of string is dependent on weather or not there is a flag
	int numSubstrings = flag ? (argc - 3):(argc -2);

	//create buffer for substrings
	//char* strBuffer=(char*) malloc (sizeof(char)*numSubstrings);

	//for(int i = 0; i < numSubstrings; i++){
	//	//get substrings based on weather or not flag
	//	char* str = flag ? argv[i + 3]:argv[i + 2];
	//	//lower case string to make case insensitive
	//	for(char *p=str; *p; p++) 
	//		*p = tolower(*p);
	//	strBuffer[i] = str;
	//}

	if(!flag){
		
		//check if file exists
		// if(access(argv[1], F_OK) != 0){
		// 	printf("File does not exist");
		// 	return 1;
		// }
		unsigned int count = 0;

		//pointer to file
		FILE *pFile;
		size_t sz = 0; 
		ssize_t len; 
		char* line = NULL; //line


		//open the file
		pFile = fopen(argv[1],"rb");
		if(pFile==NULL){
			perror("Reading error\n");
			exit(1);
		}

		//for each line in the file...
		while((len = getline(&line, &sz, pFile)) >= 0) {

			printf("%s",line);
			// //for each substring passed in
			// for(int i = 0; i < size; i++){

			// //get the substring
			// char* substring =  buffer[i];

			// //temp pointer to line in file
			// const char* word = line;

			// //strstr points to beginning of occurence of substring in string, otherwise returns null
			// while((word = strstr(word,substring)) != NULL){
			// 	count++;
			// 	word++;
			// }

			// //print the count of the substring in the file for the word
			// printf("%d\n",count);
			// //put count back to zero
			// count = 0;

			// }


		}
		free(line);


			
	}
	else { //flag true
		//code
	}
	
	return 0;
}

// FILE *pFile;
// long lSize;
// char*buffer; 
// size_t read;

// //check file err
// pFile = fopen(argv[1],"rb");
// if(pFile==NULL){
// 	perror("Reading error\n");
// 	exit(1);
// }
// buffer = (char*)malloc(sizeof(char*)lSize);
// if(buffer == NULL){
// 	perror("Memory error\n");
// 	exit(2);
// }
// result = fread(buffer,1,lSize,pFile);
// if(result != lSize) {
// 	perror("Reading error\n");
// 	exit(3);
// }

// int count = 0;
// while((read = fread(buffer,1, sizeof(char), pFile)) >0){

// 	printf("%s",buffer);
// }
// fclose(pFile);
