#include "table_element.h"

bool TableElement::CompareStr(const TableElement& first, const TableElement& second) {
    if (first.str_code.size() < second.str_code.size()) {
        return true;
    } else if (first.str_code.size() < second.str_code.size()) {
        return false;
    }
    for (int i = 0; i < first.str_code.size(); i++) {
        if (first.str_code[i] < second.str_code[i]) {
            return true;
        }
        if (first.str_code[i] > second.str_code[i]) {
            return false;
        }
    }

    return first.sign < second.sign;
}

TableElement::TableElement(std::string str_code, int sign) : sign(sign), str_code(str_code) {
}

TableElement::TableElement(int code, int sign) : code(code), sign(sign) {
}
std::string TableElement::NumberToString(uint32_t num) {
    std::string result;
    while (num > 0) {
        result += std::to_string(num & 1);
        num = (num >> 1);
    }
    std::reverse(result.begin(), result.end());
    return result;
}
