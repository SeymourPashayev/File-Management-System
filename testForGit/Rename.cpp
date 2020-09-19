#include "Rename.h"
#include <filesystem>
#include <string>
#include <iostream>
#include <random>
#include <list>

// Constructor
Rename::Rename(std::filesystem::path& pathTo) {
	this->path = pathTo;
	this->newName = setNewName();
}

// Driver function.
void Rename::rename()
{
	// Iterate through the list of items and rename them to random strings of length 12.
	for (auto& dirEntry : std::filesystem::directory_iterator{ path }) {
		if (dirEntry.is_regular_file()) {
			std::filesystem::path dirEntryNewPath{ dirEntry };
			dirEntryNewPath.replace_filename(renameRandom() + dirEntryNewPath.extension().string());
			// Add the renamed files to the linked list.
			std::filesystem::rename(dirEntry.path(), dirEntryNewPath);
			listOfPaths.push_back(dirEntryNewPath);
		}
	}

	// Retrieve the renamed files from the linked list to rename them in order.
	int fileNum{ 1 };

	for (std::filesystem::path dirEntry : listOfPaths) {
		if (std::filesystem::exists(dirEntry)) {
			std::filesystem::path dirEntryNewPath{ dirEntry };
			dirEntryNewPath.replace_filename(setNumLeadingZeroes(getNumFilesInDir(path), fileNum) + std::to_string(fileNum) + " " + newName + dirEntryNewPath.extension().string());
			std::filesystem::rename(dirEntry, dirEntryNewPath);
			++fileNum;
		}
	}
}
// Creates a random string of length 12
std::string Rename::renameRandom() {
	
	int length = 12;
	
	static auto& chrs = "0123456789"
		"abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	thread_local static std::mt19937 rg{ std::random_device{}() };
	thread_local static std::uniform_int_distribution<std::string::size_type> pick(0, sizeof(chrs) - 2);

	std::string s;

	s.reserve(length);

	while (length--)
		s += chrs[pick(rg)];

	return s;
}


//---------//
// Setters //
//---------//


// Sets the number of leading zeroes
std::string Rename::setNumLeadingZeroes(std::size_t numFilesInDir, int fileNum)
{
	std::string zeroes;
	int numFilesInDirIterator = 0;
	int fileNumIterator = 0;

	while (numFilesInDir >= 10){
		numFilesInDirIterator++;
		numFilesInDir /= 10;
	}

	while (fileNum >= 10) {
		fileNumIterator++;
		fileNum /= 10;
	}
	
	//std::cout << "numFilesInDir: " << numFilesInDir << " FileNum: " << fileNum << std::endl;

	zeroes = std::string((numFilesInDirIterator - fileNumIterator),'0');

	return zeroes;
}


// Prompts user for a new name
std::string Rename::setNewName() {
	std::cout << std::endl << std::setw(50) << "Input new name:" << std::endl;

	if (std::cin.peek() == '\n') {
		std::cin.ignore();
	}

	std::string inNewName;
	std::getline(std::cin, inNewName);

	return inNewName;
}


//---------//
// Getters //
//---------//


// Function to check for the number of files in a directory.
// Needed to properly use leading zeroes indentation in one of the rename styles.
// @Input: std::filesystem::path type path to check.
// @Output: std::size_t number of files in a directory.

//RAUL: wrapped is_regular_file() inside a lambda, then fed lambda to the count_if(). No problem with overload resolution this way.
std::size_t Rename::getNumFilesInDir(std::filesystem::path pathToCheck) 
{
	auto isRegularFile{ [](auto pathToCheck) {return std::filesystem::is_regular_file(pathToCheck); } };
	return std::count_if(std::filesystem::directory_iterator(pathToCheck), std::filesystem::directory_iterator{}, isRegularFile);
}

