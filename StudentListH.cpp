#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <iomanip>
#include <cstring>
#include <fstream>

using namespace std;

struct Student{ //Student Struct contains 4 variables for every structure
	char firstName[100];
	char lastName[100];
	int ID;
	float GPA;
	Student* next = NULL;
};

int getHashID(Student* student, int maxLen){ //hash function
	return ((int)((float)student->ID * student->GPA) % maxLen);
}

void nullify(Student** HashTable, int maxLen){ //clears an array
	for (int i=0; i<maxLen; i++){
		HashTable[i] = NULL;
	}
}

Student* getRandomStudent(int latestID, char** firstNames, char** lastNames){ //generate random student funcion
	Student *random = new Student();
	strcpy(random->firstName, firstNames[(rand() % 1000)+1]);
	strcpy(random->lastName, lastNames[(rand() % 1000)+1]);
	random->ID = latestID;
	random->GPA = ((float) (rand()%400)+1)/100;
	return random;
}

int getLength(Student** hashTable, int hashLen){ //returns number of students in hash table
	int numberOfStudents = 0;
	for (int i=0; i<hashLen; i++){
		Student* current = hashTable[i];
		while (current != nullptr){
			numberOfStudents += 1;
			current = current->next;
		}
	}
	return numberOfStudents;
}

int main(){
	//Makes HashTable hashTABLE;
	int hashLen = 100;
	int tempHashLen = hashLen;
	Student** HashTable = new Student*[hashLen];
	nullify(HashTable, hashLen);

	//Set up allowed input prompts
	char del[] = "DELETE";
	char add[] = "ADD";
	char quit[] = "QUIT";
	char print[] = "PRINT";
	char addr[] = "ADDR";
	char num[] = "NUM";

	char command[100];
	//get first names and last names array
	char* firstNames[1000];
	char* lastNames[1000];
	for (int i=0; i<1000; i++){ //clear first names list and last names list
		firstNames[i] = NULL;
		lastNames[i] = NULL;
	}
	
	int latestID = 100000;
	srand(static_cast<unsigned int>(time(NULL)));//the random thingy that makes rand() work
	int addrNumber;
	int numberOfLinkedStudents;
	//get 1000 first names into an array
	ifstream firstNameFile("firstNames.txt");
	for (int i=0; i<1000; i++){
		firstNames[i] = new char[1000];
		firstNameFile >> firstNames[i];
	}
	//get 1000 last names into an array
	ifstream lastNameFile("lastNames.txt");
	for (int i=0; i<1000; i++){
		lastNames[i] = new char[1000];
		lastNameFile >> lastNames[i];
	}
	//add, addr, delete, and double hash variables
	int inputID;
	bool quitVar = false;
	int addHashIndex;

	bool doubleHash = false;
	
	Student* temp;
	Student* delCur;
	bool delBreak;
	Student* delPrev;
	//Welcome statement
	cout << "Welcome to Student List Hash Table. Use any of these 5 keywords: ADD, DELETE, PRINT, QUIT, ADDR, NUM" << endl;
	while(not quitVar){ //While the program has not quitted yet:
		cin >> command; //Take in a command and get rid of any excess characters
		command[6] = '\0';

		if (strcmp(command, del) == 0){ //If you entered DELETE:
			cout << "Enter ID of student to delete: "; //Get input
			cin >> inputID;
			for (int i=0; i<hashLen; i++){//goes through every element in hash table
				delCur = HashTable[i];
				
				delPrev = delCur;
				if (delCur != NULL){
					while (delCur != NULL){
						if (delCur->ID == inputID){
							if (delPrev != delCur){
								delPrev->next = delCur->next;
							}
							else{
								if (delCur->next == NULL){
									HashTable[i] = NULL;
								}
								else{
									HashTable[i] = delCur->next;
								}
							}
							delete delCur;
							delBreak = true;
							cout << "Delete successful." << endl;
							break;
						}
						delPrev = delCur;
						delCur = delCur->next;
					}
				}
			}
			if (!delBreak){ //if no student with that ID found:
				cout << "That Student ID doesn't exist!" << endl;	
			}
		}
		else if (strcmp(command, add) == 0){ //If you entered ADD:
			Student *inputStudent = new Student(); //Creates new input student
			cout << "Enter first name: " << endl; //Gets input student variables
			cin >> inputStudent->firstName;
			cout << "Enter last name: " << endl;
			cin >> inputStudent->lastName;
			cout << "Enter student ID: " << endl;
			cin >> inputStudent->ID;
			cout << "Enter student GPA: " << endl;
			cin >> inputStudent->GPA;
			addHashIndex = getHashID(inputStudent, hashLen);
			cout << "Hash index: " << addHashIndex << endl;
			if (HashTable[addHashIndex] == NULL){	
				HashTable[addHashIndex] = inputStudent; //Adds input student to the hashTABLE
			}
			else{
				numberOfLinkedStudents = 1;
				temp = HashTable[addHashIndex];
				while (temp->next != NULL){
					temp = temp->next;
					numberOfLinkedStudents += 1;
				}
				if (numberOfLinkedStudents >= 3){
					doubleHash = true;
				}
				//else{
				temp->next = inputStudent;
				//}
			}
			cout << "Added student!" << endl;
		}
		else if (strcmp(command, quit) == 0){ //If you entered QUIT:
			cout << "Quitted Program" << endl;
			quitVar = true;//Set quitVar variable to true to break the while loop
		}
		else if (strcmp(command, print) == 0){ //If you entered PRINT:
			for (int i=0; i<hashLen; i++){ //For every student in HashTable: Print out their variables
				if (HashTable[i] != NULL){
					cout << "H: " << getHashID(HashTable[i], hashLen) << " N: " << HashTable[i]->firstName  << " " << HashTable[i]->lastName << " ID: " << HashTable[i]->ID << " GPA: " << fixed << setprecision(2) << HashTable[i]->GPA << " -> ";
					temp = HashTable[i];
					while (temp->next != NULL){//prints out all the linked students with the same hash as well
						temp = temp->next;
						cout << "H: " << getHashID(HashTable[i], hashLen) << " N: " << temp->firstName  << " " << temp->lastName << " ID: " << temp->ID << " GPA: " << fixed << setprecision(2) << temp->GPA << " -> ";
					}
					cout << endl;
				}
			}
		}
		else if (strcmp(command, addr) == 0){ //If you entered ADDR: add random students
			cout << "How many random students to add?: ";
			cin >> addrNumber;
			for (int i=0; i<addrNumber; i++){ //add addrNumber of random students
				Student *random = getRandomStudent(latestID, firstNames, lastNames);
				addHashIndex = getHashID(random, hashLen);
				latestID += 1;
				if (HashTable[addHashIndex] == NULL){
					HashTable[addHashIndex] = random;
				}
				else{
					numberOfLinkedStudents = 1;
					temp = HashTable[addHashIndex];
					while (temp->next != NULL){
						temp = temp->next;
						numberOfLinkedStudents += 1;
					}
					if (numberOfLinkedStudents >= 3){//if 3 or more linked students:
						//double hash table length
						doubleHash = true;
					}
					temp->next = random;
					//else{
					//	temp->next = random;
					//}
				}
			}
			cout << "Added random student(s)." << endl;
		}
		else if (strcmp(command, num) == 0){ //function that counts number of students
			cout << "Number of students in hash table: " << getLength(HashTable, hashLen) << endl;
		}
		else{ //If you entered non of the above:
			cout << "Unrecognized input. Please try again." << endl;
		}
			
		while(doubleHash){//if a student has 3 or more links, double hash table size and rehash
			doubleHash = false;
			tempHashLen = tempHashLen * 2;
			Student** tempHash = new Student*[tempHashLen];
			nullify(tempHash, tempHashLen);
			Student* current;

			for (int i=0; i<hashLen; i++){//for every student in hash table:
				current = HashTable[i];
				while (current != NULL){//makes sure current is a student
					//add current to tempHash
					if (tempHash[getHashID(current, tempHashLen)] != NULL){ //if rehashed student's new hash is already taken:
						int lengthOfLinks = 1;
						Student* temp = tempHash[getHashID(current, tempHashLen)];
						while (temp->next != NULL){
							temp = temp->next;
							lengthOfLinks += 1;
						}
						if (lengthOfLinks >= 3){//if 3 or more linked students DURING rehashing process:
							doubleHash = true;//double hash table size and rehash again after
						}
						//else{
						temp->next = current;
						//}
					}
					else{ //if rehashed student's new hash in not taken (available):
						tempHash[getHashID(current, tempHashLen)] = current;
					}
					Student* tempL = current;
					current = current->next;
					tempL->next = NULL;	
				}
			}
			hashLen = tempHashLen;
			HashTable = tempHash;
		}	

	//After quitVar turns true, program ends
	}
	return 0;
}
