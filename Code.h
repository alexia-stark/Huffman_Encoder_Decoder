/**
* @file Code.h
 * @brief ADT representing a stack of bits for Huffman encoding[cite: 220].
 */

#include <cstdint>
#define MAX_CODE_SIZE 200

/**
 * @class Code
 * @brief A fixed-size bit stack for storing Huffman codes.
 * * This class requires no dynamic memory allocation It tracks
 * the 'top' of the stack to manage bits
 */
class Code {
private:
    uint32_t top;                     ///< Index for the top of the bit stack
    uint8_t bits[MAX_CODE_SIZE];      ///< Array storing individual bits

public:
    /** @brief Constructor: sets top to 0 and zeros out the bit array. */
    Code();

    /** @brief Returns the bit at the specified index. */
    uint8_t get_bit_at(int index);

    /** @brief Returns the current size of the code */
    int get_size();

    /** @brief Checks if the bit stack is empty */
    bool is_Empty();

    /** @brief Checks if the bit stack is full */
    bool is_Full();

    /** * @brief Pushes a bit onto the stack
     * @return False if full, True otherwise
     */
    bool push_bit(uint8_t bit);

    /** * @brief Pops a bit from the stack and returns its value
     * @return Bit value or -1/False if empty
     */
    int pop_bit();

    Code& operator=(const Code& other);
};