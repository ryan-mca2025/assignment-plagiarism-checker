#include "FileReader.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <dirent.h>
#endif

/*
-------------------------------------------------
Function Name : FileReader (Constructor)

Objective:
    Initialize directory path for file reading.

Input:
    path → Folder path where assignment files exist.

Output:
    None.

Side Effect:
    Updates internal folderPath string.

Inside Function:
Approach:
    Append trailing slash if missing to normalize path.
*/
FileReader::FileReader(const std::string& path) : folderPath(path) {

    if (!folderPath.empty() &&
        folderPath.back() != '/' &&
        folderPath.back() != '\\') {

        folderPath += "/";
    }
}

/*
-------------------------------------------------
Function Name : getFileNames()

Objective:
    Retrieve all valid assignment file names from directory.

Input:
    None (uses internal folderPath).

Output:
    vector<string> → List of file names.

Side Effect:
    Accesses local file system.

Inside Function:
Approach:
    Traverse directory and collect files ending with txt, pdf, docx.
*/
std::vector<std::string> FileReader::getFileNames() const {
    std::vector<std::string> fileNames;

#ifdef _WIN32
    std::string searchPath = folderPath + "*.*";

    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findData);

    if (hFind != INVALID_HANDLE_VALUE) {

        do {
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {

                std::string fileName = findData.cFileName;
                std::string ext = fileName.substr(fileName.find_last_of(".") + 1);
                std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

                if (ext == "txt" || ext == "pdf" || ext == "docx") {
                    fileNames.push_back(fileName);
                }
            }
        } while (FindNextFileA(hFind, &findData) != 0);

        FindClose(hFind);
    }

#else
    DIR* dir = opendir(folderPath.c_str());

    if (dir != nullptr) {
        struct dirent* entry;

        while ((entry = readdir(dir)) != nullptr) {

            std::string fileName = entry->d_name;

            if (fileName != "." && fileName != "..") {

                std::string ext = fileName.substr(fileName.find_last_of(".") + 1);
                std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

                if (ext == "txt" || ext == "pdf" || ext == "docx") {
                    fileNames.push_back(fileName);
                }
            }
        }

        closedir(dir);
    }
#endif

    return fileNames;
}

/*
-------------------------------------------------
Function Name : readTXT()

Objective:
    Read content from a TXT file.

Input:
    filename → TXT file name.

Output:
    string → File content.

Side Effect:
    Reads from file system.

Inside Function:
Approach:
    Open file and read entire content into string.
*/
std::string FileReader::readTXT(const std::string& filename) const {
    std::string filePath = folderPath + filename;
    std::ifstream file(filePath);

    if (!file.is_open()) {
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

/*
-------------------------------------------------
Function Name : readPDF()

Objective:
    Placeholder for reading PDF files.

Input:
    filename → PDF file name.

Output:
    Empty string.

Side Effect:
    Displays warning message.

Inside Function:
Approach:
    Print unsupported message and return empty result.
*/
std::string FileReader::readPDF(const std::string& filename) const {
    std::cerr << "Warning: PDF reading not implemented. File: "
              << filename << " cannot be read." << std::endl;
    return "";
}

/*
-------------------------------------------------
Function Name : readDOCX()

Objective:
    Placeholder for reading DOCX files.

Input:
    filename → DOCX file name.

Output:
    Empty string.

Side Effect:
    Displays warning message.


Approach:
    Print unsupported message and return empty result.
*/
std::string FileReader::readDOCX(const std::string& filename) const {
    std::cerr << "Warning: DOCX reading not implemented. File: "
              << filename << " cannot be read." << std::endl;
    return "";
}

/*
-------------------------------------------------
Function Name : readFile()

Objective:
    Route file reading based on extension.

Input:
    filename → File name.

Output:
    string → File content or empty string.

Side Effect:
    Calls appropriate read function.


Approach:
    Extract extension and dispatch to matching reader.

    // call readTXT()
    // call readPDF()
    // call readDOCX()
*/
std::string FileReader::readFile(const std::string& filename) const {

    std::string ext = filename.substr(filename.find_last_of(".") + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    if (ext == "txt") {
        // call readTXT()
        return readTXT(filename);
    } 
    else if (ext == "pdf") {
        // call readPDF()
        return readPDF(filename);
    } 
    else if (ext == "docx") {
        // call readDOCX()
        return readDOCX(filename);
    } 
    else {
        std::cerr << "Warning: Unsupported file type: " << ext << std::endl;
        return "";
    }
}

/*
-------------------------------------------------
Function Name : readFileByPath()

Objective:
    Read file directly using full path.

Input:
    filePath → Absolute or relative file path.

Output:
    string → File content or empty.

Side Effect:
    Reads from file system.


Approach:
    Extract extension and open file directly if TXT.
*/
std::string FileReader::readFileByPath(const std::string& filePath) {

    size_t lastDot = filePath.find_last_of(".");

    if (lastDot == std::string::npos) {
        std::cerr << "Warning: File has no extension: " << filePath << std::endl;
        return "";
    }

    std::string ext = filePath.substr(lastDot + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    if (ext == "txt") {

        std::ifstream file(filePath);

        if (!file.is_open()) {
            return "";
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
    else if (ext == "pdf") {
        std::cerr << "Warning: PDF reading not implemented. File: "
                  << filePath << " cannot be read." << std::endl;
        return "";
    }
    else if (ext == "docx") {
        std::cerr << "Warning: DOCX reading not implemented. File: "
                  << filePath << " cannot be read." << std::endl;
        return "";
    }
    else {
        std::cerr << "Warning: Unsupported file type: " << ext << std::endl;
        return "";
    }
}
