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

	ITEMS determineItems();
	SELECTIVITY determineSelectivity();

	std::deque<long long> selectItems();
	void displayItems(std::vector<long long>&, std::deque<long long>&, std::ostringstream&);

public:
	FileManagementUnit(std::filesystem::path);
	virtual ~FileManagementUnit() = default;

	std::filesystem::path getPath();
	ITEMS getItems();
	SELECTIVITY getSelectivity();
};
#endif 
