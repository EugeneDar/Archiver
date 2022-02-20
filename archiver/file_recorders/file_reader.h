#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include "../entities/values.h"

class FileReader {
private:
    std::string file_name_;

    std::ifstream input_;

    std::queue<bool> data_input_queue_;

public:
    void OpenStream(const std::string& file_name);

    bool ReadBit();

    void CloseStream();
};
