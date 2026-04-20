
#include "Code.h"

Code::Code() {
    top = 0;
    for (int i = 0;i < MAX_CODE_SIZE; i++)
        bits[i] = 0;
}
uint8_t Code::get_bit_at(int index) {
    return this->bits[index];
}
int Code::get_size() {
    return this->top;
}

bool Code::is_Empty() {
    return this->top == 0;
}

bool Code::is_Full() {
    return this->top == MAX_CODE_SIZE;
}

bool Code::push_bit(uint8_t bit) {
    if (this->is_Full())
        return 0;
    this->bits[this->top] = bit;
    this->top++;
    return 1;
}

int Code::pop_bit() {
    if (this->is_Empty())
        return -1;
    this->top--;
    uint8_t bit = this->bits[this->top];
    return bit;
}

Code& Code::operator=(const Code& other) {
    this->top = other.top;
    for (int i = 0; i < this->top; i++)
            this->bits[i] = other.bits[i];
    return *this;
}