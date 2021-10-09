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
    SWAP,
    JUMP,
    JUMP_IF,

    ADD,
    SUBTRACT,
    NEGATE,
    MULTIPLY,
    DIVIDE,
    MODULO,
    OR,
    AND,
    XOR,
    NOT,
    SMALLER,
    SMALLER_EQUAL,
    BIGGER,
    BIGGER_EQUAL,
    EQUAL,
    NOT_EQUAL,
    
    INT64_LOAD,
    BOOL_LOAD,
    STACK_PTR_LOAD,

    INT64_TO_BOOL,
    BOOL_TO_INT64,

    INT64_LOAD_FROM_ADDRESS,
    INT64_LOAD_INTO_ADDRESS,
    BOOL_LOAD_FROM_ADDRESS,
    BOOL_LOAD_INTO_ADDRESS,

    SIZE
};

const std::string disassemblyNames[InstructionType::SIZE] = {
    "PRINT", 
    "DUPLICATE",
    "SWAP",
    "JUMP",
    "JUMP_IF",

    "ADD",
    "SUBTRACT",
    "NEGATE",
    "MULTIPLY",
    "DIVIDE",
    "MODULO",
    "OR",
    "AND",
    "XOR",
    "NOT",
    "SMALLER",
    "SMALLER_EQUAL",
    "BIGGER",
    "BIGGER_EQUAL",
    "EQUAL",
    "NOT_EQUAL",
    
    "INT64_LOAD",
    "BOOL_LOAD",
    "STACK_PTR_LOAD",

    "INT64_TO_BOOL",
    "BOOL_TO_INT64",

    "INT64_LOAD_FROM_ADDRESS",
    "INT64_LOAD_INTO_ADDRESS",
    "BOOL_LOAD_FROM_ADDRESS",
    "BOOL_LOAD_INTO_ADDRESS",
};
class VirtualMachine {
public:
    class Code {
    public:
        /**
        * @brief Bytecode
        * 
        */
        std::vector<Byte> bytes;

        /**
         * @brief Table in which the VM looks up the jump locations
         * 
         */
        std::vector<size_t> lookupTable;

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
         * @brief Reset code in its initial state
         * 
         */
        void reset();

        /**
         * @brief Construct a new Code object
         * 
         * @param _bytes 
         */
        Code(const std::vector<Byte> &_bytes = {}, const std::vector<size_t> &_lookupTable = {});

        /**
         * @brief Destroy the Code object
         * 
         */
        ~Code();
    };

private:
    /**
     * @brief Stack memory - currently statically alocated memory used for storing variables
     * 
     */
    uint8_t *stack;

    /**
     * @brief Stack which emulates 2 register operations
     * 
     */
    std::vector<MemoryCell> byteStack;

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

    /**
     * @brief Virtual machine code
     * 
     */
     Code code;

public:
    /**
     * @brief Construct a new Virtual Machine object
     * 
     * @param code 
     */
    VirtualMachine(const Code &_code);

    /**
     * @brief Destroy the Virtual Machine object
     * 
     */
    ~VirtualMachine();

    /**
     * @brief Execute the VM
     * 
     */
    void execute();

    /**
     * @brief Disassemble the VM
     * 
     */
    void disassemble();
};

};

#endif //VIRTUAL_MACHINE_H