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

VirtualMachine::Code::Code(const std::vector<Byte> &_bytes, const std::vector<size_t> &_lookupTable) 
    : bytes(_bytes), lookupTable(_lookupTable), nextByte(0) {}

VirtualMachine::Code::~Code() {}

Byte VirtualMachine::Code::peek() {
    if(this->bytes.size() <= this->nextByte) {
        VMError("Not enough bytes in code to advance");
    }
    return this->bytes[this->nextByte];
}

Byte VirtualMachine::Code::advance() {
    if(this->bytes.size() <= this->nextByte) {
        VMError("Not enough bytes in code to advance");
    }
    return this->bytes[this->nextByte ++];
}

void VirtualMachine::Code::reset() {
    this->nextByte = 0;
}


VirtualMachine::VirtualMachine(const Code &_code) {
    this->code = _code;

    // TODO: parse this from command line
    const uint64_t STACK_SIZE = 1 << 16;
    this->stack = (uint8_t*)malloc(STACK_SIZE);    
}

VirtualMachine::~VirtualMachine() {
    free(this->stack);
}

MemoryCell VirtualMachine::top() {
    if(this->byteStack.empty()) {
        VMError("Cannot access byteStack.top(), because it is empty");
    }
    return this->byteStack.back();
}

MemoryCell VirtualMachine::pop() {
    if(this->byteStack.empty()) {
        VMError("Cannot pop from byteStack, because it is empty");
    }
    MemoryCell value = this->byteStack.back();
    this->byteStack.pop_back();
    return value;
}

void VirtualMachine::push(const MemoryCell &cell) {
    this->byteStack.push_back(cell);
}

void VirtualMachine::execute() {
    this->code.reset();

    while(this->code.nextByte < this->code.bytes.size()) {
        Byte next = this->code.advance();

        switch(next) {
        case InstructionType::DUPLICATE: {
            auto a = this->pop();
            this->push(a);
            this->push(a);
            break;
        }
        case InstructionType::PRINT: {
            memoryCellPrint(this->top());
            break;
        }
        case InstructionType::SWAP: {
            auto a = this->pop();
            auto b = this->pop();
            this->push(a);
            this->push(b);
            break;
        }
        case InstructionType::JUMP: {
            auto location = this->code.advance();
            this->code.nextByte = this->code.lookupTable[location];
            break;
        }
        case InstructionType::JUMP_IF: {
            auto location = this->code.advance();
            auto cond = this->pop(); // This has to be true;
            if(cond.as.BOOL) {
                this->code.nextByte = this->code.lookupTable[location];            
            }
            break;
        }

        case InstructionType::ADD: {
            auto a = this->pop(), b = this->pop();
            this->push(memoryCellAdd(b, a));
            break;
        }
        case InstructionType::SUBTRACT: {
            auto a = this->pop(), b = this->pop();
            this->push(memoryCellSubtract(b, a));
            break;
        }
        case InstructionType::NEGATE: {
            auto a = this->pop(), b = int64MemoryCell(0);
            this->push(memoryCellSubtract(b, a));
            break;
        }
        case InstructionType::MULTIPLY: {
            auto a = this->pop(), b = this->pop();
            this->push(memoryCellMultiply(b, a));
            break;
        }
        case InstructionType::DIVIDE: {
            auto a = this->pop(), b = this->pop();
            this->push(memoryCellDivide(b, a));
            break;
        }
        case InstructionType::MODULO: {
            auto a = this->pop(), b = this->pop();
            this->push(memoryCellModulo(b, a));
            break;
        }
        case InstructionType::OR: {
            auto a = this->pop(), b = this->pop();
            this->push(memoryCellOr(b, a));
            break;
        }
        case InstructionType::AND: {
            auto a = this->pop(), b = this->pop();
            this->push(memoryCellAnd(b, a));
            break;
        }
        case InstructionType::XOR: {
            auto a = this->pop(), b = this->pop();
            this->push(memoryCellXor(b, a));
            break;
        }
        case InstructionType::NOT: {
            auto a = this->pop();
            this->push(memoryCellNot(a));
            break;
        }
        case InstructionType::SMALLER: {
            auto a = this->pop(), b = this->pop();
            this->push(memoryCellSmaller(b, a));
            break;
        }
        case InstructionType::SMALLER_EQUAL: {
            auto a = this->pop(), b = this->pop();
            this->push(memoryCellSmallerEqual(b, a));
            break;
        }
        case InstructionType::BIGGER: {
            auto a = this->pop(), b = this->pop();
            this->push(memoryCellBigger(b, a));
            break;
        }
        case InstructionType::BIGGER_EQUAL: {
            auto a = this->pop(), b = this->pop();
            this->push(memoryCellBiggerEqual(b, a));
            break;
        }
        case InstructionType::EQUAL: {
            auto a = this->pop(), b = this->pop();
            this->push(memoryCellEqual(b, a));
            break;
        }
        case InstructionType::NOT_EQUAL: {
            auto a = this->pop(), b = this->pop();
            this->push(memoryCellNotEqual(b, a));
            break;
        }

        case InstructionType::STACK_PTR_LOAD: { // This needs offset 
            auto offset = this->pop();
            this->push(rawptrMemoryCell(this->stack + offset.as.INT64));
            break;
        }   
        case InstructionType::INT64_LOAD: {
            // Load next integer
            this->push(int64MemoryCell((int64_t)this->code.advance()));
            break;
        }
        case InstructionType::BOOL_LOAD: {
            this->push(boolMemoryCell((bool)this->code.advance()));
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

        case InstructionType::INT64_LOAD_FROM_ADDRESS: { // Requires address to get from
            auto address = this->pop();
            uint64_t val = *(int64_t*)(address.as.RAW_PTR);
            this->push(int64MemoryCell(val));
            break;
        }
        case InstructionType::BOOL_LOAD_FROM_ADDRESS: { // Requires address to get from
            auto address = this->pop();
            bool val = *(bool*)(address.as.RAW_PTR);
            this->push(boolMemoryCell(val));
            break;
        }
        case InstructionType::INT64_LOAD_INTO_ADDRESS: { // Requires address to store into
            auto address = this->pop();
            auto a = this->pop();
            *(int64_t*)(address.as.RAW_PTR) = a.as.INT64;
            break;
        }
        case InstructionType::BOOL_LOAD_INTO_ADDRESS: { // Requires address to store into
            auto address = this->pop();
            auto a = this->pop();
            *(bool*)(address.as.RAW_PTR) = a.as.BOOL;
            break;
        }
        default:
            VMError("Unsupported instruction type: ", next);
        }
    }
}

void VirtualMachine::disassemble() {
    this->code.reset();

    while(this->code.nextByte < this->code.bytes.size()) {
        Byte next = this->code.advance();
        std::cout << this->code.nextByte - 1 << ": " << disassemblyNames[next] << " ";

        switch(next) {
        case InstructionType::JUMP: {
            auto location = this->code.advance();
            std::cout << this->code.lookupTable[location];
            break;
        }
        case InstructionType::JUMP_IF: {
            auto location = this->code.advance();
            std::cout << this->code.lookupTable[location];
            break;
        }

        case InstructionType::INT64_LOAD: {
            // Load next integer
            std::cout << (int64_t)this->code.advance();
            break;
        }
        case InstructionType::BOOL_LOAD: {
            std::cout << (bool)this->code.advance();
            break;
        }
        }
        std::cout << "\n";
    }
}

};
