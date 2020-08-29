#ifndef DELETE_H
#define DELETE_H

#include "FileManagementUnit.h"
#include <filesystem>
#include <string>

class Delete : public FileManagementUnit
{
private:
	void deleteFiles();
	void deleteDirectories();

public:
	Delete(const std::filesystem::path&);
	Delete(const std::filesystem::path&, const int, const int, const std::deque<long long>&);
	void deletion();
};
#endif

