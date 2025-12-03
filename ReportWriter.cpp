#include "ReportWriter.h"
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <iostream>

/*
-------------------------------------------------
Function Name : ReportWriter (Constructor)

Objective:
    Initialize output file path and similarity threshold.

Input:
    path   → Output CSV file path.
    thresh → Similarity threshold.

Output:
    ReportWriter object initialized.

Side Effect:
    Stores path and threshold internally.

Inside Function:
Approach:
    Assign parameters to member variables.
*/
ReportWriter::ReportWriter(const std::string& path, double thresh) 
    : outputPath(path), threshold(thresh) {
}

/*
-------------------------------------------------
Function Name : setThreshold()

Objective:
    Update plagiarism threshold.

Input:
    thresh → New threshold value.

Output:
    None.

Side Effect:
    Modifies internal threshold value.

Inside Function:
Approach:
    Assign new value to threshold variable.
*/
void ReportWriter::setThreshold(double thresh) {
    threshold = thresh;
}

/*
-------------------------------------------------
Function Name : writeCSV()

Objective:
    Generate plagiarism report as CSV file.

Input:
    results → Vector of results containing:
              (document1, document2, similarity score).

Output:
    A CSV file written to disk.

Side Effect:
    Writes output file and prints to console.

Inside Function:
Approach:
    Open file, write header, write each result row, and close file.
*/
void ReportWriter::writeCSV(
        const std::vector<std::tuple<std::string, std::string, double>>& results) const {

    std::ofstream file(outputPath);

    if (!file.is_open()) {
        std::cerr << "Error: Cannot open output file: " << outputPath << std::endl;
        return;
    }

    file << "Student Pair,Similarity Percentage,Plagiarized\n";

    for (const auto& result : results) {

        std::string student1 = std::get<0>(result);
        std::string student2 = std::get<1>(result);
        double similarity = std::get<2>(result);

        std::string pair = student1 + " vs " + student2;

        double percentage = similarity * 100.0;

        std::string plagiarized = (similarity > threshold) ? "Yes" : "No";

        file << std::fixed << std::setprecision(2);

        file << "\"" << pair << "\"," << percentage << "%," << plagiarized << "\n";
    }

    file.close();

    std::cout << "Report written to: " << outputPath << std::endl;
}
