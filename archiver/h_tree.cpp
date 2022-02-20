#include "h_tree.h"

#include <iostream>

using namespace values;

std::vector<int> HTree::SearchFrequency() {
    std::vector<int> frequency_list;

    for (size_t i = 0; i < MAX_CHAR + 1; ++i) {
        frequency_list.push_back(0);
    }

    std::ifstream input;
    input.open(input_file_name_, std::ios::in);

    try {
        while (!input.eof()) {

            unsigned char ch = input.get();
            int value = static_cast<int>(ch);

            if (!input.eof()) {
                ++frequency_list[value];
            }
        }
    } catch (std::exception exception) {
        std::cout << exception.what();
    }


    int file_name_start_pos = input_file_name_.find_last_of('/') + 1;
    for (char ch : input_file_name_.substr(file_name_start_pos)) {
        ++frequency_list[static_cast<int>(ch)];
    }

    ++frequency_list[ONE_MORE_FILE];
    ++frequency_list[ARCHIVE_END];
    ++frequency_list[FILENAME_END];

    return frequency_list;
}

void HTree::Build() {

    std::vector<int> frequency_list = SearchFrequency();

    auto compare = [](std::shared_ptr<Node> a, std::shared_ptr<Node> b) {
        return std::tie(b->frequency, b->value) < std::tie(a->frequency, a->value);
    };
    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, decltype(compare)> queue(compare);

    for (size_t i = 0; i < frequency_list.size(); ++i) {
        if (frequency_list[i] == 0) {
            continue;
        }
        queue.push(std::make_unique<Node>(frequency_list[i], static_cast<int>(i), false));
    }

    while (queue.size() > static_cast<size_t>(1)) {

        std::shared_ptr<Node> first_sign(std::move(queue.top()));
        queue.pop();
        std::shared_ptr<Node> second_sign = std::move(queue.top());
        queue.pop();
        std::shared_ptr<Node> parent(
            std::make_unique<Node>(first_sign->frequency + second_sign->frequency, EMPTY_CELL_VALUE, true));
        parent->left = first_sign;
        parent->right = second_sign;

        queue.push(parent);
    }

    root_ = std::move(queue.top());
}

std::vector<TableElement> RecursionCodes(std::shared_ptr<Node> curr_node, std::string curr_code) {
    std::vector<TableElement> result;

    if (!curr_node->is_parent) {
        result.push_back({curr_code, curr_node->value});
        return result;
    }

    std::vector<TableElement> left_res = RecursionCodes(std::move(curr_node->left), curr_code + '0');
    std::vector<TableElement> right_res = RecursionCodes(std::move(curr_node->right), curr_code + '1');

    for (auto element : left_res) {
        result.push_back(element);
    }
    for (auto element : right_res) {
        result.push_back(element);
    }

    return result;
}

std::vector<std::string> HTree::GetCodeSizesTable() {

    std::vector<TableElement> table_elements = RecursionCodes(root_, "");

    std::vector<std::string> result;
    for (size_t i = 0; i <= MAX_CHAR; ++i) {
        result.push_back("");
    }

    for (const TableElement& element : table_elements) {
        result[element.sign] = element.str_code;
    }

    return result;
}

HTree::HTree(std::string file_name) {
    this->input_file_name_ = file_name;
    Build();
}
