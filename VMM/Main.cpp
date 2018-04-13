#include<iostream>
#include<stdio.h>
#include<vector>
#include<string>
#include<fstream>
#include<algorithm>
#include<array>
using namespace std;

int pageNumber(int address) {
	int pageNmb = address;
	pageNmb = pageNmb & 65535; //mask for page number and offset
	pageNmb = pageNmb >> 8;
	return pageNmb;
}

int offset(int address) {
	int offset = address;
	offset = offset & 255; //mask for offset
	return offset;
}

int searchTLB(int page, vector<int> pageTLB) {
	for (int j = 0; j < pageTLB.size(); j++) {
		if (pageTLB[j] == page) {
			return j; //return index
		}

	}
	return -1; //TLB-miss
}

void updateTLB(int page, int frame, int indexx, vector<int>& pageTLB, vector<int>& frameTLB) { //LRU
	pageTLB.insert(pageTLB.begin() + 0, page); //insert page at beginning
	frameTLB.insert(frameTLB.begin() + 0, frame); //insert frame at beginning
	pageTLB.erase(pageTLB.begin() + indexx);   //delete specified index
	frameTLB.erase(frameTLB.begin() + indexx);

}

void print(int virtAddress, int frame, signed val, int offset, ofstream &output) {
	int physAddress = frame * 256 + offset;
	cout << "Virtual address: " << virtAddress << " Physical address: " << physAddress << " Value: " << val << endl;
	output << "Virtual address: " << virtAddress << " Physical address: " << physAddress << " Value: " << val << endl;
}



void populateBin(int (&okay)[256], char* buf, int pageNum) {
	for (int i = 0; i < 256; i++) {
		okay[i] = (int)buf[pageNum * 256 + i];
	}
}
int main() {
	FILE * backingStore;
	char * buffer;
	size_t result;
	long fileSize;
	ifstream addresses("addresses.txt");
	ofstream output("outputPart2.txt");

	string readz;
	int  entries = 0;
	signed value = 0;
	vector<int> addressEntries;
	vector<int> pagesTLB(16, -1);   //TLB size 16 of value -1
	vector<int> framesTLB(16, -1);  //TLB size 16
	int pageTable[256];   //page table size 256 of value -1
	fill_n(pageTable, 256, -1);

	int bin[256];
	vector<int*>physicalMemory;
	physicalMemory.reserve(256);
	int physAddress;
	int pageNum = 0;
	int offsett = 0;
	int frame = 0;
	int* ptrToArrayInVector;
	int tempo = 0;
	backingStore = fopen("BACKING_STORE.bin", "rb");

	if (backingStore == NULL) {
		fputs("File error", stderr); exit(1);
	}

	fseek(backingStore, 0, SEEK_END);
	fileSize = ftell(backingStore);
	rewind(backingStore);

	while (!addresses.eof()) {
		getline(addresses, readz, '\n');
		addressEntries.push_back(stoi(readz, nullptr, entries));

		tempo++;
	}

	buffer = (char*)malloc(sizeof(char)*fileSize);
	if (buffer == NULL) { fputs("File error", stderr); exit(2); }
	result = fread(buffer, 1, fileSize, backingStore);
	if (result != fileSize) {
		fputs("Reading error", stderr); exit(3);
	}

	int i = 0;
	int nextFreeFrame = 0;
	int frameCounter = 0;
	int index;
	bool checkIfFull = false;
	double pageFault = 0;
	double tlbHit = 0;
	//for (int j = )
	while (i < 1000) {
		pageNum = pageNumber(addressEntries[i]);
		offsett = offset(addressEntries[i]);
		index = searchTLB(pageNum, pagesTLB);
		if (index == -1) {    //TLB-miss
			if (pageTable[pageNum] == -1) { //page Fault
				pageFault++;
				populateBin(bin, buffer, pageNum);
				if (checkIfFull == false) {
					physicalMemory.push_back(new int[256]);
					
				}
				
				ptrToArrayInVector = physicalMemory[nextFreeFrame];
				for (int i = 0; i < 256; i++) {
					ptrToArrayInVector[i] = bin[i];

				}
				
				frame = nextFreeFrame;
				
				for (int j = 0; j < 256; j++) {
					if (pageTable[j] == frame) {
						pageTable[j] = -1;
						break;
					}
				}
				updateTLB(pageNum, frame, 15, pagesTLB, framesTLB);
				pageTable[pageNum] = frame;
				
				ptrToArrayInVector = physicalMemory[frame];
				print(addressEntries[i], frame, ptrToArrayInVector[offsett], offsett,output);
				nextFreeFrame++;
				//if (nextFreeFrame == 128) {
				//	 if (checkIfFull == false) {
				//		checkIfFull = true;
						
				//	}
				//	nextFreeFrame = 0;
				//}
				

			}
			else {   //page found in Page Table
				frame = pageTable[pageNum];
				
				ptrToArrayInVector = physicalMemory[frame];
				value = ptrToArrayInVector[offsett];
				
				updateTLB(pageNum, frame, 15, pagesTLB, framesTLB);
				print(addressEntries[i], frame, value, offsett,output);

			}
		}
		else {   //page found in TLB
			frame = framesTLB[index];
			ptrToArrayInVector = physicalMemory[frame];
			
			value = ptrToArrayInVector[offsett];
			updateTLB(pageNum, frame, index, pagesTLB, framesTLB);
			tlbHit++;
			print(addressEntries[i], frame, value, offsett,output);
		}


		i++;
	}
	
	if (output.is_open()) {
		output << "Page fault: " << pageFault << "\n";
		output << "Page fault rate: " << pageFault / 1000 * 100 << "%\n";
		output << "TLB hit: " << tlbHit << "\n";
		output << "TLB Hit-rate: " << tlbHit / 1000 * 100 << "%\n";
		output.close();
	}
	cout << "Page fault: " << pageFault << "\n";
	cout << "Page fault rate: " << pageFault / 1000 * 100 << "%\n";
	cout << "TLB hit: " << tlbHit << "\n";
	cout << "TLB Hit-rate: " << tlbHit / 1000 * 100 << "%\n";
	


	system("pause");
	return 0;
	//output in file
}