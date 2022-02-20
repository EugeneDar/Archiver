#pragma once

#include <string>
#include <tuple>

struct TableElement {
public:
    int code = -1;
    int sign;
    std::string str_code;

    static std::string NumberToString(uint32_t num);

    TableElement(int code, int sign);

    TableElement(std::string str_code, int sign);

    static bool CompareStr(const TableElement& first, const TableElement& second);
};
