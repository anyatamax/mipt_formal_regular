#include <iostream>
#include "include/regular.h"

int main() {
    std::string reg_s = "ab+c.aba.*.bac.+.+*";
    RegularParse reg(reg_s, 'b', 2);
    std::cout << reg.ParsingRegular();
    return 0;
}
