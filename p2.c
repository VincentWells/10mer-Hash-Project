#include <stdio.h>
#include <stdlib.h>

/*
  this program takes a couple minutes to run for me; most of the time is just the printing.  It will also probably use a decent amount of memory.
*/

//simple dynamic array
struct dA{
	int *vals;
	int index;
	int size;
};

//initialize a dynamic array
int initDA(struct dA* a, int s){
	a->size = s;
	a->index = 0;
	a->vals = (int*)malloc(s*sizeof(int));
	return 0;
}

//insert an index to a given dynamic array
int insert(struct dA* a, int i){
	if(a->index == a->size - 1){
		a->size = a->size * 2;
		a->vals = (int*)realloc(a->vals, a->size * sizeof(int));
	}
	a->vals[a->index++] = i;
	return 0;
}

//struct the hashtable will be composed of
struct hash{
	char key[10];
	struct dA* indices;
};

struct hash ht[1048576]; //this is 4^10, the max number of entries

//initialized the hashtable
int init(){
	for(int i = 0; i < 1048576; i++){
		//printf("init check\n");
		ht[i].key[0] = 'X';
		struct dA* d = (struct dA*)malloc(sizeof(struct dA));
		ht[i].indices = d;
		initDA(ht[i].indices, 10);
	}
}

//One of the assignment's requirements was that case be ignored, so this funtion converts everything to capitalization
char convert(char c){
	switch(c){
	case 'a':
	case 'A':
		return 'A';
	case 'c':
	case 'C':
		return 'C';
	case 'g':
	case 'G':
		return 'G';
	case 't':
	case 'T':
		return 'T';
	default:
		return 'X';
	}
}

int digify(char c){
	switch(c){
	case 'A':
		return 0;
	case 'C':
		return 1;
	case 'G':
		return 2;
	case 'T':
		return 3;
	default:
		return -1;
	}
	return 0;
}

//computers proper index for a 10mer and inserts it there
int enhash(char str[], int i){
	int hIndex = 0;
	int digit = 0;
	for(int j = 0; j < 10; j++){
		digit = digify(str[j]);
		for(int k = 0; k < j; k++){
			digit <<= 2;
		}
		hIndex += digit;
	}
	if(ht[hIndex].key[0] == 'X'){
		for(int l = 0; l < 10; l++){
			ht[hIndex].key[l] = str[l];
		}
	}
	insert(ht[hIndex].indices, i);
	return 0;
}

int main(int argc, char* argv[]){
	printf("Enter the file address: ");
	char address[256]; //arbitrary length that should be long enough
	scanf("%s", &address);
	FILE *fp;
	fp = fopen(address, "r");
	if(fp != 0){
		init();
		char c;
		char str[10];
		int i = 1;
		int m = -9;
		int f = 0;
		//initialize the list
		for(int k = 0; k < 10; k++){
			if((c = fgetc(fp)) != EOF){
				c = convert(c);
				if(c == 'X'){
					f = 1;
				}
				str[k] = c;
			}
		}
		if(f == 0){
			enhash(str, 0);
		}
		while((c = fgetc(fp)) != EOF){
			if(c == '\n'){
				i++;
				m++;
				continue;
			}
			c = convert(c);
			for(int j = 0; j < 9; j++){
				str[j] = str[j + 1];
			}
			str[9] = c;
			if( c == 'X'){
				m = i;
			} else if(i - m == 10){
				//printf("%d\n", i);
				enhash(str, i);
				m++;
			}
			i++;
		}
		for(int h = 0; h < 1048576; h++){
			if(ht[h].key[0] != 'X'){
				printf(ht[h].key);
				printf(" ");
				for(int c = 0; c < ht[h].indices->index; c++){
					printf("%d", ht[h].indices->vals[c]);
					printf("   ");
				}
				printf("\n");
			}
		}
	} else {
		printf("Cannot open file; check that the address is valid and the correct permissions are applied.");
	}
	return 0;
}

