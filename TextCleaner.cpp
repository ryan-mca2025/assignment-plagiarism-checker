#include "TextCleaner.h"
#include <algorithm>   // For std::transform
#include <sstream>     // For std::istringstream (tokenization)
#include <cctype>      // For std::isalnum, std::isspace
#include <iostream>    // For debugging (optional)

/*
    ========================================================================
                                Constructor
    ========================================================================

    Objective:
        Initialize the TextCleaner object and populate the stopWords set.

    Input:
        None.

    Output:
        None.

    Side Effects:
        Calls initializeStopWords() to load common English stopwords.
*/
TextCleaner::TextCleaner() {
    initializeStopWords();
}

/*
    ========================================================================
                           initializeStopWords()
    ========================================================================

    Objective:
        Fill the stopWords set with frequently occurring English words that 
        typically carry low semantic meaning and do not contribute to 
        plagiarism detection.

    Input:
        None.

    Output:
        None.

    Side Effects:
        Inserts dozens of predefined stopwords into the internal set.

    Notes:
        - These words are common function/structure words (e.g., "the", "is").
        - Using a vector for list and then inserting into a set gives O(n) load time.
        - Lookup during filtering is O(1) average due to unordered_set.
*/
void TextCleaner::initializeStopWords() {

    // Predefined English stopwords
    std::vector<std::string> words = {
        "a", "an", "and", "are", "as", "at", "be", "by", "for", "from",
        "has", "he", "in", "is", "it", "its", "of", "on", "that", "the",
        "to", "was", "will", "with", "the", "this", "but", "they", "have",
        "had", "what", "said", "each", "which", "their", "time", "if",
        "up", "out", "many", "then", "them", "these", "so", "some", "her",
        "would", "make", "like", "into", "him", "has", "two", "more",
        "very", "after", "words", "long", "than", "first", "been", "call",
        "who", "oil", "sit", "now", "find", "down", "day", "did", "get",
        "come", "made", "may", "part"
    };

    // Insert each word into the unordered_set
    for (const auto& word : words) {
        stopWords.insert(word);
    }
}

/*
    ========================================================================
                                 toLower()
    ========================================================================

    Objective:
        Convert the entire input string into lowercase characters.

    Input:
        text → original or partially cleaned string.

    Output:
        Lowercase version of the string.

    Side Effects:
        None.

    Approach:
        - Use std::transform() to apply ::tolower on each character.
*/
std::string TextCleaner::toLower(const std::string& text) const {
    std::string result = text;

    // Convert each character to lowercase using ::tolower
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);

    return result;
}

/*
    ========================================================================
                           removePunctuation()
    ========================================================================

    Objective:
        Remove punctuation from the input string while preserving alphanumeric
        characters and whitespace. Punctuation is replaced with spaces to avoid
        merging words incorrectly.

    Input:
        text → raw or lowercased string.

    Output:
        A new string without punctuation.

    Side Effects:
        None.

    Approach:
        - Loop through each character:
              If alphanumeric or whitespace → keep it
              Else → replace with space
        - reserve() used to avoid reallocations
*/
std::string TextCleaner::removePunctuation(const std::string& text) const {
    std::string result;
    result.reserve(text.size());  // Optimization: reserve memory

    for (char c : text) {
        if (std::isalnum(c) || std::isspace(c)) {
            result += c;      // Keep valid characters
        } else {
            result += ' ';    // Replace punctuation with space
        }
    }

    return result;
}

/*
    ========================================================================
                           removeStopWords()
    ========================================================================

    Objective:
        Remove all stopwords from a vector of tokens.

    Input:
        tokens → vector<string> that may contain stopwords.

    Output:
        New vector<string> with stopwords removed.

    Side Effects:
        None.

    Approach:
        - Iterate through tokens.
        - If token not in stopWords → keep it.
        - Ignore empty strings.
*/
std::vector<std::string> TextCleaner::removeStopWords(
        const std::vector<std::string>& tokens) const {

    std::vector<std::string> filtered;

    for (const auto& token : tokens) {
        // Only keep tokens not in stopword set
        if (stopWords.find(token) == stopWords.end() && !token.empty()) {
            filtered.push_back(token);
        }
    }

    return filtered;
}

/*
    ========================================================================
                                tokenize()
    ========================================================================

    Objective:
        Split a string into individual tokens based on whitespace.

    Input:
        text → cleaned or partially cleaned string.

    Output:
        vector<string> of individual tokens.

    Side Effects:
        None.

    Approach:
        - Use std::istringstream to extract space-separated words.
*/
std::vector<std::string> TextCleaner::tokenize(const std::string& text) const {

    std::vector<std::string> tokens;
    std::istringstream iss(text);   // Create stream from text
    std::string token;

    // Extract tokens separated by whitespace
    while (iss >> token) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }

    return tokens;
}

/*
    ========================================================================
                                preprocess()
    ========================================================================

    Objective:
        Run the complete text preprocessing pipeline:
            1. Convert text to lowercase
            2. Remove punctuation
            3. Tokenize text into words
            4. Remove stopwords

    Input:
        text → raw document content.

    Output:
        Vector<string> containing cleaned, normalized tokens.

    Side Effects:
        None.

    Approach:
        - Each transformation step feeds into the next.
*/
std::vector<std::string> TextCleaner::preprocess(const std::string& text) const {

    // Step 1: Lowercase the text
    std::string lowerText = toLower(text);

    // Step 2: Remove punctuation
    std::string noPunct = removePunctuation(lowerText);

    // Step 3: Tokenize into words
    std::vector<std::string> tokens = tokenize(noPunct);

    // Step 4: Remove stopwords
    std::vector<std::string> cleaned = removeStopWords(tokens);

    return cleaned;
}
