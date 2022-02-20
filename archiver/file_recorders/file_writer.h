#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include "../entities/values.h"

class FileWriter {
private:
    std::string file_name_;

    std::ofstream output_;

    std::queue<bool> data_output_queue_;
public:
    void OpenStream(const std::string& file_name);

    void WriteBit(bool data);

    void WriteNineBites(int data);

    void CloseStream();
};
