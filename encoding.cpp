/**
* @file encoding.cpp
 * @brief Implementation of the Huffman compression process.
 */
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>
#include "Huffman_Tree.h"
#include <iomanip>

#define MAGIC 0xDEADEAEF
/**
 * @struct Header
 * @brief Structure of the header saved at the beginning of the compressed file.
* * The header ensures the decoder can verify the file format and reconstruct the tree correctly.
 */
struct Header {
    uint32_t magic;
    uint16_t tree_size;
    uint64_t file_size;
};
/**
* @brief Constructs and writes the binary header to the output file.
 * * This step is crucial for "Lossless" compression, as it provides the decoder
 * with the necessary metadata to perfectly reconstruct the original data.
 * @param input_filename Name of the original file (to calculate size).
 * @param nr_unique_symbols Used to calculate the tree dump size.
 * @param fout The output file stream.
 */
void write_Header(const std::string& input_filename, const std::string& output_filename, int nr_unique_symbols, std::ofstream& fout) {
    if (!fout.is_open()) {
        std::cerr << "Error: Output file is not open in write_Header!\n";
        return;
    }
    Header my_header;
    my_header.magic = MAGIC;
    my_header.tree_size = (3 * nr_unique_symbols) - 1;
    my_header.file_size = std::filesystem::file_size(input_filename);

    fout.write(reinterpret_cast<const char*>(&my_header), sizeof(my_header));
}
/**
 * @brief Packs individual bits into a byte buffer before writing to disk.
 * * Since hardware cannot write single bits, this function uses bitwise OR and Left Shifts
 * to fill a uint8_t buffer. Once 8 bits are collected, the byte is flushed to the file.
 * * @param bit The bit to add (0 or 1).
 * @param byte_buffer Reference to the 8-bit storage.
 * @param bit_count Tracks how many bits are currently in the buffer.
 */
void add_bit_in_file(uint8_t bit, uint8_t& byte_buffer, int& bit_count, std::ofstream& fout) {
    if (bit == 1) {
        byte_buffer |= (1 << (7 - bit_count));
    }
    bit_count++;

    if (bit_count == 8) {
        fout.write(reinterpret_cast<const char*>(&byte_buffer), sizeof(byte_buffer));
        byte_buffer = 0;
        bit_count = 0;
    }
}
/**
 * @brief Orchestrates the multi-step encoding process.
 * * Algorithm:
 * 1. Write the Header
 * 2. Perform Post-order traversal to dump the Huffman Tree ('L' for leaves, 'I' for internal)
 * 3. Read the input file again and write the variable-length codes for each symbol.
 * 4. Flush the final byte if it contains remaining bits[cite: 167].
 */
void encode_file(const std::string& input_filename, const std::string& output_filename, Huffman_Tree& h_tree, int unique_symbols) {
    

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

    write_Header(input_filename, output_filename, unique_symbols, fout);

    h_tree.dump_tree_in_outfile(h_tree.get_root(), fout);

    uint8_t byte_buffer = 0;
    int bit_count = 0;
    char c;

    while (fin.get(c)) {

        Code char_code = h_tree.get_code_for_character(c);

        for (int i = 0; i < char_code.get_size(); i++) {

            uint8_t current_bit = char_code.get_bit_at(i);
            add_bit_in_file(current_bit, byte_buffer, bit_count, fout);
        }
    }

    if (bit_count > 0) {
        fout.write(reinterpret_cast<const char*>(&byte_buffer), sizeof(byte_buffer));
    }

    fin.close();
    fout.close();
}

void print_encoding_menu() {
    std::cout<<"-------The following commands are avaliable-------"<<'\n';
    std::cout<<"-h: Prints out a help message and exits the program afterwards."<<'\n';
    std::cout<<"----------------------------------------------------------------"<<'\n';
    std::cout<<"-i infile: Specifies the input file to encode using Huffman coding. default = stdin"<<'\n';
    std::cout<<"----------------------------------------------------------------"<<'\n';
    std::cout<<"-o outfile: Specifies the output file to write the compressed input to" <<'\n';
    std::cout<<"--------------------THE OUTFILE COMMAND IS MANDATORY---------------"<<'\n';
    std::cout<<"-s: Prints compression statistics. These statistics include the uncompressed file size, the compressed file size, and space saving."<<'\n';
}
/**
 * @brief Calculates and prints compression statistics like space saving.
 */
void print_stats(uint64_t compressed_size,uint64_t uncompressed_size) {

    double space_saving = 0;
    if (uncompressed_size > 0) {
        space_saving = 100.0 * (1.0 - (static_cast<double>(compressed_size) / uncompressed_size));
    }

    std::cout << "\n------- Compression Statistics -------" << std::endl;
    std::cout << "Uncompressed file size: " << uncompressed_size << " bytes" << std::endl;
    std::cout << "Compressed file size:   " << compressed_size << " bytes" << std::endl;
    std::cout << "Space saving:           " << std::fixed << std::setprecision(2) << space_saving << "%" << std::endl;
    std::cout << "--------------------------------------" << std::endl;

}
/**
 * @brief Main entry point for the encoder. Handles command line arguments.
 */
int main(int argc, char* argv[]) {

    std::string infile = "";
    std::string outfile = "";
    bool show_stats = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h") {
            print_encoding_menu();
            return 0;
        } else if (arg == "-i" && i + 1 < argc) {
            infile = argv[++i];
        } else if (arg == "-o" && i + 1 < argc) {
            outfile = argv[++i];
        } else if (arg == "-s") {
            show_stats = true;
        }
    }

    if (outfile.empty() || infile.empty()) {
        std::cerr << "Error: Both -i and -o are required!" << std::endl;
        return 1;
    }

    std::ifstream temp_in(infile, std::ios::binary);
    if (!temp_in.is_open()) {
        std::cerr << "Error: Could not open input file: " << infile << std::endl;
        return 1;
    }

    std::string text((std::istreambuf_iterator<char>(temp_in)), std::istreambuf_iterator<char>());
    temp_in.close();

    Huffman_Tree my_tree(text);
    int unique_symbols = my_tree.number_unique_symbols(text);
    encode_file(infile, outfile, my_tree, unique_symbols);
    std::cout << "File compressed successfully!\n";

    if (show_stats) {
        try {
            uint64_t uncompressed_size = std::filesystem::file_size(infile);
            uint64_t compressed_size = std::filesystem::file_size(outfile);
            print_stats(compressed_size, uncompressed_size);
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Could not calculate stats: " << e.what() << std::endl;
        }
    }

    return 0;
}