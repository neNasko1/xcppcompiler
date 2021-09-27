#pragma once
#ifndef VIRTUAL_MACHINE_H
#define VIRTUAL_MACHINE_H

#include <vector>

namespace VM {

/**
 * @brief Print the parameters given and exit
 * 
 * @param T 
 */
template <typename... T> 
void VMError(T... t);

typedef int64_t Byte;

// Todo: maybe move this to a different class
enum VariableType {
    INT64,
};

class MemoryCell {
private:
    /**
     * @brief Type of value this cell is holding
     * 
     */
    VariableType type;

    /**
     * @brief Value
     * 
     */
    union {
        int64_t int64Val;
    } value;

public: 

    /**
     * @brief Construct a new Memory Cell object
     * 
     */
    MemoryCell();

    /**
     * @brief Destroy the Memory Cell object
     * 
     */
    ~MemoryCell();

    static MemoryCell int64Add(const MemoryCell &a, const MemoryCell &b);
    static MemoryCell int64Subtract(const MemoryCell &a, const MemoryCell &b);
    static MemoryCell int64Multiply(const MemoryCell &a, const MemoryCell &b);
    static MemoryCell int64Divide(const MemoryCell &a, const MemoryCell &b);  
    static void int64Print(const MemoryCell &a);
    static MemoryCell int64MemoryCell(const int64_t val);
};

/**
 * @brief Types of instructions
 * 
 */
enum InstructionType : Byte {
    PRINT, 

    INT64_LOAD, // parameters: value
    INT64_ADD,
    INT64_SUBTRACT,
    INT64_MULTIPLY,
    INT64_DIVIDE,

    SIZE
};

class VirtualMachine {
private:
    /**
        * @brief Memory stack
        * 
        */
    std::vector<MemoryCell> stack;

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