#pragma once
#ifndef TYPE_CHECKING_H
#define TYPE_CHECKING_H

#include <vector>
#include <map>
#include <string>

#include "Lexer.h"
#include "Grammar.h"
#include "VirtualMachine.h"

namespace Grammar {

/**
 * @brief Class implementing type construct
 * 
 */
class Type {
public:

    /**
     * @brief Size of type
     * 
     */
    uint64_t size;

    /**
     * @brief Index in global type list
     * 
     */
    int32_t index;

    /**
     * @brief Type name
     * 
     */
    std::string name;

    /**
     * @brief Global type list
     * 
     */
    static std::vector<Type> globalTypes; 

    /**
     * @brief A map which helps to find which type you are looking at
     * 
     */
    static std::map<std::string, uint64_t> typeMapping;

    /**
     * @brief Construct a new Type object
     * 
     * @param size 
     * @param index 
     * @param name 
     */
    Type(const uint64_t size, const int32_t index, const std::string &name);

    /**
     * @brief Destroy the Type object
     * 
     */
    ~Type();

    /**
     * @brief Add a new type in the global type list
     * 
     * @param size 
     * @param name 
     * @return Type 
     */
    static uint64_t addType(const uint64_t size, const std::string &name);

    /**
     * @brief Find an existing type in the global type list
     * 
     */
    static uint64_t findType(const std::string &name);
};

// Initialize with some types

enum TypeIndexes {
    BOOL,
    INT64,
    VOID
};

};

#endif //TYPE_CHECKING_H