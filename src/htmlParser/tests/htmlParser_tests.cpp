
#include <CppUTest/TestHarness.h>

#include "htmlParser/htmltag.hpp"
#include "htmlParser/htmlparser.hpp"


TEST_GROUP(HtmlTag)
{
};


TEST(HtmlTag, ShouldBeConstructableWithProperType)
{
    HtmlTag openTag;
    HtmlTag closeTag(true);

    CHECK_EQUAL(false, openTag.isClosing());
    CHECK_EQUAL(true, openTag.isOpening());
    CHECK_EQUAL(true, closeTag.isClosing());
    CHECK_EQUAL(false, closeTag.isOpening());
}


TEST(HtmlTag, ShouldSaveItsLevel)
{
    HtmlTag tag;
    tag.setLevel(1);

    CHECK_EQUAL(1, tag.getLevel());
}


TEST(HtmlTag, ShouldSaveItsType)
{
    HtmlTag tag;

    tag.setType(true);
    CHECK_EQUAL(true, tag.isClosing());
    CHECK_EQUAL(false, tag.isOpening());

    tag.setType(false);
    CHECK_EQUAL(false, tag.isClosing());
    CHECK_EQUAL(true, tag.isOpening());
}


TEST(HtmlTag, ShouldHasNoAttributesWhenConstructed)
{
    HtmlTag tag;

    CHECK_EQUAL(true, tag.getAttrs().empty());
}


TEST(HtmlTag, ShouldRememberAttributes)
{
    HtmlTag tag;

    HtmlTag::Attr attribute1("name", "value");
    HtmlTag::Attr attribute2("name2", "value2");
    HtmlTag::Attr attribute3("name3", "value3");

    tag.addAttr(attribute1);
    tag.addAttr(attribute2);
    tag.addAttr(attribute3);

    //existing
    CHECK_EQUAL(true, tag.hasAttr("name"));
    CHECK_EQUAL(true, tag.hasAttr("name2"));
    CHECK_EQUAL(true, tag.hasAttr("name3"));

    //nonexisting
    CHECK_EQUAL(false, tag.hasAttr("name4"));
    CHECK_EQUAL(false, tag.hasAttr("name1"));
}


TEST(HtmlTag, ShouldRememberItsId)
{
    HtmlTag tag;

    tag.setId("1234");

    CHECK_EQUAL("1234", tag.getId());
}


TEST(HtmlTag, ShouldRememberItsText)
{
    HtmlTag tag;

    tag.setText("tag text");

    CHECK_EQUAL("tag text", tag.getText());
}


TEST(HtmlTag, ShouldAllowToSearchForSingleAttributes)
{
    HtmlTag tag;
    HtmlTag::Attr attr("attribute", "value");

    tag.addAttr(attr);
    int size = tag.getAttrs().size();
    bool hasAttribute = tag.hasAttr("attribute");
    HtmlTag::Attr foundAttrib = tag.getAttr("attribute");

    CHECK_EQUAL(1, size);
    CHECK_EQUAL(true, hasAttribute);
    CHECK_EQUAL(foundAttrib.name, attr.name);
    CHECK_EQUAL(foundAttrib.value, attr.value);

    //
    HtmlTag::Attr new_attr("new_attribute", "new_value");

    tag.addAttr(new_attr);
    size = tag.getAttrs().size();
    bool hasOldAttribute = tag.hasAttr("attribute");
    bool hasNewAttribute = tag.hasAttr("new_attribute");
    HtmlTag::Attr foundOldAttrib = tag.getAttr("attribute");
    foundAttrib = tag.getAttr("new_attribute");

    CHECK_EQUAL(2, size);
    CHECK_EQUAL(true, hasOldAttribute);
    CHECK_EQUAL(foundOldAttrib.name, attr.name);
    CHECK_EQUAL(foundOldAttrib.value, attr.value);
    CHECK_EQUAL(true, hasNewAttribute);
    CHECK_EQUAL(foundAttrib.name, new_attr.name);
    CHECK_EQUAL(foundAttrib.value, new_attr.value);
}
