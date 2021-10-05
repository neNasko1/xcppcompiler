#pragma once
#ifndef GRAMMAR_BYTECODE_H
#define GRAMMAR_BYTECODE_H

#include <vector>
#include <string>
#include <map>

#include "Lexer.h"
#include "Grammar.h"
#include "VirtualMachine.h"
#include "GrammarTypeChecking.h"

namespace Grammar {

class Variable {
public:
    /**
     * @brief Name of variable
     * 
     */
    const std::string name;

    /**
     * @brief Type of variable
     * 
     */
    const uint64_t type;

    /**
     * @brief Offset from the beginning of the stack
     * 
     */
    uint64_t offset;

    /**
     * @brief Construct a new Variable object
     * 
     * @param _name 
     * @param _size 
     * @param _offset 
     */
    Variable(const std::string &_name, const uint64_t _type, const uint64_t _offset);

    /**
     * @brief Destroy the Variable object
     * 
     */
    ~Variable();
};

class Context {
public:
    /**
     * @brief Currently active variables
     * 
     */
    std::vector<Variable> variables;

    /**
     * @brief A map which helps to search for a variable by name
     * 
     */
    std::map<std::string, uint64_t> variablesMapping;

    /**
     * @brief Current stack offset
     * 
     */
    uint64_t offset;

    /**
     * @brief Construct a new Context object
     * 
     */
    Context();

    /**
     * @brief Destroy the Context object
     * 
     */
    ~Context();

    /**
     * @brief Add a new variable to the context
     * 
     * @param _name 
     * @param _type 
     * @param _offset 
     */
    uint64_t addVariable(const std::string &_name, const uint64_t _type);

    /**
     * @brief Search for a variable  by name
     * 
     * @param name 
     * @return uint64_t 
     */
    uint64_t findVariable(const std::string &name);
};  

};  

#endif