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

MemoryCell::MemoryCell() {}
MemoryCell::~MemoryCell() {}

void printMemoryCell(const MemoryCell &cell) {
    switch(cell.type) {
    case INT64: {
        int64Print(cell);
        break;
    }
    case BOOL: {
        boolPrint(cell);
        break;
    }
    default: {
        VMError("Unsupported type of cell to print: ", cell.type, "\n");
    }
    }
}

MemoryCell int64MemoryCell(const int64_t val) {
    MemoryCell cell;
    cell.type = INT64;
    cell.as.INT64 = val;
    return cell;
}

MemoryCell boolMemoryCell(const bool val) {
    MemoryCell cell;
    cell.type = BOOL;
    cell.as.BOOL = val;
    return cell;
}


MemoryCell int64Add(const MemoryCell &a, const MemoryCell &b) {
    return int64MemoryCell(a.as.INT64 + b.as.INT64);
}

MemoryCell int64Subtract(const MemoryCell &a, const MemoryCell &b) {
    return int64MemoryCell(a.as.INT64 - b.as.INT64);
}

MemoryCell int64Multiply(const MemoryCell &a, const MemoryCell &b) {
    return int64MemoryCell(a.as.INT64 * b.as.INT64);
}

MemoryCell int64Divide(const MemoryCell &a, const MemoryCell &b) {
    return int64MemoryCell(a.as.INT64 / b.as.INT64);
}

MemoryCell int64Modulo(const MemoryCell &a, const MemoryCell &b) {
    return int64MemoryCell(a.as.INT64 % b.as.INT64);
}

MemoryCell int64Or(const MemoryCell &a, const MemoryCell &b) {
    return int64MemoryCell(a.as.INT64 | b.as.INT64);
}

MemoryCell int64And(const MemoryCell &a, const MemoryCell &b) {
    return int64MemoryCell(a.as.INT64 & b.as.INT64);
}

MemoryCell int64Xor(const MemoryCell &a, const MemoryCell &b) {
    return int64MemoryCell(a.as.INT64 ^ b.as.INT64);
}

MemoryCell int64BitwiseNot(const MemoryCell &a) {
    return int64MemoryCell(~(a.as.INT64));
}  

MemoryCell int64Smaller(const MemoryCell &a, const MemoryCell &b) {
    return boolMemoryCell(a.as.INT64 < b.as.INT64);
}

MemoryCell int64SmallerEqual(const MemoryCell &a, const MemoryCell &b) {
    return boolMemoryCell(a.as.INT64 <= b.as.INT64);
}

MemoryCell int64Bigger(const MemoryCell &a, const MemoryCell &b) {
    return boolMemoryCell(a.as.INT64 > b.as.INT64);
}

MemoryCell int64BiggerEqual(const MemoryCell &a, const MemoryCell &b) {
    return boolMemoryCell(a.as.INT64 >= b.as.INT64);
}

void int64Print(const MemoryCell &a) {
    // Todo remove
    std::cout << a.as.INT64 << "\n";
}


MemoryCell boolOr(const MemoryCell &a, const MemoryCell &b) {
    return boolMemoryCell(a.as.BOOL | b.as.BOOL);
}

MemoryCell boolAnd(const MemoryCell &a, const MemoryCell &b) {
    return boolMemoryCell(a.as.BOOL & b.as.BOOL);
}

MemoryCell boolXor(const MemoryCell &a, const MemoryCell &b) {
    return boolMemoryCell(a.as.BOOL ^ b.as.BOOL);
}

MemoryCell boolNot(const MemoryCell &a) {
    return boolMemoryCell(!a.as.BOOL);
}

void boolPrint(const MemoryCell &a) {
    // Todo remove
    if(a.as.BOOL) {
        std::cout << "true" << std::endl;
    } else {
        std::cout << "false" << std::endl;
    }
}


MemoryCell int64ToBool(const MemoryCell &a) {
    return boolMemoryCell(bool(a.as.INT64));
}

MemoryCell boolToint64(const MemoryCell &a) {
    return int64MemoryCell(int64_t(a.as.BOOL));
}


};