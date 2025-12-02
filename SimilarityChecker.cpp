#include "SimilarityChecker.h"
#include <cmath>       // For sqrt()
#include <algorithm>   // For std::min, std::max if needed

/*
    ========================================================================
                                Constructor
    ========================================================================

    Objective:
        Initialize internal TF-IDF vector storage and document names.

    Input:
        vectors : A vector containing TF-IDF maps for each document.
        names   : A vector containing corresponding document names.

    Output:
        None.

    Side Effects:
        Stores TF-IDF vectors and names for later similarity computations.
*/
SimilarityChecker::SimilarityChecker(
        const std::vector<std::map<std::string, double>>& vectors,
        const std::vector<std::string>& names)
    : tfidfVectors(vectors), documentNames(names) {
    // Nothing else required — data stored and ready for computation
}

/*
    ========================================================================
                                 dotProduct()
    ========================================================================

    Objective:
        Compute the dot product of two sparse TF-IDF vectors.

    Input:
        vec1, vec2 → maps<string term, double tfidfValue>

    Output:
        Double representing sum(term_i(tfidf1 * tfidf2))

    Side Effects:
        None.

    Approach:
        - Choose the smaller vector for iteration (optimization).
        - For each term in smaller vector:
              - Check if the term exists in the larger vector.
              - If found → multiply the two TF-IDF values and accumulate.
*/
double SimilarityChecker::dotProduct(
        const std::map<std::string, double>& vec1,
        const std::map<std::string, double>& vec2) const {

    double result = 0.0;

    // Determine which vector is smaller, to reduce lookup operations
    const auto& smaller = (vec1.size() < vec2.size()) ? vec1 : vec2;
    const auto& larger  = (vec1.size() < vec2.size()) ? vec2 : vec1;

    // Iterate only through smaller vector
    for (const auto& pair : smaller) {
        const std::string& term = pair.first;
        double value1 = pair.second;

        // Find matching term in larger vector
        auto it = larger.find(term);
        if (it != larger.end()) {
            double value2 = it->second;

            // Accumulate TF-IDF1 * TF-IDF2
            result += value1 * value2;
        }
    }

    return result;
}

/*
    ========================================================================
                                 magnitude()
    ========================================================================

    Objective:
        Compute Euclidean norm (magnitude) of a TF-IDF vector.

    Input:
        vec → map<string term, double tfidfValue>

    Output:
        sqrt(sum of squares of values)

    Side Effects:
        None.

    Approach:
        - Square each TF-IDF weight and accumulate.
        - Apply sqrt() to get final magnitude.
*/
double SimilarityChecker::magnitude(
        const std::map<std::string, double>& vec) const {

    double sum = 0.0;

    // Compute sum of squares
    for (const auto& pair : vec) {
        double value = pair.second;
        sum += value * value;
    }

    // Square root of sum of squares
    return std::sqrt(sum);
}

/*
    ========================================================================
                              cosineSimilarity()
    ========================================================================

    Objective:
        Compute cosine similarity between two TF-IDF document vectors:
            similarity = (vec1 · vec2) / (|vec1| * |vec2|)

    Input:
        doc1Index → index of first document
        doc2Index → index of second document

    Output:
        - Value between:
              0.0 (no similarity)
              1.0 (identical documents)

    Side Effects:
        None.

    Edge Cases:
        - Invalid indices → return 0.0
        - Comparing same document → return 1.0
        - Zero-magnitude vector → similarity = 0.0
*/
double SimilarityChecker::cosineSimilarity(int doc1Index, int doc2Index) const {

    // Validate indices
    if (doc1Index < 0 || doc1Index >= static_cast<int>(tfidfVectors.size()) ||
        doc2Index < 0 || doc2Index >= static_cast<int>(tfidfVectors.size())) {
        return 0.0;
    }

    // A document is always perfectly similar to itself
    if (doc1Index == doc2Index) {
        return 1.0;
    }

    const auto& vec1 = tfidfVectors[doc1Index];
    const auto& vec2 = tfidfVectors[doc2Index];

    // Compute dot product and magnitudes
    double dot  = dotProduct(vec1, vec2);
    double mag1 = magnitude(vec1);
    double mag2 = magnitude(vec2);

    // Avoid division by zero
    if (mag1 == 0.0 || mag2 == 0.0) {
        return 0.0;
    }

    // Cosine similarity formula
    return dot / (mag1 * mag2);
}

/*
    ========================================================================
                               compareAll()
    ========================================================================

    Objective:
        Compare every unique pair of documents and compute their similarity.

    Output:
        Vector of tuples (docName1, docName2, similarityScore)

    Side Effects:
        None.

    Approach:
        - Nested loop:
            i = 0 → N-1
            j = i+1 → N-1
        - Avoids duplicate comparisons (i,j) and (j,i)
        - Avoids self-comparisons
        - Retrieves names safely; if missing, uses fallback names
*/
std::vector<std::tuple<std::string, std::string, double>> 
SimilarityChecker::compareAll() const {

    std::vector<std::tuple<std::string, std::string, double>> results;

    int numDocs = static_cast<int>(tfidfVectors.size());

    // Iterate over all unique pairs
    for (int i = 0; i < numDocs; i++) {
        for (int j = i + 1; j < numDocs; j++) {

            // Compute similarity
            double similarity = cosineSimilarity(i, j);

            // Retrieve document names safely
            std::string name1 = (i < static_cast<int>(documentNames.size()))
                                ? documentNames[i]
                                : "Document" + std::to_string(i);

            std::string name2 = (j < static_cast<int>(documentNames.size()))
                                ? documentNames[j]
                                : "Document" + std::to_string(j);

            // Store result tuple
            results.push_back(std::make_tuple(name1, name2, similarity));
        }
    }

    return results;
}
