#include <smile/QsysObject.hpp>
#include <smile/Program.hpp>
#include <smile/Exception.hpp>
#include "ByteOrder.hpp"
#include "AttributeUtility.hpp"
#include <assert.h>
#include <math.h>
#include <sstream>

namespace
{

struct Qus_OBJD0400_t
{
    uint32_t Bytes_Returned;
    uint32_t Bytes_Available;
    uint8_t Object_Name[10];
    uint8_t Object_Lib_Name[10];
    uint8_t Object_Type[10];
    uint8_t Return_Lib[10];
    uint32_t Aux_Storage_Pool;
    uint8_t Object_Owner[10];
    uint8_t Object_Domain[2];
    uint8_t Create_Date_Time[13];
    uint8_t Change_Date_Time[13];
    uint8_t Extended_Obj_Attr[10];
    uint8_t Text_Description[50];
    uint8_t Source_File_Name[10];
    uint8_t Source_File_Lib_Name[10];
    uint8_t Source_File_Mbr_Name[10];
    uint8_t Source_File_Update_Date_Time[13];
    uint8_t Object_Saved_Date_Time[13];
    uint8_t Object_Restored_Date_Time[13];
    uint8_t Creator_User_Profile[10];
    uint8_t System_Object_Creation[8];
    uint8_t Reset_Date[7];
    uint32_t Save_Size;
    uint32_t Save_Sequence_Number;
    uint8_t Storage[10];
    uint8_t Save_Command[10];
    uint8_t Save_Volume_ID[71];
    uint8_t Save_Device[10];
    uint8_t Save_File_Name[10];
    uint8_t Save_File_Lib_Name[10];
    uint8_t Save_Label[17];
    uint8_t System_Level[9];
    uint8_t Compiler[16];
    uint8_t Object_Level[8];
    uint8_t User_Changed;
    uint8_t Licensed_Program[16];
    uint8_t PTF[10];
    uint8_t APAR[10];
    uint8_t Last_Used_Date[7];
    uint8_t Usage_Information_Update;
    uint32_t Days_Used_Count;
    uint32_t Object_Size;
    uint32_t Object_Size_Multiplier;
    uint8_t Object_Compress_Status;
    uint8_t Allow_Change;
    uint8_t Changed_By_Program;
    uint8_t User_Defined_Attr[10];
    uint8_t Overflow_ASP_Indicator;
    uint8_t Save_Active_Date_Time[13];
    uint8_t Object_Audit_Value[10];
    uint8_t Primary_Group[10];
    uint8_t Journal_Status;
    uint8_t Journal_Name[10];
    uint8_t Journal_Library[10];
    uint8_t Journal_Images;
    uint8_t Journal_Entries_Omitted;
    uint8_t Journal_Start_Date_Time[13];
    uint8_t Digitally_Signed;
    uint32_t Save_Size_In_Units;
    uint32_t Save_Size_Multiplier;
    uint32_t Lib_Aux_Storage_Pool_Number;
    uint8_t Object_Aux_Storage_Pool_Device[10];
    uint8_t Lib_Aux_Storage_Pool_Device[10];
    uint8_t Digitally_Signed_Sys_Trust;
    uint8_t Digitally_Signed_Multiple;
    uint8_t Reserved1[2];
    uint32_t Associated_Space_Size;
    uint8_t Optimum_Space_Alignment;
    uint8_t Object_ASP_Group[10];
    uint8_t Library_ASP_Group[10];
    uint8_t Starting_Jrn_Rcv_For_Apply[10];
    uint8_t Starting_Jrn_Rcv_Lib[10];
    uint8_t Starting_Jrn_Rcv_Lib_ASP_Dev[10];
    uint8_t Starting_Jrn_Rcv_Lib_ASP_Group[10];
    uint8_t Reserved;
};

const smile::Text STORAGE_KEEP(SMILE_WSTR("*KEEP     "));
const smile::Text STORAGE_FREE(SMILE_WSTR("*FREE     "));
const smile::Text SAVE_DEVICE_SAVF(SMILE_WSTR("*SAVF     "));
const smile::Text SAVE_DEVICE_DKT(SMILE_WSTR("*DKT      "));
const smile::Text SAVE_DEVICE_TAP(SMILE_WSTR("*TAP      "));
const smile::Text SAVE_DEVICE_OPT(SMILE_WSTR("*OPT      "));
const smile::Text OBJD0400(SMILE_WSTR("OBJD0400"));
const smile::Text QSYS(SMILE_WSTR("QSYS"));
const smile::Text QUSROBJD(SMILE_WSTR("QUSROBJD"));

unsigned getNumber(const smile::Text& text, unsigned offset, size_t digits)
{
    unsigned value = 0;
    for (unsigned i = 0; i < digits; i++)
    {
        UChar cur = text.getAt(offset + i);
        if (cur < 0x0030 || cur > 0x0039)
            break;
        value += static_cast<unsigned>(
            pow(10.0, static_cast<double>(digits - i - 1)) * (cur - 0x0030));
    }
    return value;
}

smile::Time dateTextToTime(const uint8_t* const bytes, int ccsid, UCalendar* calendar)
{
    smile::Text text(bytes, 7, ccsid);
    if (!text.isOnlyDigits())
        return 0;
    int32_t year = (getNumber(text, 0, 1) * 100) + 1900 + getNumber(text, 1, 2);
    int32_t month = getNumber(text, 3, 2);
    int32_t day = getNumber(text, 5, 2);
    UErrorCode uerr = U_ZERO_ERROR;
    ucal_setDateTime(calendar, year, month, day, 0, 0, 0, &uerr);
    if (!U_SUCCESS(uerr))
        throw smile::IcuException(__FILE__, __LINE__, "Error setting date", uerr);
    uerr = U_ZERO_ERROR;
    UDate millis = ucal_getMillis(calendar, &uerr);
    if (!U_SUCCESS(uerr))
        throw smile::IcuException(__FILE__, __LINE__, "Error getting calendar time", uerr);
    return static_cast<smile::Time>(millis);
}

smile::Time dateTimeTextToTime(const uint8_t* const bytes, int ccsid, UCalendar* calendar)
{
    smile::Text text(bytes, 13, ccsid);
    if (!text.isOnlyDigits())
        return 0;
    int32_t year = (getNumber(text, 0, 1) * 100) + 1900 + getNumber(text, 1, 2);
    int32_t month = getNumber(text, 3, 2);
    int32_t day = getNumber(text, 5, 2);
    int32_t hour = getNumber(text, 7, 2);
    int32_t minute = getNumber(text, 9, 2);
    int32_t second = getNumber(text, 11, 2);
    UErrorCode uerr = U_ZERO_ERROR;
    ucal_setDateTime(calendar, year, month, day, hour, minute, second, &uerr);
    if (!U_SUCCESS(uerr))
        throw smile::IcuException(__FILE__, __LINE__, "Error setting date and time", uerr);
    uerr = U_ZERO_ERROR;
    UDate millis = ucal_getMillis(calendar, &uerr);
    if (!U_SUCCESS(uerr))
        throw smile::IcuException(__FILE__, __LINE__, "Error getting calendar time", uerr);
    return static_cast<smile::Time>(millis);
}

}

namespace smile
{

QsysObject::QsysObject(AS400& as400, const QsysObjectName& objectName)
    : m_as400(&as400),
      m_name(objectName),
      m_auxiliaryStoragePool(0),
      m_owner(),
      m_domain(Domain_User),
      m_creationTime(0),
      m_changeTime(0),
      m_extendedAttribute(),
      m_description(),
      m_sourceFile(),
      m_sourceMember(),
      m_sourceFileUpdatedTime(0),
      m_savedTime(0),
      m_restoredTime(0),
      m_creator(),
      m_systemWhereCreated(),
      m_resetTime(0),
      m_savedSize(0),
      m_savedSequence(0),
      m_storage(Storage_Free),
      m_saveCommand(),
      m_saveVolumeId(),
      m_saveDevice(SaveDevice_None),
      m_saveFile(),
      m_saveLabel(),
      m_versionWhereCreated(),
      m_compiler(),
      m_objectLevel(),
      m_userChanged(false),
      m_licensedProgram(),
      m_programTemporaryFix(),
      m_authorizedProgramAnalysisReport(),
      m_lastUsedTime(0),
      m_usageInformationUpdated(false),
      m_daysUsed(0),
      m_size(0),
      m_compression(Compression_NotDetermined),
      m_allowChangeByProgram(false),
      m_changedByProgram(false),
      m_userDefinedAttribute(),
      m_overflowedAsp(false),
      m_saveActiveTime(0),
      m_auditMode(AuditMode_NotAvailable),
      m_group(),
      m_journaled(false),
      m_journalName(),
      m_journalsBeforeImage(false),
      m_journalOmitOpenCloseEntries(false),
      m_journalStartTime(0),
      m_signed(false),
      m_libraryAsp(0),
      m_aspDevice(),
      m_libraryAspDevice(),
      m_systemSigned(false),
      m_multipleSignatures(false),
      m_primaryAssociatedSpaceSize(0),
      m_spaceAlignment(SpaceAlignment_None),
      m_auxiliaryStoragePoolGroup(),
      m_libraryAspGroup(),
      m_startingJournalReceiverForApply(),
      m_startingJournalReceiverAspDevice(),
      m_startingJournalReceiverAspGroup()
{
}

QsysObject::QsysObject(const QsysObject& object)
    : m_as400(object.m_as400),
      m_name(object.m_name),
      m_auxiliaryStoragePool(object.m_auxiliaryStoragePool),
      m_owner(object.m_owner),
      m_domain(object.m_domain),
      m_creationTime(object.m_creationTime),
      m_changeTime(object.m_changeTime),
      m_extendedAttribute(object.m_extendedAttribute),
      m_description(object.m_description),
      m_sourceFile(object.m_sourceFile),
      m_sourceMember(object.m_sourceMember),
      m_sourceFileUpdatedTime(object.m_sourceFileUpdatedTime),
      m_savedTime(object.m_savedTime),
      m_restoredTime(object.m_restoredTime),
      m_creator(object.m_creator),
      m_systemWhereCreated(object.m_systemWhereCreated),
      m_resetTime(object.m_resetTime),
      m_savedSize(object.m_savedSize),
      m_savedSequence(object.m_savedSequence),
      m_storage(object.m_storage),
      m_saveCommand(object.m_saveCommand),
      m_saveVolumeId(object.m_saveVolumeId),
      m_saveDevice(object.m_saveDevice),
      m_saveFile(object.m_saveFile),
      m_saveLabel(object.m_saveLabel),
      m_versionWhereCreated(object.m_versionWhereCreated),
      m_compiler(object.m_compiler),
      m_objectLevel(object.m_objectLevel),
      m_userChanged(object.m_userChanged),
      m_licensedProgram(object.m_licensedProgram),
      m_programTemporaryFix(object.m_programTemporaryFix),
      m_authorizedProgramAnalysisReport(object.m_authorizedProgramAnalysisReport),
      m_lastUsedTime(object.m_lastUsedTime),
      m_usageInformationUpdated(object.m_usageInformationUpdated),
      m_daysUsed(object.m_daysUsed),
      m_size(object.m_size),
      m_compression(object.m_compression),
      m_allowChangeByProgram(object.m_allowChangeByProgram),
      m_changedByProgram(object.m_changedByProgram),
      m_userDefinedAttribute(object.m_userDefinedAttribute),
      m_overflowedAsp(object.m_overflowedAsp),
      m_saveActiveTime(object.m_saveActiveTime),
      m_auditMode(object.m_auditMode),
      m_group(object.m_group),
      m_journaled(object.m_journaled),
      m_journalName(object.m_journalName),
      m_journalsBeforeImage(object.m_journalsBeforeImage),
      m_journalOmitOpenCloseEntries(object.m_journalOmitOpenCloseEntries),
      m_journalStartTime(object.m_journalStartTime),
      m_signed(object.m_signed),
      m_libraryAsp(object.m_libraryAsp),
      m_aspDevice(object.m_aspDevice),
      m_libraryAspDevice(object.m_libraryAspDevice),
      m_systemSigned(object.m_systemSigned),
      m_multipleSignatures(object.m_multipleSignatures),
      m_primaryAssociatedSpaceSize(object.m_primaryAssociatedSpaceSize),
      m_spaceAlignment(object.m_spaceAlignment),
      m_auxiliaryStoragePoolGroup(object.m_auxiliaryStoragePoolGroup),
      m_libraryAspGroup(object.m_libraryAspGroup),
      m_startingJournalReceiverForApply(object.m_startingJournalReceiverForApply),
      m_startingJournalReceiverAspDevice(object.m_startingJournalReceiverAspDevice),
      m_startingJournalReceiverAspGroup(object.m_startingJournalReceiverAspGroup)
{
}

QsysObject::~QsysObject()
{
}

QsysObject& QsysObject::operator= (const QsysObject& object)
{
    if (&object != this)
    {
        m_as400 = object.m_as400;
        m_name = object.m_name;
        m_auxiliaryStoragePool = object.m_auxiliaryStoragePool;
        m_owner = object.m_owner;
        m_domain = object.m_domain;
        m_creationTime = object.m_creationTime;
        m_changeTime = object.m_changeTime;
        m_extendedAttribute = object.m_extendedAttribute;
        m_description = object.m_description;
        m_sourceFile = object.m_sourceFile;
        m_sourceMember = object.m_sourceMember;
        m_sourceFileUpdatedTime = object.m_sourceFileUpdatedTime;
        m_savedTime = object.m_savedTime;
        m_restoredTime = object.m_restoredTime;
        m_creator = object.m_creator;
        m_systemWhereCreated = object.m_systemWhereCreated;
        m_resetTime = object.m_resetTime;
        m_savedSize = object.m_savedSize;
        m_savedSequence = object.m_savedSequence;
        m_storage = object.m_storage;
        m_saveCommand = object.m_saveCommand;
        m_saveVolumeId = object.m_saveVolumeId;
        m_saveDevice = object.m_saveDevice;
        m_saveFile = object.m_saveFile;
        m_saveLabel = object.m_saveLabel;
        m_versionWhereCreated = object.m_versionWhereCreated;
        m_compiler = object.m_compiler;
        m_objectLevel = object.m_objectLevel;
        m_userChanged = object.m_userChanged;
        m_licensedProgram = object.m_licensedProgram;
        m_programTemporaryFix = object.m_programTemporaryFix;
        m_authorizedProgramAnalysisReport = object.m_authorizedProgramAnalysisReport;
        m_lastUsedTime = object.m_lastUsedTime;
        m_usageInformationUpdated = object.m_usageInformationUpdated;
        m_daysUsed = object.m_daysUsed;
        m_size = object.m_size;
        m_compression = object.m_compression;
        m_allowChangeByProgram = object.m_allowChangeByProgram;
        m_changedByProgram = object.m_changedByProgram;
        m_userDefinedAttribute = object.m_userDefinedAttribute;
        m_overflowedAsp = object.m_overflowedAsp;
        m_saveActiveTime = object.m_saveActiveTime;
        m_auditMode = object.m_auditMode;
        m_group = object.m_group;
        m_journaled = object.m_journaled;
        m_journalName = object.m_journalName;
        m_journalsBeforeImage = object.m_journalsBeforeImage;
        m_journalOmitOpenCloseEntries = object.m_journalOmitOpenCloseEntries;
        m_journalStartTime = object.m_journalStartTime;
        m_signed = object.m_signed;
        m_libraryAsp = object.m_libraryAsp;
        m_aspDevice = object.m_aspDevice;
        m_libraryAspDevice = object.m_libraryAspDevice;
        m_systemSigned = object.m_systemSigned;
        m_multipleSignatures = object.m_multipleSignatures;
        m_primaryAssociatedSpaceSize = object.m_primaryAssociatedSpaceSize;
        m_spaceAlignment = object.m_spaceAlignment;
        m_auxiliaryStoragePoolGroup = object.m_auxiliaryStoragePoolGroup;
        m_libraryAspGroup = object.m_libraryAspGroup;
        m_startingJournalReceiverForApply = object.m_startingJournalReceiverForApply;
        m_startingJournalReceiverAspDevice = object.m_startingJournalReceiverAspDevice;
        m_startingJournalReceiverAspGroup = object.m_startingJournalReceiverAspGroup;
    }
    return *this;
}

void QsysObject::ensureData()
{
    if (m_owner.isEmpty())
        refresh();
    assert(!m_owner.isEmpty());
}

void QsysObject::refresh()
{
    resetAttributes();
    ProgramParameterList parameters;
    parameters.addParameter(OutParameter(sizeof(Qus_OBJD0400_t)));
    parameters.addParameter(Int32InParameter(sizeof(Qus_OBJD0400_t)));
    parameters.addParameter(TextInParameter(OBJD0400));
    parameters.addParameter(QsysObjectNameInParameter(m_name));
    parameters.addParameter(OutParameter(8));
    Program qusrobjd(*m_as400,
                     QsysObjectName(QSYS, QUSROBJD),
                     parameters);
    if (!qusrobjd.run())
        throw OS400Exception(__FILE__, __LINE__, "Error running program QUSROBJD", qusrobjd.getMessages());
    const Qus_OBJD0400_t* objd = reinterpret_cast<const Qus_OBJD0400_t*>(
        &dynamic_cast<const OutParameter&>(qusrobjd.getParameters()[0]).getOutData()[0]);
    assert(objd->Bytes_Returned >= sizeof(Qus_OBJD0400_t));
    int ccsid = m_as400->getCCSID();
    UErrorCode uerr = U_ZERO_ERROR;
    UCalendar* calendar = ucal_open(0, 0, uloc_getDefault(), UCAL_TRADITIONAL, &uerr);
    if (!U_SUCCESS(uerr))
        throw smile::IcuException(__FILE__, __LINE__, "Error opening calendar", uerr);
    try
    {
        m_auxiliaryStoragePool = N32_TO_H(objd->Aux_Storage_Pool);
        m_owner = Text(objd->Object_Owner, 10, ccsid).trim();
        Text domainText(objd->Object_Domain, 2, ccsid);
        m_domain = domainText.getAt(1) == 0x0053 ? Domain_System : Domain_User;
        m_creationTime = dateTimeTextToTime(objd->Create_Date_Time, ccsid, calendar);
        m_changeTime = dateTimeTextToTime(objd->Change_Date_Time, ccsid, calendar);
        m_extendedAttribute = Text(objd->Extended_Obj_Attr, 10, ccsid);
        m_description = Text(objd->Text_Description, 50, ccsid).trim();
        m_sourceFile = QsysObjectName(Text(objd->Save_File_Lib_Name, 10, ccsid),
                                      Text(objd->Source_File_Name, 10, ccsid));
        m_sourceMember = Text(objd->Source_File_Mbr_Name, 10, ccsid).trim();
        m_sourceFileUpdatedTime = dateTimeTextToTime(objd->Source_File_Update_Date_Time, ccsid, calendar);
        m_savedTime = dateTimeTextToTime(objd->Object_Saved_Date_Time, ccsid, calendar);
        m_restoredTime = dateTimeTextToTime(objd->Object_Restored_Date_Time, ccsid, calendar);
        m_creator = Text(objd->Creator_User_Profile, 10, ccsid).trim();
        m_systemWhereCreated = Text(objd->System_Object_Creation, 8, ccsid).trim();
        m_resetTime = dateTextToTime(objd->Reset_Date, ccsid, calendar);
        if (static_cast<int32_t>(N32_TO_H(objd->Save_Size)) == -1)
        {
            m_savedSize = static_cast<uint64_t>(N32_TO_H(objd->Save_Size_In_Units)) *
                static_cast<uint64_t>(N32_TO_H(objd->Save_Size_Multiplier));
        }
        else
        {
            m_savedSize = N32_TO_H(objd->Save_Size);
        }
        m_savedSequence = N32_TO_H(objd->Save_Sequence_Number);
        Text storageText(objd->Storage, 10, ccsid);
        if (storageText == STORAGE_KEEP)
        {
            m_storage = Storage_Keep;
        }
        else if (storageText == STORAGE_FREE)
        {
            m_storage = Storage_Free;
        }
        else
        {
            std::ostringstream stream;
            stream << "Unexpected storage value: " << storageText;
            throw InvalidStateException(__FILE__, __LINE__, stream.str().c_str());
        }
        m_saveCommand = Text(objd->Save_Command, 10, ccsid).trim();
        m_saveVolumeId = Text(objd->Save_Volume_ID, 71, ccsid).trim();
        Text saveDeviceText(objd->Save_Device, 10, ccsid);
        if (saveDeviceText == SAVE_DEVICE_SAVF)
            m_saveDevice = SaveDevice_SaveFile;
        else if (saveDeviceText == SAVE_DEVICE_DKT)
            m_saveDevice = SaveDevice_Diskette;
        else if (saveDeviceText == SAVE_DEVICE_TAP)
            m_saveDevice = SaveDevice_Tape;
        else if (saveDeviceText == SAVE_DEVICE_OPT)
            m_saveDevice = SaveDevice_Optical;
        else
            m_saveDevice = SaveDevice_None;
        m_saveFile = QsysObjectName(Text(objd->Save_File_Lib_Name, 10, ccsid),
                                    Text(objd->Save_File_Name, 10, ccsid));
        m_saveLabel = Text(objd->Save_Label, 17, ccsid).trim();
        m_versionWhereCreated = OS400Version(Text(objd->System_Level, 9, ccsid));
        m_compiler = Text(objd->Compiler, 16, ccsid).trim();
        m_objectLevel = Text(objd->Object_Level, 8, ccsid).trim();
        m_userChanged = Text(&objd->User_Changed, 1, ccsid).getAt(0) == 0x0031;
        m_licensedProgram = Text(objd->Licensed_Program, 16, ccsid).trim();
        m_programTemporaryFix = Text(objd->PTF, 10, ccsid).trim();
        m_authorizedProgramAnalysisReport = Text(objd->APAR, 10, ccsid).trim();
        m_lastUsedTime = dateTextToTime(objd->Last_Used_Date, ccsid, calendar);
        m_usageInformationUpdated = Text(&objd->Usage_Information_Update, 1, ccsid).getAt(0) == 0x0059;
        m_daysUsed = N32_TO_H(objd->Days_Used_Count);
        m_size = static_cast<uint64_t>(N32_TO_H(objd->Object_Size)) *
            static_cast<uint64_t>(N32_TO_H(objd->Object_Size_Multiplier));
        switch (Text(&objd->Object_Compress_Status, 1, ccsid).getAt(0))
        {
        case 0x0059:
            m_compression = Compression_Compressed;
            break;
        case 0x004e:
            m_compression = Compression_PermanentlyDecompressedAndCompressible;
            break;
        case 0x0058:
            m_compression = Compression_PermanentlyDecompressedAndNotCompressible;
            break;
        case 0x0054:
            m_compression = Compression_TemporarilyDecompressed;
            break;
        default:
            m_compression = Compression_NotDetermined;
            break;
        }
        m_allowChangeByProgram = Text(&objd->Allow_Change, 1, ccsid).getAt(0) == 0x0031;
        m_changedByProgram = Text(&objd->Changed_By_Program, 1, ccsid).getAt(0) == 0x0031;
        m_userDefinedAttribute = Text(objd->User_Defined_Attr, 10, ccsid).trim();
        m_overflowedAsp = Text(&objd->Overflow_ASP_Indicator, 1, ccsid).getAt(0) == 0x0031;
        m_saveActiveTime = dateTimeTextToTime(objd->Save_Active_Date_Time, ccsid, calendar);
        m_auditMode = AttributeUtility::getAuditMode(objd->Object_Audit_Value, ccsid);
        m_group = Text(objd->Primary_Group, 10, ccsid).trim();
        m_journaled = Text(&objd->Journal_Status, 1, ccsid).getAt(0) == 0x0031;
        m_journalName = QsysObjectName(Text(objd->Journal_Library, 10, ccsid),
                                       Text(objd->Journal_Name, 10, ccsid));
        m_journalsBeforeImage = Text(&objd->Journal_Images, 1, ccsid).getAt(0) == 0x0031;
        m_journalOmitOpenCloseEntries = Text(&objd->Journal_Entries_Omitted, 1, ccsid).getAt(0) == 0x0031;
        m_journalStartTime = dateTimeTextToTime(objd->Journal_Start_Date_Time, ccsid, calendar);
        m_signed = Text(&objd->Digitally_Signed, 1, ccsid).getAt(0) == 0x0031;
        m_libraryAsp = N32_TO_H(objd->Lib_Aux_Storage_Pool_Number);
        m_aspDevice = Text(objd->Object_Aux_Storage_Pool_Device, 10, ccsid).trim();
        m_libraryAspDevice = Text(objd->Lib_Aux_Storage_Pool_Device, 10, ccsid).trim();
        m_systemSigned = Text(&objd->Digitally_Signed_Sys_Trust, 1, ccsid).getAt(0) == 0x0031;
        m_multipleSignatures = Text(&objd->Digitally_Signed_Multiple, 1, ccsid).getAt(0) == 0x0031;
        m_primaryAssociatedSpaceSize = N32_TO_H(objd->Associated_Space_Size);
        switch (Text(&objd->Optimum_Space_Alignment, 1, ccsid).getAt(0))
        {
        case 0x0030:
            m_spaceAlignment = SpaceAlignment_NonOptimal;
            break;
        case 0x0031:
            m_spaceAlignment = SpaceAlignment_Optimal;
            break;
        default:
            m_spaceAlignment = SpaceAlignment_None;
            break;
        }
        m_auxiliaryStoragePoolGroup = Text(objd->Object_ASP_Group, 10, ccsid).trim();
        m_libraryAspGroup = Text(objd->Library_ASP_Group, 10, ccsid).trim();
        m_startingJournalReceiverForApply = QsysObjectName(Text(objd->Starting_Jrn_Rcv_Lib, 10, ccsid),
                                                           Text(objd->Starting_Jrn_Rcv_For_Apply, 10, ccsid));
        m_startingJournalReceiverAspDevice = Text(objd->Starting_Jrn_Rcv_Lib_ASP_Dev, 10, ccsid).trim();
        m_startingJournalReceiverAspGroup = Text(objd->Starting_Jrn_Rcv_Lib_ASP_Group, 10, ccsid).trim();
    }
    catch (...)
    {
        ucal_close(calendar);
        throw;
    }
    ucal_close(calendar);
}

void QsysObject::resetAttributes()
{
    m_auxiliaryStoragePool = 0;
    m_owner = Text();
    m_domain = Domain_User;
    m_creationTime = 0;
    m_changeTime = 0;
    m_extendedAttribute = Text();
    m_description = Text();
    m_sourceFile = QsysObjectName();
    m_sourceMember = Text();
    m_sourceFileUpdatedTime = 0;
    m_savedTime = 0;
    m_restoredTime = 0;
    m_creator = Text();
    m_systemWhereCreated = Text();
    m_resetTime = 0;
    m_savedSize = 0;
    m_savedSequence = 0;
    m_storage = Storage_Free;
    m_saveCommand = Text();
    m_saveVolumeId = Text();
    m_saveDevice = SaveDevice_None;
    m_saveFile = QsysObjectName();
    m_saveLabel = Text();
    m_versionWhereCreated = OS400Version();
    m_compiler = Text();
    m_objectLevel = Text();
    m_userChanged = false;
    m_licensedProgram = Text();
    m_programTemporaryFix = Text();
    m_authorizedProgramAnalysisReport = Text();
    m_lastUsedTime = 0;
    m_usageInformationUpdated = false;
    m_daysUsed = 0;
    m_size = 0;
    m_compression = Compression_NotDetermined;
    m_allowChangeByProgram = false;
    m_changedByProgram = false;
    m_userDefinedAttribute = Text();
    m_overflowedAsp = false;
    m_saveActiveTime = 0;
    m_auditMode = AuditMode_NotAvailable;
    m_group = Text();
    m_journaled = false;
    m_journalName = QsysObjectName();
    m_journalsBeforeImage = false;
    m_journalOmitOpenCloseEntries = false;
    m_journalStartTime = 0;
    m_signed = false;
    m_libraryAsp = 0;
    m_aspDevice = Text();
    m_libraryAspDevice = Text();
    m_systemSigned = false;
    m_multipleSignatures = false;
    m_primaryAssociatedSpaceSize = 0;
    m_spaceAlignment = SpaceAlignment_None;
    m_auxiliaryStoragePoolGroup = Text();
    m_libraryAspGroup = Text();
    m_startingJournalReceiverForApply = QsysObjectName();
    m_startingJournalReceiverAspDevice = Text();
    m_startingJournalReceiverAspGroup = Text();
}

}
