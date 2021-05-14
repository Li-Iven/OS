#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <ctype.h>

void low(char* string, int l){
	for(int i = 0; i < l; ++i){
		string[i] = tolower(string[i]);
	}
}

int main(int argc, char* argv[]){
	void* adr;
	int fd = open(argv[1], O_RDWR, S_IRWXU);
	if (fd == -1){
		perror("Error creating shared child file!\n");
		exit(errno);
	}

	/*struct stat statBuf;
	if(fstat(fd, &statBuf) < 0){
		perror("fstat error");
		exit(errno);
	}*/

	char* string = (char*) malloc(getpagesize());
	if(string == NULL){
		printf("malloc error");
		return 1;
	}

	adr = mmap(NULL, getpagesize(), PROT_WRITE | PROT_READ , MAP_SHARED, fd, 0);
	if (adr == MAP_FAILED){
		perror("mmap error");
		exit(errno);
	}
	close(fd);

	strcpy(string, (char*)adr);
	int len = strlen(string);
	low(string, len);
	sprintf((char*) adr, "%s", string);

	if (munmap(adr, getpagesize()) < 0) {
        perror("Can't msync files");
        exit(1);
    }
}