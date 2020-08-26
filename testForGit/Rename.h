#pragma once
#include <filesystem>
#include <string>

class Rename
{
private:
	// Enum classes for selectivity.
	enum class RENAME_NEW_NAME { YES = 1, NO };

	// Intstance variables.
	std::filesystem::path path;
	std::string newName;
	std::string leadingZeroes;
	RENAME_NEW_NAME renameNewNameSelectivity;

	// Setters.
	RENAME_NEW_NAME setNewNameSelectivity();
	std::string setNewName();
	std::string setNumLeadingZeroes(std::size_t);

	// Getters.
	std::size_t getNumFilesInDir(std::filesystem::path);
	int zeroesToRemove(int);
	

public:

	Rename(std::filesystem::path&);
	void rename();

};

