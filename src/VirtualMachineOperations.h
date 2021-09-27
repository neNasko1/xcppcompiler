#pragma once
#ifndef VIRTUAL_MACHINE_OPERATIONS_H
#define VIRTUAL_MACHINE_OPERATIONS_H

#include <iostream>

#include "VirtualMachine.h"

namespace VM {

enum VariableType {
    INT64,
};

class MemoryCell {
private:
public: 
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
    // TODO: make this public

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
};

MemoryCell int64Add(const MemoryCell &a, const MemoryCell &b);
MemoryCell int64Subtract(const MemoryCell &a, const MemoryCell &b);
MemoryCell int64Multiply(const MemoryCell &a, const MemoryCell &b);
MemoryCell int64Divide(const MemoryCell &a, const MemoryCell &b);  
MemoryCell int64Modulo(const MemoryCell &a, const MemoryCell &b);  
MemoryCell int64Or(const MemoryCell &a, const MemoryCell &b);  
MemoryCell int64And(const MemoryCell &a, const MemoryCell &b);  
MemoryCell int64Xor(const MemoryCell &a, const MemoryCell &b);  
MemoryCell int64BitwiseNot(const MemoryCell &a);  
void int64Print(const MemoryCell &a);
MemoryCell int64MemoryCell(const int64_t val);

};

#endif