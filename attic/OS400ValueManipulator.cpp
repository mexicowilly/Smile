#include "OS400ValueManipulator.hpp"
#include <smile/Program.hpp>
#include <smile/Exception.hpp>
#include <smile/Command.hpp>
#include "ByteOrder.hpp"
#include "StaticInitializer.hpp"
#include <assert.h>
#include <sstream>
#include <algorithm>

namespace
{

smile::Text QSYS(SMILE_WSTR("QSYS"));
smile::Text QWCRSVAL(SMILE_WSTR("QWCRSVAL"));
smile::Text QWCRNETA(SMILE_WSTR("QWCRNETA"));

class DeleteValue : public std::unary_function<std::pair<const smile::Text, smile::OS400GenericValue*>, void>
{
public:
    void operator() (const std::pair<const smile::Text, smile::OS400GenericValue*>& p)
    {
        delete p.second;
    }
};

uint32_t bytesToInt(const std::vector<uint8_t>& data, unsigned offset)
{
    uint32_t scratch;
    memcpy(&scratch, &data[offset], sizeof(scratch));
    return N32_TO_H(scratch);
}

}

namespace smile
{

OS400ValueManipulator::OS400ValueManipulator()
{
    // System values
    m_metadata[SMILE_WSTR("QABNORMSW")] = ValueMetadata(ValueType_Text, 1);
    m_metadata[SMILE_WSTR("QACGLVL")] = ValueMetadata(ValueType_TextArray, 10, 8);
    m_metadata[SMILE_WSTR("QACTJOB")] = ValueMetadata(ValueType_Int32, 4);
    m_metadata[SMILE_WSTR("QADLACTJ")] = ValueMetadata(ValueType_Int32, 4);
    m_metadata[SMILE_WSTR("QADLSPLA")] = ValueMetadata(ValueType_Int32, 4);
    m_metadata[SMILE_WSTR("QADLTOTJ")] = ValueMetadata(ValueType_Int32, 4);
    m_metadata[SMILE_WSTR("QALWJOBITP")] = ValueMetadata(ValueType_Text, 1);
    m_metadata[SMILE_WSTR("QALWOBJRST")] = ValueMetadata(ValueType_TextArray, 10, 15);
    m_metadata[SMILE_WSTR("QALWUSRDMN")] = ValueMetadata(ValueType_TextArray, 10, 50);
    m_metadata[SMILE_WSTR("QASTLVL")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("QATNPGM")] = ValueMetadata(ValueType_Text, 20);
    m_metadata[SMILE_WSTR("QAUDCTL")] = ValueMetadata(ValueType_TextArray, 10, 5);
    m_metadata[SMILE_WSTR("QAUDENDACN")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("QAUDFRCLVL")] = ValueMetadata(ValueType_Int32, 4);
    m_metadata[SMILE_WSTR("QAUDLVL")] = ValueMetadata(ValueType_TextArray, 10, 16);
    m_metadata[SMILE_WSTR("QAUDLVL2")] = ValueMetadata(ValueType_TextArray, 10, 99);
    m_metadata[SMILE_WSTR("QAUTOCFG")] = ValueMetadata(ValueType_Text, 1);
    m_metadata[SMILE_WSTR("QAUTORMT")] = ValueMetadata(ValueType_Text, 1);
    m_metadata[SMILE_WSTR("QAUTOSPRPT")] = ValueMetadata(ValueType_Text, 1);
    m_metadata[SMILE_WSTR("QAUTOVRT")] = ValueMetadata(ValueType_Int32, 4);
    m_metadata[SMILE_WSTR("QBASACTLVL")] = ValueMetadata(ValueType_Int32, 4);
    m_metadata[SMILE_WSTR("QBASPOOL")] = ValueMetadata(ValueType_Int32, 4);
    m_metadata[SMILE_WSTR("QBOOKPATH")] = ValueMetadata(ValueType_TextArray, 63, 5);
    m_metadata[SMILE_WSTR("QCCSID")] = ValueMetadata(ValueType_Int32, 4);
    m_metadata[SMILE_WSTR("QCENTURY")] = ValueMetadata(ValueType_Text, 1);
    m_metadata[SMILE_WSTR("QCFGMSGQ")] = ValueMetadata(ValueType_Text, 20);
    m_metadata[SMILE_WSTR("QCHRID")] = ValueMetadata(ValueType_Text, 20);
    m_metadata[SMILE_WSTR("QCHRIDCTL")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("QCMNARB")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("QCMNRCYLMT")] = ValueMetadata(ValueType_Text, 20);
    m_metadata[SMILE_WSTR("QCNTRYID")] = ValueMetadata(ValueType_Text, 2);
    m_metadata[SMILE_WSTR("QCONSOLE")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("QCRTAUT")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("QCRTOBJAUD")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("QCTLSBSD")] = ValueMetadata(ValueType_Text, 20);
    m_metadata[SMILE_WSTR("QCURSYM")] = ValueMetadata(ValueType_Text, 1);
    m_metadata[SMILE_WSTR("QDATE")] = ValueMetadata(ValueType_Text, 7);
    m_metadata[SMILE_WSTR("QDATETIME")] = ValueMetadata(ValueType_Text, 20);
    m_metadata[SMILE_WSTR("QDATFMT")] = ValueMetadata(ValueType_Text, 3);
    m_metadata[SMILE_WSTR("QDATSEP")] = ValueMetadata(ValueType_Text, 1);
    m_metadata[SMILE_WSTR("QDAY")] = ValueMetadata(ValueType_Text, 3);
    m_metadata[SMILE_WSTR("QDAYOFWEEK")] = ValueMetadata(ValueType_Text, 4);
    m_metadata[SMILE_WSTR("QDBFSTCCOL")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("QDBRCVYWT")] = ValueMetadata(ValueType_Text, 1);
    m_metadata[SMILE_WSTR("QDECFMT")] = ValueMetadata(ValueType_Text, 1);
    m_metadata[SMILE_WSTR("QDEVNAMING")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("QDEVRCYACN")] = ValueMetadata(ValueType_Text, 20);
    m_metadata[SMILE_WSTR("QDSCJOBITV")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("QDSPSGNINF")] = ValueMetadata(ValueType_Text, 1);
    m_metadata[SMILE_WSTR("QDYNPTYADJ")] = ValueMetadata(ValueType_Text, 1);
    m_metadata[SMILE_WSTR("QDYNPTYSCD")] = ValueMetadata(ValueType_Text, 1);
    m_metadata[SMILE_WSTR("QENDJOBLMT")] = ValueMetadata(ValueType_Int32, 4);
    m_metadata[SMILE_WSTR("QFRCCVNRST")] = ValueMetadata(ValueType_Text, 1);
    m_metadata[SMILE_WSTR("QHOUR")] = ValueMetadata(ValueType_Text, 2);
    m_metadata[SMILE_WSTR("QHSTLOGSIZ")] = ValueMetadata(ValueType_Int32, 4);
    m_metadata[SMILE_WSTR("QIGC")] = ValueMetadata(ValueType_Text, 1);
    m_metadata[SMILE_WSTR("QIGCCDEFNT")] = ValueMetadata(ValueType_Text, 20);
    m_metadata[SMILE_WSTR("QIGCFNTSIZ")] = ValueMetadata(ValueType_Int32, 4);
    m_metadata[SMILE_WSTR("QINACTITV")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("QINACTMSGQ")] = ValueMetadata(ValueType_Text, 20);
    m_metadata[SMILE_WSTR("QIPLDATTIM")] = ValueMetadata(ValueType_Text, 13);
    m_metadata[SMILE_WSTR("QIPLSTS")] = ValueMetadata(ValueType_Text, 1);
    m_metadata[SMILE_WSTR("QIPLTYPE")] = ValueMetadata(ValueType_Text, 1);
    m_metadata[SMILE_WSTR("QJOBMSGQFL")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("QJOBMSGQMX")] = ValueMetadata(ValueType_Int32, 4);
    m_metadata[SMILE_WSTR("QJOBMSGQSZ")] = ValueMetadata(ValueType_Int32, 4);
    m_metadata[SMILE_WSTR("QJOBMSGQTL")] = ValueMetadata(ValueType_Int32, 4);
    m_metadata[SMILE_WSTR("QJOBSPLA")] = ValueMetadata(ValueType_Int32, 4);
    m_metadata[SMILE_WSTR("QKBDBUF")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("QKBDTYPE")] = ValueMetadata(ValueType_Text, 3);
    m_metadata[SMILE_WSTR("QLANGID")] = ValueMetadata(ValueType_Text, 3);
    m_metadata[SMILE_WSTR("QLEAPADJ")] = ValueMetadata(ValueType_Int32, 4);
    m_metadata[SMILE_WSTR("QLIBLCKLVL")] = ValueMetadata(ValueType_Text, 1);
    m_metadata[SMILE_WSTR("QLMTDEVSSN")] = ValueMetadata(ValueType_Text, 1);
    m_metadata[SMILE_WSTR("QLMTSECOFR")] = ValueMetadata(ValueType_Text, 1);
    m_metadata[SMILE_WSTR("QLOCALE")] = ValueMetadata(ValueType_Text, 2080);
    m_metadata[SMILE_WSTR("QLOGOUTPUT")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("QMAXACTLVL")] = ValueMetadata(ValueType_Int32, 4);
    m_metadata[SMILE_WSTR("QMAXJOB")] = ValueMetadata(ValueType_Int32, 4);
    m_metadata[SMILE_WSTR("QMAXSGNACN")] = ValueMetadata(ValueType_Text, 1);
    m_metadata[SMILE_WSTR("QMAXSIGN")] = ValueMetadata(ValueType_Text, 6);
    m_metadata[SMILE_WSTR("QMAXSPLF")] = ValueMetadata(ValueType_Int32, 4);
    m_metadata[SMILE_WSTR("QMCHPOOL")] = ValueMetadata(ValueType_Int32, 4);
    m_metadata[SMILE_WSTR("QMINUTE")] = ValueMetadata(ValueType_Text, 2);
    m_metadata[SMILE_WSTR("QMLTTHDACN")] = ValueMetadata(ValueType_Text, 1);
    m_metadata[SMILE_WSTR("QMODEL")] = ValueMetadata(ValueType_Text, 4);
    m_metadata[SMILE_WSTR("QMONTH")] = ValueMetadata(ValueType_Text, 2);
    m_metadata[SMILE_WSTR("QPASTHRSVR")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("QPFRADJ")] = ValueMetadata(ValueType_Text, 1);
    m_metadata[SMILE_WSTR("QPRBFTR")] = ValueMetadata(ValueType_Text, 20);
    m_metadata[SMILE_WSTR("QPRBHLDITV")] = ValueMetadata(ValueType_Int32, 4);
    m_metadata[SMILE_WSTR("QPRCFEAT")] = ValueMetadata(ValueType_Text, 4);
    m_metadata[SMILE_WSTR("QPRCMLTTSK")] = ValueMetadata(ValueType_Text, 1);
    m_metadata[SMILE_WSTR("QPRTDEV")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("QPRTKEYFMT")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("QPRTTXT")] = ValueMetadata(ValueType_Text, 30);
    m_metadata[SMILE_WSTR("QPWDCHGBLK")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("QPWDEXPITV")] = ValueMetadata(ValueType_Text, 6);
    m_metadata[SMILE_WSTR("QPWDEXPWRN")] = ValueMetadata(ValueType_Int32, 4);
    m_metadata[SMILE_WSTR("QPWDLMTAJC")] = ValueMetadata(ValueType_Text, 1);
    m_metadata[SMILE_WSTR("QPWDLMTCHR")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("QPWDLMTREP")] = ValueMetadata(ValueType_Text, 1);
    m_metadata[SMILE_WSTR("QPWDLVL")] = ValueMetadata(ValueType_Int32, 4);
    m_metadata[SMILE_WSTR("QPWDMAXLEN")] = ValueMetadata(ValueType_Int32, 4);
    m_metadata[SMILE_WSTR("QPWDMINLEN")] = ValueMetadata(ValueType_Int32, 4);
    m_metadata[SMILE_WSTR("QPWDPOSDIF")] = ValueMetadata(ValueType_Text, 1);
    m_metadata[SMILE_WSTR("QPWDRQDDGT")] = ValueMetadata(ValueType_Text, 1);
    m_metadata[SMILE_WSTR("QPWDRQDDIF")] = ValueMetadata(ValueType_Text, 1);
    m_metadata[SMILE_WSTR("QPWDRULES")] = ValueMetadata(ValueType_TextArray, 15, 50);
    m_metadata[SMILE_WSTR("QPWDVLDPGM")] = ValueMetadata(ValueType_Text, 20);
    m_metadata[SMILE_WSTR("QPWRDWNLMT")] = ValueMetadata(ValueType_Int32, 4);
    m_metadata[SMILE_WSTR("QPWRRSTIPL")] = ValueMetadata(ValueType_Text, 1);
    m_metadata[SMILE_WSTR("QQRYDEGREE")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("QQRYTIMLMT")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("QRCLSPLSTG")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("QRETSVRSEC")] = ValueMetadata(ValueType_Text, 1);
    m_metadata[SMILE_WSTR("QRMTIPL")] = ValueMetadata(ValueType_Text, 1);
    m_metadata[SMILE_WSTR("QRMTSIGN")] = ValueMetadata(ValueType_Text, 20);
    m_metadata[SMILE_WSTR("QRMTSRVATR")] = ValueMetadata(ValueType_Text, 1);
    m_metadata[SMILE_WSTR("QSAVACCPTH")] = ValueMetadata(ValueType_Text, 1);
    m_metadata[SMILE_WSTR("QSCANFS")] = ValueMetadata(ValueType_TextArray, 10, 20);
    m_metadata[SMILE_WSTR("QSCANFSCTL")] = ValueMetadata(ValueType_TextArray, 10, 20);
    m_metadata[SMILE_WSTR("QSCPFCONS")] = ValueMetadata(ValueType_Text, 1);
    m_metadata[SMILE_WSTR("QSECOND")] = ValueMetadata(ValueType_Text, 2);
    m_metadata[SMILE_WSTR("QSECURITY")] = ValueMetadata(ValueType_Text, 2);
    m_metadata[SMILE_WSTR("QSETJOBATR")] = ValueMetadata(ValueType_TextArray, 10, 16);
    m_metadata[SMILE_WSTR("QSFWERRLOG")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("QSHRMEMCTL")] = ValueMetadata(ValueType_Text, 1);
    m_metadata[SMILE_WSTR("QSPCENV")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("QSPLFACN")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("QSRLNBR")] = ValueMetadata(ValueType_Text, 8);
    m_metadata[SMILE_WSTR("QSRTSEQ")] = ValueMetadata(ValueType_Text, 20);
    m_metadata[SMILE_WSTR("QSRVDMP")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("QSSLCSL")] = ValueMetadata(ValueType_TextArray, 40, 32);
    m_metadata[SMILE_WSTR("QSSLCSLCTL")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("QSSLPCL")] = ValueMetadata(ValueType_TextArray, 10, 10);
    m_metadata[SMILE_WSTR("QSTGLOWACN")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("QSTGLOWLMT")] = ValueMetadata(ValueType_Int32, 4);
    m_metadata[SMILE_WSTR("QSTRPRTWTR")] = ValueMetadata(ValueType_Text, 1);
    m_metadata[SMILE_WSTR("QSTRUPPGM")] = ValueMetadata(ValueType_Text, 20);
    m_metadata[SMILE_WSTR("QSTSMSG")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("QSVRAUTITV")] = ValueMetadata(ValueType_Int32, 4);
    m_metadata[SMILE_WSTR("QSYSLIBL")] = ValueMetadata(ValueType_TextArray, 10, 15);
    m_metadata[SMILE_WSTR("QTHDRSCADJ")] = ValueMetadata(ValueType_Text, 1);
    m_metadata[SMILE_WSTR("QTHDRSCAFN")] = ValueMetadata(ValueType_Text, 20);
    m_metadata[SMILE_WSTR("QTIMADJ")] = ValueMetadata(ValueType_Text, 30);
    m_metadata[SMILE_WSTR("QTIME")] = ValueMetadata(ValueType_Text, 9);
    m_metadata[SMILE_WSTR("QTIMSEP")] = ValueMetadata(ValueType_Text, 1);
    m_metadata[SMILE_WSTR("QTIMZON")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("QTOTJOB")] = ValueMetadata(ValueType_Int32, 4);
    m_metadata[SMILE_WSTR("QTSEPOOL")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("QUPSDLYTIM")] = ValueMetadata(ValueType_Text, 20);
    m_metadata[SMILE_WSTR("QUPSMSGQ")] = ValueMetadata(ValueType_Text, 20);
    m_metadata[SMILE_WSTR("QUSEADPAUT")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("QUSRLIBL")] = ValueMetadata(ValueType_TextArray, 10, 25);
    m_metadata[SMILE_WSTR("QUTCOFFSET")] = ValueMetadata(ValueType_Text, 5);
    m_metadata[SMILE_WSTR("QVFYOBJRST")] = ValueMetadata(ValueType_Text, 1);
    m_metadata[SMILE_WSTR("QYEAR")] = ValueMetadata(ValueType_Text, 2);

    // Network attributes
    m_metadata[SMILE_WSTR("ALRBCKFP")] = ValueMetadata(ValueType_Text, 16);
    m_metadata[SMILE_WSTR("ALRCTLD")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("ALRDFTFP")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("ALRFTR")] = ValueMetadata(ValueType_Text, 20);
    m_metadata[SMILE_WSTR("ALRHLDCNT")] = ValueMetadata(ValueType_Int32, 4);
    m_metadata[SMILE_WSTR("ALRLOGSTS")] = ValueMetadata(ValueType_Text, 7);
    m_metadata[SMILE_WSTR("ALRPRIFP")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("ALRRQSFP")] = ValueMetadata(ValueType_Text, 16);
    m_metadata[SMILE_WSTR("ALRSTS")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("ALWADDCLU")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("ALWANYNET")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("ALWHPRTWR")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("ALWVRTAPPN")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("VRTAUTODEV")] = ValueMetadata(ValueType_Int32, 4);
    m_metadata[SMILE_WSTR("DDMACC")] = ValueMetadata(ValueType_Text, 20);
    m_metadata[SMILE_WSTR("DFTCNNLST")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("DFTMODE")] = ValueMetadata(ValueType_Text, 8);
    m_metadata[SMILE_WSTR("DFTNETTYPE")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("DTACPR")] = ValueMetadata(ValueType_Int32, 4);
    m_metadata[SMILE_WSTR("DTACPRINM")] = ValueMetadata(ValueType_Int32, 4);
    m_metadata[SMILE_WSTR("HPRPTHTMR")] = ValueMetadata(ValueType_Text, 40);
    m_metadata[SMILE_WSTR("JOBACN")] = ValueMetadata(ValueType_Text, 10);
    m_metadata[SMILE_WSTR("LCLCPNAME")] = ValueMetadata(ValueType_Text, 8);
    m_metadata[SMILE_WSTR("LCLLOCNAME")] = ValueMetadata(ValueType_Text, 8);
    m_metadata[SMILE_WSTR("LCLNETID")] = ValueMetadata(ValueType_Text, 8);
    m_metadata[SMILE_WSTR("MAXINTSSN")] = ValueMetadata(ValueType_Int32, 4);
    m_metadata[SMILE_WSTR("MAXHOP")] = ValueMetadata(ValueType_Int32, 4);
    m_metadata[SMILE_WSTR("MDMCNTRYID")] = ValueMetadata(ValueType_Text, 2);
    m_metadata[SMILE_WSTR("MSGQ")] = ValueMetadata(ValueType_Text, 20);
    m_metadata[SMILE_WSTR("NETSERVER")] = ValueMetadata(ValueType_Text, 85);
    m_metadata[SMILE_WSTR("NODETYPE")] = ValueMetadata(ValueType_Text, 8);
    m_metadata[SMILE_WSTR("NWSDOMAIN")] = ValueMetadata(ValueType_Text, 8);
    m_metadata[SMILE_WSTR("OUTQ")] = ValueMetadata(ValueType_Text, 20);
    m_metadata[SMILE_WSTR("PNDSYSNAME")] = ValueMetadata(ValueType_Text, 8);
    m_metadata[SMILE_WSTR("PCSACC")] = ValueMetadata(ValueType_Text, 20);
    m_metadata[SMILE_WSTR("RAR")] = ValueMetadata(ValueType_Int32, 4);
    m_metadata[SMILE_WSTR("SYSNAME")] = ValueMetadata(ValueType_Text, 8);
}

OS400ValueManipulator& OS400ValueManipulator::getInstance()
{
    return StaticInitializer::getInstance().getOS400ValueManipulator();
}

std::map<Text, OS400GenericValue*> OS400ValueManipulator::parseOsData(std::vector<uint8_t> data, int ccsid)
{
    TextConverter converter(SMILE_DEFAULT_CONVERTER, ccsid);
    std::vector<uint8_t> constants = converter.convert(reinterpret_cast<const uint8_t*>(" L"));
    std::map<Text, OS400GenericValue*> output;
    uint32_t count = bytesToInt(data, 0);
    try
    {
        for (uint32_t i = 0; i < count; i++)
        {
            uint32_t offset = bytesToInt(data, 4 + i * 4);
            if (data[offset + 10] != constants[0])
            {
                Text name(&data[offset], 10, ccsid);
                name.trim();
                if (data[offset + 11] == constants[1])
                {
                    std::ostringstream stream;
                    stream << name << " is locked";
                    throw OS400ErrnoException(__FILE__, __LINE__,
                        stream.str().c_str(), OS400ErrnoException::Errno_EBUSY);
                }
                std::map<Text, ValueMetadata>::iterator found = m_metadata.find(name);
                if (found == m_metadata.end())
                {
                    std::ostringstream stream;
                    stream << name << " was returned by the system but could not be found";
                    throw InvalidStateException(__FILE__, __LINE__, stream.str().c_str());
                }
                OS400GenericValue* value = 0;
                assert(bytesToInt(data, offset + 12) == found->second.getTotalSize());
                switch (found->second.getType())
                {
                case ValueType_Int32:
                {
                    assert(bytesToInt(data, offset + 12) == 4);
                    value = new OS400Value<uint32_t>(bytesToInt(data, offset + 16));
                }
                    break;
                case ValueType_Text:
                {
                    Text textValue(&data[offset + 16], found->second.getUnitSize(), ccsid);
                    textValue.trim();
                    value = new OS400Value<Text>(textValue);
                }
                    break;
                case ValueType_TextArray:
                {
                    std::vector<Text> array;
                    for (size_t i = 0; i < found->second.getArraySize(); i++)
                    {
                        Text textValue(&data[offset + 16 + (i * found->second.getUnitSize())],
                                       found->second.getUnitSize(),
                                       ccsid);
                        textValue.trim();
                        array.push_back(textValue);
                    }
                    value = new OS400Value<std::vector<Text> >(array);
                }
                    break;
                }
                assert(value != 0);
                output[name] = value;
            }
        }
    }
    catch (...)
    {
        std::for_each(output.begin(), output.end(), DeleteValue());
        throw;
    }
    return output;
}

std::map<Text, OS400GenericValue*> OS400ValueManipulator::retrieveValues(AS400& as400,
                                                                         const std::vector<Text> names,
                                                                         bool systemValues)
{
    int ccsid = as400.getCCSID();
    size_t outSize = 4;
    std::vector<UChar> apiUnicodeNames;
    apiUnicodeNames.resize(names.size() * 10, 0x0020);
    int i = 0;
    for (std::vector<Text>::const_iterator itor = names.begin();
         itor != names.end();
         itor++, i += 10)
    {
        u_strncpy(&apiUnicodeNames[i], (*itor).getUChars(), (*itor).getLength());
        std::map<Text, ValueMetadata>::iterator found = m_metadata.find(*itor);
        if (found == m_metadata.end())
        {
            std::ostringstream stream;
            stream << *itor << " could not be found";
            throw InvalidStateException(__FILE__, __LINE__, stream.str().c_str());
        }
        outSize += found->second.getApiSize();
    }
    ProgramParameterList params;
    params.addParameter(OutParameter(outSize));
    params.addParameter(Int32InParameter(outSize));
    params.addParameter(Int32InParameter(names.size()));
    params.addParameter(TextInParameter(Text(&apiUnicodeNames[0], apiUnicodeNames.size())));
    std::vector<uint8_t> errorParam(8, 0);
    params.addParameter(InParameter(errorParam));
    QsysObjectName programName(QSYS, systemValues ? QWCRSVAL : QWCRNETA);
    Program getEm(as400, programName, params);
    if (!getEm.run())
    {
        std::ostringstream stream;
        stream << "Error running program " << programName;
        throw InvalidStateException(__FILE__, __LINE__, stream.str().c_str());
    }
    return parseOsData(dynamic_cast<const OutParameter&>(getEm.getParameters()[0]).getOutData(), ccsid);
}

void OS400ValueManipulator::setValue(AS400& as400,
                                     const Text& name,
                                     const OS400GenericValue& value,
                                     bool systemValue)
{
    Text commandText;
    if (systemValue)
    {
        commandText = SMILE_WSTR("CHGSYSVAL SYSVAL(");
        commandText += name;
        commandText += SMILE_WSTR(") VALUE(");
    }
    else
    {
        commandText = SMILE_WSTR("CHGNETA ");
        commandText += name;
        commandText += SMILE_WSTR("(");
    }
    commandText += value.toText();
    commandText += SMILE_WSTR(")");
    Command command(as400, commandText);
    if (!command.run())
    {
        std::ostringstream stream;
        stream << "Error running command to set the system value " << name;
        throw OS400Exception(__FILE__, __LINE__, stream.str().c_str(), command.getMessages());
    }
}

OS400ValueManipulator::ValueMetadata::ValueMetadata()
    : m_type(OS400ValueManipulator::ValueType_Text), m_unitSize(0), m_arraySize(0)
{
}

OS400ValueManipulator::ValueMetadata::ValueMetadata(ValueType type, size_t unitSize)
    : m_type(type), m_unitSize(unitSize), m_arraySize(1)
{
}

OS400ValueManipulator::ValueMetadata::ValueMetadata(ValueType type, size_t unitSize, size_t arraySize)
    : m_type(type), m_unitSize(unitSize), m_arraySize(arraySize)
{
}

OS400ValueManipulator::ValueMetadata::ValueMetadata(const ValueMetadata& metadata)
    : m_type(metadata.m_type), m_unitSize(metadata.m_unitSize), m_arraySize(metadata.m_arraySize)
{
}

OS400ValueManipulator::ValueMetadata& OS400ValueManipulator::ValueMetadata::operator= (const ValueMetadata& metadata)
{
    m_type = metadata.m_type;
    m_unitSize = metadata.m_unitSize;
    m_arraySize = metadata.m_arraySize;
    return *this;
}

}
