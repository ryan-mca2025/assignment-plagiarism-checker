#ifndef FEATUREEXTRACTOR_H
#define FEATUREEXTRACTOR_H

#include <vector>
#include <string>
#include <map>
#include <unordered_map>

/*
    ===========================================
                CLASS : FeatureExtractor
    ===========================================

    Objective:
        Convert tokenized documents into TF-IDF vectors by:
        - building a vocabulary
        - computing term frequency (TF)
        - computing inverse document frequency (IDF)
        - combining TF and IDF into TF-IDF values

    Input:
        docs : std::vector<std::vector<std::string>>
               A list of tokenized documents.

    Output:
        - Vocabulary of unique terms.
        - TF-IDF vector for a given document.
        - TF-IDF vectors for all documents.

    Side Effects:
        - Stores computed vocabulary internally.
        - Stores TF-IDF vectors internally.
*/

class FeatureExtractor {
private:
    // List of tokenized documents
    std::vector<std::vector<std::string>> documents;

    // Vocabulary of unique words
    std::vector<std::string> vocabulary;

    // TF-IDF vectors: term -> tfidf value for each doc
    std::vector<std::map<std::string, double>> tfidfVectors;

    /*
        Objective:
            Build vocabulary using all unique terms across documents.

        Input:
            None (uses internal 'documents').

        Output:
            Populates the 'vocabulary' vector.

        Side Effects:
            Modifies the internal 'vocabulary' vector.

        Approach:
            Use a std::set for uniqueness, then convert to vector.
    */
    void buildVocabulary();

    /*
        Objective:
            Compute Term Frequency for one document.

        Input:
            document : vector<string>

        Output:
            A map: term -> TF value (double)

        Side Effects:
            None.

        Approach:
            Count occurrences, divide by total words.
    */
    std::map<std::string, double> computeTF(const std::vector<std::string>& document) const;

    /*
        Objective:
            Compute IDF for all terms across all documents.

        Input:
            None (uses internal 'documents' and 'vocabulary').

        Output:
            Map of term -> IDF value.

        Side Effects:
            None.

        Approach:
            For each term, count number of documents containing it.
            Apply IDF formula: log10(totalDocs / docCount).
    */
    std::map<std::string, double> computeIDF() const;

public:
    /*
        Objective:
            Initialize class with given documents.

        Input:
            docs : list of tokenized documents.

        Output:
            None.

        Side Effects:
            Stores documents and builds vocabulary.
    */
    FeatureExtractor(const std::vector<std::vector<std::string>>& docs);

    /*
        Objective:
            Compute TF-IDF vectors for all documents.

        Input:
            None.

        Output:
            Populates internal tfidfVectors.

        Side Effects:
            Modifies tfidfVectors vector.

        Approach:
            Compute IDF once, compute TF for each doc,
            multiply TF * IDF for each term.
    */
    void computeTFIDF();

    /*
        Objective:
            Retrieve TF-IDF vector for a specific document.

        Input:
            docIndex : int

        Output:
            Map<string, double> representing TF-IDF values.

        Side Effects:
            None.
    */
    std::map<std::string, double> getTFIDFVector(int docIndex) const;

    /*
        Objective:
            Retrieve TF-IDF vectors for all documents.

        Input:
            None.

        Output:
            Vector of maps.

        Side Effects:
            None.
    */
    std::vector<std::map<std::string, double>> getAllTFIDFVectors() const;

    /*
        Objective:
            Retrieve the vocabulary list.

        Input:
            None.

        Output:
            Vector<string> of unique terms.

        Side Effects:
            None.
    */
    std::vector<std::string> getVocabulary() const;
};

#endif // FEATUREEXTRACTOR_H
