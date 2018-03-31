#include "PropertiesTest.hpp"
#include <smile/Properties.hpp>
#include <fstream>

void PropertiesTest::testAccessors()
{
    smile::Properties props;
    smile::Text key("junky");
    props.set(key, smile::Text("monkey"));
    predicateEquals(HERE, smile::Text("monkey"), props.get(key));
    props.setBool(key, true);
    predicateEquals(HERE, true, props.getBool(key));
    props.setInt(key, 123456789);
    predicateEquals(HERE, static_cast<int64_t>(123456789), props.getInt(key));
    std::vector<std::pair<smile::Text, smile::Text> > all = props.getAll();
    predicateEquals(HERE, static_cast<size_t>(1), all.size());
    predicateEquals(HERE, key, all[0].first);
    predicateEquals(HERE, smile::Text("123456789"), all[0].second);
}

void PropertiesTest::testComparing()
{
    smile::Properties props;
    props.set("my dog", "has fleas");
    props.setBool("and", true);
    props.setInt("so", 11);
    smile::Properties props2;
    predicate(HERE, props != props2);
    props2.setInt("so", 11);
    props2.setBool("and", true);
    predicate(HERE, props != props2);
    props2.set("my dog", "has fleas");
    predicate(HERE, props == props2);
    props.setBool("and", false);
    predicate(HERE, props != props2);
}

void PropertiesTest::testCopying()
{
    smile::Properties props;
    props.setInt(smile::Text("one"), 1);
    smile::Properties props2(props);
    predicateEquals(HERE, static_cast<size_t>(1), props2.size());
    predicate(HERE, props2.hasProperty(smile::Text("one")));
    predicateEquals(HERE, static_cast<int64_t>(1), props2.getInt(smile::Text("one")));
    props.setInt(smile::Text("two"), 2);
    props2 = props;
    predicateEquals(HERE, static_cast<size_t>(2), props2.size());
    predicate(HERE, props2.hasProperty(smile::Text("one")));
    predicateEquals(HERE, static_cast<int64_t>(1), props2.getInt(smile::Text("one")));
    predicate(HERE, props2.hasProperty(smile::Text("two")));
    predicateEquals(HERE, static_cast<int64_t>(2), props2.getInt(smile::Text("two")));
}

void PropertiesTest::testFileLoading()
{
    char tmpFile[L_tmpnam + 1];
    tmpnam(tmpFile);
    std::ofstream out(tmpFile);
    out << "# This is a comment" << std::endl;
    out << "    \t# This is a comment, too" << std::endl;
    out << "       \t\t\t      " << std::endl;
    out << std::endl;
    out << "one=one" << std::endl;
    out << " two\t\t   =                two                " << std::endl;
    out << "three=        \"t h r e e \"" << std::endl;
    out << "      four = ' f o u r'" << std::endl;
    out << "five=           five six seven" << std::endl;
    out << "six = \\s\\i\\x" << std::endl;
    out << "seven = '\\t\\n\\f\\r'" << std::endl;
    out << "eight = eight \\" << std::endl;
    out << "        nine" << std::endl;
    out << "nine = nine \\" << std::endl;
    out << "      ten \\" << std::endl;
    out << "eleven" << std::endl;
    out << "ten =" << std::endl;
    out << "eleven = \"\"" << std::endl;
    out << "twelve =        \t       ";
    out.close();
    try
    {
        smile::Properties props(tmpFile);
        predicateEquals(HERE, static_cast<size_t>(12), props.size());
        predicate(HERE, props.hasProperty(smile::Text("one")));
        predicateEquals(HERE, smile::Text("one"), props.get(smile::Text("one")));
        predicate(HERE, props.hasProperty(smile::Text("two")));
        predicateEquals(HERE, smile::Text("two"), props.get(smile::Text("two")));
        predicate(HERE, props.hasProperty(smile::Text("three")));
        predicateEquals(HERE, smile::Text("t h r e e "), props.get(smile::Text("three")));
        predicate(HERE, props.hasProperty(smile::Text("four")));
        predicateEquals(HERE, smile::Text(" f o u r"), props.get(smile::Text("four")));
        predicate(HERE, props.hasProperty(smile::Text("five")));
        predicateEquals(HERE, smile::Text("five six seven"), props.get(smile::Text("five")));
        predicate(HERE, props.hasProperty(smile::Text("six")));
        predicateEquals(HERE, smile::Text("six"), props.get(smile::Text("six")));
        predicate(HERE, props.hasProperty(smile::Text("seven")));
        const smile::Text& value = props.get(smile::Text("seven"));
        predicateEquals(HERE, static_cast<size_t>(4), value.getLength());
        predicateEquals(HERE, static_cast<UChar>(0x0009), value.getAt(0));
        predicateEquals(HERE, static_cast<UChar>(0x000a), value.getAt(1));
        predicateEquals(HERE, static_cast<UChar>(0x000c), value.getAt(2));
        predicateEquals(HERE, static_cast<UChar>(0x000d), value.getAt(3));
        predicate(HERE, props.hasProperty(smile::Text("eight")));
        predicateEquals(HERE, smile::Text("eight nine"), props.get(smile::Text("eight")));
        predicate(HERE, props.hasProperty(smile::Text("nine")));
        predicateEquals(HERE, smile::Text("nine ten eleven"), props.get(smile::Text("nine")));
        predicate(HERE, props.hasProperty(smile::Text("ten")));
        predicate(HERE, props.get(smile::Text("ten")).getLength() == 0);
        predicate(HERE, props.hasProperty(smile::Text("eleven")));
        predicate(HERE, props.get(smile::Text("eleven")).getLength() == 0);
        predicate(HERE, props.hasProperty(smile::Text("twelve")));
        predicate(HERE, props.get(smile::Text("twelve")).getLength() == 0);
    }
    catch (...)
    {
        unlink(tmpFile);
        throw;
    }
    unlink(tmpFile);
}

void PropertiesTest::testMerge()
{
    smile::Properties props;
    props.setInt(smile::Text("one"), 1);
    props.setInt(smile::Text("two"), 2);
    smile::Properties props2;
    props2.setInt(smile::Text("two"), -2);
    props2.setInt(smile::Text("three"), 3);
    props.merge(props2);
    predicate(HERE, props.hasProperty(smile::Text("one")));
    predicate(HERE, props.hasProperty(smile::Text("two")));
    predicate(HERE, props.hasProperty(smile::Text("three")));
    predicateEquals(HERE, static_cast<size_t>(3), props.size());
    predicateEquals(HERE, static_cast<int64_t>(1), props.getInt(smile::Text("one")));
    predicateEquals(HERE, static_cast<int64_t>(-2), props.getInt(smile::Text("two")));
    predicateEquals(HERE, static_cast<int64_t>(3), props.getInt(smile::Text("three")));
}
