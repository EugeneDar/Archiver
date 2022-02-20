#pragma once

#include <vector>
#include <string>
#include "h_tree.h"
#include "entities/table_element.h"
#include "file_recorders/file_reader.h"
#include "file_recorders/file_writer.h"
#include <map>
#include <unordered_map>
#include "entities/values.h"

using CodeTable = std::vector<int>;

class Archiver {
private:
    std::string output_file_name_;
    std::vector<std::string> input_files_names_;
    int files_counter_ = 0;

    std::vector<std::string> CanonizeCode(const std::vector<std::string>& code_table);

public:
    void SetOutputFile(std::string file_name);
    void SetInputFiles(std::vector<std::string> input_files_names);

    std::vector<std::string> MakeCodeTable(const std::string& curr_input_file_name);

    void Encode();

    void EncodeFile(std::vector<std::string> code_table, std::string& curr_input_file_name, FileWriter& writer);

    bool DecodeFile(FileReader& reader);

    void Decode();
};
