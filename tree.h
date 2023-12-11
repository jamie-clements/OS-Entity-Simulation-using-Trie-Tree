/**
 * Name: Jamie Clements
 * RedID: 131174989
 *
 * tree.h
 *
 * Summary:
 * This is the header file for the Trie Tree Operations within the program
 *
 * Providing declarations and definitions:
 * 1. Defines structure of Trie node (CharNode)
 * 2. Declares functions that manipulate Trie
 * 3. MAX_CHILD_NODES - Trie node constant (indicating maximum number of potential children for a Trie node)
 */
#ifndef TREE_H
#define TREE_H

#include <stdbool.h>

/*
 * Maximum Number of potential children nodes (31)
 * Including: 26 a-z, 4 special chars ('-', '_', ' ', ''') and the \0 null terminator
*/
#define MAX_CHILD_NODES 31

/*
 * Definition of Trie node
 * This trie node is used to hold an array of pointers to its children (31 potential)
*/
typedef struct CharNode {
    struct CharNode *next[MAX_CHILD_NODES]; // Array of children's pointers
} CharNode;

/*
 * Functions for the Trie operations
*/
int charToIndex(char c);
bool insertIntoTrie(CharNode *currentNode, const char *strToInsert);
bool searchTrie(CharNode *currentNode, const char *strToSearch);
void freeTrie(CharNode *node);


#endif // TREE_H
