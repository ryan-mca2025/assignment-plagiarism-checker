#include "ReportWriter.h"
#include <fstream>      // For writing CSV file output
#include <iomanip>      // For controlling floating-point formatting (setprecision)
#include <algorithm>    // General utilities (not required but safe)
#include <iostream>     // For console output (std::cout and std::cerr)

/*
    ========================================================================
                                Constructor
    ========================================================================

    Objective:
        Store the output file path and plagiarism threshold so that 
        writeCSV() can use them later.

    Input:
        - path : Location where CSV will be written.
        - thresh : Threshold above which similarity indicates plagiarism.

    Output:
        None.

    Side Effects:
        - Initializes internal private members.
*/
ReportWriter::ReportWriter(const std::string& path, double thresh) 
    : outputPath(path), threshold(thresh) {
    // Nothing else needed here; initialization is complete
}

/*
    ========================================================================
                               setThreshold
    ========================================================================

    Objective:
        Update the similarity threshold.

    Input:
        thresh : a double value usually between 0.0 and 1.0.

    Output:
        None.

    Side Effects:
        Alters the behavior of writeCSV() and how it marks plagiarism.

    Usage Example:
        writer.setThreshold(0.80);   // Only mark > 80% as plagiarism
*/
void ReportWriter::setThreshold(double thresh) {
    threshold = thresh;
}

/*
    ========================================================================
                                 writeCSV
    ========================================================================

    Objective:
        Generate a CSV file containing plagiarism comparison results.

    Input:
        results: vector of tuples in the form:
            ( studentName1, studentName2, similarityScore )

        similarityScore is expressed as:
            - A double between 0.0 and 1.0 where
                  0.0 = no similarity,
                  1.0 = identical.

    Output:
        A fully formatted CSV file written to disk.

    Side Effects:
        - Opens and writes to outputPath.
        - Prints messages to console.
        - Overwrites existing CSV without warning.

    Approach:
        1. Attempt to open output file using ofstream.
        2. Write CSV header: "Student Pair,Similarity Percentage,Plagiarized"
        3. Iterate through results:
                - Extract student names and similarity score.
                - Format similarity into percentage.
                - Check if > threshold → "Yes" else "No".
                - Write row to CSV.
        4. Close file.
        5. Print confirmation message.

    Notes:
        - setprecision(2) ensures similarity percentage shows exactly 2 decimals.
        - Quotes around the pair ensure commas inside names don't break CSV.
*/
void ReportWriter::writeCSV(const std::vector<std::tuple<std::string, std::string, double>>& results) const {

    std::ofstream file(outputPath);   // Step 1: Try to open file for writing

    if (!file.is_open()) {
        // If file couldn't be opened, print an error and exit early
        std::cerr << "Error: Cannot open output file: " << outputPath << std::endl;
        return;
    }

    // Step 2: Write CSV header row
    // The header explains what each column represents.
    file << "Student Pair,Similarity Percentage,Plagiarized\n";

    // Step 3: Process each result tuple
    for (const auto& result : results) {

        // Extract individual fields from tuple
        std::string student1 = std::get<0>(result);     // Name #1
        std::string student2 = std::get<1>(result);     // Name #2
        double similarity = std::get<2>(result);        // Value 0.0–1.0

        // Format "Student1 vs Student2"
        std::string pair = student1 + " vs " + student2;

        // Convert similarity (0–1) to percentage (0–100)
        double percentage = similarity * 100.0;

        // Determine plagiarism flag
        // Uses the stored threshold:
        //      similarity > threshold  → "Yes"
        //      otherwise               → "No"
        std::string plagiarized = (similarity > threshold) ? "Yes" : "No";

        // Write formatted data into CSV
        // Additional formatting:
        //  - setprecision(2) prints values like "87.32%"
        //  - fixed ensures trailing zeros if needed ("87.30")
        file << std::fixed << std::setprecision(2);

        // Output row:
        //   "Student1 vs Student2",87.32%,Yes
        // Quotes around pair prevent comma issues in CSV parsing.
        file << "\"" << pair << "\"," << percentage << "%," << plagiarized << "\n";
    }

    // Step 4: Close file once writing is complete
    file.close();

    // Step 5: Notify user via console
    std::cout << "Report written to: " << outputPath << std::endl;
}
