#include <gtest/gtest.h>
#include "archiver.h"
#include <filesystem>

TEST(ArchiverTest, a_few_files) {
    std::string init_dir = std::filesystem::current_path();
    init_dir += '/';
    //init_dir = "/home/eugene/CLionProjects/hse_test/cpp-pilot-hse/tasks/archiver/";

    const char *file1_data = "abcde";
    const char *file2_data = "abacababaa";

    std::ofstream ofstream;
    ofstream.open(init_dir + "test_example.txt");
    ofstream.write(file1_data, 5);
    ofstream.close();
    ofstream.open(init_dir + "test_example2.txt");
    ofstream.write(file2_data, 10);
    ofstream.close();

    std::string archive_name = "archive_file";

    if (!archive_name.starts_with("/")) {
        archive_name = init_dir + archive_name;
    }

    std::vector<std::string> inputs = {init_dir + "test_example.txt", init_dir + "test_example2.txt"};

    Archiver archiver;
    archiver.SetOutputFile(archive_name);
    archiver.SetInputFiles(inputs);
    archiver.Encode();

    Archiver archiver2;
    archiver2.SetInputFiles({archive_name});
    archiver2.SetOutputFile(init_dir);
    archiver2.Decode();

    std::string data;
    std::string data2;
    {
        std::ifstream input;
        input.open(init_dir + "test_example.txt");

        while (!input.eof()) {
            char buffer[1];
            input.read(buffer, 1);
            if (input.eof()) {
                break;
            }
            data += buffer[0];
        }
        input.close();
    }
    {
        std::ifstream input;
        input.open(init_dir + "test_example2.txt");

        while (!input.eof()) {
            char buffer[1];
            input.read(buffer, 1);
            if (input.eof()) {
                break;
            }
            data2 += buffer[0];
        }
        input.close();
    }

    ASSERT_TRUE(data == file1_data);
    ASSERT_TRUE(data2 == file2_data);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}