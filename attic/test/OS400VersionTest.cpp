#include "OS400VersionTest.hpp"
#include <smile/OS400Version.hpp>

void OS400VersionTest::testOS400Version()
{
    smile::OS400Version ver1(5, 4, 0);
    predicate(HERE, ver1.getPacked() == 0x00050400);
    smile::OS400Version ver2((5 << 16) + (4 << 8));
    predicate(HERE, ver1 == ver2);
    smile::OS400Version ver3(5, 3, 0);
    predicate(HERE, ver1 > ver3);
    predicate(HERE, ver3 < ver2);
    predicate(HERE, ver1 != ver3);
    predicate(HERE, ver2.getVersion() == 5);
    predicate(HERE, ver2.getRelease() == 4);
    predicate(HERE, ver2.getModification() == 0);
    ver1 = ver3;
    predicate(HERE, ver1 == ver3);
    smile::OS400Version ver4(ver3);
    predicate(HERE, ver4 == ver3);
    predicate(HERE, ver4.getPacked() == 0x00050300);
}
