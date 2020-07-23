#pragma once
#include <filesystem>
#include <string>

class CreateNew
{
private:
	// Enum classes for selection during runtime
	enum class CREATE_TYPE { FILE = 1, DIRECTORY };

	// Instance variables
	std::filesystem::path path;
	std::string name;
	std::string ext;
	std::string fileNameAndExt;
	CREATE_TYPE createType;
	int createCount;

	// Checkers
	int determineCreateCount();
	CREATE_TYPE determineCreateType();
	std::string determineName();
	std::string determineExtension();
	
	// Helper Functions
	void createNewFile();
	void createNewDirectory();

public:
	CreateNew(std::filesystem::path&);
	void createNew();
};

