#include "IfsFileHandleTest.hpp"
#include <smile/IfsFileHandle.hpp>
#include <smile/OS400ErrnoException.hpp>
#include "TestEnvironment.hpp"

void IfsFileHandleTest::setUp()
{
    Properties& props = TestEnvironment::getInstance().getProperties();
    smile::Text dirName(props.get("smile.test.existingDirectory").c_str(), "US-ASCII");
    m_file = new smile::IfsFile(*m_as400, smile::IfsObjectName(dirName, "IfsFileHandleTest"));
    m_handle = new smile::IfsFileHandle(*m_file);
}

void IfsFileHandleTest::tearDown()
{
    try
    {
        m_handle->close();
    }
    catch(...)
    {
    }
    delete m_handle;
    try
    {
        m_file->remove();
    }
    catch (...)
    {
    }
    delete m_file;
}

void IfsFileHandleTest::testOpenClose()
{
    predicate(HERE, !m_handle->isOpen());
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__,
        m_handle->open(smile::IfsFileHandle::Access_Read, smile::IfsFileHandle::OpenMode_CreateTruncate));
    predicate(HERE, m_handle->isOpen());
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, m_handle->close());
    predicate(HERE, !m_handle->isOpen());
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__,
        m_handle->open(smile::IfsFileHandle::Access_Write, smile::IfsFileHandle::OpenMode_CreateAppend));
    predicate(HERE, m_handle->isOpen());
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, m_handle->close());
    predicate(HERE, !m_handle->isOpen());
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__,
        m_handle->open(smile::IfsFileHandle::Access_ReadWrite, smile::IfsFileHandle::OpenMode_CreateTruncate));
    predicate(HERE, m_handle->isOpen());
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, m_handle->close());
    predicate(HERE, !m_handle->isOpen());
    HOMINID_PREDICATE_THROW(__FILE__, __LINE__,
        m_handle->open(smile::IfsFileHandle::Access_ReadWrite, smile::IfsFileHandle::OpenMode_MustCreate),
        smile::OS400ErrnoException);
    predicate(HERE, !m_handle->isOpen());
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__,
        m_handle->open(smile::IfsFileHandle::Access_ReadWrite, smile::IfsFileHandle::OpenMode_Append));
    predicate(HERE, m_handle->isOpen());
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, m_handle->close());
    predicate(HERE, !m_handle->isOpen());
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__,
        m_handle->open(smile::IfsFileHandle::Access_ReadWrite, smile::IfsFileHandle::OpenMode_Truncate));
    predicate(HERE, m_handle->isOpen());
    HOMINID_PREDICATE_NO_THROW(__FILE__, __LINE__, m_handle->close());
    predicate(HERE, !m_handle->isOpen());
    m_file->remove();
    HOMINID_PREDICATE_THROW(__FILE__, __LINE__,
        m_handle->open(smile::IfsFileHandle::Access_ReadWrite, smile::IfsFileHandle::OpenMode_Append),
        smile::OS400ErrnoException);
    predicate(HERE, !m_handle->isOpen());
    HOMINID_PREDICATE_THROW(__FILE__, __LINE__,
        m_handle->open(smile::IfsFileHandle::Access_ReadWrite, smile::IfsFileHandle::OpenMode_Truncate),
        smile::OS400ErrnoException);
    predicate(HERE, !m_handle->isOpen());
}

void IfsFileHandleTest::testReadWrite()
{
    m_handle->open(smile::IfsFileHandle::Access_Write, smile::IfsFileHandle::OpenMode_CreateTruncate);
    uint8_t bytes[100];
    for (int i = 0; i < 100; i++)
        bytes[i] = i % 10;
    m_handle->write(bytes, 100);
    m_handle->close();
    m_handle->open(smile::IfsFileHandle::Access_Read, smile::IfsFileHandle::OpenMode_Beginning);
    uint8_t bytes2[100];
    size_t bytesRead = 100;
    predicate(HERE, m_handle->read(bytes2, bytesRead));
    predicateEquals(HERE, static_cast<size_t>(100), bytesRead);
    predicate(HERE, memcmp(bytes, bytes2, 100) == 0);
    bytesRead = 10;
    predicate(HERE, !m_handle->read(bytes2, bytesRead));
    predicateEquals(HERE, static_cast<size_t>(0), bytesRead);
    m_handle->close();
    m_handle->open(smile::IfsFileHandle::Access_Read, smile::IfsFileHandle::OpenMode_Beginning);
    uint8_t bytes3[200];
    memset(bytes3, 0x7e, 200);
    bytesRead = 100;
    predicate(HERE, m_handle->read(bytes3, bytesRead));
    predicateEquals(HERE, static_cast<size_t>(100), bytesRead);
    predicate(HERE, memcmp(bytes, bytes3, 100) == 0);
    bytesRead = 100;
    predicate(HERE, !m_handle->read(&bytes3[100], bytesRead));
    predicateEquals(HERE, static_cast<size_t>(0), bytesRead);
    for (int i = 100; i < 200; i++)
        predicateEquals(HERE, static_cast<uint8_t>(0x7e), bytes3[i]);
}

void IfsFileHandleTest::testSeekTell()
{
    m_handle->open(smile::IfsFileHandle::Access_ReadWrite, smile::IfsFileHandle::OpenMode_CreateTruncate);
    predicateEquals(HERE, static_cast<uint64_t>(0), m_handle->tell());
    uint8_t bytes[100];
    for (int i = 0; i < 100; i++)
        bytes[i] = i % 10;
    m_handle->write(bytes, 100);
    predicateEquals(HERE, static_cast<uint64_t>(100), m_handle->tell());
    m_handle->seek(smile::IfsFileHandle::SeekFrom_Beginning, 10);
    predicateEquals(HERE, static_cast<uint64_t>(10), m_handle->tell());
    m_handle->seek(smile::IfsFileHandle::SeekFrom_End, 10);
    predicateEquals(HERE, static_cast<uint64_t>(90), m_handle->tell());
    m_handle->write(bytes, 2);
    predicateEquals(HERE, static_cast<uint64_t>(92), m_handle->tell());
    m_handle->seek(smile::IfsFileHandle::SeekFrom_Current, -72);
    predicateEquals(HERE, static_cast<uint64_t>(20), m_handle->tell());
    m_handle->close();
}
