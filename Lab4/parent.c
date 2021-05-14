#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/wait.h>

const char* FILE_NAME = "file.txt";

void getString(char* str, int* cap, int* len){
	char c = getchar();
		while (c != EOF) {
        	str[(*len)++] = c;
        	if ((*len) >= (*cap)) {
            	(*cap) *= 2;
            	str = (char*) realloc(str, (*cap) * sizeof(char)); 
        	}
        	c = getchar();        
    	}
    	str[(*len)] = '\0';
}

int main(){
	struct stat statBuf;
	int len = 0;    
	int cap = 1;
	char *str = (char*) malloc(cap * sizeof(char));

	printf("Введите строки:\n");
	char c = getchar();
		while (c != EOF) {
        	str[(len)++] = c;
        	if (len >= cap) {
            	cap *= 2;
            	str = (char*) realloc(str, cap * sizeof(char)); 
        	}
        	c = getchar();        
    	}
    str[len] = '\0';

	int tmpfd = open(FILE_NAME, O_CREAT | O_RDWR, S_IREAD | S_IWRITE);
	if (tmpfd == -1){
		perror("Ошибка создания файла!\n");
		exit(errno);
	}

	if(ftruncate(tmpfd,getpagesize()) == -1){
        printf("Error: ftruncate\n");
        return -1;
    }

	//write(tmpfd, str, strlen(str));
	

	//if(fstat(tmpfd, &statBuf) < 0){
	//	perror("fstat error");
	//	exit(errno);
	//}
	
	// MAP_SHARED - изменения в памяти немедленно будут отражаться в основном файле
	// Возвращает указатель на начало памяти
	char* mappedFile = mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED, tmpfd, 0);

	if (mappedFile == MAP_FAILED){
		perror("mmap error");
		exit(errno);
	}

	strcpy(mappedFile, str);

	free(str);	

	close(tmpfd);

	pid_t id = fork();
    if(id == -1){
        printf("Fork error\n");
        exit(0);
    }  
	else if(id > 0){ //PARENT
    	int status1;
    	waitpid(id, &status1, 0);

    	pid_t id2 = fork();
    	if(id2 == -1){
    		printf("Fork error!\n");
    		exit(-1);
    	} 
		else if(id2 > 0){	//PARENT
    		int status2;
    		waitpid(id2, &status2, 0);

    		char* result = (char *)malloc(len * sizeof(char));
	    	strcpy(result, mappedFile);
	    	printf("\nРезультат работы:\n");
	    	printf("%s\n", result);

	    	free(result);
	    	remove(FILE_NAME);
	    	if (munmap(mappedFile, statBuf.st_size) < 0) {
				perror("Can't munmap file");
				exit(1);
	    	}
    	} 
		else {
			execl("child2", "child2", FILE_NAME, NULL);
		}
    	
    } 
	else if (id == 0) 
		execl("child1", "child1", FILE_NAME, NULL);
	else{
        printf("Fork error 2\n");
        exit(-1);
    }
	return 0;
}