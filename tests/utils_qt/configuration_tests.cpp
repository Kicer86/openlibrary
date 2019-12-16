
#include <gmock/gmock.h>

#include "OpenLibrary/utils_qt/configuration.hpp"
#include "iconfiguration_storage_mock.hpp"


TEST(Configuration, isEmptyAfterConstruction)
{
    IConfigStorageMock storage;
    Configuration config(storage);

    EXPECT_TRUE(config.getEntry("test1").isNull());
    EXPECT_TRUE(config.getEntry("test2").isNull());
    EXPECT_TRUE(config.getEntry("test3").isNull());
}
