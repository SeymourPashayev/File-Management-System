#include "Copy.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <deque>
#include <algorithm>
#include <regex>

Copy::Copy(std::filesystem::path& pathFrom) : FileManagementUnit(pathFrom) {
    this->pathTo = determineTargetDirectory();
    this->nameConflictResolution = determineNameConflictResolution();
}

void Copy::copy() {
    std::filesystem::copy_options cpyOpt{ std::filesystem::copy_options::skip_existing };
    cpyOpt = (nameConflictResolution == NAME_CONFLICT_RESOLUTION::KEEP ? cpyOpt :
        std::filesystem::copy_options::overwrite_existing);

    switch (FileManagementUnit::getItems()) {

    case FileManagementUnit::ITEMS::ALL: {
        std::filesystem::copy(FileManagementUnit::getPath(), pathTo, (cpyOpt | std::filesystem::copy_options::recursive));
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

void Copy::copyFiles(std::filesystem::copy_options& cpyOpt) {
    switch (FileManagementUnit::getSelectivity()) {

    case FileManagementUnit::SELECTIVITY::ALL: {
        for (auto& dirEntry : std::filesystem::directory_iterator{ FileManagementUnit::getPath() }) {
            if (dirEntry.is_regular_file()) {
                std::filesystem::copy_file(dirEntry.path(), pathTo / dirEntry.path().filename(), cpyOpt);
            }
        }
        break;
    }

    case FileManagementUnit::SELECTIVITY::CUSTOM: {
        std::deque<long long> selectedFiles{ FileManagementUnit::selectItems() };
        long long count{ 1 };

        for (auto& dirEntry : std::filesystem::directory_iterator{ FileManagementUnit::getPath() }) {
            if (!selectedFiles.empty() && count == selectedFiles.front()) {
                std::filesystem::copy_file(dirEntry.path(), pathTo / (dirEntry.path().filename()), cpyOpt);
                selectedFiles.pop_front();
            }

            count++;
        }
        break;
    }

    }
}

void Copy::copyDirectories(std::filesystem::copy_options& cpyOpt) {
    switch (FileManagementUnit::getSelectivity()) {

    case FileManagementUnit::SELECTIVITY::ALL: {
        std::filesystem::copy(FileManagementUnit::getPath(), pathTo, cpyOpt | std::filesystem::copy_options::recursive | std::filesystem::copy_options::directories_only);
        break;
    }

    case FileManagementUnit::SELECTIVITY::CUSTOM: {
        std::deque<long long> selectedDirectories{ selectItems() };
        long long count{ 1 };

        for (std::filesystem::recursive_directory_iterator next{ FileManagementUnit::getPath()}, end; next != end; ++next) {
            if (next.depth() == 0) {

                if (!selectedDirectories.empty() && count == selectedDirectories.front()) {
                    std::filesystem::create_directory(pathTo / (*next).path().filename()); //when directory is 1 level below pathFrom, copy path = pathTo + directory name 

                    if (selectedDirectories.size() > 1) {
                        selectedDirectories.pop_front();
                    }
                }

                count++;
            }

            else {
                //depth can be anything > 0, so we substitute pathFrom w/ pathTo, leaving path at greater depth intact. 

                std::string parentDirectoryStr{ (*next).path().parent_path().string() };
                std::regex parentReg{ regexStrParser(FileManagementUnit::getPath().string()) };
                std::filesystem::path targetParentDirectory{ std::regex_replace(parentDirectoryStr, parentReg, pathTo.string()) };

                if (std::filesystem::exists(targetParentDirectory) && (*next).is_directory()) { 
                    std::string outPath{ (*next).path().string() };

                    std::string regStr{ FileManagementUnit::getPath().string() };
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
