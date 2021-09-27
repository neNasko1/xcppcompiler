#include <vector>
#include <iostream>

#include "VirtualMachine.h"

namespace VM {

template <typename... T> 
void VMErrorPrint(T... t) {
    (std::cerr << ... << t) << "\n";
}

template <typename... T> 
void VMError(T... t) {
    std::cerr << "There was an error while virtual machine was executing " << "\n";
    VMErrorPrint(t...);
    exit(0);
}

MemoryCell::MemoryCell() {}
MemoryCell::~MemoryCell() {}

MemoryCell MemoryCell::int64Add(const MemoryCell &a, const MemoryCell &b) {
    return int64MemoryCell(a.value.int64Val + b.value.int64Val);
}

MemoryCell MemoryCell::int64Subtract(const MemoryCell &a, const MemoryCell &b) {
    return int64MemoryCell(a.value.int64Val - b.value.int64Val);
}

MemoryCell MemoryCell::int64Multiply(const MemoryCell &a, const MemoryCell &b) {
    return int64MemoryCell(a.value.int64Val * b.value.int64Val);
}

MemoryCell MemoryCell::int64Divide(const MemoryCell &a, const MemoryCell &b) {
    return int64MemoryCell(a.value.int64Val / b.value.int64Val);
}

void MemoryCell::int64Print(const MemoryCell &a) {
    // Todo remove
    std::cout << a.value.int64Val << "\n";
}

MemoryCell MemoryCell::int64MemoryCell(const int64_t val) {
    MemoryCell cell;
    cell.type = INT64;
    cell.value.int64Val = val;
    return cell;
}


VirtualMachine::VirtualMachine(std::vector<Byte> &_code) : code(_code), nextByte(0) {}

VirtualMachine::~VirtualMachine() {}

Byte VirtualMachine::peek() {
    VMError("Not enough bytes in code");
    return this->code[this->nextByte];
}

Byte VirtualMachine::advance() {
    if(this->code.size() <= this->nextByte) {
        VMError("Not enough bytes in code");
    }
    return this->code[this->nextByte ++];
}

MemoryCell VirtualMachine::top() {
    if(this->stack.empty()) {
        VMError("Cannot access stack.top(), because it is empty");
    }
    return this->stack.back();
}

MemoryCell VirtualMachine::pop() {
    if(this->stack.empty()) {
        VMError("Cannot pop from stack, because it is empty");
    }
    MemoryCell value = this->stack.back();
    this->stack.pop_back();
    return value;
}

void VirtualMachine::push(const MemoryCell &cell) {
    this->stack.push_back(cell);
}

void VirtualMachine::execute() {
    while(this->nextByte < this->code.size()) {
        Byte next = this->advance();
        switch(next) {
        case InstructionType::PRINT:
            MemoryCell::int64Print(this->top());
            break;
        case InstructionType::INT64_LOAD:
            // Load next integer
            this->push(MemoryCell::int64MemoryCell((int64_t)this->advance()));
            break;
        case InstructionType::INT64_ADD: {
            auto a = this->pop(), b = this->pop();
            this->push(MemoryCell::int64Add(b, a));
            break;
        }
        case InstructionType::INT64_SUBTRACT: {
            auto a = this->pop(), b = this->pop();
            this->push(MemoryCell::int64Subtract(b, a));
            break;
        }
        case InstructionType::INT64_MULTIPLY: {
            auto a = this->pop(), b = this->pop();
            this->push(MemoryCell::int64Multiply(b, a));
            break;
        }
        case InstructionType::INT64_DIVIDE: {
            auto a = this->pop(), b = this->pop();
            this->push(MemoryCell::int64Divide(b, a));
            break;
        }
        default:
            VMError("Unsupported instruction type: ", next);
        }
    }
}

};
