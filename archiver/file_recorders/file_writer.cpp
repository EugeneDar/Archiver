#include "file_writer.h"

using namespace values;

void FileWriter::OpenStream(const std::string& file_name) {
    output_.open(file_name, std::ios::out | std::ios::binary);
}

void FileWriter::WriteBit(bool data) {

    data_output_queue_.push(data);

    if (data_output_queue_.size() >= EIGHT_BITES) {
        std::vector<bool> vec;
        for (int i = 0; i < EIGHT_BITES; i++) {
            bool b = data_output_queue_.front();
            data_output_queue_.pop();
            vec.push_back(b);
        }
        std::reverse(vec.begin(), vec.end());
        int num = 0;
        for (int i = 0; i < EIGHT_BITES; i++) {
            num *= 2;
            if (vec[i]) {
                num += 1;
            }
        }
        char buffer[1] = {static_cast<char>(num)};
        output_.write(buffer, 1);
    }
}

void FileWriter::WriteNineBites(int data) {
    std::vector<bool> converted;
    while (converted.size() < NINE_BITES) {
        converted.push_back(data % 2);
        data /= 2;
    }
    std::reverse(converted.begin(), converted.end());

    for (bool b : converted) {
        WriteBit(b);
    }
}

void FileWriter::CloseStream() {

    while (!data_output_queue_.empty()) {
        while (data_output_queue_.size() < EIGHT_BITES) {
            data_output_queue_.push(false);
        }

        std::vector<bool> vec;
        for (int i = 0; i < EIGHT_BITES; i++) {
            bool b = data_output_queue_.front();
            data_output_queue_.pop();
            vec.push_back(b);
        }
        std::reverse(vec.begin(), vec.end());
        int num = 0;
        for (int i = 0; i < EIGHT_BITES; i++) {
            num *= 2;
            if (vec[i]) {
                num += 1;
            }
        }
        char buffer[1];
        buffer[0] = static_cast<char>(num);
        output_.write(buffer, 1);
    }
    output_.close();
}
