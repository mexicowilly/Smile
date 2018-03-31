#include "TextTest.hpp"
#include <smile/Text.hpp>
#include <smile/Exception.hpp>
#include <iostream>

void TextTest::testAccess()
{
    smile::Text text1("abc");
    std::cout << "The size of Text is " << sizeof(text1) << std::endl;
    predicateEquals(HERE, static_cast<size_t>(3), text1.getLength());
    predicateEquals(HERE, static_cast<UChar>(0x61), text1.getAt(0));
    predicateEquals(HERE, static_cast<UChar>(0x62), text1.getAt(1));
    predicateEquals(HERE, static_cast<UChar>(0x63), text1.getAt(2));
    const UChar* chars = text1.getUChars();
    predicateEquals(HERE, static_cast<UChar>(0x61), chars[0]);
    predicateEquals(HERE, static_cast<UChar>(0x62), chars[1]);
    predicateEquals(HERE, static_cast<UChar>(0x63), chars[2]);
}

void TextTest::testCompare()
{
    smile::Text text1("abc");
    smile::Text text2("bcd");
    smile::Text text3("abc");
    predicate(HERE, text1.compare(text2) < 0);
    predicate(HERE, text2.compare(text1) > 0);
    predicateEquals(HERE, 0, text1.compare(text3));
    predicateEquals(HERE, text1, text3);
    predicateEquals(HERE, text3, text1);
    predicate(HERE, text1 != text2);
    predicate(HERE, text2 != text1);
    predicate(HERE, text1 < text2);
    predicate(HERE, text2 > text3);
    text1 = smile::Text("AbC");
    text2 = smile::Text("bCd");
    text3 = smile::Text("aBc");
    predicate(HERE, text1.compareNoCase(text2) < 0);
    predicate(HERE, text2.compareNoCase(text1) > 0);
    predicateEquals(HERE, 0, text1.compareNoCase(text3));
}

void TextTest::testConvert()
{
    uint8_t chars[] = { 0xe6, 0x89, 0x93, 0x93 };
    smile::Text text1(chars, 4, "ibm-37");
    predicateEquals(HERE, static_cast<size_t>(4), text1.getLength());
    predicateEquals(HERE, static_cast<UChar>(0x57), text1.getAt(0));
    predicateEquals(HERE, static_cast<UChar>(0x69), text1.getAt(1));
    predicateEquals(HERE, static_cast<UChar>(0x6c), text1.getAt(2));
    predicateEquals(HERE, static_cast<UChar>(0x6c), text1.getAt(3));
    std::vector<uint8_t> utf = text1.convert("UTF-32");
    predicateEquals(HERE, static_cast<size_t>(20), utf.size());
    uint32_t value;
    memcpy(&value, &utf[0], 4);
    predicateEquals(HERE, 0xfeffU, value);
    memcpy(&value, &utf[4], 4);
    predicateEquals(HERE, 0x57U, value);
    memcpy(&value, &utf[8], 4);
    predicateEquals(HERE, 0x69U, value);
    memcpy(&value, &utf[12], 4);
    predicateEquals(HERE, 0x6cU, value);
    memcpy(&value, &utf[16], 4);
    predicateEquals(HERE, 0x6cU, value);
}

void TextTest::testCopy()
{
    smile::Text text1("abc");
    smile::Text text2(text1);
    predicate(HERE, text1 == text2);
    text1 = smile::Text("bcd");
    predicate(HERE, text1 != text2);
    text2 = text1;
    predicate(HERE, text1 == text2);
}

void TextTest::testFind()
{
    smile::Text t1;
    predicateEquals(HERE, smile::Text::npos, t1.findFirst(0x002f));
    predicateEquals(HERE, smile::Text::npos, t1.findLast(0x002f));
    t1 = "My dog has fleas";
    predicateEquals(HERE, 9U, t1.findFirst(0x0073));
    predicateEquals(HERE, 15U, t1.findLast(0x0073));
    predicateEquals(HERE, smile::Text::npos, t1.findFirst(0x00ff));
    predicateEquals(HERE, smile::Text::npos, t1.findLast(0x00ff));
}

void TextTest::testSubText()
{
    smile::Text text("My dog has fleas");
    smile::Text sub1 = text.subText(3, 3);
    predicateEquals(HERE, static_cast<size_t>(3), sub1.getLength());
    predicateEquals(HERE, smile::Text("dog"), sub1);
    sub1 = text.subText(11);
    predicateEquals(HERE, static_cast<size_t>(5), sub1.getLength());
    predicateEquals(HERE, smile::Text("fleas"), sub1);
    sub1 = text.subText(0);
    predicateEquals(HERE, text, sub1);
    HOMINID_PREDICATE_THROW(__FILE__, __LINE__, sub1 = text.subText(75), smile::InvalidStateException);
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, sub1 = text.subText(7, 7000));
    predicateEquals(HERE, static_cast<size_t>(9), sub1.getLength());
    predicateEquals(HERE, smile::Text("has fleas"), sub1);
}

void TextTest::testTokenize()
{
    smile::Text text("My  dog     has fleas       ");
    std::vector<smile::Text> tokens = text.tokenize(smile::Text(" "));
    predicateEquals(HERE, static_cast<std::vector<smile::Text>::size_type>(4), tokens.size());
    predicateEquals(HERE, smile::Text("My"), tokens[0]);
    predicateEquals(HERE, smile::Text("dog"), tokens[1]);
    predicateEquals(HERE, smile::Text("has"), tokens[2]);
    predicateEquals(HERE, smile::Text("fleas"), tokens[3]);
    tokens = text.tokenize(smile::Text("/"));
    predicateEquals(HERE, static_cast<std::vector<smile::Text>::size_type>(1), tokens.size());
    predicateEquals(HERE, text, tokens[0]);
    text = smile::Text();
    tokens = text.tokenize(smile::Text("/"));
    predicateEquals(HERE, static_cast<std::vector<smile::Text>::size_type>(0), tokens.size());
}

void TextTest::testToUppercase()
{
    smile::Text text1("abc");
    text1.toUppercase();
    predicateEquals(HERE, static_cast<size_t>(3), text1.getLength());
    predicateEquals(HERE, static_cast<UChar>(0x41), text1.getAt(0));
    predicateEquals(HERE, static_cast<UChar>(0x42), text1.getAt(1));
    predicateEquals(HERE, static_cast<UChar>(0x43), text1.getAt(2));
}

void TextTest::testTrim()
{
    smile::Text text1("abc");
    smile::Text text2(text1);
    text2.trim();
    predicateEquals(HERE, static_cast<size_t>(3), text1.getLength());
    predicateEquals(HERE, text1, text2);
    text2 = smile::Text(" \t   abc");
    predicate(HERE, text2.getLength() != 3);
    predicate(HERE, text1 != text2);
    text2.trim();
    predicateEquals(HERE, static_cast<size_t>(3), text2.getLength());
    predicateEquals(HERE, text1, text2);
    text2 = smile::Text("abc\t\t\t           ");
    predicate(HERE, text2.getLength() != 3);
    predicate(HERE, text1 != text2);
    text2.trim();
    predicateEquals(HERE, static_cast<size_t>(3), text2.getLength());
    predicateEquals(HERE, text1, text2);
    text2 = smile::Text(" abc\t\t\t           ");
    predicate(HERE, text2.getLength() != 3);
    predicate(HERE, text1 != text2);
    text2.trim();
    predicateEquals(HERE, static_cast<size_t>(3), text2.getLength());
    predicateEquals(HERE, text1, text2);
}
