#ifndef TEXTCLEANER_H
#define TEXTCLEANER_H

#include <vector>
#include <string>
#include <unordered_set>

/*
    ========================================================================
                            CLASS : TextCleaner
    ========================================================================

    Objective:
        The TextCleaner class provides text preprocessing utilities used in 
        preparing raw assignment content for TF-IDF vectorization. The class 
        standardizes text by:
            - Lowercasing text
            - Removing punctuation
            - Splitting text into tokens (tokenization)
            - Removing common English stopwords
        These preprocessing steps improve analysis quality and reduce noise.

    Input:
        - Raw text strings (entire document contents)
        - Tokenized lists of words

    Output:
        - Cleaned and tokenized vector<string> suitable for FeatureExtractor.

    Side Effects:
        - None externally.
        - Internally initializes a stopword set but does not modify inputs.
*/

class TextCleaner {
private:

    /*
        Objective:
            Store a set of English stopwords for fast lookup.

        Input:
            Populated internally during construction.

        Output:
            None.

        Side Effects:
            Used by removeStopWords() to eliminate low-value words.
    */
    std::unordered_set<std::string> stopWords;

    /*
        Objective:
            Populate the stopWords set with common English function words.

        Input:
            None.

        Output:
            None.

        Side Effects:
            Modifies the 'stopWords' set during initialization.

        Approach:
            - Insert hardcoded stopwords into an unordered_set for O(1) lookup.
    */
    void initializeStopWords();

public:

    /*
        Objective:
            Initialize TextCleaner and load stopwords.

        Input:
            None.

        Output:
            None.

        Side Effects:
            Calls initializeStopWords() internally.
    */
    TextCleaner();

    /*
        Objective:
            Convert all characters in a given string to lowercase.

        Input:
            text → raw or partially cleaned string.

        Output:
            Lowercase version of the input string.

        Side Effects:
            None.

        Notes:
            Lowercasing helps reduce vocabulary size by normalizing words.
    */
    std::string toLower(const std::string& text) const;

    /*
        Objective:
            Remove punctuation characters from a string.

        Input:
            text → original or lowercased string.

        Output:
            A string containing only:
                - Alphanumeric characters (letters, numbers)
                - Whitespace characters

        Side Effects:
            None.

        Approach:
            - Keep letters, digits, and spaces.
            - Replace punctuation with a space to maintain token boundaries.
    */
    std::string removePunctuation(const std::string& text) const;

    /*
        Objective:
            Filter out stopwords from a token list.

        Input:
            tokens → vector<string> of individual words.

        Output:
            New vector without stopwords or empty tokens.

        Side Effects:
            None.
    */
    std::vector<std::string> removeStopWords(const std::vector<std::string>& tokens) const;

    /*
        Objective:
            Split text into tokens (words) separated by whitespace.

        Input:
            text → cleaned or partially cleaned text.

        Output:
            vector<string> where each element is a word.

        Side Effects:
            None.

        Approach:
            - Uses stringstream to split on whitespace.
    */
    std::vector<std::string> tokenize(const std::string& text) const;

    /*
        Objective:
            Run the complete preprocessing pipeline:
                1. Lowercase
                2. Remove punctuation
                3. Tokenize
                4. Remove stopwords

        Input:
            text → raw document content.

        Output:
            vector<string> → fully cleaned, filtered, tokenized text.

        Side Effects:
            None.
    */
    std::vector<std::string> preprocess(const std::string& text) const;
};

#endif // TEXTCLEANER_H
