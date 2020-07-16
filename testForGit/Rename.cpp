#include "Rename.h"
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

// Constructor
Rename::Rename(std::filesystem::path& pathTo) {
	this->path = pathTo;
	this->renameSelectivity = determineRenameSelectivity();
	//this->keepOrigText = determineKeepOrigText();
	this->renameStyle = determineRenameStyle();
	this->keepOrigText = determineKeepOrigText();
	if (!keepOrigText) {
		this->renameTo = determineAddNewName();
	}
}

// Rename driver
void Rename::rename() {
	switch (renameSelectivity) {
	
	case Rename::RENAME_SELECTIVITY::ALL_HERE: {
		renameHere();
		break;
	}

	case Rename::RENAME_SELECTIVITY::ALL_INCL_SUBDIR: {
		/*renameRecursive();*/
		break;
	}
		
	case Rename::RENAME_SELECTIVITY::CUSTOM: {
		/*renameCustom();*/
		break;
	}

	}
}

// The function prompts user whether they want to rename 
// all items or just the selected ones.
// @Input: -
// @Output: Rename::RENAME_SELECTIVITY enum class selection.
Rename::RENAME_SELECTIVITY Rename::determineRenameSelectivity() {
	std::cout << std::endl << std::setw(50) << "Select rename option:" << std::endl;
	std::cout << std::setw(50) << "1 - All items only in this folder (no subfolders)." << std::endl;
	std::cout << std::setw(50) << "2 - All items including subdirectories (N/A)" << std::endl;
	std::cout << std::setw(50) << "3 - Custom selection. (N/A)" << std::endl;

	int renameSelectivity;
	std::cin >> renameSelectivity;

	while (renameSelectivity < 1 || renameSelectivity > 3) {
		std::cerr << "Invalid input. Enter an integer 1 - 3." << std::endl << std::endl;
		std::cin >> renameSelectivity;
	}

	return static_cast<Rename::RENAME_SELECTIVITY>(renameSelectivity);
}


// The function prompts user whether they want to keep original text
// after the enumeration or just keep the enumeration.
// @Input: -
// @Output: boolean.
bool Rename::determineKeepOrigText() {
	std::cout << std::endl << std::setw(50) << "Keep original text after enumeration? " << std::endl;
	std::cout << std::setw(50) << "1 - Yes.  (ex: 001 fileName)." << std::endl;
	std::cout << std::setw(50) << "2 - No.   (ex: 001)." << std::endl;

	int keepOrigTextSelectivity;
	std::cin >> keepOrigTextSelectivity;

	while (keepOrigTextSelectivity < 1 || keepOrigTextSelectivity > 2) {
		std::cerr << "Invalid input. Enter an integer 1 - 2." << std::endl << std::endl;
		std::cin >> keepOrigTextSelectivity;
	}

	bool keepOrigTextBool = (keepOrigTextSelectivity == 1) ? (true) : (false);

	return keepOrigTextBool;
}

// This function prompts user to select whether they want to add new names to the files.
// @Input: -
// @Output: std::string
std::string Rename::determineAddNewName() {
	std::cout << std::endl << std::setw(50) << "Add new names to the files? " << std::endl;
	std::cout << std::setw(50) << "1 - Yes." << std::endl;
	std::cout << std::setw(50) << "2 - No." << std::endl;

	int addNewNameSelectivity;
	std::cin >> addNewNameSelectivity;

	while (addNewNameSelectivity < 1 || addNewNameSelectivity > 2) {
		std::cerr << "Invalid input. Enter an integer 1 - 2." << std::endl << std::endl;
		std::cin >> addNewNameSelectivity;
	}

	std::string newName = "";

	if (addNewNameSelectivity == 1) {
		std::cout << std::endl << std::setw(50) << "New name: " << std::endl;
		std::getline(std::cin, newName);
		std::cout << "test" << std::endl;
		// FIXME: Try to find a way to get a full line of input to have white spaces in the files
		//std::cin >> newName;
	}

	return newName;
}

// The function prompts user to select the style of the rename function.
// @Input: -
// @Output: Rename::RENAME_STYLE_SELECTIVITY enum class selection.
Rename::RENAME_STYLE_SELECTIVITY Rename::determineRenameStyle() {
	std::cout << std::endl << std::setw(50) << "Select the rename style: " << std::endl;
	std::cout << std::setw(50) << "1 - Plain numbers.  (ex: 1, 2, 3)." << std::endl;
	std::cout << std::setw(50) << "2 - Leading zeroes.   (ex: 001, 002, 003)." << std::endl;

	int renameStyleSelectivity;
	std::cin >> renameStyleSelectivity;

	while (renameStyleSelectivity < 1 || renameStyleSelectivity > 2) {
		std::cerr << "Invalid input. Enter an integer 1 - 2." << std::endl << std::endl;
		std::cin >> renameStyleSelectivity;
	}

	return static_cast<Rename::RENAME_STYLE_SELECTIVITY>(renameStyleSelectivity);
}

// The function renames all of the files in the selected directory according to the selected style.
// @Input: -
// @Output: -
void Rename::renameHere() {

	switch (renameStyle) {
		//FIX: Assuming we only select style now
	case Rename::RENAME_STYLE_SELECTIVITY::PLAIN_NUMBERS: {
		renameCore(0, renameTo);
		break;
	}

	case Rename::RENAME_STYLE_SELECTIVITY::LEADING_ZERO: {
		int numberOfZeroesToIndent = 4;
		int numFilesInDir = number_of_files_in_directory(path);

		// Helper function to find the right number of zeroes to indent
		while (numFilesInDir >= 100) {
			numFilesInDir %= 10;
			numberOfZeroesToIndent++;
		}

		renameCore(numberOfZeroesToIndent, renameTo);
		break;
	}
	}

}

void Rename::renameCore(int numLeadingZeroes, std::string renameTo) {
	int fileNum = 1;
	// --Resolved--: fileNum won't iterate properly, for some reason it would skip some numbers.
	// fileNum would only increment when fileNum is actually converted to a string representation to be used in a path.

	// FIXME: File numbers around a thousand would cause an additional "1" to be indented in the beginning of the file name.
	// Update: That happens in both the leading zeroes formatting and the plain numbers formatting.
	// Update: Sometimes the numbering would jump orders of magnitude. It would start with 1,2,3,4 and then continue on to 100, 101, 102.
	// Sometimes it would even make another leap to the next order of magnitude. Talk to me about unexpected behavior...
	// Update: This seems to be somewhat related to the number of padded zeroes??? It is more of an issue with a plain enumeration than padded.

	for (auto& dirEntry : std::filesystem::directory_iterator{ path }) {
		if (dirEntry.is_regular_file()) {
			// Turn dirEntry into a path format.
			std::filesystem::path dirEntryPath{ dirEntry };
			// Turnt the dirEntryPath to string.
			std::string stringPath = dirEntryPath.string();

			// Find the last occurance of '\' to find the filename.
			int fileNameStart = stringPath.find_last_of('\\') + 1;

			// Find the last occurance of '.' to find the file format.
			int fileFormatStart = stringPath.find_last_of('.') + 1;

			// Create a newName from the number + original substr.
			std::string newName;

			if (renameStyle == Rename::RENAME_STYLE_SELECTIVITY::LEADING_ZERO) {
				std::ostringstream oss;
				// --Resolved--: There's a problem with leading zeroes when renaming large numbers of files.
				// Added more padding zeroes to the enumeration and used a function to determine the number of files to be renamed 
				// to determine how many zeroes to indent.
				oss << std::setfill('0') << std::setw(numLeadingZeroes) << std::to_string(fileNum);
				fileNum++;
				newName = oss.str();
				std::setfill(' ');
			}
			else {
				newName = std::to_string(fileNum);
				fileNum++;
			}

			// if user want to keep original text
			if (keepOrigText) {
				newName += ". " + stringPath.substr(fileNameStart, fileFormatStart);
				// --Resolved--: Keep original text includes the file extension an additional time for some reason.
				// Adding the file format to the else statement helped. this line above should not have added the extension back, but for some reason it did.
			}
			else {
				if (renameTo == "") {
					newName = newName;
				}
				else {
					// This separation is to keep the "001. .txt" files from happening. It will be "001.txt"
					newName += ". " + renameTo;
				}
					// Add back the file format
				newName += stringPath.substr(fileFormatStart - 1);
			}

			
			// Create a new string representation of the path.
			std::string newPathStrRepr = stringPath.substr(0, fileNameStart) + newName;
			// Create a new path representation from the new string representation of the path.
			std::filesystem::path newPathPathRepr{ newPathStrRepr };

			// Rename the file
			std::filesystem::rename(dirEntry, newPathPathRepr);

		}
	}

}

// Function to check for the number of files in a directory.
// Needed to properly use leading zeroes indentation in one of the rename styles.
// @Input: std::filesystem::path type path to check.
// @Output: std::size_t number of files in a directory.
std::size_t Rename::number_of_files_in_directory(std::filesystem::path pathToCheck)
{
	using std::filesystem::directory_iterator;
	using fp = bool (*)(const std::filesystem::path&);
	return std::count_if(directory_iterator(pathToCheck), directory_iterator{}, (fp)std::filesystem::is_regular_file);
}
