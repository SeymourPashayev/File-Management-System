#pragma once
#include "FileManagementUnit.h"
#include <filesystem>
#include <string>

class Rename_v2 : public FileManagementUnit
{
private:
	// Enum classes for selectivity.
	enum class RENAME_NEW_NAME { YES = 1, NO };
	enum class RENAME_STYLE { PLAIN_NUMBERS = 1, LEADING_ZEROES };

	// Intstance variables.
	std::filesystem::path path;
	std::string newName;
	RENAME_NEW_NAME renameNewNameSelectivity;
	RENAME_STYLE renameStyleSelectivity;

	// Setters.
	RENAME_NEW_NAME setNewNameSelectivity();
	RENAME_STYLE setStyleSelectivity();
	std::string setNewName();

	// Getters.
	std::size_t getNumFilesInDir(std::filesystem::path);
	

public:

	Rename_v2(std::filesystem::path&);
	void rename();

};

