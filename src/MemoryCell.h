#pragma once
#ifndef VIRTUAL_MACHINE_OPERATIONS_H
#define VIRTUAL_MACHINE_OPERATIONS_H

#include <iostream>
#include <cassert>

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
    BOOL,
    INT64,
    RAW_PTR,
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
        bool BOOL;
        int64_t INT64;
        uint8_t *RAW_PTR;
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

void memoryCellPrint(const MemoryCell &cell);
MemoryCell int64MemoryCell(const int64_t val);
MemoryCell boolMemoryCell(const bool val);
MemoryCell rawptrMemoryCell(uint8_t *val);

MemoryCell memoryCellAdd(const MemoryCell &a, const MemoryCell &b);
MemoryCell memoryCellSubtract(const MemoryCell &a, const MemoryCell &b);
MemoryCell memoryCellMultiply(const MemoryCell &a, const MemoryCell &b);
MemoryCell memoryCellDivide(const MemoryCell &a, const MemoryCell &b);
MemoryCell memoryCellModulo(const MemoryCell &a, const MemoryCell &b);
MemoryCell memoryCellOr(const MemoryCell &a, const MemoryCell &b);
MemoryCell memoryCellAnd(const MemoryCell &a, const MemoryCell &b);
MemoryCell memoryCellXor(const MemoryCell &a, const MemoryCell &b);
MemoryCell memoryCellNot(const MemoryCell &a);
MemoryCell memoryCellSmaller(const MemoryCell &a, const MemoryCell &b);
MemoryCell memoryCellSmallerEqual(const MemoryCell &a, const MemoryCell &b);
MemoryCell memoryCellBigger(const MemoryCell &a, const MemoryCell &b);
MemoryCell memoryCellBiggerEqual(const MemoryCell &a, const MemoryCell &b);
MemoryCell memoryCellEqual(const MemoryCell &a, const MemoryCell &b);
MemoryCell memoryCellNotEqual(const MemoryCell &a, const MemoryCell &b);

void int64Print(const MemoryCell &a);
void boolPrint(const MemoryCell &a);
void rawptrPrint(const MemoryCell &a);

MemoryCell int64ToBool(const MemoryCell &a);
MemoryCell boolToint64(const MemoryCell &a);

};

#endif