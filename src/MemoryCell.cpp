#include "VirtualMachine.h"
#include "MemoryCell.h"
#include "GrammarTypeChecking.h"

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

MemoryCell rawptrMemoryCell(uint8_t *val) {
    MemoryCell cell;
    cell.type = RAW_PTR;
    cell.as.RAW_PTR = val;
    return cell;
}

MemoryCell memoryCellAdd(const MemoryCell &a, const MemoryCell &b) {
    assert(a.type == b.type); // This is always true

    switch(a.type) {
    case VariableType::INT64: return int64MemoryCell(a.as.INT64 + b.as.INT64); break;
    }

    VMError("Operation addition is not defined for type ", Grammar::Type::globalTypes[a.type].name);
    return MemoryCell();
}

MemoryCell memoryCellSubtract(const MemoryCell &a, const MemoryCell &b) {
    assert(a.type == b.type); // This is always true

    switch(a.type) {
    case VariableType::INT64: return int64MemoryCell(a.as.INT64 - b.as.INT64); break;
    }

    VMError("Operation subtraction is not defined for type ", Grammar::Type::globalTypes[a.type].name);
    return MemoryCell();
}

MemoryCell memoryCellMultiply(const MemoryCell &a, const MemoryCell &b) {
    assert(a.type == b.type); // This is always true

    switch(a.type) {
    case VariableType::INT64: return int64MemoryCell(a.as.INT64 * b.as.INT64); break;
    }

    VMError("Operation multiplication is not defined for type ", Grammar::Type::globalTypes[a.type].name);
    return MemoryCell();
}

MemoryCell memoryCellDivide(const MemoryCell &a, const MemoryCell &b) {
    assert(a.type == b.type); // This is always true

    switch(a.type) {
    case VariableType::INT64: return int64MemoryCell(a.as.INT64 / b.as.INT64); break;
    }

    VMError("Operation division is not defined for type ", Grammar::Type::globalTypes[a.type].name);
    return MemoryCell();
}

MemoryCell memoryCellModulo(const MemoryCell &a, const MemoryCell &b) {
    assert(a.type == b.type); // This is always true

    switch(a.type) {
    case VariableType::INT64: return int64MemoryCell(a.as.INT64 % b.as.INT64); break;
    }

    VMError("Operation modulo is not defined for type ", Grammar::Type::globalTypes[a.type].name);
    return MemoryCell();
}

MemoryCell memoryCellOr(const MemoryCell &a, const MemoryCell &b) {
    assert(a.type == b.type); // This is always true

    switch(a.type) {
    case VariableType::INT64: return int64MemoryCell(a.as.INT64 | b.as.INT64); break;
    case VariableType::BOOL : return  boolMemoryCell(a.as.BOOL  | b.as.BOOL); break;
    }

    VMError("Operation or is not defined for type ", Grammar::Type::globalTypes[a.type].name);
    return MemoryCell();
}

MemoryCell memoryCellAnd(const MemoryCell &a, const MemoryCell &b) {
    assert(a.type == b.type); // This is always true

    switch(a.type) {
    case VariableType::INT64: return int64MemoryCell(a.as.INT64 & b.as.INT64); break;
    case VariableType::BOOL : return  boolMemoryCell(a.as.BOOL  & b.as.BOOL); break;
    }

    VMError("Operation and is not defined for type ", Grammar::Type::globalTypes[a.type].name);
    return MemoryCell();
}

MemoryCell memoryCellXor(const MemoryCell &a, const MemoryCell &b) {
    assert(a.type == b.type); // This is always true

    switch(a.type) {
    case VariableType::INT64: return int64MemoryCell(a.as.INT64 ^ b.as.INT64); break;
    case VariableType::BOOL : return  boolMemoryCell(a.as.BOOL  ^ b.as.BOOL); break;
    }

    VMError("Operation xor is not defined for type ", Grammar::Type::globalTypes[a.type].name);
    return MemoryCell();
}

MemoryCell memoryCellNot(const MemoryCell &a) {

    switch(a.type) {
    case VariableType::INT64: return int64MemoryCell(~a.as.INT64); break;
    case VariableType::BOOL : return  boolMemoryCell(!a.as.BOOL); break;
    }

    VMError("Operation modulo is not defined for type ", Grammar::Type::globalTypes[a.type].name);
    return MemoryCell();
}

MemoryCell memoryCellSmaller(const MemoryCell &a, const MemoryCell &b) {
    assert(a.type == b.type); // This is always true

    switch(a.type) {
    case VariableType::INT64: return boolMemoryCell(a.as.INT64 < b.as.INT64); break;
    }

    VMError("Operation smaller is not defined for type ", Grammar::Type::globalTypes[a.type].name);
    return MemoryCell();
}

MemoryCell memoryCellSmallerEqual(const MemoryCell &a, const MemoryCell &b) {
    assert(a.type == b.type); // This is always true

    switch(a.type) {
    case VariableType::INT64: return boolMemoryCell(a.as.INT64 <= b.as.INT64); break;
    }

    VMError("Operation smaller or equal is not defined for type ", Grammar::Type::globalTypes[a.type].name);
    return MemoryCell();
}

MemoryCell memoryCellBigger(const MemoryCell &a, const MemoryCell &b) {
    assert(a.type == b.type); // This is always true

    switch(a.type) {
    case VariableType::INT64: return boolMemoryCell(a.as.INT64 > b.as.INT64); break;
    }

    VMError("Operation bigger is not defined for type ", Grammar::Type::globalTypes[a.type].name);
    return MemoryCell();
}

MemoryCell memoryCellBiggerEqual(const MemoryCell &a, const MemoryCell &b) {
    assert(a.type == b.type); // This is always true

    switch(a.type) {
    case VariableType::INT64: return boolMemoryCell(a.as.INT64 >= b.as.INT64); break;
    }

    VMError("Operation bigger or equal is not defined for type ", Grammar::Type::globalTypes[a.type].name);
    return MemoryCell();
}

MemoryCell memoryCellEqual(const MemoryCell &a, const MemoryCell &b) {
    assert(a.type == b.type); // This is always true

    switch(a.type) {
    case VariableType::INT64: return boolMemoryCell(a.as.INT64 == b.as.INT64); break;
    case VariableType::BOOL : return boolMemoryCell(a.as.BOOL  == b.as.BOOL); break;
    }

    VMError("Operation xor is not defined for type ", Grammar::Type::globalTypes[a.type].name);
    return MemoryCell();
}

MemoryCell memoryCellNotEqual(const MemoryCell &a, const MemoryCell &b) {
    assert(a.type == b.type); // This is always true

    switch(a.type) {
    case VariableType::INT64: return boolMemoryCell(a.as.INT64 != b.as.INT64); break;
    case VariableType::BOOL : return boolMemoryCell(a.as.BOOL  != b.as.BOOL); break;
    }

    VMError("Operation xor is not defined for type ", Grammar::Type::globalTypes[a.type].name);
    return MemoryCell();
}

void memoryCellPrint(const MemoryCell &cell) {
    std::cout << "[" << cell.type << "; ";
    switch(cell.type) {
    case INT64: {
        int64Print(cell);
        break;
    }
    case BOOL: {
        boolPrint(cell);
        break;
    }
    case RAW_PTR: {
        rawptrPrint(cell);
        break;
    }
    default: {
        VMError("Unsupported type of cell for print operation: ", cell.type, "\n");
    }
    }
    std::cout << "]\n";
}

void int64Print(const MemoryCell &a) {
    // Todo remove
    std::cout << a.as.INT64;
}

void boolPrint(const MemoryCell &a) {
    // Todo remove
    if(a.as.BOOL) {
        std::cout << "true";
    } else {
        std::cout << "false";
    }
}

void rawptrPrint(const MemoryCell &a) {
    // Todo remove
    // Without casting this is not printing : TODO investigate further
    std::cout << (uint64_t)a.as.RAW_PTR;
}

MemoryCell int64ToBool(const MemoryCell &a) {
    return boolMemoryCell(bool(a.as.INT64));
}

MemoryCell boolToint64(const MemoryCell &a) {
    return int64MemoryCell(int64_t(a.as.BOOL));
}


};