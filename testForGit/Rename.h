#pragma once
#include <filesystem>
#include <string>
#include <list>

class Rename
{
private:
	// Enum classes for selectivity.
	enum class RENAME_NEW_NAME { YES = 1, NO };

	// Intstance variables.
	std::filesystem::path path;
	std::string newName;
	std::list<std::filesystem::path> listOfPaths;

	// Setters.
	std::string setNewName();
	std::string setNumLeadingZeroes(std::size_t, int);
	std::string renameRandom();

	// Getters.
	std::size_t getNumFilesInDir(std::filesystem::path);

public:

	Rename(std::filesystem::path&);
	void rename();

};

