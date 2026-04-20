/**
 * @file Huffman_Tree.h
 * @brief Logic for Huffman lossless data compression
 * * Based on David Huffman's 1951 breakthrough
 * The method relies on Entropy $H(X)$, where common symbols use fewer bits than rare symbols.
 */

#pragma once
#include "Node.h"
#include "Code.h"
#include <string>

#define BLOCK 4096
#define ALPHABET_SIZE 256
#define MAGIC 0xDEADEAEF
#define MAX_TREE_SIZE (3 * ALPHABET_SIZE - 1)

/**
 * @class Huffman_Tree
 * @brief Manages construction, serialization, and reconstruction of the Huffman tree.
 */
class Huffman_Tree {
private:
    Node* root;                            ///< Root of the constructed tree.
    Code code_table[ALPHABET_SIZE];        ///< Array of 256 Code objects.

    /** @brief Recursively deletes the tree to free memory. */
    void delete_everything(Node* current_node);

    /** * @brief Computes a histogram of occurrences for each symbol.
     * Increments elements 0 and 255 by one as a minimum requirement.
     */
    static void create_histogram(std::string text, int histogram[ALPHABET_SIZE]);

    /** * @brief Performs a post-order traversal to generate codes.
     * Pushes 0 for left links and 1 for right links.
     */
    void generate_codes(Node* current_node, Code& c);

public:
    Huffman_Tree(std::string text);
    Huffman_Tree();
    ~Huffman_Tree();

    Node* get_root();
    Code get_code_for_character(char c);

    /** * @brief Constructs the tree using a priority queue.
     * Dequeues two nodes, joins them, and enqueues the parent.
     */
    Node& create_huffman_tree(int histogram[ALPHABET_SIZE]);

    /** @brief Populates the code table using tree traversal. */
    void construct_code_table();

    /** * @brief Writes a post-order tree dump to the file.
     * Writes 'L' + symbol for leaves and 'I' for interior nodes.
     */
    void dump_tree_in_outfile(Node* current_node, std::ofstream& fout);

    /** * @brief Reconstructs the tree from a file dump using a stack.
     * Iterate over the dump; 'L' creates a leaf, 'I' joins two nodes from the stack.
     */
    void reconstruct_from_dump(const uint8_t* tree_dump, uint16_t size);

    /** @brief Helper to count unique symbols for header calculation */
    static int number_unique_symbols(std::string text);
};