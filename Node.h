/**
* @file Node.h
 * @brief Building blocks for the Huffman Tree.
 * * Huffman trees are composed of nodes. To interpret input files as raw bytes
 * rather than strings, symbols are stored as uint8_t
 */

#include <cstdint>

/**
 * @class Node
 * @brief A node in the Huffman tree representing a symbol or an internal node
 * * Contains pointers to children, a symbol (uint8_t), and the frequency of that
 * symbol [cite: 203-210]. Internal nodes are assigned the symbol '$'
 */
class Node {
private:
    Node* left;         ///< Pointer to left child
    Node* right;        ///< Pointer to right child
    uint8_t symbol;     ///< Node's symbol stored as a raw byte
    uint64_t frequency; ///< Frequency of the symbol's occurrence
public:
    uint8_t get_symbol() const;
    uint64_t get_frequency() const;
    Node* get_left() const;
    Node* get_right() const;

    void set_symbol(uint8_t s);
    void set_frequency(uint64_t f);
    void set_left(Node* l);
    void set_right(Node* r);

    /** * @brief Parameterized constructor to set symbol and frequency
     */
    Node(uint8_t given_symbol, uint64_t given_frequency);

    /** * @brief Joins a left and right child to create a parent node
     * * The parent frequency is the sum of its children
     * The parent symbol is set to '$'
     */
    friend Node* create_parent(Node& left_child, Node& right_child);

    /** * @brief Checks if a node is a leaf (both children are null)
     */
    bool is_Leaf();
};