#include "VirtualMachine.h"
#include "VirtualMachineOperations.h"

namespace VM {

MemoryCell::MemoryCell() {}
MemoryCell::~MemoryCell() {}

MemoryCell int64Add(const MemoryCell &a, const MemoryCell &b) {
    return int64MemoryCell(a.value.int64Val + b.value.int64Val);
}

MemoryCell int64Subtract(const MemoryCell &a, const MemoryCell &b) {
    return int64MemoryCell(a.value.int64Val - b.value.int64Val);
}

MemoryCell int64Multiply(const MemoryCell &a, const MemoryCell &b) {
    return int64MemoryCell(a.value.int64Val * b.value.int64Val);
}

MemoryCell int64Divide(const MemoryCell &a, const MemoryCell &b) {
    return int64MemoryCell(a.value.int64Val / b.value.int64Val);
}

void int64Print(const MemoryCell &a) {
    // Todo remove
    std::cout << a.value.int64Val << "\n";
}

MemoryCell int64Modulo(const MemoryCell &a, const MemoryCell &b) {
    return int64MemoryCell(a.value.int64Val % b.value.int64Val);
}

MemoryCell int64Or(const MemoryCell &a, const MemoryCell &b) {
    return int64MemoryCell(a.value.int64Val | b.value.int64Val);
}

MemoryCell int64And(const MemoryCell &a, const MemoryCell &b) {
    return int64MemoryCell(a.value.int64Val & b.value.int64Val);
}

MemoryCell int64Xor(const MemoryCell &a, const MemoryCell &b) {
    return int64MemoryCell(a.value.int64Val ^ b.value.int64Val);
}

MemoryCell int64BitwiseNot(const MemoryCell &a) {
    return int64MemoryCell(~(a.value.int64Val));
}  

MemoryCell int64MemoryCell(const int64_t val) {
    MemoryCell cell;
    cell.type = INT64;
    cell.value.int64Val = val;
    return cell;
}

};