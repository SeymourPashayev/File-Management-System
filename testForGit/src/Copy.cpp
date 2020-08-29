#include "Copy.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <deque>
#include <algorithm>
#include <regex>

Copy::Copy(const std::filesystem::path& pathFrom) : 
    FileManagementUnit{ pathFrom }, pathTo{ determineTargetDirectory() }, nameConflictResolution{ determineNameConflictResolution() } {}

Copy::Copy(const std::filesystem::path& pathFrom, const FileManagementUnit::ITEMS items, const FileManagementUnit::SELECTIVITY selectivity, 
    const std::deque<long long>& fileLst):FileManagementUnit{pathFrom, items, selectivity, fileLst}, 
    pathTo{ determineTargetDirectory() }, nameConflictResolution{ determineNameConflictResolution() }{}

void Copy::copy() {
    std::filesystem::copy_options cpyOpt{ (nameConflictResolution == NAME_CONFLICT_RESOLUTION::KEEP ? 
        std::filesystem::copy_options::skip_existing :std::filesystem::copy_options::overwrite_existing) };

    switch (FileManagementUnit::items) {

    case FileManagementUnit::ITEMS::ALL: {
        copyAll(cpyOpt);
        break;
    }

    case FileManagementUnit::ITEMS::FILES: {
        copyFiles(cpyOpt);
        break;
    }

    case FileManagementUnit::ITEMS::DIRECTORIES: {
        copyDirectories(cpyOpt);
        break;
    }
    }
}

void Copy::copyAll(const std::filesystem::copy_options& cpyOpt) {
    switch (FileManagementUnit::selectivity) {
    case FileManagementUnit::SELECTIVITY::ALL: {
        std::filesystem::copy(FileManagementUnit::path, pathTo, (cpyOpt | std::filesystem::copy_options::recursive));
        break;
    }
    
    case FileManagementUnit::SELECTIVITY::CUSTOM: { //this is meant as Find's secondary action only
        std::deque<long long> originalFileLst{ FileManagementUnit::fileLst };
        try {
            copyFiles(cpyOpt);
        }

        catch (std::filesystem::filesystem_error& ex) {
            std::cout << ex.what() << std::endl;
        }

        std::cout << "File copy passed." << std::endl;
        FileManagementUnit::fileLst = originalFileLst;

        try {
            copyDirectories(cpyOpt);
        }

        catch (std::filesystem::filesystem_error& ex) {
            std::cout << ex.what() << std::endl;
        }

        std::cout << "Directory copy passed." << std::endl;
        break;
    }
    }
}

void Copy::copyFiles(const std::filesystem::copy_options& cpyOpt) {
    switch (FileManagementUnit::selectivity) {

    case FileManagementUnit::SELECTIVITY::ALL: {
        for (auto& dirEntry : std::filesystem::directory_iterator{ FileManagementUnit::path }) {
            if (dirEntry.is_regular_file()) {
                std::filesystem::copy_file(dirEntry.path(), pathTo / dirEntry.path().filename(), cpyOpt);
            }
        }

        break;
    }

    case FileManagementUnit::SELECTIVITY::CUSTOM: {
        if (FileManagementUnit::fileLst.empty()) {
            FileManagementUnit::fileLst = selectItems();
        }

        long long fileNum{ 1 };

        for (auto& dirEntry : std::filesystem::directory_iterator{ FileManagementUnit::path }) {
            if (!FileManagementUnit::fileLst.empty() && fileNum == FileManagementUnit::fileLst.front()) {
                if (dirEntry.is_regular_file()) {
                    std::filesystem::copy_file(dirEntry.path(), pathTo / (dirEntry.path().filename()), cpyOpt);
                }
                FileManagementUnit::fileLst.pop_front();
            }

            ++fileNum;
        }
        break;
    }

    }
}

void Copy::copyDirectories(const std::filesystem::copy_options& cpyOpt) {
    switch (FileManagementUnit::selectivity) {

    case FileManagementUnit::SELECTIVITY::ALL: {
        std::filesystem::copy(FileManagementUnit::path, pathTo, cpyOpt | std::filesystem::copy_options::recursive | std::filesystem::copy_options::directories_only);
        break;
    }

    case FileManagementUnit::SELECTIVITY::CUSTOM: {
        if (FileManagementUnit::fileLst.empty()) {
            FileManagementUnit::fileLst = selectItems();
        }

        long long fileNum{ 1 };

        for (std::filesystem::recursive_directory_iterator next{ FileManagementUnit::path}, end; next != end; ++next) {
            if (next.depth() == 0) {

                if (!FileManagementUnit::fileLst.empty() && fileNum == FileManagementUnit::fileLst.front()) {
                    if ((*next).is_directory()) {
                        std::filesystem::create_directory(pathTo / (*next).path().filename()); //when directory is 1 level below pathFrom, copy path = pathTo + directory name 
                    }
                    FileManagementUnit::fileLst.pop_front();
                    
                }

                ++fileNum;
            }

            else {
                //depth can be anything > 0, so we substitute pathFrom w/ pathTo, leaving path at greater depth intact. 

                std::string parentDirectoryStr{ (*next).path().parent_path().string() };
                std::regex parentReg{ regexStrParser(FileManagementUnit::path.string()) };
                std::filesystem::path targetParentDirectory{ std::regex_replace(parentDirectoryStr, parentReg, pathTo.string()) };

                if (std::filesystem::exists(targetParentDirectory) && (*next).is_directory()) { 
                    std::string outPath{ (*next).path().string() };

                    std::string regStr{ FileManagementUnit::path.string() };
                    std::regex reg{ regexStrParser(regStr) };

                    std::string parsedPath{ std::regex_replace(outPath, reg, pathTo.string()) };
                    std::filesystem::create_directory(parsedPath);
                }
            }
        }
        break;
    }
    }
}

std::filesystem::path Copy::determineTargetDirectory() {
    std::cout << std::endl << "Enter the target directory:" << std::endl;

    if (std::cin.peek() == '\n') {
        std::cin.ignore();
    }

    std::string pathToStr;
    std::getline(std::cin, pathToStr);
    std::filesystem::path tempPath{ pathToStr };

    if (!std::cin.good()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    while (!std::filesystem::exists(tempPath)) {
        std::cerr << std::endl << "Path does not exist." << std::endl;

        std::cout << "Enter the target directory:" << std::endl;

        if (std::cin.peek() == '\n') {
            std::cin.ignore();
        }

        std::getline(std::cin, pathToStr);
        tempPath = pathToStr;

        if (!std::cin.good()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    return tempPath;
}

Copy::NAME_CONFLICT_RESOLUTION Copy::determineNameConflictResolution() {
    std::cout << std::endl << std::setw(50) << "Resolution of file name conflicts:" << std::endl;
    std::cout << std::setw(50) << "1 - Keep the file in target directory." << std::endl;
    std::cout << std::setw(50) << "2 - Overwrite the file in target directory." << std::endl;

    int nameConflictResolution;
    std::cin >> nameConflictResolution;

    if (!std::cin.good()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    while (nameConflictResolution < 1 || nameConflictResolution > 2) {
        std::cerr << std::endl << "Invalid input. Enter an integer 1-2 ." << std::endl;
        std::cin >> nameConflictResolution;

        if (!std::cin.good()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    return static_cast<Copy::NAME_CONFLICT_RESOLUTION>(nameConflictResolution);
}

std::string Copy::regexStrParser(std::string regStr) {
    std::string parsedRegStr;

    for (auto& letter : regStr) {
        parsedRegStr += letter;

        if (letter == '\\') {
            parsedRegStr.append("\\");
        }

    }

    return parsedRegStr;
}
