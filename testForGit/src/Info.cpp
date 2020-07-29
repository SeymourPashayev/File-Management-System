#include "Info.h"
#include <filesystem>
#include <limits>
#include <chrono>
#include <ctime>
#include <iostream>
#include <string>

Info::Info(std::filesystem::path path) {
	this->path = path;
	this->infoItems = determineInfoItems();
}

void Info::info() {
    int itemCount{ 0 };

    for (auto& dirEntry : std::filesystem::directory_iterator{ path }) {
        if (continueDisplay[0] == 'Y' || continueDisplay[0] == 'y' || continueDisplay[0] == 'N' || continueDisplay[0] == 'n') {

            std::time_t lastModification{ std::chrono::system_clock::to_time_t(std::chrono::time_point_cast<std::chrono::system_clock::duration>(dirEntry.last_write_time()
                    - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now())) };

            struct tm lastModificationStrct;
            localtime_s(&lastModificationStrct, &lastModification);

            const size_t BUFFER_SIZE{ 32 };
            char timeBuff[BUFFER_SIZE];

            asctime_s(timeBuff, sizeof(timeBuff), &lastModificationStrct);

            if ((infoItems == Info::INFO_ITEMS::ALL) || (infoItems == Info::INFO_ITEMS::FILES && dirEntry.is_regular_file()) ||
                (infoItems == Info::INFO_ITEMS::DIRECTORIES && dirEntry.is_directory())) {

                std::cout << std::left << std::endl << std::setw(50) << "File name: " << dirEntry.path().stem().string() << std::endl;
                std::cout << std::setw(50) << "Format: " << (dirEntry.is_directory() ? "Folder" : dirEntry.path().extension().string()) << std::endl;

                if (dirEntry.is_regular_file()) {
                    std::cout << std::setw(50) << "Size: " << dirEntry.file_size() << " bytes." << std::endl;
                }

                std::cout << std::setw(50) << "Last modification: " << timeBuff << std::endl << std::endl;
            }

            itemCount++;

            if (itemCount % 10 == 0) {
                std::cout << "Would you like to continue file information display? (Yes / No)";
                std::cin >> continueDisplay;

                while (continueDisplay[0] != 'Y' && continueDisplay[0] != 'y' && continueDisplay[0] != 'N' && continueDisplay[0] != 'n') {
                    std::cout << "Invalid input. Enter: Yes / No.";
                    std::cin >> continueDisplay;
                }
            }
        }
    }
}

Info::INFO_ITEMS Info::determineInfoItems() {
    std::cout << std::endl << std::setw(50) << "Type of the items to be copied:" << std::endl;
    std::cout << std::setw(50) << "1 - All files and directories." << std::endl;
    std::cout << std::setw(50) << "2 - All files." << std::endl;
    std::cout << std::setw(50) << "3 - All directories." << std::endl;

    int copyItemType;
    std::cin >> copyItemType;

    if (!std::cin.good()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    while (copyItemType < 1 || copyItemType > 3) {
        std::cerr << std::endl << "Invalid input. Enter an integer 1-3." << std::endl;
        std::cin >> copyItemType;

        if (!std::cin.good()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    return static_cast<Info::INFO_ITEMS>(copyItemType);
}