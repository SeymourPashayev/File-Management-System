#include "Rename.h"
#include <filesystem>
#include <string>
#include <iostream>

// Constructor
Rename::Rename(std::filesystem::path& pathTo) {
	this->path = pathTo;
	this->renameNewNameSelectivity = setNewNameSelectivity();
	this->newName = ((renameNewNameSelectivity == Rename::RENAME_NEW_NAME::YES) ? setNewName() : "");
	this->leadingZeroes = setNumLeadingZeroes(getNumFilesInDir(path));
}

// Driver function.
void Rename::rename()
{
	int fileNum = 1;

	for (auto& dirEntry : std::filesystem::directory_iterator{ path }) {
		if (dirEntry.is_regular_file()) {
			
			// Turn dirEntry into a path format.
			std::filesystem::path dirEntryPath{ dirEntry.path() };
			std::cout << std::endl << dirEntryPath << "    ";

			// Add the leading zeroes to the new name.
			
			/*int numOfZeroesToRemove = zeroesToRemove(fileNum);
			while (numOfZeroesToRemove > 0 && !leadingZeroes.empty()) {
				leadingZeroes.pop_back();
				numOfZeroesToRemove--;
			}*/

			std::string renameTo = leadingZeroes + std::to_string(fileNum) + "" + newName;
			std::cout << renameTo << "    ";
			// Add the file extensions back to the newName.
			renameTo += dirEntryPath.extension().string();
			std::cout << renameTo << "      ";

			// Create the final path with the new name
			std::filesystem::path newPath{ dirEntryPath.replace_filename(renameTo) };

			std::cout << newPath << std::endl << std::endl;

			// Rename the file
			std::filesystem::rename(dirEntry, newPath);

			fileNum++;

			
		}
	}

}

//---------//
// Setters //
//---------//


// Sets the number of leading zeroes
std::string Rename::setNumLeadingZeroes(std::size_t numFilesInDir)
{
	std::string zeroes;
	while (numFilesInDir >= 10) {
		zeroes += "0";
		numFilesInDir %= 10;
	}

	return zeroes;
}

// Helps find the number of zeroes to be removed form the current fileNum.
int Rename::zeroesToRemove(int fileNum) {
	int numToRemove = 0;

	while (fileNum >= 10) {
		numToRemove++;
		fileNum %= 10;
	}

	return numToRemove;
}

// Prompts user for a new name
std::string Rename::setNewName() {
	std::cout << std::endl << std::setw(50) << "Input name:" << std::endl;

	std::string inNewName;
	std::getline(std::cin, inNewName);

	return inNewName;
}


// Prompts user to answer whether they want a new name or not.
Rename::RENAME_NEW_NAME Rename::setNewNameSelectivity() {
	std::cout << std::endl << std::setw(50) << "Do you want to name your files?" << std::endl;
	std::cout << std::setw(50) << "1 - Yes." << std::endl;
	std::cout << std::setw(50) << "2 - No." << std::endl;

	int selectivity;
	std::cin >> selectivity;

	if (!std::cin.good()) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	while (selectivity < 1 || selectivity > 2) {
		std::cerr << "Invalid input. Enter an integer 1 - 2 ." << std::endl;
		std::cin >> selectivity;

		if (!std::cin.good()) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}

	return static_cast<Rename::RENAME_NEW_NAME>(selectivity);
}
//---------//
// Getters //
//---------//


// Function to check for the number of files in a directory.
// Needed to properly use leading zeroes indentation in one of the rename styles.
// @Input: std::filesystem::path type path to check.
// @Output: std::size_t number of files in a directory.
std::size_t Rename::getNumFilesInDir(std::filesystem::path pathToCheck)
{
	using std::filesystem::directory_iterator;
	using fp = bool (*)(const std::filesystem::path&);
	return std::count_if(directory_iterator(pathToCheck), directory_iterator{}, (fp)std::filesystem::is_regular_file);
}