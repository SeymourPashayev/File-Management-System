#ifndef FILE_MANAGEMENT_UNIT_H
#define FILE_MANAGEMENT_UNIT_H

#include <filesystem>
#include <vector>
#include <deque>
#include <sstream>

class FileManagementUnit
{
protected:
	enum class ITEMS{ ALL = 1, FILES, DIRECTORIES};
	enum class SELECTIVITY{ ALL = 1, CUSTOM};

	ITEMS items;
	SELECTIVITY selectivity;
	std::filesystem::path path;
	std::deque<long long> fileLst;

	ITEMS determineItems();
	SELECTIVITY determineSelectivity();

	std::deque<long long> selectItems();
	void displayItems(std::vector<long long>&, std::deque<long long>&, std::ostringstream&);

public:
	FileManagementUnit(const std::filesystem::path&);
	FileManagementUnit(const std::filesystem::path&, const SELECTIVITY); //ctor to set SELECTIVITY to ALL w/o user, needed in Find
	FileManagementUnit(const std::filesystem::path&, const ITEMS, const SELECTIVITY, const std::deque<long long>&); 
	virtual ~FileManagementUnit() = default;
};
#endif 
