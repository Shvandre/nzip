extern "C" {
    #include "Source/Tools.h"
    #include "Source/ArgParse.h"
    #include "Source/FileMapping.h"
}

#include <gtest/gtest.h>
#include <sys/mman.h>

TEST(TestArgs, TestArgs1) {
    Arguments arguments = {nullptr};
    char* argv[] = {"zip", "--compression=9", "file.txt", "LOL.zip"};
    parse_args(4, argv, &arguments);
    ASSERT_STREQ(arguments.args[0], "file.txt");
    ASSERT_STREQ(arguments.args[1], "LOL.zip");
    ASSERT_EQ(arguments.block_size_int, 4096);
}

TEST(TestArgs, TestArgs2) {
    Arguments arguments = {nullptr};
    char* argv[] = {"zip", "--bs=8K", "-s", "-c=7", "Makefile"};
    parse_args(5, argv, &arguments);
   
    ASSERT_STREQ(arguments.args[0], "Makefile");
    ASSERT_STREQ(arguments.args[1], "archive.zip");
    ASSERT_EQ(arguments.block_size_int, 8192);
    ASSERT_EQ(arguments.silent, 1);
    ASSERT_EQ(arguments.compression_level, 7);
}


TEST(TestArgs, TestWrongArgs) {
    Arguments arguments = {nullptr};
    char* argv[] = {"zip", "--wrong-arg", "file", "-ABA"};
    ASSERT_DEATH(parse_args(4, argv, &arguments), ".*");
}

TEST(TestArgs, TestWrongArgs2) {
    Arguments arguments = {nullptr};
    char* argv[] = {"zip", "--bs=8K", "-s", "-c=20", "Makefile", "archive.zip", "extra"};
    ASSERT_DEATH(parse_args(7, argv, &arguments), ".*");
}

TEST(Mapping, Test1) {
    FILE* file = fopen("IhopeUnusedfilename.txt", "wb");
    if(file == nullptr) {
        perror("Error opening file");
        exit(1);
    }

    unsigned size = rand() % 100000;
    auto* buffer = (unsigned char*)malloc(size); //4k of garbage
    unsigned char* backup = buffer;
    fwrite(buffer, 1, size, file);
    fclose(file);

    unsigned char* mapped;
    size_t mapped_size = map_file("IhopeUnusedfilename.txt", 4096, &mapped);
    while(mapped_size != -1) {
        ASSERT_NE(mapped, nullptr);
        int res = memcmp(mapped, buffer, mapped_size);
        ASSERT_EQ(res, 0);
        buffer += mapped_size;
        if (munmap(mapped, mapped_size) == -1) {
            perror("Error un-mapping the file");
            exit(1);
        }
        mapped_size = map_file(nullptr, 4096, &mapped);
    }
    free(backup);
    remove("IhopeUnusedfilename.txt");
}

TEST(RoundTests, Test1) {
    Arguments arguments = {nullptr};
    char *argv[] = {"zip", "--bs=8K", "-s", "Makefile"}; //Assuming that Makefile is 100% in our Directory
    parse_args(4, argv, &arguments);
    compressFile(&arguments);
    uncompressFile("archive.zip", "output.txt");
    //Compare Makefile and output.txt using default diff utility
    if(!system("cmp Makefile output.txt")) {
        SUCCEED(); //diff returns 0 if files are equal
    }
    else{
        FAIL();
    }
}

TEST(RoundTests, LargeFile) {
    //Creating a large file
    FILE* file = fopen("largefileWithNoNameConflict", "wb");
    if(file == nullptr) {
        perror("Error opening file");
        exit(1);
    }
    unsigned size = rand() % 100000000 + 100000000;
    auto* buffer = (unsigned char*)malloc(size);
    fwrite(buffer, 1, size, file);
    fclose(file);
    free(buffer);

    Arguments arguments = {nullptr};
    char *argv[] = {"zip", "-b=100M", "-s", "largefileWithNoNameConflict"};
    parse_args(4, argv, &arguments);
    compressFile(&arguments);
    uncompressFile("archive.zip", "output.txt");
    //Compare largefile and output.txt using default diff utility
    if(!system("cmp largefileWithNoNameConflict output.txt")) {
        //Delete 
        remove("largefileWithNoNameConflict");
        SUCCEED();
    }
    else{
        remove("largefileWithNoNameConflict");
        FAIL();
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}