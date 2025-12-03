#include "SimilarityChecker.h"
#include <cmath>
#include <algorithm>

/*
-------------------------------------------------
Function Name : SimilarityChecker (Constructor)

Objective:
    Initialize TF-IDF vectors and document names for comparison.

Input:
    vectors → TF-IDF maps for each document.
    names   → Document file names.

Output:
    Object initialized.

Side Effect:
    Stores vectors and names internally.

Approach:
    Assign vectors and names to internal variables.
*/
SimilarityChecker::SimilarityChecker(
        const std::vector<std::map<std::string, double>>& vectors,
        const std::vector<std::string>& names)
    : tfidfVectors(vectors), documentNames(names) {
}

/*
-------------------------------------------------
Function Name : dotProduct()

Objective:
    Compute scalar dot product of two TF-IDF vectors.

Input:
    vec1 → TF-IDF vector for first document.
    vec2 → TF-IDF vector for second document.

Output:
    Double representing dot product value.

Side Effect:
    None.

Approach:
    Iterate over smaller vector and multiply values where keys match.
*/
double SimilarityChecker::dotProduct(
        const std::map<std::string, double>& vec1,
        const std::map<std::string, double>& vec2) const {

    double result = 0.0;

    const auto& smaller = (vec1.size() < vec2.size()) ? vec1 : vec2;
    const auto& larger  = (vec1.size() < vec2.size()) ? vec2 : vec1;

    for (const auto& pair : smaller) {
        const std::string& term = pair.first;
        double value1 = pair.second;

        auto it = larger.find(term);
        if (it != larger.end()) {
            double value2 = it->second;
            result += value1 * value2;
        }
    }

    return result;
}

/*
-------------------------------------------------
Function Name : magnitude()

Objective:
    Compute vector magnitude (Euclidean length).

Input:
    vec → TF-IDF vector.

Output:
    Double representing magnitude.

Side Effect:
    None.

Approach:
    Square each value, sum them, then take square root.
*/
double SimilarityChecker::magnitude(
        const std::map<std::string, double>& vec) const {

    double sum = 0.0;

    for (const auto& pair : vec) {
        double value = pair.second;
        sum += value * value;
    }

    return std::sqrt(sum);
}

/*
-------------------------------------------------
Function Name : cosineSimilarity()

Objective:
    Compute similarity between two documents using cosine similarity formula.

Input:
    doc1Index → Index of first document.
    doc2Index → Index of second document.

Output:
    Double in range [0.0, 1.0].

Side Effect:
    None.


Approach:
    Use dot product divided by product of magnitudes.

    // call dotProduct()
    // call magnitude()
*/
double SimilarityChecker::cosineSimilarity(int doc1Index, int doc2Index) const {

    if (doc1Index < 0 || doc1Index >= static_cast<int>(tfidfVectors.size()) ||
        doc2Index < 0 || doc2Index >= static_cast<int>(tfidfVectors.size())) {
        return 0.0;
    }

    if (doc1Index == doc2Index) {
        return 1.0;
    }

    const auto& vec1 = tfidfVectors[doc1Index];
    const auto& vec2 = tfidfVectors[doc2Index];

    // call dotProduct()
    double dot  = dotProduct(vec1, vec2);

    // call magnitude()
    double mag1 = magnitude(vec1);
    double mag2 = magnitude(vec2);

    if (mag1 == 0.0 || mag2 == 0.0) {
        return 0.0;
    }

    return dot / (mag1 * mag2);
}

/*
-------------------------------------------------
Function Name : compareAll()

Objective:
    Compare all document pairs and compute similarity values.

Input:
    None.

Output:
    Vector of tuples (docName1, docName2, similarityScore).

Side Effect:
    None.

Approach:
    Iterate through all document pairs and compute cosine similarity.

    // call cosineSimilarity()
*/
std::vector<std::tuple<std::string, std::string, double>> 
SimilarityChecker::compareAll() const {

    std::vector<std::tuple<std::string, std::string, double>> results;

    int numDocs = static_cast<int>(tfidfVectors.size());

    for (int i = 0; i < numDocs; i++) {
        for (int j = i + 1; j < numDocs; j++) {

            // call cosineSimilarity()
            double similarity = cosineSimilarity(i, j);

            std::string name1 = (i < static_cast<int>(documentNames.size()))
                                ? documentNames[i]
                                : "Document" + std::to_string(i);

            std::string name2 = (j < static_cast<int>(documentNames.size()))
                                ? documentNames[j]
                                : "Document" + std::to_string(j);

            results.push_back(std::make_tuple(name1, name2, similarity));
        }
    }

    return results;
}
