#ifndef REPORTWRITER_H
#define REPORTWRITER_H

#include <vector>
#include <string>
#include <tuple>

/*
    ========================================================================
                              CLASS : ReportWriter
    ========================================================================

    Objective:
        The ReportWriter class is responsible for generating and exporting
        plagiarism check results into a CSV (Comma-Separated Values) file.
        It formats similarity results, applies a plagiarism threshold, and
        produces a human-readable tabular report.

    Input:
        - File path where the CSV report should be written.
        - A set of results, each containing:
              (studentName1, studentName2, similarityScore)

    Output:
        - A CSV file stored at 'outputPath'.
        - Each row includes:
              Student Pair, Similarity %, "Plagiarized" flag.

    Side Effects:
        - Creates or overwrites a CSV file on disk.
        - Writes formatted output using file I/O.
        - Prints console messages on success or failure.
*/

class ReportWriter {
private:

    /*
        Objective:
            Store the destination file path for the CSV output.

        Input:
            Provided during object construction.

        Output:
            None.

        Side Effects:
            None.
    */
    std::string outputPath;

    /*
        Objective:
            Track plagiarism threshold (0.0 to 1.0).
            Any similarity score greater than this threshold
            is flagged as "Plagiarized".

        Input:
            Provided during construction or via setter.

        Output:
            None.

        Side Effects:
            Affects behavior of writeCSV().
    */
    double threshold;

public:

    /*
        Objective:
            Initialize the ReportWriter with a file path
            and an optional similarity threshold.

        Input:
            - path : output CSV file location
            - thresh : plagiarism threshold (default = 0.7)

        Output:
            None.

        Side Effects:
            Stores internal configuration values.
    */
    ReportWriter(const std::string& path, double thresh = 0.7);

    /*
        Objective:
            Write all pairwise similarity results into a CSV file.

        Input:
            results : vector of tuples, each containing:
                ( student1 → string,
                  student2 → string,
                  similarity → double from 0.0 to 1.0 )

        Output:
            Creates a formatted CSV file at outputPath.

        Side Effects:
            - Writes to file system.
            - Overwrites existing file with same name.
            - Prints success or error messages to console.
    */
    void writeCSV(const std::vector<std::tuple<std::string, std::string, double>>& results) const;

    /*
        Objective:
            Update the plagiarism threshold used during report writing.

        Input:
            thresh : new threshold between 0.0 and 1.0.

        Output:
            None.

        Side Effects:
            Modifies internal state.
    */
    void setThreshold(double thresh);
};

#endif // REPORTWRITER_H
