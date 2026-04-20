Huffman Encoder and Decoder

This project implements a system for lossless data compression using the Huffman coding algorithm. Lossless compression ensures that the original data can be perfectly reconstructed from its compressed format.

1. Huffman Encoder

The encoder reads an input file, constructs a histogram of character frequencies, builds a Huffman tree, and writes the compressed data to an output file.

Command-Line Options:

    -h: Prints a help message describing the program and options.

    -i infile: Specifies the input file to encode (defaults to stdin).

    -o outfile: Specifies the output file for the compressed data.

    -s: Prints compression statistics, including original size, compressed size, and space saving.

Space Saving Formula:

    100×(1−compressed size/uncompressed size)
2. Huffman Decoder

The decoder takes a file compressed by the encoder and expands it back to its original uncompressed format.

Command-Line Options:

    -h: Prints a help message.

    -i infile: Specifies the compressed input file to decode.

    -o outfile: Specifies the output file for decompressed data (defaults to stdout).

    -s: Prints decompression statistics to stderr.

Technical Implementation
File Header

Each compressed file begins with a header to identify it and provide the decoder with necessary metadata:

Signature: A magic number (0xDEADEAEF) used to verify the file type.

Tree Size: The number of bytes representing the Huffman tree dump.

File Size: The size of the original uncompressed file in bytes.

Tree Representation

The Huffman tree is stored in the compressed file using a post-order traversal:

Leaf Nodes: Represented by the character 'L' followed by the 1-byte symbol.

Interior Nodes: Represented by the character 'T'.