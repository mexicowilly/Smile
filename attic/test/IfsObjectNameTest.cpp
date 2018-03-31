#include "IfsObjectNameTest.hpp"
#include <smile/IfsObjectName.hpp>
#include <smile/Exception.hpp>

void IfsObjectNameTest::testCompare()
{
    smile::IfsObjectName name1("/abc");
    smile::IfsObjectName name2("/bcd");
    smile::IfsObjectName name3("/abc");
    predicate(HERE, name1.compare(name2) < 0);
    predicate(HERE, name2.compare(name1) > 0);
    predicate(HERE, name1.compare(name3) == 0);
    predicate(HERE, name1 == name3);
    predicate(HERE, name3 == name1);
    predicate(HERE, name1 != name2);
    predicate(HERE, name2 != name1);
    predicate(HERE, name1 < name2);
    predicate(HERE, name2 > name3);
}

void IfsObjectNameTest::testConstruct()
{
    smile::IfsObjectName name;
    predicate(HERE, name.isValid());
    smile::Text myDog("/my/dog/has/fleas");
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, name = smile::IfsObjectName(myDog));
    predicateEquals(HERE, myDog, name.getAbsolutePath());
    predicate(HERE, name.isValid());
    HOMINID_PREDICATE_THROW(__FILE__, __LINE__, name = smile::IfsObjectName(smile::Text("chunky")), smile::InvalidStateException);
    HOMINID_PREDICATE_THROW(__FILE__, __LINE__, name = smile::IfsObjectName(smile::Text()), smile::InvalidStateException);
    smile::IfsObjectName name2(name);
    predicate(HERE, name == name2);
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, name = smile::IfsObjectName(myDog, smile::Text("jimmy")));
    predicate(HERE, name.getAbsolutePath() == myDog + smile::Text("/jimmy"));
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, name = smile::IfsObjectName(name, smile::Text("bluefish")));
    predicate(HERE, name.getAbsolutePath() == myDog + smile::Text("/jimmy/bluefish"));
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, name = smile::IfsObjectName(smile::Text("//////my////dog/////////has/fleas////")));
    predicate(HERE, name.getAbsolutePath() == myDog);
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, name = smile::IfsObjectName(smile::Text("/my/dog/../has/fleas")));
    predicate(HERE, name.getAbsolutePath() == smile::Text("/my/has/fleas"));
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, name = smile::IfsObjectName(smile::Text("/my/dog/./../has/./././../../fleas")));
    predicate(HERE, name.getAbsolutePath() == smile::Text("/fleas"));
    HOMINID_PREDICATE_THROW(__FILE__, __LINE__, name = smile::IfsObjectName(smile::Text("/my/dog/../../..")), smile::InvalidStateException);
}

void IfsObjectNameTest::testGetObjectName()
{
    smile::IfsObjectName name;
    predicateEquals(HERE, static_cast<size_t>(1), name.getObjectName().getLength());
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, name = smile::IfsObjectName(smile::Text("/")));
    predicateEquals(HERE, smile::Text("/"), name.getObjectName());
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, name = smile::IfsObjectName(smile::Text("/my/dog/has/fleas")));
    predicateEquals(HERE, smile::Text("fleas"), name.getObjectName());
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, name = name.getParentName());
    predicateEquals(HERE, smile::Text("has"), name.getObjectName());
}

void IfsObjectNameTest::testGetParentName()
{
    smile::IfsObjectName name;
    predicate(HERE, name.getParentName().isValid());
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, name = smile::IfsObjectName(smile::Text("/my/dog/has/fleas")));
    smile::IfsObjectName parent = name.getParentName();
    predicate(HERE, parent.isValid());
    predicate(HERE, parent.getAbsolutePath() == smile::Text("/my/dog/has"));
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, parent = parent.getParentName());
    predicate(HERE, parent.isValid());
    predicate(HERE, parent.getAbsolutePath() == smile::Text("/my/dog"));
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, parent = parent.getParentName());
    predicate(HERE, parent.isValid());
    predicate(HERE, parent.getAbsolutePath() == smile::Text("/my"));
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, parent = parent.getParentName());
    predicate(HERE, parent.isValid());
    predicate(HERE, parent.getAbsolutePath() == smile::Text("/"));
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, parent = parent.getParentName());
    predicate(HERE, parent.isValid());
    predicate(HERE, parent.getAbsolutePath() == smile::Text("/"));
}
