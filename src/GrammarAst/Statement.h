#pragma once

#include <iostream>

namespace Grammar {

class Statement {
private:
    virtual std::ostream& hiddenPrint(std::ostream &os) const = 0;

public:

	Statement();
    virtual ~Statement() = 0;

    friend std::ostream& operator <<(std::ostream &os, const Statement &expr);
};

};
