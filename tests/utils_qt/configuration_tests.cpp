
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


TEST(Configuration, variousValueTypes)
{
    IConfigStorage::Content expectedContent = {
        { {"test1"}, 1},
        { {"test2"}, 1.25f},
        { {"test3"}, 1.5},
        { {"test4"}, QString("abc123")},
        { {"test5"}, "zxc"},
        { {"test6"}, QByteArray("\001\123\255\020")},
    };

    IConfigStorageMock storage;
    EXPECT_CALL(storage, load).Times(1);
    EXPECT_CALL(storage, save(expectedContent)).Times(1);

    Configuration config(storage);

    config.setDefaultValue("test1", 1);
    config.setEntry       ("test2", 1.25f);
    config.setDefaultValue("test3", 1.5);
    config.setEntry       ("test4", QString("abc123"));
    config.setDefaultValue("test5", "zxc");
    config.setEntry       ("test6", QByteArray("\001\123\255\020"));

    EXPECT_EQ(config.getEntry("test1"), 1);
    EXPECT_EQ(config.getEntry("test2"), 1.25);
    EXPECT_EQ(config.getEntry("test3"), 1.5);
    EXPECT_EQ(config.getEntry("test4"), QString("abc123"));
    EXPECT_EQ(config.getEntry("test5"), "zxc");
    EXPECT_EQ(config.getEntry("test6"), QByteArray("\001\123\255\020"));
}
