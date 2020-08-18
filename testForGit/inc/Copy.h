#ifndef COPY_H
#define COPY_H

#include "FileManagementUnit.h"
#include <filesystem>
#include <string>

class Copy : public FileManagementUnit
{
private:
	enum class NAME_CONFLICT_RESOLUTION { KEEP = 1, OVERWRITE };

	NAME_CONFLICT_RESOLUTION nameConflictResolution;
	std::filesystem::path pathTo;

	std::filesystem::path determineTargetDirectory();
	NAME_CONFLICT_RESOLUTION determineNameConflictResolution();

	void copyFiles(std::filesystem::copy_options&);
	void copyDirectories(std::filesystem::copy_options&);

	std::string regexStrParser(std::string);

public:
	Copy(std::filesystem::path&);
	void copy();
};
#endif

