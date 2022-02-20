#include "file_reader.h"

using namespace values;

void FileReader::OpenStream(const std::string& file_name) {
    input_.open(file_name, std::ios::in | std::ios::binary);
}

bool FileReader::ReadBit() {
    if (data_input_queue_.empty()) {
        char buffer[1] = {0};
        input_.read(buffer, 1);
        uint32_t data = static_cast<int>(buffer[0]);

        std::vector<int> vec_data;
        for (int i = 0; i < EIGHT_BITES; ++i) {
            vec_data.push_back(data & 1);
            data = (data >> 1);
        }
        //        std::reverse(vec_data.begin(), vec_data.end());
        for (int num : vec_data) {
            data_input_queue_.push(num & 1);
        }
    }

    bool b = data_input_queue_.front();
    data_input_queue_.pop();
    return b;
}

void FileReader::CloseStream() {
    input_.close();
}
