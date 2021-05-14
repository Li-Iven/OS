#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(){
	int l = -1;
    char *str = (char*) malloc(sizeof(char));
    do{
        ++l;
        str = (char*) realloc(str, (l+1) * sizeof(char));
        read(0, &str[l], sizeof(char));
    }while(str[l] != '\0');

	int s = l;
	for(int i = 0; i < s + 1; ++i){
		if(str[i] == ' ')
			str[i] = '_';
	}
	str[l] = '\0';
	write(1, str, l + 1);
	free(str);
}