#ifndef DELETE_H
#define DELETE_H

#include "FileManagementUnit.h"
#include <filesystem>
#include <string>

class Delete : public FileManagementUnit
{
private:
	void deleteAll();
	void deleteFiles();
	void deleteDirectories();

public:
	Delete(const std::filesystem::path&);
	Delete(const std::filesystem::path&, const FileManagementUnit::ITEMS, const FileManagementUnit::SELECTIVITY, const std::deque<long long>&);
	void deletion();
};
#endif

