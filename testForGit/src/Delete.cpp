#include "Delete.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <deque>
#include <algorithm>

Delete::Delete(std::filesystem::path& path) : FileManagementUnit(path) {}

void Delete::deletion() {
	switch (FileManagementUnit::getItems()) {

    case FileManagementUnit::ITEMS::ALL: {
        std::filesystem::remove_all(path);
		break;
	}

	case FileManagementUnit::ITEMS::FILES: {
        deleteFiles();
		break;
	}

	case FileManagementUnit::ITEMS::DIRECTORIES: {
        deleteDirectories();
		break;
    }

	}
}

void Delete::deleteFiles() {
    switch (FileManagementUnit::getSelectivity()) {

    case FileManagementUnit::SELECTIVITY::ALL: {
        for (auto& dirEntry : std::filesystem::directory_iterator{ path }) {
            if (dirEntry.is_regular_file()) {
                std::filesystem::remove(dirEntry.path());
            }
        }
        break;
    }

    case FileManagementUnit::SELECTIVITY::CUSTOM: {
        std::deque<long long> selectedFiles{ selectItems() };
        long long count{ 1 };

        for (auto& dirEntry : std::filesystem::directory_iterator{ path }) {
            if (!selectedFiles.empty() && count == selectedFiles.front()) {
                std::filesystem::remove(dirEntry.path());
                selectedFiles.pop_front();
            }

            count++;
        }
        break;
    }

    }
}

void Delete::deleteDirectories() {
    switch (FileManagementUnit::getSelectivity()) {

    case FileManagementUnit::SELECTIVITY::ALL: {
        for (auto& dirEntry : std::filesystem::directory_iterator{ path }) {
            if (dirEntry.is_directory()) {
                std::filesystem::remove_all(dirEntry.path());
            }
        }
        break;
    }

    case FileManagementUnit::SELECTIVITY::CUSTOM: {
        std::deque<long long> selectedDirectories{ selectItems() };
        long long count{ 1 };

        for (auto& dirEntry : std::filesystem::directory_iterator{ path } ) {
            if (dirEntry.is_directory() && count == selectedDirectories.front()) {
                std::filesystem::remove_all(dirEntry.path());
                selectedDirectories.pop_front();
            }

            count++;
        }
        break;
    }
    }
}








