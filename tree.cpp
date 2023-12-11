/**
 * Name: Jamie Clements
 * RedID: 131174989
 *
 * tree.c
 *
 * Summary:
 * This file provides the implementation of functions declared in the header
 *
 * Functions:
 * - charToIndex: maps character to corresponding index in Trie
 * - insertIntoTrie: inserts given string into Trie
 * - searchTrie: searches for string in Trie
 * - freeTrie: deallocating memory that is reserved for the trie
 */
#include "tree.h"
#include <stdlib.h>
#include <ctype.h>

/*
 * Mapping character to corresponding index in Trie (used in insert and searchTrie functions)
 * Handles alphabet and special characters
 *
 * @param ch - character converted to index
 * @return int index - returns the index of corresponding characters
 */
int charToIndex(char ch) {
    ch = tolower(ch);  // Convert to lowercase, case-insensitivity
    // Ensuring only valid characters are valid & mapping corresponding to index
    if (ch >= 'a' && ch <= 'z') return ch - 'a';
    switch (ch) {
        case ' ':
            return 26; // single space
        case '\'':
            return 27; // apostrophe
        case '-':
            return 28; // hyphen
        case '_':
            return 29; // underscore
        case '\0':
            return 30; // null terminator
        default:
            return -1; // skip invalid characters
    }
}

/*
 * Function to insert a string into Trie,
 * whilst also skipping invalid characters by charToIndex function.
 *
 * @param currentNode- Points to current Trie node
 * @param strToInsert - String to be inserted to Trie
 * @return bool True - If string inserted successfully, else false
*/
bool insertIntoTrie(CharNode *currentNode, const char *strToInsert) {
    // If string is empty or at end/checking existence of terminator
    if (!strToInsert || *strToInsert == '\0') {

        // If the terminator is not set, allocate memory to it, initializing its children as NULL
        if (!currentNode->next[30]) {
            currentNode->next[30] = (CharNode *)malloc(sizeof(CharNode));
            for (int i = 0; i < MAX_CHILD_NODES; i++) {
                currentNode->next[30]->next[i] = NULL;
            }
            return true;
        }
        // If the terminator already exists, returning false. (String already inserted)
        return false;
    }

    // Used to convert the current character to its corresponding Trie index.
    int index = charToIndex(*strToInsert);
    // If the character is invalid, skip this and go to the next character within the string
    if (index == -1) {
        return insertIntoTrie(currentNode, strToInsert + 1);
    }

    // If the Trie node for the character doesn't exist, allocate memory and initialise its children to NULL
    if (!currentNode->next[index]) {
        currentNode->next[index] = (CharNode *)malloc(sizeof(CharNode));
        for (int i = 0; i < MAX_CHILD_NODES; i++) {
            currentNode->next[index]->next[i] = NULL;
        }
    }
    // Continuing to insert the remaining part of the string
    return insertIntoTrie(currentNode->next[index], strToInsert + 1);
}

/*
 * Function to searchTrie for a string in Trie, checks for characters in Trie,
 * false if character/string not found. (Using charToIndex function)
 *
 * @param currentNode - Points to current Trie node
 * @param strToSearch - String to be searched within Trie
 * @return bool True - If string is found successfully, else false
*/
bool searchTrie(CharNode *currentNode, const char *strToSearch) {
    // If string is empty or at end/checking existence of terminator
    if (!strToSearch || *strToSearch == '\0') {
        return currentNode->next[30] != NULL;
    }

    // Used to convert the current character to its corresponding Trie index.
    int index = charToIndex(*strToSearch);
    // If fails/character doesn't exist, return false
    if (index == -1 || !currentNode->next[index]) {
        return false;
    }

    // Recursively searching for the next character within the string.
    return searchTrie(currentNode->next[index], strToSearch + 1);
}

/*
 * This is a function to recursively delete the child nodes of the Trie Tree,
 * and free the memory of the current Trie node.
 */
void freeTrie(CharNode *node) {
    // If the node ill null, return
    if (!node) return;

    // To recursively free the child nodes
    for (int i = 0; i < MAX_CHILD_NODES; i++) {
        freeTrie(node->next[i]);
    }
    // Freeing the memory of the current node
    free(node);
}