
#define CPPUTEST_MEM_LEAK_DETECTION_DISABLED

#include <CppUTest/CommandLineTestRunner.h>

IMPORT_TEST_GROUP(HtmlTag);
IMPORT_TEST_GROUP(HtmlTagList);

int main(int argc, char** argv)
{
    return RUN_ALL_TESTS(argc, argv);
}
