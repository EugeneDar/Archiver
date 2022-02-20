#include "archiver.h"

using namespace values;

std::string StringPlus(const std::string& str) {
    int num = 0;
    for (int i = 0; i < str.size(); i++) {
        num *= 2;
        if (str[i] == '1') {
            num += 1;
        }
    }
    ++num;

    std::string result = TableElement::NumberToString(num);
    while (result.size() < str.size()) {
        result = '0' + result;
    }
    return result;
}

std::vector<std::string> Archiver::MakeCodeTable(const std::string& curr_input_file_name) {

    HTree h_tree(curr_input_file_name);

    std::vector<std::string> vec = h_tree.GetCodeSizesTable();

    return CanonizeCode(vec);
}

std::vector<std::string> Archiver::CanonizeCode(const std::vector<std::string>& code_table) {
    std::vector<TableElement> table_elements;

    for (size_t i = 0; i < code_table.size(); ++i) {
        if (code_table[i].empty()) {
            continue;
        } else {
            table_elements.push_back({code_table[i], static_cast<int>(i)});
        }
    }

    auto compare = [](const TableElement& a, const TableElement& b) {
        if (a.str_code.size() < b.str_code.size()) {
            return true;
        } else if (a.str_code.size() > b.str_code.size()) {
            return false;
        }

        return a.sign < b.sign;
    };

    std::sort(table_elements.begin(), table_elements.end(), compare);

    std::vector<int> sizes;

    for (TableElement element : table_elements) {
        sizes.push_back(element.str_code.size());
    }

    std::vector<std::string> result;
    for (int i = 0; i <= MAX_CHAR + 1; ++i) {
        result.push_back("");
    }

    std::string curr;
    for (int i = 0; i < table_elements[0].str_code.size(); i++) {
        curr += '0';
    }

    for (size_t i = 0; i < sizes.size(); ++i) {
        result[table_elements[i].sign] = curr;

        curr = StringPlus(curr);
        if (i != sizes.size() - 1) {
            for (int j = sizes[i];j < sizes[i + 1];j++) {
                curr += '0';
            }
        }
    }

    return result;
}

void Archiver::Encode() {

    FileWriter writer;
    writer.OpenStream(this->output_file_name_);

    for (int i = 0; i < input_files_names_.size(); ++i) {

        ++files_counter_;
        EncodeFile(MakeCodeTable(input_files_names_[i]), input_files_names_[i], writer);
    }

    writer.CloseStream();
}

bool Archiver::DecodeFile(FileReader& reader) {

    std::unordered_map<std::string, int> codes_map;

    /**
     * 1. Amount letters in alphabet
     */

    int symbols_count = 0;
    for (int i = 0; i < 9; ++i) {
        symbols_count *= 2;
        bool value = reader.ReadBit();
        if (value) {
            ++symbols_count;
        }
    }

    /**
     * 2. Recovery data block
     */

    /**
     * 2.1. Read letters
     */

    std::vector<int> letters;

    {
        for (int amount = 0; amount < symbols_count; ++amount) {
            int value = 0;
            for (int i = 0; i < 9; ++i) {
                value *= 2;
                if (reader.ReadBit()) {
                    ++value;
                }
            }
            letters.push_back(value);
        }
    }

    /**
     * 2.2. Code sizes
     */

    std::vector<int> sizes_amount;

    int was_sizes_counter = 0;
    while (was_sizes_counter < symbols_count) {
        int amount = 0;
        for (int j = 0; j < 9; ++j) {
            amount *= 2;
            if (reader.ReadBit()) {
                ++amount;
            }
        }
        was_sizes_counter += amount;
        sizes_amount.push_back(amount);
    }

    /**
     * 2.3 Make codes
     */

    {
        std::string curr;
        std::vector<int> sizes;
        for (int i = 0; i < sizes_amount.size(); ++i) {
            for (int j = 0; j < sizes_amount[i]; ++j) {
                sizes.push_back(i + 1);
            }
        }

        for (int i = 0; i < sizes[0]; i++) {
            curr += '0';
        }

        for (size_t i = 0; i < sizes.size(); ++i) {
            codes_map.insert({curr, letters[i]});

            curr = StringPlus(curr);
            if (i != sizes.size() - 1) {
                for (int j = sizes[i];j < sizes[i + 1];j++) {
                    curr += '0';
                }
            }
        }
    }

    /**
     * 3. File name
     */

    std::string file_name;

    {
        std::string curr_str;
        while (true) {
            if (reader.ReadBit()) {
                curr_str += "1";
            } else {
                curr_str += "0";
            }
            if (!codes_map.contains(curr_str)) {
                continue;
            }

            auto it = codes_map.find(curr_str);
            int value = it->second;
            if (value == FILENAME_END) {
                break;
            } else {
                file_name += char(value);
                curr_str = "";
            }
        }
    }

    /**
     * 4. File data
     */

    std::ofstream output;

    output.open(output_file_name_ + file_name, std::ios::out);

    while (true) {
        std::string curr_str;
        while (true) {
            if (reader.ReadBit()) {
                curr_str += "1";
            } else {
                curr_str += "0";
            }
            if (!codes_map.contains(curr_str)) {
                continue;
            }

            auto it = codes_map.find(curr_str);
            int value = it->second;
            if (value == ONE_MORE_FILE) {
                output.close();
                return true;
            }
            if (value == ARCHIVE_END) {
                output.close();
                return false;
            }
            char buffer[1] = {static_cast<char>(value)};
            output.write(buffer, 1);

            break;
        }
    }
    return false;
}

void Archiver::SetOutputFile(std::string file_name) {
    output_file_name_ = file_name;
}

void Archiver::SetInputFiles(std::vector<std::string> input_files_names) {
    for (std::string name : input_files_names) {
        input_files_names_.push_back(name);
    }
}

void Archiver::EncodeFile(std::vector<std::string> code_table, std::string& curr_input_file_name, FileWriter& writer) {
    int symbols_count;

    int max_symbol_code_size = 0;

    /**
     * Init
     */

    std::vector<TableElement> table_elements;
    for (size_t i = 0; i < code_table.size(); ++i) {
        if (code_table[i].empty()) {
            continue;
        } else {
            table_elements.push_back({code_table[i], static_cast<int>(i)});
        }
    }
    std::sort(table_elements.begin(), table_elements.end(), TableElement::CompareStr);

    /**
     * 1. Amount letters in alphabet
     */

    symbols_count = table_elements.size();

    writer.WriteNineBites(symbols_count);

    /**
     * 2. Recovery data block
     */

    /**
     * 2.1. Put letters
     */

    for (TableElement element : table_elements) {
        writer.WriteNineBites(element.sign);
    }

    /**
     * 2.2. Code sizes
     */

    std::vector<int> symbol_code_sizes;
    for (size_t i = 0; i <= MAX_CHAR + 1; ++i) {
        symbol_code_sizes.push_back(0);
    }
    for (TableElement element : table_elements) {
        int curr_size = element.str_code.size();
        if (curr_size > max_symbol_code_size) {
            max_symbol_code_size = curr_size;
        }
        ++symbol_code_sizes[curr_size];
    }
    for (size_t i = 1; i <= max_symbol_code_size; ++i) {
        writer.WriteNineBites(symbol_code_sizes[i]);
    }

    /**
     * 3. File name
     */

    std::map<int, std::string> codes_map;
    for (TableElement element : table_elements) {
        codes_map.insert({element.sign, element.str_code});
    }
    int file_name_start_pos = curr_input_file_name.find_last_of('/') + 1;
    for (char letter : curr_input_file_name.substr(file_name_start_pos)) {
        auto it = codes_map.find(letter);
        for (char ch : it->second) {
            if (ch == '0') {
                writer.WriteBit(0);
            } else {
                writer.WriteBit(1);
            }
        }
    }

    /**
     * 4. File name end
     */

    for (char ch : code_table[FILENAME_END]) {
        if (ch == '0') {
            writer.WriteBit(0);
        } else {
            writer.WriteBit(1);
        }
    }

    /**
     * 5. File data
     */
    {
        std::ifstream input;
        input.open(curr_input_file_name);

        while (!input.eof()) {
            char buffer[1];
            input.read(buffer, 1);
            if (input.eof()) {
                break;
            }

            for (const char& ch : code_table[buffer[0]]) {
                if (ch == '0') {
                    writer.WriteBit(false);
                } else {
                    writer.WriteBit(true);
                }
            }
        }
    }

    if (files_counter_ == input_files_names_.size()) {

        for (char ch : code_table[ARCHIVE_END]) {
            if (ch == '0') {
                writer.WriteBit(0);
            } else {
                writer.WriteBit(1);
            }
        }
    } else {

        for (char ch : code_table[ONE_MORE_FILE]) {
            if (ch == '0') {
                writer.WriteBit(0);
            } else {
                writer.WriteBit(1);
            }
        }
    }
}

void Archiver::Decode() {
    FileReader reader;
    reader.OpenStream({input_files_names_[0]});

    while (DecodeFile(reader)) {
    }

    reader.CloseStream();
}
