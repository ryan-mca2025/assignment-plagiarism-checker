#include "TextCleaner.h"
#include <algorithm>
#include <sstream>
#include <cctype>
#include <iostream>

/*
-------------------------------------------------
Function Name : TextCleaner (Constructor)

Objective:
    Initialize TextCleaner and load stopwords.

Input:
    None.

Output:
    TextCleaner object initialized.

Side Effect:
    Loads stopwords into internal set.

Approach:
    Calls stopword initialization routine.

    // call initializeStopWords()
*/
TextCleaner::TextCleaner() {
    // call initializeStopWords()
    initializeStopWords();
}

/*
-------------------------------------------------
Function Name : initializeStopWords()

Objective:
    Fill stopWords set with common English words.

Input:
    None.

Output:
    None.

Side Effect:
    Populates internal stopWords container.

Approach:
    Insert predefined words into unordered_set.
*/
void TextCleaner::initializeStopWords() {

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

    for (const auto& word : words) {
        stopWords.insert(word);
    }
}

/*
-------------------------------------------------
Function Name : toLower()

Objective:
    Convert input text to lowercase.

Input:
    text → input string.

Output:
    Lowercase version of the text.

Side Effect:
    None.

Approach:
    Apply ::tolower to each character.
*/
std::string TextCleaner::toLower(const std::string& text) const {
    std::string result = text;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

/*
-------------------------------------------------
Function Name : removePunctuation()

Objective:
    Remove punctuation and replace with space.

Input:
    text → string with punctuation.

Output:
    Cleaned string with punctuation removed.

Side Effect:
    None.


Approach:
    Replace non-alphanumeric characters with space.
*/
std::string TextCleaner::removePunctuation(const std::string& text) const {
    std::string result;
    result.reserve(text.size());

    for (char c : text) {
        if (std::isalnum(c) || std::isspace(c)) {
            result += c;
        } else {
            result += ' ';
        }
    }

    return result;
}

/*
-------------------------------------------------
Function Name : removeStopWords()

Objective:
    Remove common meaningless words from token list.

Input:
    tokens → vector of words.

Output:
    Vector without stopwords.

Side Effect:
    None.

Approach:
    Keep tokens that are not in stopWords set.
*/
std::vector<std::string> TextCleaner::removeStopWords(
        const std::vector<std::string>& tokens) const {

    std::vector<std::string> filtered;

    for (const auto& token : tokens) {
        if (stopWords.find(token) == stopWords.end() && !token.empty()) {
            filtered.push_back(token);
        }
    }

    return filtered;
}

/*
-------------------------------------------------
Function Name : tokenize()

Objective:
    Split text into words.

Input:
    text → cleaned string.

Output:
    Vector of tokens.

Side Effect:
    None.

Inside Function:
Approach:
    Use string stream to extract words.
*/
std::vector<std::string> TextCleaner::tokenize(const std::string& text) const {

    std::vector<std::string> tokens;
    std::istringstream iss(text);
    std::string token;

    while (iss >> token) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }

    return tokens;
}

/*
-------------------------------------------------
Function Name : preprocess()

Objective:
    Perform full text cleaning.

Input:
    text → raw document.

Output:
    Vector of cleaned tokens.

Side Effect:
    None.

Approach:
    Apply lowercase, punctuation removal, tokenization and stopword filtering.

    // call toLower()
    // call removePunctuation()
    // call tokenize()
    // call removeStopWords()
*/
std::vector<std::string> TextCleaner::preprocess(const std::string& text) const {

    // call toLower()
    std::string lowerText = toLower(text);

    // call removePunctuation()
    std::string noPunct = removePunctuation(lowerText);

    // call tokenize()
    std::vector<std::string> tokens = tokenize(noPunct);

    // call removeStopWords()
    std::vector<std::string> cleaned = removeStopWords(tokens);

    return cleaned;
}
