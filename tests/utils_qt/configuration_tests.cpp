
#include <gmock/gmock.h>

#include "OpenLibrary/utils_qt/configuration.hpp"
#include "iconfiguration_storage_mock.hpp"

using testing::Return;


TEST(Configuration, isEmptyAfterConstruction)
{
    IConfigStorageMock storage;
    EXPECT_CALL(storage, load).Times(1);
    EXPECT_CALL(storage, save).Times(0);    // config not modified, no save needed

    Configuration config(storage);

    EXPECT_TRUE(config.getEntry("test1").isNull());
    EXPECT_TRUE(config.getEntry("test2").isNull());
    EXPECT_TRUE(config.getEntry("test3").isNull());
}


TEST(Configuration, usesDefaultsWhenConfigEmpty)
{
    IConfigStorageMock storage;
    EXPECT_CALL(storage, load).Times(1);
    EXPECT_CALL(storage, save).Times(1);

    Configuration config(storage);

    config.setDefaultValue("test1", "a");
    config.setDefaultValue("test2", "aa");
    config.setDefaultValue("test3", "aaa");

    EXPECT_EQ(config.getEntry("test1"), "a");
    EXPECT_EQ(config.getEntry("test2"), "aa");
    EXPECT_EQ(config.getEntry("test3"), "aaa");
}


TEST(Configuration, doesNotUseDefaultsWhenConfigProvided)
{
    IConfigStorage::Content content = {
        { {"test2"}, "bb" },
    };

    IConfigStorageMock storage;
    EXPECT_CALL(storage, load).WillOnce(Return(content));
    EXPECT_CALL(storage, save).Times(1);

    Configuration config(storage);

    config.setDefaultValue("test1", "a");
    config.setDefaultValue("test2", "aa");
    config.setDefaultValue("test3", "aaa");

    EXPECT_EQ(config.getEntry("test1"), "a");
    EXPECT_EQ(config.getEntry("test2"), "bb");          // bb comes from config so wins
    EXPECT_EQ(config.getEntry("test3"), "aaa");
}
