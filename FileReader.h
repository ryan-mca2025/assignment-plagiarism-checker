#ifndef FILEREADER_H
#define FILEREADER_H

#include <vector>
#include <string>

/*
    ===========================================
                  CLASS : FileReader
    ===========================================

    Objective:
        The FileReader class manages reading files from a given folder.
        It provides functionality to:
            - Retrieve names of files in a directory
            - Read text content from supported file formats
            - (TXT fully supported; PDF/DOCX implemented as placeholders)

    Input:
        - Path to a folder containing assignment files.
        - Individual filenames when reading file content.

    Output:
        - A vector of file names from the folder.
        - The full textual content of a file as a string (when supported).

    Side Effects:
        - None on external systems.
        - No modification to external files.
        - Messages printed to stderr for unsupported or unimplemented types.
*/

class FileReader {
private:
    /*
        Objective:
            Store the path of the folder from which files are to be read.

        Input:
            Provided once during object creation.

        Output:
            None.

        Side Effects:
            None.
    */
    std::string folderPath;

public:
    /*
        Objective:
            Initialize FileReader with a given folder path.

        Input:
            path : string — path to folder with files.

        Output:
            None.

        Side Effects:
            Ensures folder path ends with '/' or '\\'.
    */
    FileReader(const std::string& path);

    /*
        Objective:
            Retrieve all file names inside the folder.

        Input:
            None.

        Output:
            vector<string> — list of filenames.

        Side Effects:
            Reads the directory from disk.
    */
    std::vector<std::string> getFileNames() const;

    /*
        Objective:
            Read content of a plain text (.txt) file.

        Input:
            filename : string — name of file inside folder.

        Output:
            File content as a single string.

        Side Effects:
            Performs file I/O from storage.
    */
    std::string readTXT(const std::string& filename) const;

    /*
        Objective:
            Placeholder reader for PDF files.

        Input:
            filename : string

        Output:
            Empty string (until implemented)

        Side Effects:
            Prints warning to stderr.
    */
    std::string readPDF(const std::string& filename) const;

    /*
        Objective:
            Placeholder reader for DOCX files.

        Input:
            filename : string

        Output:
            Empty string (until implemented)

        Side Effects:
            Prints warning to stderr.
    */
    std::string readDOCX(const std::string& filename) const;

    /*
        Objective:
            Read content of a file based on its extension.

        Input:
            filename : string

        Output:
            Content as string (or empty if unsupported).

        Side Effects:
            Uses the appropriate format reader internally.
    */
    std::string readFile(const std::string& filename) const;

    /*
        Objective:
            Read content from a file using its full path.

        Input:
            filePath : full path including filename.

        Output:
            Content as string (txt supported only).

        Side Effects:
            Prints warnings for unsupported file types.
    */
    static std::string readFileByPath(const std::string& filePath);
};

#endif // FILEREADER_H
