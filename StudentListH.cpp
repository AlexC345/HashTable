#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <iomanip>

using namespace std;

struct Student{ //Student Struct contains 4 variables for every structure
	char firstName[100];
	char lastName[100];
	int ID;
	float GPA;
	Student* next = NULL;
};

int getHashID(Student* student, int maxLen){
	return ((int)((float)student->ID * 6.9420) % maxLen);
}

void nullify(Student** HashTable, int maxLen){
	for (int i=0; i<maxLen; i++){
		HashTable[i] = NULL;
	}
}

Student* getRandomStudent(){
	Student *random = new Student();
	return random;
}

int main(){
	//Makes HashTable hashTABLE;
	Student* HashTable[100];
	int hashLen = 100;
	nullify(HashTable, hashLen);
	//Set up allowed input prompts
	char del[] = "DELETE";
	char add[] = "ADD";
	char quit[] = "QUIT";
	char print[] = "PRINT";

	char command[100];
	//get first names and last names array
	char* firstNames[1000];
	char* lastNames[1000];
	
	ifstream firstNameFile("firstNames.txt");
	for (int i=0; i<1000; i++){
		firstNames[i] = new char[1000];
		firstNameFile >> firstNames[i];
	}

	ifstream lastNameFile("lastNames.txt");
	for (int i=0; i<1000; i++){
		lastNames[i] = new char[1000];
		lastNameFile >> lastNames[i];
	}

	int inputID;
	bool quitVar = false;
	int addHashIndex;
	Student* temp;
	//Welcome statement
	cout << "Welcome to Student List. Use any of these 4 keywords: ADD, DELETE, PRINT, QUIT" << endl;
	while(not quitVar){ //While the program has not quitted yet:
		cin >> command; //Take in a command and get rid of any excess characters
		command[6] = '\0';
		/*
		if (strcmp(command, del) == 0){ //If you entered DELETE:
			cout << "Enter ID of student to delete: "; //Get input
			cin >> inputID;
			int i=0;
			for (Student* stud : HashTable){ //For every student in HashTable:
				if (stud->ID == inputID){ //If that student has the ID to delete:
					HashTable.erase(HashTable.begin() + i); //Erase that structure from the vector
					cout << "Delete successful" << endl;
				}
				i++;
			}
		}
		*/
		if (strcmp(command, add) == 0){ //If you entered ADD:
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
				int numberOfLinkedStudents = 1;
				temp = HashTable[addHashIndex];
				while (temp->next != NULL){
					temp = temp->next;
					numberOfLinkedStudents += 1;
				}
				if (numberOfLinkedStudents == 3){
					cout << "Can't have more than 3 collisions when chaining!" << endl;
				}
				else{
					temp->next = inputStudent;
				}
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
					while (temp->next != NULL){
						temp = temp->next;
						cout << "H: " << getHashID(HashTable[i], hashLen) << " N: " << temp->firstName  << " " << temp->lastName << " ID: " << temp->ID << " GPA: " << fixed << setprecision(2) << temp->GPA << " -> ";
					}
					cout << endl;
				}
			}
		}
		else{ //If you entered non of the above:
			cout << "Unrecognized input. Please try again." << endl;
		}
	//After quitVar turns true, program ends
	}
	return 0;
}
