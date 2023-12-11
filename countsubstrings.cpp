/**
 * Name: Jamie Clements
 * RedID: 131174989
 *
 * countsubstrings.c
 *
 * Summary:
 * This program reads a vocabulary file which is used to populate a Trie Tree data structure,
 * A test file is then evaluated to count the number of substrings from the test file exist within the Trie. (From within the vocabulary file).
 *
 * Functions:
 * - isValidSubstringPresent - Checking if a given 'substr' of a specific 'length' already exists within the array 'substrings'. (Duplicates)
 * - getValidSubstring - This is a function to extract the valid substring without invalid characters.
 * - countSubstringsUsingTrie: Used to read the test file and count the number of valid substrings.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tree.h"

/*
 * Using 'strncmp' function to match 'length' characters and ensure that the substring
 * within the array is the specified length, using the null terminator.
 * (Checking for duplicate valid substrings if any).
 *
 * @param substrings - Array of the previously found substrings
 * @param count - Num of substrings within array.
 * @param substr - Substring to check for within the array
 * @param length - Substring length (to compare)
 * @return bool True - If there is a match for substring in array: return true
 * @return bool False - If there is no match: return false
 */

bool isValidSubstringPresent(const char **substrings, int count, const char *substr, int length) {
    // Iterate through all of the found substrings in the array
    for (int i = 0; i < count; i++) {
        // Ensure that current entry in 'substrings' array is not null
        if (substrings[i] == NULL) {
            continue;
        }

        // Check if current substring is of the expected length and matches the given substring
        if (strlen(substrings[i]) == length && strncmp(substrings[i], substr, length) == 0) {
            return true;  // substring found
        }
    }

    return false;  // substring not found
}

/*
 * This is a function to extract the valid substring without invalid characters.
 *
 * @param originalLine - Original originalLine of the characters that the valid substring will be extracted from.
 * @param startPos - Starting position in line (to begin extraction).
 * @param endPos - Ending position in line (Finish extraction).
 * @return char* - Returning a pointer to the newly created (valid) substring.
 * Memory for this substring is allocated dynamically.
 */
char* getValidSubstring(const char* originalLine, int startPos, int endPos) {
    // Allocating memory for substring, with extra space for null terminator.
    char* validSub = (char*) malloc(sizeof(char) * (endPos - startPos + 2));
    int index = 0; // Index for inserting the characters into the string

    // Extracting the valid characters from original line (within startPos and endPos).
    for (int i = startPos; i <= endPos; i++) {
        if (charToIndex(originalLine[i]) != -1) { // Is the character valid?
            validSub[index++] = tolower(originalLine[i]); // Converts character to lowercase (insensitivity) & stores.
        }

    }
    validSub[index] = '\0'; // Null-terminating substring
    return validSub; // Returning the newly created substring
}


/*
 * This function is used to count the distinct valid substrings within a given line
 * that exists within the trie structure. For each of the possible substrings, this function
 * will navigate through the trie and determine the validity of the word based on the substring.
 * Whilst also ensuring only valid characters are considered whilst searching.
 * All valid substrings are stored within a dynamic array which is used to prevent counting duplicates.
 *
 * @param root - This is the root node of the trie (where substrings searched)
 * @param line - This is the line of text substrings will be identified in.
 * @return int - This is the count of the valid substrings found within the line (that also exist in trie).
 */
int countSubstringsUsingTrie(CharNode *root, const char *line) {
    int lineLength = strlen(line); // Getting length of line
    int substrCount = 0; // Initializing count for the substrings found in Trie
    const char **foundSubstrings = NULL; // Dynamic array to store substrings that are already found.

    // Double loop, generating all of possible substrings from current position 'i' within the line.
    for (int i = 0; i < lineLength; i++) {
        CharNode *currentNode = root; // Starting at root of Trie

        for (int j = i; j < lineLength; j++) {
            if (charToIndex(line[j]) == -1) continue;  // Skip invalid characters

            int index = charToIndex(line[j]); // Convert character to corresponding index in Trie.

            if (!currentNode->next[index]) break; // If doesn't exist in trie, break inner loop

            currentNode = currentNode->next[index]; // Moving to node in trie

            // If end of word has been reached in trie
            if (currentNode->next[30]) {
                char *sub = getValidSubstring(line, i, j);  // Extracting substring from the line, without invalid chars

                // Ig the substring isn't found before, we will add it to the list.
                if (!isValidSubstringPresent(foundSubstrings, substrCount, sub, j - i + 1)) {
                    // Allocating space for a new substring and storing it
                    foundSubstrings = realloc(foundSubstrings, (substrCount + 1) * sizeof(char *));
                    foundSubstrings[substrCount] = strndup(sub, j - i + 1);
                    substrCount++; // Increasing the count for the valid substrings found.
                }
                free(sub); // This is freeing the memory allocated for the substring (De-allocation).
            }
        }
    }
    // Free the dynamic array and its contents of found substrings
    for (int i = 0; i < substrCount; i++) {
        free((void *)foundSubstrings[i]);
    }
    free(foundSubstrings); // Freeing the memory of the array


    return substrCount; // Returning the count of the valid substrings
}


/*
 * Main function of the program
 *
 * @para argc - Count of the command line arguments
 * @para argv - Array of the command line arguments
 * @return - 0 is returned if successful, 1 for error
 */
int main(int argc, char **argv) {
    // Checking number of command-line arguments.
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <vocabulary.txt> <testfile.txt>\n", argv[0]);
        return 1;
    }

    // Opening vocabulary file to be inserted to trie
    FILE *vocabFile = fopen(argv[1], "r");
    if (!vocabFile) {
        perror("Error opening vocabulary file");
        return 1;
    }

    // Opening test file
    FILE *testFile = fopen(argv[2], "r");
    if (!testFile) {
        perror("Error opening test file");
        fclose(vocabFile);
        return 1;
    }

    // Initializing the root node of the Trie
    CharNode *root = (CharNode *)malloc(sizeof(CharNode));
    for (int i = 0; i < MAX_CHILD_NODES; i++) {
        root->next[i] = NULL; // Originally initialising all child pointers to NULL
    }

    char line[1024]; // Buffer which holds lines read from the file
    while (fgets(line, sizeof(line), vocabFile)) { // Reading the vocabulary file line by line.
        int len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0'; // Removing the new line character
        }
        insertIntoTrie(root, line); // Inserting each word from vocabulary.txt into the Trie
    }

    // Used to count the number of substrings in trie, for each line in the test file
    while (fgets(line, sizeof(line), testFile)) {
        int len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0'; // Removing the new line character
        }

        int count = countSubstringsUsingTrie(root, line); // Counting substrings for this line
        printf("%d\n", count); // Printing the count of valid substrings
    }

    fclose(vocabFile); // Closing the vocabulary file
    fclose(testFile); // Closing the test file
    freeTrie(root); // Free the Trie memory

    return 0;
}
