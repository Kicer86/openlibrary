

#include <CppUTest/TestHarness.h>

#include "private/htmltaglist.hpp"


TEST_GROUP(HtmlTagList)
{
};


TEST(HtmlTagList, ShouldNotChangeLevelOfFirstOpeningAndLastClosingElements)
{
    HtmlTagList list;

    HtmlTag topen(false);
    HtmlTag tclose(true);

    list.addElement(topen);
    list.addElement(tclose);

    const HtmlTagList::HtmlTags& tags = list.getHtmlTags();

    CHECK_EQUAL(0, tags.front().getLevel());
    CHECK_EQUAL(0, tags.back().getLevel());
}


TEST(HtmlTagList, ShouldIncreaseLevelOfSecondOpenTag)
{
    HtmlTagList list;

    HtmlTag topen1(false);
    HtmlTag topen2(false);
    HtmlTag tclose1(true);
    HtmlTag tclose2(true);

    list.addElement(topen1);
    list.addElement(topen2);
    list.addElement(tclose2);
    list.addElement(tclose1);

    const HtmlTagList::HtmlTags& tags = list.getHtmlTags();

    CHECK_EQUAL(0, tags[0].getLevel());
    CHECK_EQUAL(1, tags[1].getLevel());
    CHECK_EQUAL(1, tags[2].getLevel());
    CHECK_EQUAL(0, tags[3].getLevel());
}


TEST(HtmlTagList, ShouldIncreaseLevelOfOpenTagPlacesAfterClosedTag)
{
    HtmlTagList list;

    HtmlTag topen1(false);
    HtmlTag topen2(false);
    HtmlTag tclose1(true);
    HtmlTag tclose2(true);

    list.addElement(topen1);
    list.addElement(tclose1);
    list.addElement(topen2);
    list.addElement(tclose2);

    const HtmlTagList::HtmlTags& tags = list.getHtmlTags();

    CHECK_EQUAL(0, tags[0].getLevel());
    CHECK_EQUAL(0, tags[1].getLevel());
    CHECK_EQUAL(0, tags[2].getLevel());
    CHECK_EQUAL(0, tags[3].getLevel());
}


TEST(HtmlTagList, ShouldAllowToSearchForOpenTags)
{
    /*
    HtmlTagList list;

    HtmlTag tag1;
    tag1.setId("tag");           //first proper tag

    HtmlTag tag2;
    tag2.setId("tag2");

    HtmlTag tag3(true);          //closing one
    tag3.setId("tag");

    HtmlTag tag4;
    tag4.setId("anothertag");

    HtmlTag tag5;                //second proper tag
    tag5.setId("tag");
    tag5.setLevel(5);
    tag5.setText("text");

    HtmlTag tag6;
    tag6.setId("notatag");
    HtmlTag::Attr attr("tag", "tag");
    tag6.addAttr(attr);

    HtmlTag tag7;
    tag7.setId("atag");
    tag7.setText("tag");

    list.addElement(tag1);
    list.addElement(tag2);
    list.addElement(tag3);
    list.addElement(tag4);
    list.addElement(tag5);
    list.addElement(tag6);
    list.addElement(tag7);

    HtmlTagList::HtmlTags& tags = list.findAll();

    CHECK_EQUAL(0, tags[0].getLevel());
    CHECK_EQUAL(0, tags[1].getLevel());
    CHECK_EQUAL(0, tags[2].getLevel());
    CHECK_EQUAL(0, tags[3].getLevel());
    */
}
