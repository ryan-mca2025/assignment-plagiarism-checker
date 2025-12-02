#ifndef SIMILARITYCHECKER_H
#define SIMILARITYCHECKER_H

#include <vector>
#include <string>
#include <map>
#include <tuple>

/*
    ========================================================================
                          CLASS : SimilarityChecker
    ========================================================================

    Objective:
        The SimilarityChecker class computes similarity scores between 
        documents based on their TF-IDF vector representations. The primary
        metric used is cosine similarity, one of the most widely used 
        algorithms for vector-based document comparison.

    Input:
        - A vector of TF-IDF vectors, where each document is represented as:
                map<string term, double tfidfValue>
        - A vector of document names (strings).

    Output:
        - Cosine similarity between any two documents.
        - A list of all pairwise similarity scores.

    Side Effects:
        - None. This class does not modify input vectors or write to files.
        - All computations are performed in-memory.
*/

class SimilarityChecker {
private:

    /*
        Objective:
            Store TF-IDF vectors for all documents.

        Input:
            Passed once during construction.

        Output:
            None.

        Side Effects:
            None.
    */
    std::vector<std::map<std::string, double>> tfidfVectors;

    /*
        Objective:
            Store the human-readable names of documents.

        Input:
            Passed during construction.

        Output:
            None.

        Side Effects:
            Used only for labeling compareAll() output.
    */
    std::vector<std::string> documentNames;

    /*
        Objective:
            Compute the dot product of two sparse TF-IDF vectors.

        Input:
            vec1, vec2 → maps of term : TF-IDF value.

        Output:
            Double value representing dot product.

        Side Effects:
            None.

        Approach:
            - Iterate through the smaller map to reduce time.
            - Multiply values only for matching terms.
    */
    double dotProduct(const std::map<std::string, double>& vec1,
                      const std::map<std::string, double>& vec2) const;

    /*
        Objective:
            Calculate magnitude (Euclidean norm) of a TF-IDF vector.

        Input:
            vec → map<string, double>

        Output:
            sqrt(sum of squares of values)

        Side Effects:
            None.
    */
    double magnitude(const std::map<std::string, double>& vec) const;

public:

    /*
        Objective:
            Construct SimilarityChecker with precomputed TF-IDF vectors 
            and corresponding document names.

        Input:
            vectors → List of TF-IDF maps.
            names → Document names.

        Output:
            None.

        Side Effects:
            Stores internal state.
    */
    SimilarityChecker(const std::vector<std::map<std::string, double>>& vectors,
                      const std::vector<std::string>& names);

    /*
        Objective:
            Compute cosine similarity between two documents.

        Input:
            doc1Index → index of first document
            doc2Index → index of second document

        Output:
            Value between 0.0 and 1.0

        Side Effects:
            None.

        Special Cases:
            - If both indices refer to the same document → returns 1.0
            - If either vector has zero magnitude → returns 0.0
    */
    double cosineSimilarity(int doc1Index, int doc2Index) const;

    /*
        Objective:
            Compare all unique document pairs and produce similarity results.

        Input:
            None.

        Output:
            Vector of tuples:
                (documentName1, documentName2, similarityScore)

        Side Effects:
            None.
    */
    std::vector<std::tuple<std::string, std::string, double>> compareAll() const;
};

#endif // SIMILARITYCHECKER_H
