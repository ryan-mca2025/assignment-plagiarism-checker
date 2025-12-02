#include "FeatureExtractor.h"
#include <algorithm>
#include <cmath>
#include <set>

/*
    Constructor

    Approach:
        - Store incoming documents.
        - Immediately build vocabulary from them.
*/
FeatureExtractor::FeatureExtractor(const std::vector<std::vector<std::string>>& docs) 
    : documents(docs) {
    buildVocabulary();
}

/*
    buildVocabulary()

    Approach:
        - Use std::set to maintain unique sorted terms.
        - Insert all terms from all documents.
        - Convert set to vector for final vocabulary.

    Line Explanation:
        vocabSet.insert(word);     -> ensures uniqueness
        vocabulary.assign(...)     -> converts set into vector
*/
void FeatureExtractor::buildVocabulary() {
    std::set<std::string> vocabSet;

    // Collect all unique words
    for (const auto& doc : documents) {
        for (const auto& word : doc) {
            vocabSet.insert(word);
        }
    }

    // Convert set into vector for indexing and consistency
    vocabulary.assign(vocabSet.begin(), vocabSet.end());
}

/*
    computeTF()

    Approach:
        - Count each term's occurrences using a map.
        - Divide each count by the total number of words to get TF.
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

    // Convert counts to term frequency
    for (const auto& pair : termCount) {
        tf[pair.first] = static_cast<double>(pair.second) / totalTerms;
    }

    return tf;
}

/*
    computeIDF()

    Approach:
        - For each term in vocabulary, count number of documents containing it.
        - Apply IDF formula: log10(totalDocs / docCount)

    Line Explanation:
        std::find(doc.begin(), doc.end(), term) != doc.end()
            → checks whether the term exists in the document.
*/
std::map<std::string, double> FeatureExtractor::computeIDF() const {
    std::map<std::string, double> idf;

    if (documents.empty()) {
        return idf;
    }

    double totalDocs = static_cast<double>(documents.size());

    for (const auto& term : vocabulary) {
        int docCount = 0;

        // Count documents containing the term
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
    computeTFIDF()

    Approach:
        - Clear previous TF-IDF results.
        - Compute IDF once.
        - For each document:
            • Compute TF
            • Multiply TF * IDF for every term in vocabulary.
*/
void FeatureExtractor::computeTFIDF() {
    tfidfVectors.clear();

    if (documents.empty()) {
        return;
    }

    std::map<std::string, double> idf = computeIDF();

    for (const auto& doc : documents) {
        std::map<std::string, double> tf = computeTF(doc);
        std::map<std::string, double> tfidf;

        // Compute TF-IDF for each vocabulary term
        for (const auto& term : vocabulary) {
            double tfValue = (tf.find(term) != tf.end()) ? tf.at(term) : 0.0;
            double idfValue = idf.at(term);
            tfidf[term] = tfValue * idfValue;
        }

        tfidfVectors.push_back(tfidf);
    }
}

/*
    getTFIDFVector()

    Approach:
        - Safely return the TF-IDF vector if index is valid.
*/
std::map<std::string, double> FeatureExtractor::getTFIDFVector(int docIndex) const {
    if (docIndex >= 0 && docIndex < static_cast<int>(tfidfVectors.size())) {
        return tfidfVectors[docIndex];
    }
    return {};
}

/*
    getAllTFIDFVectors()

    Approach:
        - Simply return full TF-IDF matrix.
*/
std::vector<std::map<std::string, double>> FeatureExtractor::getAllTFIDFVectors() const {
    return tfidfVectors;
}

/*
    getVocabulary()

    Approach:
        - Return the vocabulary as-is.
*/
std::vector<std::string> FeatureExtractor::getVocabulary() const {
    return vocabulary;
}
