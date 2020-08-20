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
	Delete(std::filesystem::path& path);
	void deletion();
};
#endif

