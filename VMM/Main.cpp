#include<iostream>
#include<stdio.h>
#include<vector>
#include<string>
#include<fstream>
#include<algorithm>
using namespace std;

int pageNumber(int address){
    int pageNmb = address;
    pageNmb = pageNmb & 65535; //mask for page number and offset
    pageNmb >> 15;
    return pageNmb;
}

int offset(int address){
    int offset = address;
    offset = offset & 255; //mask for offset
    return offset;
}

int searchTLB(int page, vector<int> pageTLB){
    for(int i = 0; i < pageTLB.size(); i++){
        if(pageTLB[i]== page){
            return i; //return index
        }
        else return -1; //TLB-miss
    }
}

void updateTLB(int page, int frame, int index, vector<int> pageTLB, vector<int> frameTLB){ //LRU
    pageTLB.insert(pageTLB.begin(),page); //insert page at beginning
    frameTLB.insert(pageTLB.begin(),frame); //insert frame at beginning
    pageTLB.erase(pageTLB.begin()+index);   //delete specified index
    frameTLB.erase(pageTLB.begin()+index);

}

int main() {

	FILE * backingStore;
char * buffer;
size_t result;
long fileSize;
ifstream addresses("addresses.txt");
string readz;
int  entries = 0;
vector<int> addressEntries;
vector<int> pagesTLB(16, -1);   //TLB size 16 of value -1
vector<int> framesTLB(16, -1);  //TLB size 16
int pageTable[256];   //page table size 256 of value -1//////////////////////////////////
					  //vector<int> framesPTable (256, -1);   //page table size 256
int pageNum = 0;
int offsett = 0;
int frame = 0;
int pointerTLB = 0;
	int tempo = 0;
	backingStore = fopen("BACKING_STORE.bin", "rb");
	int bin[256][256];
	if (backingStore == NULL) {
		fputs("File error", stderr); exit(1);
	}
	
	fseek(backingStore, 0, SEEK_END);
	fileSize = ftell(backingStore);
	rewind(backingStore);
	
	while (!addresses.eof()) {
		getline(addresses, readz,'\n');
		addressEntries.push_back(stoi(readz, nullptr, entries));
		
	}
	
	buffer = (char*)malloc(sizeof(char)*fileSize);
	if(buffer == NULL) { fputs("File error", stderr); exit(2); }
	result = fread(buffer, 1, fileSize, backingStore);
	if (result != fileSize) {
		fputs("Reading error", stderr); exit(3);
	}
	tempo = 0;
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 256; j++) {
			bin[i][j] = (int)buffer[tempo++];
			
		}
		
		
	}
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 256; j++) {
			cout << bin[i][j] << " ";
		}
		cout << "\n";
	}
	int i;
	int index;
	while (i < addressEntries.size()) {
		pageNum = pageNumber(addressEntries[i]);
		offsett = offset(addressEntries[i]);
		index = searchTLB(pageNum, pagesTLB);
		if (index == -1) {    //TLB-miss
			if (pageTable[pageNum] == -1) { //page Fault
											/////////////////////////////////////////////
											/////read backing_store.bin
											/////store in available page frame in main memory
											///store frame number into frame
											//frame =
											/////page table tlb update
											//pageTable[index] =
				updateTLB(pageNum, frame, 16, pagesTLB, framesTLB);

			}
			else {
				frame = pageTable[index];
			}
			if (index == -1) {    //page fault
								  /////////////////////////////////////////////////////////////////////////////////

			}
			else {  //page found into Page Table
				frame = frameTable[index];
				updateTLB(pageNum, frame, index, pagesPTable, framesPTable);    //update Page Table
				updateTLB(pageNum, frame, 16, pagesTLB, framesTLB);    //update TLB

																	   ////////////////////////////////frame, offset, need to fetch from physical memory
			}

		}



		i++;
	}
	system("pause");
	return 0;

}