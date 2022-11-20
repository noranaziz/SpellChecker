#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include "TimeInterval.h"
std::string dict[200];
int arrayLength = sizeof(dict) / sizeof(std::string);
TimeInterval suggestionTimer;

// create Node for linked list in hash table (chaining)
class Node {
public:
	std::string data;
	Node* next;

	Node(std::string x) {
		data = x;
		next = NULL;
	}
};

// create hash table
Node* hashTable[26];

// initialize hash table
void initHashTable() {
	for (int i = 0; i < 25; i++) {
		hashTable[i] = NULL;
	}
}

// print entire hash table (for testing purposes)
void printHashTable() {
	for (int i = 0; i < 25; i++) {
		if (hashTable[i] != NULL) {
			Node* current = hashTable[i];
			std::cout << current->data[0] << "--> ";
			while (current != NULL) {
				std::cout << current->data << " ";
				current = current->next;
			}
			std::cout << "\n";
		}
	}
}

// insert words to hash table from dict array
// words are inserted in alphabetical order w/chaining
void insert(std::string word) {
	// convert string to lowercase
	for (int i = 0; i < word.length(); i++)
	{
		word[i] = tolower(word[i]);
	}
	// hash function
	int slotNum = (int)word[0] - 97;

	if (hashTable[slotNum] == NULL) {
		hashTable[slotNum] = new Node(word);
	}
	else {
		Node* key;
		for (key = hashTable[slotNum]; key->next != NULL; key = key->next) {
		}
		key->next = new Node(word);
	}
}

void readFile() {
	// open and read from Dictionary.txt
	std::ifstream inFile;
	inFile.open("Dictionary.txt");
	std::string text;
	int i = 0;

	// insert text into hashTable
	while (getline(inFile, text)) {
		dict[i] = text;
		i++;
		// std::cout << text << "\n";
	}
	inFile.close();
}

// check if userWord is in dictionary
bool inDict(std::string userWord) {
	int slotNum = (int)userWord[0] - 97;
	Node* key = hashTable[slotNum];
	while (key != NULL) {
		if (key->data == userWord) {
			return true;
		}
		key = key->next;
	}
	return false;
}

// (case 1) The program must respond with “True” if the word exists in the dictionary, 
// and must suggest words (from Dictionary) starting with the same first two letters. 
// At the end of each suggestion the exec. time (in micro-sec) must be displayed 
// (see "TimeInterval.h" in Files section).
void caseOne(std::string userWord) {
	std::string output = "";
	std::cout << "True\n";
	std::cout << "Suggested words:\n";
	int slotNum = (int)userWord[0] - 97;
	Node* key = hashTable[slotNum];
	suggestionTimer.start();
	while (key != NULL) {
		if (userWord.length() > 1 && key->data[1] == userWord[1] && key->data != userWord) {
			output += key->data + "\n";
		}
		else if (userWord.length() == 1 && key->data[0] == userWord[0] && key->data != userWord) {
			output += key->data + "\n";
		}
		key = key->next;
	}
	suggestionTimer.stop();
	if (output == "") {
		output = "There are no suggested words.\n";
	}
	std::cout << output;
	std::cout << "Time taken: " << suggestionTimer.GetInterval() << "\n\n";
}

// (case 2) If the input word does not exist in Dictionary due to user misspelling, 
// and considering the first letter is correct, 
// the program should return a recommended word from the dictionary (only one word). 
// For example, instead of the word sentence,  the user enters snetence, setnence, sentens, or setence, etc; 
// the program should return: "do you mean sentence?" If "no", return "false". 
// If "yes", return  "true", and continues as normal (returns words starting with the same two letters).

// (case 3) If the input word does not exist in Dictionary (no "resemblance" with an existing word), 
// the program should return "false".
void casesTwoAndThree(std::string userWord) {
	std::string alphabet = "abcdefghijklmnopqrstuvwxyz";
	std::string input = "";
	std::string word = userWord;
	int slotNum = (int)userWord[0] - 97;
	Node* key;
	// check for wrong letters in word (replace each letter in word with another letter)
	for (int i = 1; i < word.length(); i++) {
		for (int j = 0; j < alphabet.length(); j++) {
			word[i] = alphabet[j];
			// std::cout << word << "\n";
			for (key = hashTable[slotNum]; key != NULL; key = key->next) {
				// std::cout << key->data << "\n\n";
				if (key->data == word) {
					std::cout << "do you mean " << word << "? (yes/no)\n";
					std::cin >> input;
					if (input == "yes" || input == "Yes") {
						caseOne(word);
						return;
					} 
					else if (input == "no" || input == "No") {
						std::cout << "False\n\n";
						return;
					}
				}
			}
		}
		word = userWord;
	}

	// check for extra letters in word
	for (int i = 1; i < word.length(); i++) {
		word = word.erase(i, 1);
		// std::cout << word << "\n";
		for (key = hashTable[slotNum]; key != NULL; key = key->next) {
			// std::cout << key->data << "\n\n";
			if (key->data == word) {
				std::cout << "do you mean " << word << "? (yes/no)\n";
				std::cin >> input;
				if (input == "yes" || input == "Yes") {
					caseOne(word);
					return;
				}
				else if (input == "no" || input == "No") {
					std::cout << "False\n\n";
					return;
				}
			}
		}
		word = userWord;
	}
	
	// check for missing letters in word
	for (int i = 1; i < word.length(); i++) {
		for (int j = 0; j < alphabet.length(); j++) {
			word.insert(i, 1, alphabet[j]);
			// std::cout << word << "\n";
			for (key = hashTable[slotNum]; key != NULL; key = key->next) {
				// std::cout << key->data << "\n\n";
				if (key->data == word) {
					std::cout << "do you mean " << word << "? (yes/no)\n";
					std::cin >> input;
					if (input == "yes" || input == "Yes") {
						caseOne(word);
						return;
					}
					else if (input == "no" || input == "No") {
						std::cout << "False\n\n";
						return;
					}
				}
			}
			word = userWord;
		}
		word = userWord;
	}
	
	// check for a pair of adjacent transposed letters (letters that need to be flipped around)
	for (int i = 1; i < word.length() - 1; i++) {
		std::swap(word[i], word[i + 1]);
		// std::cout << word << "\n";
		for (key = hashTable[slotNum]; key != NULL; key = key->next) {
			// std::cout << key->data << "\n\n";
			if (key->data == word) {
				std::cout << "do you mean " << word << "? (yes/no)\n";
				std::cin >> input;
				if (input == "yes" || input == "Yes") {
					caseOne(word);
					return;
				}
				else if (input == "no" || input == "No") {
					std::cout << "False\n\n";
					return;
				}
			}
		}
		word = userWord;
	}

	// check for similar letter(s)
	for (int i = 1; i < word.length() - 1; i++) {
		std::swap(word[i], word[i + 1]);
		// std::cout << word << "\n";
		for (key = hashTable[slotNum]; key != NULL; key = key->next) {
			// std::cout << key->data << "\n\n";
			if (key->data == word) {
				std::cout << "do you mean " << word << "? (yes/no)\n";
				std::cin >> input;
				if (input == "yes" || input == "Yes") {
					caseOne(word);
					return;
				}
				else if (input == "no" || input == "No") {
					std::cout << "False\n\n";
					return;
				}
			}
		}
		word = userWord;
	}

	// case 3
	if (userWord != "q") {
		std::cout << "False\n";
	}
}


int main() {
	std::string userWord = "";
	initHashTable();
	readFile();
	for (int i = 0; i < arrayLength; i++) {
		insert(dict[i]);
	}
	std::cout << "DICTIONARY HASH TABLE\n";
	printHashTable();
	while(userWord != "q") {
		std::cout << "\n\nPlease enter a word (all lowercase/enter q to quit):\n";
		std::cin >> userWord;
		// case 1
		if (inDict(userWord)) {
			caseOne(userWord);
		}
		// cases 2 and 3
		else {
			casesTwoAndThree(userWord);
		}
	}
	return 0;
}