extern "C" {
    #include "Source/Tools.h"
    #include "Source/ArgParse.h"
}

#include <gtest/gtest.h>

TEST(TestArgs, TestArgs1) {
    Arguments arguments = {0};
    char* argv[] = {"zip", "-v", "file.txt", "LOL.zip"};
    parse_args(4, argv, &arguments);
    ASSERT_STREQ(arguments.args[0], "file.txt");
    ASSERT_STREQ(arguments.args[1], "LOL.zip");
    ASSERT_EQ(arguments.block_size_int, 4096);
}

TEST(TestArgs, TestArgs2) {
    Arguments arguments = {0};
    char* argv[] = {"zip", "--bs=8K", "-v", "Makefile"};
    parse_args(4, argv, &arguments);
    ASSERT_STREQ(arguments.args[0], "Makefile");
    ASSERT_STREQ(arguments.args[1], "archive.zip");
    ASSERT_EQ(arguments.block_size_int, 8192);
    ASSERT_EQ(arguments.verbose, 1);
}

TEST(TestArgs, TestWrongArgs) {
    Arguments arguments;
    char* argv[] = {"zip", "--wrong-arg", "file", "-ABA"};
    ASSERT_DEATH(parse_args(4, argv, &arguments), ".*");
}
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}