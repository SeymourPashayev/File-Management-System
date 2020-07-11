
#include <iostream>
#include <filesystem>
#include <string>
#include "Copy.h"

int menu();

int main() { 
	enum class CHOICE{ NEW = 1, RENAME, DELETE, INFO, COPY, SORT, FIND, END};

    CHOICE choice{ static_cast<CHOICE>(menu()) };

    while (choice != CHOICE::END) {
        std::cout << std::endl << "Enter the" << (choice == CHOICE::COPY ? " original" : "") << " directory:" << std::endl;
        std::cin.ignore(); //ignore last crlf

        std::string pathFromStr;
        std::getline(std::cin, pathFromStr);
        std::filesystem::path pathFrom{ pathFromStr };

        switch (choice) {

        case CHOICE::NEW: {
            break;
        }

        case CHOICE::RENAME: {
            break;
        }

        case CHOICE::DELETE: {
            break;
        }

        case CHOICE::INFO: {
            break;
        }

        case CHOICE::COPY: {
            Copy copyObj{ pathFrom };
            copyObj.copy();
            break;
        }

        case CHOICE::SORT: {
          
            break;
        }

        case CHOICE::FIND: {
            break;
        }

        }

        choice = static_cast<CHOICE>(menu());
    }
}

int menu() {
    std::cout << std::endl << std::left << std::setw(50) << "1 - Create new files / directories." << std::endl;
    std::cout << std::setw(50) << "2 - Rename an existing file / directory." << std::endl;
    std::cout << std::setw(50) << "3 - Delete an existing file / directory." << std::endl;
    std::cout << std::setw(50) << "4 - Get information on an existing file / directory." << std::endl;
    std::cout << std::setw(50) << "5 - Copy an existing file / directory to another directory." << std::endl;
    std::cout << std::setw(50) << "6 - Sort files in a directory with a key." << std::endl;
    std::cout << std::setw(50) << "7 - Find file(s) in a directory with a key." << std::endl;
    std::cout << std::setw(50) << "8 - Exit program." << std::endl << std::endl;

    int choice;
    std::cin >> choice;

    while (choice < 1 || choice > 8) {
        std::cerr << "Invalid input. Enter an integer 1-8 ." << std::endl;
    }

    return choice;
}


