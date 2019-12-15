
#ifndef ICONFIGURATION_STORAGE_MOCK_HPP_INCLUDED
#define ICONFIGURATION_STORAGE_MOCK_HPP_INCLUDED

#include <gmock/gmock.h>

#include <OpenLibrary/utils_qt/configuration.hpp>

struct IConfigStorageMock: IConfigStorage
{
    MOCK_METHOD(Content, load, (), (override));
    MOCK_METHOD(void, save, (const Content &), (override));
};

#endif // ICONFIGURATION_STORAGE_MOCK_HPP_INCLUDED
