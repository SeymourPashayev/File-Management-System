#include "Delete.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <deque>
#include <algorithm>

Delete::Delete(const std::filesystem::path& path) : FileManagementUnit{ path } {}

Delete::Delete(const std::filesystem::path& path, const FileManagementUnit::ITEMS items, const FileManagementUnit::SELECTIVITY selectivity, 
    const std::deque<long long>& fileLst) : FileManagementUnit{path, items, selectivity, fileLst}{}

void Delete::deletion() { 
	switch (FileManagementUnit::items) {

    case FileManagementUnit::ITEMS::ALL: {
        deleteAll();
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

void Delete::deleteAll() {
    switch (FileManagementUnit::selectivity) {
    case FileManagementUnit::SELECTIVITY::ALL: {
        std::filesystem::remove_all(path);
        break;
    }

    case FileManagementUnit::SELECTIVITY::CUSTOM: { //this is meant as Find's secondary action only 
        int fileNum{ 1 };

        for (const auto& dirEntry : std::filesystem::directory_iterator{ FileManagementUnit::path }) {
            if (!FileManagementUnit::fileLst.empty() && fileNum == FileManagementUnit::fileLst.front()) {
                std::filesystem::remove(dirEntry.path());
                FileManagementUnit::fileLst.pop_front();
            }

            ++fileNum;
        }
    }

    }
}

void Delete::deleteFiles() {
    switch (FileManagementUnit::selectivity) {

    case FileManagementUnit::SELECTIVITY::ALL: {
        for (const auto& dirEntry : std::filesystem::directory_iterator{ path }) {
            if (dirEntry.is_regular_file()) {
                std::filesystem::remove(dirEntry.path());
            }
        }
        break;
    }

    case FileManagementUnit::SELECTIVITY::CUSTOM: {

        if (FileManagementUnit::fileLst.empty()) {
            FileManagementUnit::fileLst = selectItems();
        }

        long long fileNum{ 1 };

        for (const auto& dirEntry : std::filesystem::directory_iterator{ path }) {
            if (!FileManagementUnit::fileLst.empty() && fileNum == FileManagementUnit::fileLst.front()) {
                std::filesystem::remove(dirEntry.path());
                FileManagementUnit::fileLst.pop_front();
            }

            ++fileNum;
        }
        break;
    }

    }
}

void Delete::deleteDirectories() {
    switch (FileManagementUnit::selectivity) {

    case FileManagementUnit::SELECTIVITY::ALL: {
        for (const auto& dirEntry : std::filesystem::directory_iterator{ path }) {
            if (dirEntry.is_directory()) {
                std::filesystem::remove_all(dirEntry.path());
            }
        }
        break;
    }

    case FileManagementUnit::SELECTIVITY::CUSTOM: {
        if (FileManagementUnit::fileLst.empty()) {
            FileManagementUnit::fileLst = selectItems();
        }

        long long fileNum{ 1 };

        for (const auto& dirEntry : std::filesystem::directory_iterator{ path } ) {
            if (dirEntry.is_directory() && fileNum == FileManagementUnit::fileLst.front()) {
                std::filesystem::remove_all(dirEntry.path());
                FileManagementUnit::fileLst.pop_front();
            }

            ++fileNum;
        }

        break;
    }
    }
}








