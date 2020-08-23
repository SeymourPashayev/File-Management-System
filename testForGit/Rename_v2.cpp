#include "Rename_v2.h"
#include "filesystem"
#include "string"


Rename_v2::Rename_v2(std::filesystem::path& pathTo) : FileManagementUnit(pathTo) {
	this->path = pathTo;
	this->renameNewNameSelectivity = setNewNameSelectivity();
	this->newName = ((renameNewNameSelectivity == Rename_v2::RENAME_NEW_NAME::YES) ? "" : setNewName());
	this->renameStyleSelectivity = setStyleSelectivity();
}
