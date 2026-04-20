
#include "Huffman_Tree.h"
#include <queue>
#include <fstream>
#include <stack>

Node* Huffman_Tree::get_root() {
    return this->root;
}
Code Huffman_Tree::get_code_for_character(char c) {
    return this->code_table[(unsigned char)c];
}
void Huffman_Tree::create_histogram(std::string  text, int histogram[]) {
    for (int i = 0; i < ALPHABET_SIZE; i++)
        histogram[i] = 0;
    histogram[0] = 1;
    histogram[255] = 1;
    int length = text.size();
    for (int i = 0 ; i < length; i++ ) {
        histogram[(unsigned char)text[i]] ++;
    }
}
int Huffman_Tree::number_unique_symbols(std::string text) {
    int unique_symbols = 0;
    int histogram[ALPHABET_SIZE];
    create_histogram(text, histogram);
    for (int i = 0; i < 256; i++)
        if (histogram[i] > 0) unique_symbols++;
    return unique_symbols;
}
struct CompareNodes {
    bool operator()(Node* a, Node* b) {
        return a->get_frequency() > b->get_frequency();
    }
};

Node& Huffman_Tree::create_huffman_tree(int histogram[ALPHABET_SIZE]) { // sets the root
    std::priority_queue<Node*, std::vector<Node*>, CompareNodes> tree;
    for (int i = 0; i < ALPHABET_SIZE; i++)
        if (histogram[i] > 0){
            Node* new_node = new Node(i, histogram[i]);
            tree.push(new_node);
        }
    while (tree.size() > 1) {
        Node* left_child = tree.top();
        tree.pop();
        Node* right_child = tree.top();
        tree.pop();

        Node* parent = create_parent(*left_child, *right_child);
        tree.push(parent);
    }
    this->root = tree.top();
    tree.pop();
    return *root;
}

void Huffman_Tree::generate_codes(Node* current_node, Code& c) {
    if (current_node == nullptr) {
        return;
    }
    if(current_node->is_Leaf()) {
        this->code_table[current_node->get_symbol()] = c;
        return;
    }
    //not a leaf

    // to the left
    c.push_bit(0);
    generate_codes(current_node->get_left(), c);
    c.pop_bit();

    //to the right
    c.push_bit(1);
    generate_codes(current_node->get_right(), c);
    c.pop_bit();
}

void Huffman_Tree::construct_code_table() {
    Code c;
    generate_codes(this->root, c);
}

Huffman_Tree::Huffman_Tree() : root(nullptr) {
    for (int i = 0; i < ALPHABET_SIZE; i++)
        code_table[i] = Code();
}

Huffman_Tree::Huffman_Tree(std::string text) {
    int histogram[ALPHABET_SIZE];
    create_histogram(text, histogram);
    create_huffman_tree(histogram);
    construct_code_table();
}

void Huffman_Tree::delete_everything(Node* current_node) {
    if (current_node == nullptr)
        return;

    delete_everything(current_node->get_left());
    delete_everything(current_node->get_right());
    delete current_node;
}

Huffman_Tree::~Huffman_Tree() {
    if (root != nullptr) {
        delete_everything(this->root);
    }
}


void Huffman_Tree::dump_tree_in_outfile(Node* current_node, std::ofstream& fout) {
    if (current_node == nullptr) {
        return;
    }

    dump_tree_in_outfile(current_node->get_left(), fout);
    dump_tree_in_outfile(current_node->get_right(), fout);

    if (current_node->is_Leaf()) {
        char marker = 'L';
        fout.write(&marker, sizeof(marker));

        uint8_t symbol = current_node->get_symbol();
        fout.write(reinterpret_cast<const char*>(&symbol), sizeof(symbol));
    }
    else {
        char marker = 'I';
        fout.write(&marker, sizeof(marker));
    }
}

void Huffman_Tree::reconstruct_from_dump(const uint8_t* tree_dump, uint16_t size) {

    std::stack<Node*> node_stack;
    for (uint16_t i = 0; i < size; ++i) {
        if (tree_dump[i] == 'L') {
            uint8_t symbol = tree_dump[++i];
            Node* leaf = new Node(symbol, 0);
            node_stack.push(leaf);
        }
        else if (tree_dump[i] == 'I') {
            Node* right = node_stack.top();
            node_stack.pop();

            Node* left = node_stack.top();
            node_stack.pop();

            Node* parent = create_parent(*left, *right);
            node_stack.push(parent);
        }
    }

    if (!node_stack.empty()) {
        this->root = node_stack.top();
    }
}