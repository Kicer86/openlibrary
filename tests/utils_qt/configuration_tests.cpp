
#include <gmock/gmock.h>

#include "OpenLibrary/utils_qt/configuration.hpp"
#include "iconfiguration_storage_mock.hpp"


TEST(Configuration, isEmptyAfterConstruction)
{
    IConfigStorageMock storage;
    Configuration config(storage);
}
