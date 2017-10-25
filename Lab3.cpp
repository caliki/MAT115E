#include <iostream>
#include <cstdio>
#include <cstring>
#include <new>
using namespace std;

struct Record {	
	char phoneNumber[15], name[30];
};

typedef struct Record Record;
struct PhoneBook {
	Record* records;
	int size,count;
	void create();
	void add(Record& inRecord);
	void remove(int index);
	void list();
	void sort();
};

void PhoneBook::create() {
	size = 1;
	count = 0;
	records = new Record[1];
}

void PhoneBook::add(Record& inRecord) {
	if(size == count) {
		Record* tempRecord = new (nothrow) Record[size];
		memcpy(tempRecord, records, sizeof(Record)*size );
		delete [] records;
		size = size*2;
		records = new (nothrow) Record[size];
		memcpy(records, tempRecord, sizeof(Record)*(size/2) );
	}
	strcpy(records[count].phoneNumber, inRecord.phoneNumber);
	strcpy(records[count].name, inRecord.name);
	count++;
	sort();
}

void PhoneBook::remove(int index) {
	strcpy(records[index-1].name,"");
	strcpy(records[index-1].phoneNumber,"");
	count--;
	sort();
}

void PhoneBook::list() {
	for(int i=0; i<count;i++)
		cout<<i+1<<". "<<records[i].name<<" "<<records[i].phoneNumber<<endl;
}

void PhoneBook::sort() {
	for(int i=0;i<size-1;i++) {
		for(int j=0; j<size-i-1;j++) {
			int k;
			for(k=0;k<30;k++) {
				if(records[j].name[k] != records[j+1].name[k]) {
					break;
				}
			}
			if(k != 30 && ( (records[j].name[k] > records[j+1].name[k] && records[j+1].name[0] != 0) || records[j].name[0]==0)) {
				Record tempRecord;
				strcpy(tempRecord.name, records[j].name);
				strcpy(tempRecord.phoneNumber, records[j].phoneNumber);
				strcpy(records[j].name, records[j+1].name);
				strcpy(records[j].phoneNumber, records[j+1].phoneNumber);
				strcpy(records[j+1].name, tempRecord.name);
				strcpy(records[j+1].phoneNumber, tempRecord.phoneNumber);
			}
		}
	}
}
typedef struct PhoneBook PhoneBook;

int main() {
	PhoneBook myBook;
	myBook.create();
	Record tempRecord;
	strcpy(tempRecord.phoneNumber, "555555");
	strcpy(tempRecord.name, "ssdsds");
	myBook.add(tempRecord);
	myBook.add(tempRecord);
	strcpy(tempRecord.phoneNumber, "1555555");
	strcpy(tempRecord.name, "adsadasda");
	myBook.add(tempRecord);
	myBook.add(tempRecord);
	myBook.remove(1);
	myBook.list();
	return 0;
}