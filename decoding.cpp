/**
* @file decoding.cpp
 * @brief Implementation of the Huffman decompression process.
* * * Reconstructs the original "lossless" data by reading the tree dump and traversing it
 * bit-by-bit until leaf nodes are reached.
 */
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <filesystem>
#include "Huffman_Tree.h"

#define MAGIC 0xDEADEAEF

struct Header {
    uint32_t magic;
    uint16_t tree_size;
    uint64_t file_size;
};

/**
 * @brief Extracts bits one-by-one from a byte using Right Shifts and bitwise AND.
 * * This function handles the logic of reading a new byte from the file only when
 * the current 8 bits are exhausted.
 * * @param bit_count Index (0-7) of the next bit to extract.
 * @return True if the bit is 1, False if it is 0.
 */
bool get_bit_from_file(uint8_t& byte_buffer, int& bit_count, std::ifstream& fin) {
    if (bit_count == 0) {
        if (!fin.read(reinterpret_cast<char*>(&byte_buffer), 1)) {
            return false;
        }
    }

    bool bit = (byte_buffer >> (7 - bit_count)) & 1;
    bit_count = (bit_count + 1) % 8;

    return bit;
}
/**
 * @brief Decodes the compressed file into its original format.
 * * Steps:
 * 1. Verify the Header Magic Number (0xDEADEAEF).
 * 2. Load the tree dump and reconstruct the Huffman Tree using a stack.
 * 3. Traverse the tree bit-by-bit: 0 = Left, 1 = Right.
 * 4. When a leaf is reached, write its symbol and return to the root.
 */
void decode_file(const std::string& input_filename, const std::string& output_filename) {

    std::ifstream fin(input_filename, std::ios::binary);
    std::ofstream fout(output_filename, std::ios::binary);

    if (!fin.is_open()) {
        std::cerr << "Error at the input file\n";
        return;
    }
    if (!fout.is_open()) {
        std::cerr << "Error at the output file\n";
        return;
    }

   ///step 1
    Header header;
    fin.read(reinterpret_cast<char*>(&header), sizeof(header));

    if (header.magic != MAGIC) {
        std::cerr << "Invalid file format (Magic number mismatch)!\n";
        return;
    }

    // Tree reconstruction phase
    std::vector<uint8_t> tree_dump(header.tree_size);
    fin.read(reinterpret_cast<char*>(tree_dump.data()), header.tree_size);

    Huffman_Tree h_tree;
    h_tree.reconstruct_from_dump(tree_dump.data(), header.tree_size);

    // Bit-by-bit traversal phase
    uint64_t decoded_symbols = 0;
    uint8_t byte_buffer = 0;
    int bit_count = 0;

    Node* current = h_tree.get_root();

    while (decoded_symbols < header.file_size) {
        bool bit = get_bit_from_file(byte_buffer, bit_count, fin);

        if (bit == 0) {
            current = current->get_left();
        }
        else {
            current = current->get_right();
        }

        if (current->is_Leaf()) {
            uint8_t symbol = current->get_symbol();
            fout.put(static_cast<char>(symbol));

            decoded_symbols++;
            current = h_tree.get_root();
        }
    }

    fin.close();
    fout.close();
}
void print_decoding_menu() {
    std::cout<<"-------The following commands are avaliable-------"<<'\n';

    std::cout<<"-h: Prints out a help message and exits the program afterwards."<<'\n';
    std::cout<<"----------------------------------------------------------------"<<'\n';
    std::cout<<"-i <infile>: Specifies the input file to decode using Huffman coding"<<'\n';
    std::cout<<"--------------------THE INFILE COMMAND IS MANDATORY---------------"<<'\n';
    std::cout<<"-o <outfile>: Specifies the output file to write the decompressed input to default = stdout." <<'\n';
    std::cout<<"----------------------------------------------------------------"<<'\n';
    std::cout<<"-s: Prints decompression statistics to stderr. These statistics include the compressed file size, the decompressed file size, and space saving."<<'\n';
}
/**
 * @brief Calculates and prints decompression statistics like space saving.
 */
void print_stats(uint64_t compressed_size, uint64_t decompressed_size) {
    double space_saving = 0;
    if (decompressed_size > 0) {
        space_saving = 100.0 * (1.0 - (static_cast<double>(compressed_size) / decompressed_size));
    }

    std::cerr << "\n------- Statistics (to stderr) -------" << std::endl;
    std::cerr << "Compressed file size:   " << compressed_size << " bytes" << std::endl;
    std::cerr << "Decompressed file size: " << decompressed_size << " bytes" << std::endl;
    std::cerr << "Space saving:           " << std::fixed << std::setprecision(2)
              << space_saving << "%" << std::endl;
    std::cerr << "--------------------------------------" << std::endl;
}
/**
 * @brief Main entry point for the decoder. Handles command line arguments.
 */
int main(int argc, char* argv[]) {
    std::string infile = "";
    std::string outfile = "stdout";
    bool show_stats = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-h") {
            print_decoding_menu();
            return 0;
        }
        if (arg == "-i") {
            if (i + 1 < argc) {
                infile = argv[++i];
            }
            else {
                std::cerr << "Error: -i requires a filename." << std::endl;
                return 1;
            }
        }
        else if (arg == "-o") {
                if (i + 1 < argc) {
                    outfile = argv[++i];
                }
                else {
                    std::cerr << "Error: -o requires a filename." << std::endl;
                    return 1;
            }
        }
        else if (arg == "-s") {
            show_stats = true;
        }
        else {
            std::cerr << "Unknown option: " << arg << std::endl;
            print_decoding_menu();
            return 1;
        }
    }

    if (infile.empty()) {
        std::cerr << "Error: Input file (-i) is required for decoding." << std::endl;
        return 1;
    }
    decode_file(infile, outfile);
    std::cout << "File decompressed successfully!\n";
    if (show_stats) {
        uint64_t c_size = std::filesystem::file_size(infile);
        uint64_t d_size = std::filesystem::file_size(outfile);
        print_stats(c_size, d_size);

    }
    return 0;
}