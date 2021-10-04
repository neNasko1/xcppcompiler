#pragma once
#ifndef VIRTUAL_MACHINE_OPERATIONS_H
#define VIRTUAL_MACHINE_OPERATIONS_H

#include <iostream>

#include "VirtualMachine.h"

namespace VM {

/**
 * @brief Print the parameters given and exit
 * 
 * @param T 
 */
template <typename... T> 
void VMError(T... t);

enum VariableType {
    INT64,
    BOOL
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
        int64_t INT64;
        bool BOOL;
    } as;
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

void printMemoryCell(const MemoryCell &cell);
MemoryCell int64MemoryCell(const int64_t val);
MemoryCell boolMemoryCell(const bool val);

MemoryCell int64Add(const MemoryCell &a, const MemoryCell &b);
MemoryCell int64Subtract(const MemoryCell &a, const MemoryCell &b);
MemoryCell int64Multiply(const MemoryCell &a, const MemoryCell &b);
MemoryCell int64Divide(const MemoryCell &a, const MemoryCell &b);  
MemoryCell int64Modulo(const MemoryCell &a, const MemoryCell &b);  
MemoryCell int64Or(const MemoryCell &a, const MemoryCell &b);  
MemoryCell int64And(const MemoryCell &a, const MemoryCell &b);  
MemoryCell int64Xor(const MemoryCell &a, const MemoryCell &b);  
MemoryCell int64BitwiseNot(const MemoryCell &a);  
MemoryCell int64Smaller(const MemoryCell &a, const MemoryCell &b);
MemoryCell int64SmallerEqual(const MemoryCell &a, const MemoryCell &b);
MemoryCell int64Bigger(const MemoryCell &a, const MemoryCell &b);
MemoryCell int64BiggerEqual(const MemoryCell &a, const MemoryCell &b);
void int64Print(const MemoryCell &a);

MemoryCell boolOr(const MemoryCell &a, const MemoryCell &b);
MemoryCell boolAnd(const MemoryCell &a, const MemoryCell &b);
MemoryCell boolXor(const MemoryCell &a, const MemoryCell &b);
MemoryCell boolNot(const MemoryCell &a);
void boolPrint(const MemoryCell &a);

MemoryCell int64ToBool(const MemoryCell &a);
MemoryCell boolToint64(const MemoryCell &a);

};

#endif