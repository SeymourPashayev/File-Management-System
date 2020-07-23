#include "CreateNew.h"
#include <string>
#include <filesystem>
#include <iostream>

// Constructor 
CreateNew::CreateNew(std::filesystem::path& pathTo) {
	this->path = pathTo;
	this->createCount = determineCreateCount();
	this->createType = determineCreateType();
	this->name = determineName();
	this->ext = (createType == CreateNew::CREATE_TYPE::FILE) ? (determineExtension()) : ("");
	this->fileNameAndExt = name + "." + ext;
}

// CreateNew driver
void CreateNew::createNew() {
	switch (createType) {
	
	case CreateNew::CREATE_TYPE::FILE:{
		createNewFile();
		break;
	}
	case CreateNew::CREATE_TYPE::DIRECTORY: {
		createNewDirectory();
		break;
	}

	}
}

// This function prompts user to select the number of items they want to create.
// @Input: -
// @Output: (int) number of items to create.
int CreateNew::determineCreateCount()
{
	std::cout << std::endl << std::setw(50) << "Select the number of items you wish to create: " << std::endl;

	int numToBeCreated;
	std::cin >> numToBeCreated;

	while (numToBeCreated < 1 || numToBeCreated > 1000) {
		std::cerr << "Invalid input. Enter an integer 1 - 1000." << std::endl << std::endl;
		std::cin >> numToBeCreated;
	}

	return numToBeCreated;
}

// This function prompts user to select a regural file creation or a directory creation.
// @Input: -
// @Output: CreateNew::CREATE_TYPE enum class selection.
CreateNew::CREATE_TYPE CreateNew::determineCreateType()
{
	std::cout << std::endl << std::setw(50) << "Select create type:" << std::endl;
	std::cout << std::setw(50) << "1 - Regular File." << std::endl;
	std::cout << std::setw(50) << "2 - Directory" << std::endl;

	int createTypeSelectivity;
	std::cin >> createTypeSelectivity;

	while (createTypeSelectivity < 1 || createTypeSelectivity > 2) {
		std::cerr << "Invalid input. Enter an integer 1 - 2." << std::endl << std::endl;
		std::cin >> createTypeSelectivity;
	}

	return static_cast<CreateNew::CREATE_TYPE>(createTypeSelectivity);
}

// This function prompts user to enter the file name(s).
// @Input: -
// @Output: (std::string) the file name(s).
std::string CreateNew::determineName()
{
	std::cout << std::endl << std::setw(50) << "Input file name(s):" << std::endl;
	
	std::string name;
	std::cin >> name;

	return name;
}

// This function prompts user to enter the file extension(s).
// @Input: -
// @Output: (std::string) the file name(s).
std::string CreateNew::determineExtension()
{
	std::cout << std::endl << std::setw(50) << "Input file extension(s):" << std::endl;

	std::string ext;
	std::cin >> ext;

	return ext;
}

// This function serves as a helper function to create new file(s).
void CreateNew::createNewFile() {
	switch (createCount) {

	case 1: {
		// FIXME: No way to create files at all.
		std::filesystem::path pathToCreate = path /= name;
		std::filesystem::create_directories(pathToCreate.parent_path());
		break;
	}
		// FIXME: No way to create multiple files yet.
	}
	
}

// This function serves as a helper function to create new Director(y/ies).
void CreateNew::createNewDirectory() {
	switch (createCount) {
	
	case 1: {
		std::filesystem::path pathToCreate = path /= name;
		std::filesystem::create_directory(pathToCreate);
		break;
	}
		// FIXME: No way to create multiple directories yet.
		//std::filesystem::create_directories();
	}
}