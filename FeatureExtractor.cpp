#include "FeatureExtractor.h"
#include <algorithm>
#include <cmath>
#include <set>

/*
-------------------------------------------------
Function Name : FeatureExtractor (Constructor)

Objective:
    Initialize the FeatureExtractor with preprocessed documents and prepare vocabulary.

Input:
    docs → Vector containing tokenized words of all documents.

Output:
    Object of FeatureExtractor with documents loaded.

Side Effect:
    Builds vocabulary immediately after object creation.


Approach:
    Assign input documents and immediately call buildVocabulary().

    // call that function
*/
FeatureExtractor::FeatureExtractor(const std::vector<std::vector<std::string>>& docs) 
    : documents(docs) {
    // call buildVocabulary()
    buildVocabulary();
}

/*
-------------------------------------------------
Function Name : buildVocabulary()

Objective:
    Create a unique list of all words across all documents.

Input:
    None (uses internal documents list).

Output:
    Updates the vocabulary vector with unique words.

Side Effect:
    Modifies internal vocabulary container.


Approach:
    Collect words in a set to avoid duplicates and convert into vector.
*/
void FeatureExtractor::buildVocabulary() {
    std::set<std::string> vocabSet;

    // Collect all unique words
    for (const auto& doc : documents) {
        for (const auto& word : doc) {
            vocabSet.insert(word);
        }
    }

    // Convert set into vector
    vocabulary.assign(vocabSet.begin(), vocabSet.end());
}

/*
-------------------------------------------------
Function Name : computeTF()

Objective:
    Calculate term frequency for a single document.

Input:
    document → Tokenized single document.

Output:
    Map of word → Term Frequency (TF) value.

Side Effect:
    None.


Approach:
    Count word occurrences and divide by total word count.
*/
std::map<std::string, double> FeatureExtractor::computeTF(const std::vector<std::string>& document) const {
    std::map<std::string, double> tf;

    if (document.empty()) {
        return tf;
    }

    std::map<std::string, int> termCount;

    // Count frequency of each term
    for (const auto& term : document) {
        termCount[term]++;
    }

    double totalTerms = static_cast<double>(document.size());

    // Convert counts to TF values
    for (const auto& pair : termCount) {
        tf[pair.first] = static_cast<double>(pair.second) / totalTerms;
    }

    return tf;
}

/*
-------------------------------------------------
Function Name : computeIDF()

Objective:
    Compute Inverse Document Frequency (IDF) for each vocabulary word.

Input:
    None (uses global vocabulary and documents).

Output:
    Map of word → IDF value.

Side Effect:
    None.


Approach:
    Count how many documents contain each word and apply IDF formula.
*/
std::map<std::string, double> FeatureExtractor::computeIDF() const {
    std::map<std::string, double> idf;

    if (documents.empty()) {
        return idf;
    }

    double totalDocs = static_cast<double>(documents.size());

    for (const auto& term : vocabulary) {
        int docCount = 0;

        // Count documents that contain the term
        for (const auto& doc : documents) {
            if (std::find(doc.begin(), doc.end(), term) != doc.end()) {
                docCount++;
            }
        }

        // Apply IDF formula
        if (docCount > 0) {
            idf[term] = std::log10(totalDocs / static_cast<double>(docCount));
        } else {
            idf[term] = 0.0;
        }
    }

    return idf;
}

/*
-------------------------------------------------
Function Name : computeTFIDF()

Objective:
    Generate TF-IDF vectors for all documents.

Input:
    None (uses stored documents).

Output:
    Stores TF-IDF vectors internally.

Side Effect:
    Modifies tfidfVectors container.


Approach:
    Calculate TF and IDF, multiply them, and store per document.

    // call computeIDF()
    // call computeTF()
*/
void FeatureExtractor::computeTFIDF() {
    tfidfVectors.clear();

    if (documents.empty()) {
        return;
    }

    // call computeIDF()
    std::map<std::string, double> idf = computeIDF();

    for (const auto& doc : documents) {
        // call computeTF()
        std::map<std::string, double> tf = computeTF(doc);
        std::map<std::string, double> tfidf;

        // Compute TF-IDF values
        for (const auto& term : vocabulary) {
            double tfValue = (tf.find(term) != tf.end()) ? tf.at(term) : 0.0;
            double idfValue = idf.at(term);
            tfidf[term] = tfValue * idfValue;
        }

        tfidfVectors.push_back(tfidf);
    }
}

/*
-------------------------------------------------
Function Name : getTFIDFVector()

Objective:
    Retrieve TF-IDF vector for a specific document.

Input:
    docIndex → Document index.

Output:
    Corresponding TF-IDF vector map.

Side Effect:
    None.


Approach:
    Check bounds and return requested vector if valid.
*/
std::map<std::string, double> FeatureExtractor::getTFIDFVector(int docIndex) const {
    if (docIndex >= 0 && docIndex < static_cast<int>(tfidfVectors.size())) {
        return tfidfVectors[docIndex];
    }
    return {};
}

/*
-------------------------------------------------
Function Name : getAllTFIDFVectors()

Objective:
    Retrieve all TF-IDF vectors.

Input:
    None.

Output:
    Vector of TF-IDF maps.

Side Effect:
    None.


Approach:
    Return full TF-IDF container directly.
*/
std::vector<std::map<std::string, double>> FeatureExtractor::getAllTFIDFVectors() const {
    return tfidfVectors;
}

/*
-------------------------------------------------
Function Name : getVocabulary()

Objective:
    Retrieve vocabulary list.

Input:
    None.

Output:
    Vector of unique words.

Side Effect:
    None.


Approach:
    Return vocabulary container.
*/
std::vector<std::string> FeatureExtractor::getVocabulary() const {
    return vocabulary;
}
