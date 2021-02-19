#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

int main(int argc, char* argv[]){

	//check flag
	int flag = strcmp(argv[1], "--systemcalls") == 0 ? 1:0;
	
	printf("%d",argc);
	int numSubstrings = flag ? (argc - 3):(argc -2);

	char* strBuffer=(char*) malloc (sizeof(char)*numSubstrings);
	for(int i = 0; i < numSubstrings; i++){
		//get substrings based on weather or not flag
		char* str = flag ? argv[i + 3]:argv[i + 2];
		//lower case string to make case insensitive
		for(char *p=str; *p; p++) 
			*p = tolower(*p);

		strBuffer[i] = str;
	}
	if(!flag){
		
		//check if file exists
		if(access(argv[1], F_OK) != 0){
			printf("File does not exist");
			return 1;
		}
	
		FILE *pFile;
		long lSize;
		char*buffer;
		size_t read;

		//check file err
		pFile = fopen(argv[1],"rb");
		if(pFile==NULL){
			perror("Reading error\n");
			exit(1);
		}
		buffer = (char*)malloc(sizeof(char*)lSize);
		if(buffer == NULL){
			perror("Memory error\n");
			exit(2);
		}
		result = fread(buffer,1,lSize,pFile);
		if(result != lSize) {
			perror("Reading error\n");
			exit(3);
		}

		int count = 0;
		while((read = fread(buffer,1, sizeof(char), pFile)) >0){

			printf("%s",buffer);
		}
		fclose(pFile);

			
	}
	else { //flag true
		//code
	}
	printf("final exit");
	return 0;
}
