#if !defined(SMILE_IFSATTRIBUTES_HPP__)
#define SMILE_IFSATTRIBUTES_HPP__

namespace smile
{

enum Qp0lAttr
{
    Qp0lAttr_OBJTYPE                        = 0,
    Qp0lAttr_DATA_SIZE                      = 1,
    Qp0lAttr_ALLOC_SIZE                     = 2,
    Qp0lAttr_EXTENDED_ATTR_SIZE             = 3,
    Qp0lAttr_CREATE_TIME                    = 4,
    Qp0lAttr_ACCESS_TIME                    = 5,
    Qp0lAttr_CHANGE_TIME                    = 6,
    Qp0lAttr_MODIFY_TIME                    = 7,
    Qp0lAttr_STG_FREE                       = 8,
    Qp0lAttr_CHECKED_OUT                    = 9,
    Qp0lAttr_LOCAL_REMOTE                   = 10,
    Qp0lAttr_AUTH                           = 11,
    Qp0lAttr_FILE_ID                        = 12,
    Qp0lAttr_ASP                            = 13,
    Qp0lAttr_DATA_SIZE_64                   = 14,
    Qp0lAttr_ALLOC_SIZE_64                  = 15,
    Qp0lAttr_USAGE_INFORMATION              = 16,
    Qp0lAttr_PC_READ_ONLY                   = 17,
    Qp0lAttr_PC_HIDDEN                      = 18,
    Qp0lAttr_PC_SYSTEM                      = 19,
    Qp0lAttr_PC_ARCHIVE                     = 20,
    Qp0lAttr_SYSTEM_ARCHIVE                 = 21,
    Qp0lAttr_CODEPAGE                       = 22,
    Qp0lAttr_FILE_FORMAT                    = 23,
    Qp0lAttr_UDFS_DEFAULT_FORMAT            = 24,
    Qp0lAttr_JOURNAL_INFORMATION            = 25,
    Qp0lAttr_ALWCKPWRT                      = 26,
    Qp0lAttr_CCSID                          = 27,
    Qp0lAttr_SIGNED                         = 28,
    Qp0lAttr_SYS_SIGNED                     = 29,
    Qp0lAttr_MULTI_SIGS                     = 30,
    Qp0lAttr_DISK_STG_OPT                   = 31,
    Qp0lAttr_MAIN_STG_OPT                   = 32,
    Qp0lAttr_DIR_FORMAT                     = 33,
    Qp0lAttr_AUDIT                          = 34,
    Qp0lAttr_CRTOBJSCAN                     = 35,
    Qp0lAttr_SCAN                           = 36,
    Qp0lAttr_SCAN_INFO                      = 37,
    Qp0lAttr_ALWSAV                         = 38,
    Qp0lAttr_RSTDRNMUNL                     = 39,
    Qp0lAttr_JOURNAL_EXTENDED_INFORMATION   = 40,
    Qp0lAttr_CRTOBJAUD                      = 41,
    Qp0lAttr_SYSTEM_USE                     = 42,
    Qp0lAttr_RESET_DATE                     = 200,
    Qp0lAttr_SUID                           = 300,
    Qp0lAttr_SGID                           = 301
};

}

#endif
