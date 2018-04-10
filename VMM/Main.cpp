#include<iostream>
#include<stdio.h>
#include<vector>
#include<string>
#include<fstream>
using namespace std;

int pageNumber(int address){
    int pageNmb = address;
    pageNmb = pageNmb & 65535; //mask
    pageNmb >> 15;
    return pageNmb;
}

int searchTLB(int page, vector<int> pageTLB, vector<int> frameTLB){
    for(int i = 0; i < frameTLB.size(); i++){
        if(pageTLB[i]== page){
            return frameTLB[i];
        }
        else return -1; //TLB-miss
    }
}

int main() {
	FILE * backingStore;
	char * buffer;
	size_t result;
	long fileSize;
	ifstream addresses("addresses.txt");
	string readz;
	int  entries = 0;
	vector<int> entriez;
	int tempo = 0;
	backingStore = fopen("BACKING_STORE.bin", "rb");
	//vector<char> inputs(1000);
	if (backingStore == NULL) {
		fputs("File error", stderr); exit(1);
	}
	
	fseek(backingStore, 0, SEEK_END);
	fileSize = ftell(backingStore);
	rewind(backingStore);
	//entriez.push_back(fileSize);
	while (!addresses.eof()) {
		getline(addresses, readz,'\n');
		entriez.push_back(stoi(readz, nullptr, entries));
		//entriez[tempo] = stoi(readz, nullptr, entries);
		
		
		tempo++;
	}
	for (int i = 0; i < entriez.size(); i++) {
		cout << entriez[i] << "\n";
	}
	buffer = (char*)malloc(sizeof(char)*fileSize);
	if(buffer == NULL) { fputs("File error", stderr); exit(2); }
	result = fread(buffer, 1, fileSize, backingStore);
	if (result != fileSize) {
		fputs("Reading error", stderr); exit(3);
	}
	//for (int i = 0; i < fileSize; i++) {
	//	cout << (int)buffer[i];
	//}
	system("pause");
	return 0;
}