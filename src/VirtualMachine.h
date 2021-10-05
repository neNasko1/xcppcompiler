#pragma once
#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

#include <vector>

#include "MemoryCell.h"

namespace VM {

/**
 * @brief Print the parameters given and exit
 * 
 * @param T 
 */
template <typename... T> 
void VMError(T... t);

typedef int64_t Byte;

/**
 * @brief Types of instructions
 * 
 */
enum InstructionType : Byte {
    PRINT, 
    DUPLICATE,

    INT64_LOAD, // parameters: value
    INT64_ADD,
    INT64_SUBTRACT,
    INT64_NEGATE,
    INT64_MULTIPLY,
    INT64_DIVIDE,
    INT64_MODULO,
    INT64_OR,
    INT64_AND,
    INT64_XOR,
    INT64_NOT,
    INT64_SMALLER,
    INT64_SMALLER_EQUAL,
    INT64_BIGGER,
    INT64_BIGGER_EQUAL,
    INT64_EQUAL,
    INT64_NOT_EQUAL,

    BOOL_LOAD,
    BOOL_OR,
    BOOL_AND,
    BOOL_XOR,
    BOOL_NOT,
    BOOL_EQUAL,
    BOOL_NOT_EQUAL,

    INT64_TO_BOOL,
    BOOL_TO_INT64,

    INT64_LOAD_FROM_STACK,
    INT64_LOAD_INTO_STACK,
    BOOL_LOAD_FROM_STACK,
    BOOL_LOAD_INTO_STACK,

    SIZE
};

class VirtualMachine {
private:
    /**
     * @brief Stack memory - currently statically alocated memory used for storing variables
     * 
     */
    uint8_t *stack;

    /**
     * @brief Stack which emulates 3 register operations
     * 
     */
    std::vector<MemoryCell> byteStack;

    /**
     * @brief Bytecode
     * 
     */
    std::vector<Byte> code;

    /**
     * @brief Next byte
     * 
     */
    uint64_t nextByte;

    /**
     * @brief Peek at next byte in code
     * 
     * @return Byte 
    */
    Byte peek();

    /**
     * @brief Get next byte and advance
     * 
     * @return Byte 
     */
    Byte advance();

    /**
     * @return Byte top of stack
     */
    MemoryCell top();

    /**
     * @brief Pop top of stack
     * 
     * @return Byte top of stack
     */
    MemoryCell pop();

    /**
     * @brief Push to top of stack
     * 
     * @param cell Value to push
     * @return Byte 
     */
    void push(const MemoryCell &cell);

public:
    /**
        * @brief Construct a new Virtual Machine object
        * 
        * @param code 
        */
    VirtualMachine(std::vector<Byte> &_code);

    /**
        * @brief Destroy the Virtual Machine object
        * 
        */
    ~VirtualMachine();

    /**
        * @brief Execute bytecode
        * 
        */
    void execute();
};

};

#endif //VIRTUAL_MACHINE_H