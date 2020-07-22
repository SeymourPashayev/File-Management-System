#pragma once
#include <filesystem>
#include <string>

class Rename
{
private:
	// Enum classes for selection during the runtime
	enum class RENAME_SELECTIVITY { ALL_HERE = 1, ALL_INCL_SUBDIR, CUSTOM };
	enum class RENAME_KEEP_ORIG_TEXT { YES = 1, NO };
	enum class RENAME_STYLE_SELECTIVITY { PLAIN_NUMBERS = 1, LEADING_ZERO };

	// Instance Variables
	RENAME_SELECTIVITY renameSelectivity;
	RENAME_STYLE_SELECTIVITY renameStyle;
	RENAME_KEEP_ORIG_TEXT keepOrigText;
	std::string renameTo;
	std::filesystem::path path;

	//Helper functions
	void renameCore(int, std::string);
	void renameHere();
	//void renameRecursive();
	//void renameCustom();

	// Checkers
	RENAME_SELECTIVITY determineRenameSelectivity();
	RENAME_STYLE_SELECTIVITY determineRenameStyle();
	RENAME_KEEP_ORIG_TEXT determineKeepOrigText();
	std::string determineAddNewName();
	std::size_t number_of_files_in_directory(std::filesystem::path);

public:

	Rename(std::filesystem::path&);
	void rename();
};

