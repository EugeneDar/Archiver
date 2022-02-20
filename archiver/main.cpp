#include <iostream>
#include <queue>
#include "archiver.h"
#include <filesystem>

int main(int argc, char *argv[]) {

    const std::string help_message =
        "archiver -c archive_name file1 [file2 ...] - encode files fil1, file2, ... and put results in file "
        "archive_name\n"
        "archiver -d archive_name - decode archive_name and put results in curr dir\n"
        "archiver -h - help\n";

    std::string init_dir = std::filesystem::current_path();
    init_dir += '/';
 //   init_dir = "/home/eugene/CLionProjects/hse_test/cpp-pilot-hse/tasks/archiver/";

    std::string type = argv[1];

    if (type == "-h") {
        std::cout << help_message;
    } else if (type == "-d") {
        std::string archive_name = argv[2];
        if (!archive_name.starts_with("/")) {
            archive_name = init_dir + archive_name;
        }

        Archiver archiver;
        archiver.SetInputFiles({archive_name});
        archiver.SetOutputFile(init_dir);
        archiver.Decode();
    } else if (type == "-c") {
        std::string archive_name = argv[2];

        if (!archive_name.starts_with("/")) {
            archive_name = init_dir + archive_name;
        }

        std::vector<std::string> inputs;
        for (int i = 3; i < argc; i++) {
            std::string temp = argv[i];
            if (!temp.starts_with("/")) {
                temp = init_dir + temp;
            }
            inputs.push_back(temp);
        }

        Archiver archiver;
        archiver.SetOutputFile(archive_name);
        archiver.SetInputFiles(inputs);
        archiver.Encode();
    }
}
