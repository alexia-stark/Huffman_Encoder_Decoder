cmake_minimum_required(VERSION 4.1)
project(Huffman_encoder_decoder)

set(CMAKE_CXX_STANDARD 20)

#ENCODER
add_executable(huff_enc
        Node.cpp
        Code.cpp
        Huffman_Tree.cpp
        encoding.cpp
)

# DECODER
add_executable(huff_dec
        Node.cpp
        Code.cpp
        Huffman_Tree.cpp
        decoding.cpp
)