#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main(){
    int fd1[2];
    int fd2[2];
    int fd3[2];
    if(pipe(fd1) < 0 || pipe(fd2) < 0){
    	printf("Pipe error!\n");
		exit(-1);
    }

    pid_t id = fork();
    if(id == -1){
        printf("Fork error\n");
        exit(0);
    }
    else if(id > 0){
        printf("Введите строки:\n");
		char *str = NULL, c;
        int len = 1;
        str = (char*) malloc(sizeof(char));
        while((c = getchar()) != EOF) {
            str[len - 1] = c;
            len++;
            str = (char*) realloc(str, len);
        }
        str[len - 1] = '\0';
        close(fd1[0]);
        write(fd1[1], str, len + 1); 
        close(fd1[1]); 
        free(str);
        if(pipe(fd3) < 0){
            printf("Pipe error!\n");
            exit(-1);
        }

        pid_t id2 = fork();
        if(id2 == -1){
            perror("Fork error!");
            return -1;
        }
        else if(id2 > 0){
            close(fd2[0]);
            close(fd2[1]);

            int l = -1;
            char *strr = (char*) malloc(sizeof(char));
            close(fd3[1]);
            do{
                ++l;
                strr = (char*) realloc(strr, (l+1) * sizeof(char));
                read(fd3[0], &strr[l], sizeof(char));
            }while(strr[l] != '\0');
            close(fd3[0]);
            printf("\nРезультат работы:\n");
            printf("%s\n", strr);
            free(strr);
        }
        else if(id2 == 0){
            dup2(fd2[0], 0);
            close(fd2[0]);
            close(fd2[1]);

            dup2(fd3[1], 1); 
            close(fd3[0]);
            close(fd3[1]);

            execl("child2", "child2",  NULL);
        }   
    }
    else if(id == 0){
        dup2(fd1[0], 0);
        close(fd1[0]);
        close(fd1[1]);

        dup2(fd2[1], 1);
        close(fd2[0]);
        close(fd2[1]);

        execl("child1", "child1",  NULL);
    }

    return 0;
}
