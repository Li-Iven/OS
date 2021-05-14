#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main(){
	char *str = NULL, c;
    int len = 1;
    str = (char*) malloc(sizeof(char));
    while((c = getchar()) != EOF) {
        str[len - 1] = c;
        len++;
        str = (char*) realloc(str, len);
    }
    str[len - 1] = '\0';
	
	for(int i = 0; i < len + 1; ++i){
        str[i] = tolower(str[i]);
    }
	write(1, str, len + 1);
	free(str);
    return 0;
}