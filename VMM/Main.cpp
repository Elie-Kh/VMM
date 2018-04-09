#include<iostream>
#include<stdio.h>
#include<vector>
using namespace std;

int main() {
	FILE * backingStore;
	char * buffer;
	size_t result;
	long fileSize;
	backingStore = fopen("BACKING_STORE.bin", "rb");
	//vector<char> inputs(1000);
	if (backingStore == NULL) {
		fputs("File error", stderr); exit(1);
	}

	fseek(backingStore, 0, SEEK_END);
	fileSize = ftell(backingStore);
	rewind(backingStore);
	buffer = (char*)malloc(sizeof(char)*fileSize);
	if(buffer == NULL) { fputs("File error", stderr); exit(2); }
	result = fread(buffer, 1, fileSize, backingStore);
	if (result != fileSize) {
		fputs("Reading error", stderr); exit(3);
	}
	for (int i = 0; i < fileSize; i++) {
		cout << (int)buffer[i];
	}
	system("pause");
	return 0;
}