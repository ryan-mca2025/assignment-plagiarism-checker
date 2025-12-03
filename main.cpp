#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <iomanip>
#include <tuple>

#include "FileReader.h"
#include "TextCleaner.h"
#include "FeatureExtractor.h"
#include "SimilarityChecker.h"
#include "ReportWriter.h"

/*
-------------------------------------------------
Function Name : main()

Objective:
    Execute the complete plagiarism detection pipeline.

Input:
    Command-line arguments:
        Mode 1 (Folder Mode):
            ./checker folderName output.csv threshold

        Mode 2 (File Mode):
            ./checker -f file1.txt file2.txt output.csv threshold

Output:
    - Displays similarity scores on console.
    - Generates CSV report on disk.

Side Effect:
    - Reads files from disk.
    - Writes CSV output file.
    - Prints status messages.

Inside Function:
Approach:
    Parse user input, load files, process text, compute similarity and generate report.

    // call FileReader()
    // call TextCleaner()
    // call FeatureExtractor()
    // call SimilarityChecker()
    // call ReportWriter()
*/
int main(int argc, char* argv[]) {

    /*
    -------------------------------------------------
    Section : Default Configuration Setup

    Objective:
        Initialize default system values.

    Input:
        None.

    Output:
        Configuration variables set.

    Side Effect:
        Sets default output path and threshold.

    Inside Section:
    Approach:
        Assign default fallback parameters.
    */
    std::string outputFile = "plagiarism_report.csv";
    double threshold = 0.70;
    std::vector<std::string> filePaths;
    std::vector<std::string> documentNames;
    bool useFileMode = false;


    /*
    -------------------------------------------------
    Section : Command-Line Argument Processing

    Objective:
        Determine execution mode and parse inputs.

    Input:
        argv[] command-line arguments.

    Output:
        filePaths, outputFile and threshold populated.

    Side Effect:
        None.

  
    Approach:
        Detect "-f" flag or folder mode and extract parameters.
    */
    if (argc > 1) {
        std::string firstArg = argv[1];

        // ---------------- FILE MODE ----------------
        if (firstArg == "-f" || firstArg == "--files") {
            useFileMode = true;

            for (int i = 2; i < argc; i++) {
                std::string arg = argv[i];

                if (arg.length() > 4 &&
                    (arg.substr(arg.length() - 4) == ".txt" ||
                     arg.substr(arg.length() - 4) == ".pdf" ||
                     (arg.length() > 5 && arg.substr(arg.length() - 5) == ".docx"))) {

                    filePaths.push_back(arg);
                }
                else {
                    if (arg.find('.') != std::string::npos &&
                        arg.find('/') == std::string::npos &&
                        arg.find('\\') == std::string::npos) {
                        outputFile = arg;
                    }
                    else {
                        try {
                            threshold = std::stod(arg);
                            if (threshold < 0.0 || threshold > 1.0) {
                                threshold = 0.70;
                            }
                        } catch (...) {}
                    }
                }
            }
        }

        // ---------------- FOLDER MODE ----------------
        else {
            std::string inputFolder = firstArg;

            if (argc > 2) outputFile = argv[2];

            if (argc > 3) {
                threshold = std::stod(argv[3]);
                if (threshold < 0.0 || threshold > 1.0) {
                    threshold = 0.70;
                }
            }

            // call FileReader()
            FileReader reader(inputFolder);
            std::vector<std::string> fileNames = reader.getFileNames();

            if (fileNames.empty()) {
                std::cerr << "Error: No supported files found.\n";
                return 1;
            }

            for (const auto& name : fileNames) {
                filePaths.push_back(inputFolder + "/" + name);
                documentNames.push_back(name);
            }
        }
    }

    /*
    -------------------------------------------------
    Section : Default Folder Usage

    Objective:
        Use "assignments" directory if no arguments provided.

    Input:
        None.

    Output:
        Files loaded from assignments folder.

    Side Effect:
        None.

  
    Approach:
        Load files using FileReader class.

        // call FileReader()
    */
    else {
        std::string inputFolder = "assignments";
        FileReader reader(inputFolder);
        std::vector<std::string> fileNames = reader.getFileNames();

        if (fileNames.empty()) {
            std::cerr << "Error: No supported files.\n";
            return 1;
        }

        for (const auto& name : fileNames) {
            filePaths.push_back(inputFolder + "/" + name);
            documentNames.push_back(name);
        }
    }

    /*
    -------------------------------------------------
    Section : Extract File Names in File Mode

    Objective:
        Extract file names from file paths.

    Input:
        Full file paths.

    Output:
        documentNames list.

    Side Effect:
        Populates documentNames vector.


    Approach:
        Extract substring after last slash or backslash.
    */
    if (useFileMode && documentNames.empty()) {
        for (const auto& path : filePaths) {
            size_t lastSlash = path.find_last_of("/\\");
            std::string name = (lastSlash != std::string::npos)
                                 ? path.substr(lastSlash + 1)
                                 : path;
            documentNames.push_back(name);
        }
    }

    /*
    -------------------------------------------------
    Section : Validation

    Objective:
        Ensure at least one file exists.

    Input:
        filePaths vector.

    Output:
        Program exit if invalid.

    Side Effect:
        Terminates program on failure.
    */
    if (filePaths.empty()) {
        std::cerr << "Error: No files specified.\n";
        return 1;
    }

    /*
    -------------------------------------------------
    Section : Banner Display

    Objective:
        Display program configuration.

    Input:
        None.

    Output:
        Console informational text.

    Side Effect:
        Prints to stdout.
    */
    std::cout << "=== Smart Assignment Plagiarism Checker ===\n";
    std::cout << "Output file: " << outputFile << "\n";
    std::cout << "Threshold: " << threshold * 100 << "%\n";


    /*
    -------------------------------------------------
    Section : Read & Preprocess Documents

    Objective:
        Convert raw files into cleaned token lists.

    Input:
        filePaths vector.

    Output:
        processedDocuments vector.

    Side Effect:
        None.

   
    Approach:
        Load each file and clean text.

        // call TextCleaner()
        // call FileReader::readFileByPath()
    */
    TextCleaner cleaner;
    std::vector<std::vector<std::string>> processedDocuments;

    for (size_t i = 0; i < filePaths.size(); i++) {
        std::string content = FileReader::readFileByPath(filePaths[i]);

        if (content.empty()) continue;

        std::vector<std::string> tokens = cleaner.preprocess(content);
        processedDocuments.push_back(tokens);
    }

    if (processedDocuments.empty()) {
        std::cerr << "Error: No valid data.\n";
        return 1;
    }


    /*
    -------------------------------------------------
    Section : TF-IDF Generation

    Objective:
        Convert documents into TF-IDF vectors.

    Input:
        processedDocuments.

    Output:
        TF-IDF vectors.

    Side Effect:
        None.

 
    Approach:
        Initialize extractor and compute vectors.

        // call FeatureExtractor()
        // call computeTFIDF()
    */
    FeatureExtractor extractor(processedDocuments);
    extractor.computeTFIDF();
    std::vector<std::map<std::string, double>> tfidfVectors =
        extractor.getAllTFIDFVectors();


    /*
    -------------------------------------------------
    Section : Similarity Computation

    Objective:
        Compare documents for similarity.

    Input:
        TF-IDF vectors.

    Output:
        similarity results list.

    Side Effect:
        None.

    
    Approach:
        Compute cosine similarity.

        // call SimilarityChecker()
        // call compareAll()
    */
    SimilarityChecker checker(tfidfVectors, documentNames);
    std::vector<std::tuple<std::string, std::string, double>> results =
        checker.compareAll();


    /*
    -------------------------------------------------
    Section : Report Generation

    Objective:
        Save results in CSV format.

    Input:
        results list.

    Output:
        CSV file.

    Side Effect:
        Writes to disk.

   
    Approach:
        Create report writer and export CSV.

        // call ReportWriter()
        // call writeCSV()
    */
    ReportWriter writer(outputFile, threshold);
    writer.writeCSV(results);

    return 0;
}
