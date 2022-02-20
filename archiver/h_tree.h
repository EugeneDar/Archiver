#pragma once

#include <vector>
#include <string>
#include <queue>
#include <tuple>
#include <memory>
#include "entities/table_element.h"
#include "entities/node.h"
#include <fstream>
#include <iostream>
#include "entities/values.h"

class HTree {
private:
    std::string input_file_name_;

    std::shared_ptr<Node> root_;

    std::vector<int> SearchFrequency();

public:
    HTree(std::string file_name);

    void Build();

    std::vector<std::string> GetCodeSizesTable();
};
