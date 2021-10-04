#include <vector>
#include <iostream>

#include "VirtualMachine.h"
#include "MemoryCell.h"

namespace VM {

template <typename... T> 
static void VMErrorPrint(T... t) {
    (std::cerr << ... << t) << "\n";
}

template <typename... T> 
void VMError(T... t) {
    std::cerr << "There was an error while virtual machine was executing " << "\n";
    VMErrorPrint(t...);
    exit(0);
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
        case InstructionType::DUPLICATE: {
            auto a = this->pop();
            this->push(a);
            this->push(a);
            break;
        }
        case InstructionType::PRINT: {
            printMemoryCell(this->top());
            break;
        }

        case InstructionType::INT64_LOAD: {
            // Load next integer
            this->push(int64MemoryCell((int64_t)this->advance()));
            break;
        }
        case InstructionType::INT64_ADD: {
            auto a = this->pop(), b = this->pop();
            this->push(int64Add(b, a));
            break;
        }
        case InstructionType::INT64_SUBTRACT: {
            auto a = this->pop(), b = this->pop();
            this->push(int64Subtract(b, a));
            break;
        }
        case InstructionType::INT64_MULTIPLY: {
            auto a = this->pop(), b = this->pop();
            this->push(int64Multiply(b, a));
            break;
        }
        case InstructionType::INT64_DIVIDE: {
            auto a = this->pop(), b = this->pop();
            this->push(int64Divide(b, a));
            break;
        }
        case InstructionType::INT64_MODULO: {
            auto a = this->pop(), b = this->pop();
            this->push(int64Modulo(b, a));
            break;
        }
        case InstructionType::INT64_OR: {
            auto a = this->pop(), b = this->pop();
            this->push(int64Or(b, a));
            break;
        }
        case InstructionType::INT64_AND: {
            auto a = this->pop(), b = this->pop();
            this->push(int64And(b, a));
            break;
        }
        case InstructionType::INT64_XOR: {
            auto a = this->pop(), b = this->pop();
            this->push(int64Xor(b, a));
            break;
        }
        case InstructionType::INT64_NOT: {
            auto a = this->pop();
            this->push(int64BitwiseNot(a));
            break;
        }
        case InstructionType::INT64_SMALLER: {
            auto a = this->pop(), b = this->pop();
            this->push(int64Smaller(b, a));
            break;
        }
        case InstructionType::INT64_SMALLER_EQUAL: {
            auto a = this->pop(), b = this->pop();
            this->push(int64SmallerEqual(b, a));
            break;
        }
        case InstructionType::INT64_BIGGER: {
            auto a = this->pop(), b = this->pop();
            this->push(int64Bigger(b, a));
            break;
        }
        case InstructionType::INT64_BIGGER_EQUAL: {
            auto a = this->pop(), b = this->pop();
            this->push(int64BiggerEqual(b, a));
            break;
        }

        case InstructionType::BOOL_LOAD: {
            this->push(boolMemoryCell((bool)this->advance()));
            break;
        }
        case InstructionType::BOOL_OR: {
            auto a = this->pop(), b = this->pop();
            this->push(boolOr(b, a));
            break;
        }
        case InstructionType::BOOL_AND: {
            auto a = this->pop(), b = this->pop();
            this->push(boolAnd(b, a));
            break;
        }
        case InstructionType::BOOL_XOR: {
            auto a = this->pop(), b = this->pop();
            this->push(boolXor(b, a));
            break;
        }
        case InstructionType::BOOL_NOT: {
            auto a = this->pop();
            this->push(boolNot(a));
            break;
        }

        case InstructionType::BOOL_TO_INT64: {
            auto a = this->pop();
            this->push(boolToint64(a));
            break;
        }
        case InstructionType::INT64_TO_BOOL: {
            auto a = this->pop();
            this->push(int64ToBool(a));
            break;
        }
        default:
            VMError("Unsupported instruction type: ", next);
        }
    }
}

};
