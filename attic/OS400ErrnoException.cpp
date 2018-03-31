#include <smile/OS400ErrnoException.hpp>
#include <sstream>
#include <algorithm>

namespace smile
{

OS400ErrnoException::OS400ErrnoException(const char* const fileName,
                                         int lineNumber,
                                         const char* const msg,
                                         int err)
    : ErrnoException(fileName, lineNumber, msg, err)
{
    Errno converted = convertErrno(err);
    std::ostringstream stream;
    stream << msg << ": " << getErrnoMessage(converted);
    if (converted == Errno_ESMILEUNKNOWN)
        stream << " (" << err << ")";
    m_what = new char[stream.str().length() + 1];
    strcpy(m_what, stream.str().c_str());
}

OS400ErrnoException::OS400ErrnoException(const OS400ErrnoException& except)
    : ErrnoException(except)
{
}

OS400ErrnoException& OS400ErrnoException::operator= (const OS400ErrnoException& except)
{
    ErrnoException::operator=(except);
    return *this;
}

OS400ErrnoException::Errno OS400ErrnoException::convertErrno(int rawErrno)
{
    static int validErrnos[] =
    {
        3001, 3002, 3003, 3004, 3005, 3006, 3007, 3008, 3009, 3010,
        3011, 3012, 3013, 3014, 3015, 3017, 3018, 3019, 3020, 3021,
        3022, 3025, 3026, 3027, 3028, 3029, 3040, 3041, 3042, 3043,
        3044, 3080, 3081, 3101, 3102, 3401, 3403, 3404, 3405, 3406,
        3407, 3408, 3409, 3415, 3418, 3419, 3420, 3421, 3422, 3423,
        3424, 3425, 3426, 3427, 3428, 3429, 3430, 3431, 3432, 3433,
        3434, 3435, 3436, 3437, 3438, 3439, 3440, 3441, 3442, 3443,
        3444, 3445, 3446, 3447, 3448, 3450, 3452, 3453, 3455, 3456,
        3457, 3459, 3460, 3462, 3463, 3464, 3465, 3466, 3467, 3468,
        3469, 3470, 3471, 3472, 3474, 3475, 3476, 3477, 3478, 3479,
        3480, 3481, 3482, 3484, 3485, 3486, 3487, 3488, 3489, 3490,
        3491, 3492, 3493, 3494, 3495, 3496, 3497, 3498, 3499, 3500,
        3501, 3502, 3503, 3504, 3505, 3506, 3507, 3509, 3510, 3511,
        3512, 3513, 3515, 3516, 3517, 3520, 3523, 3524, 3525, 3526,
        3527, 3528, 3529, 3530, 3531, 3532, 3533, 3534, 3535, 3536,
        3540, 3543, 3544, 3545, 3546
    };
    static int* end = validErrnos + (sizeof(validErrnos) / sizeof(validErrnos[0]));

    int* found = std::find(validErrnos, end, rawErrno);
    return (found == end) ? Errno_ESMILEUNKNOWN : static_cast<Errno>(*found);
}

Exception* OS400ErrnoException::duplicate() const
{
    return new OS400ErrnoException(*this);
}

const char* OS400ErrnoException::getErrnoMessage(Errno err) const
{
    switch (err)
    {
    case Errno_EDOM:
        return "A domain error occurred in a math function";
    case Errno_ERANGE:
        return "A range error occurred";
    case Errno_ETRUNC:
        return "Data was truncated on an input, output or update operation";
    case Errno_ENOTOPEN:
        return "File is not open";
    case Errno_ENOTREAD:
        return "File is not open for read operations";
    case Errno_EIO:
        return "Input/output error";
    case Errno_ENODEV:
        return "No such device";
    case Errno_ERECIO:
        return "Cannot get single character for files opened for record I/O";
    case Errno_ENOTWRITE:
        return "File is not open for write operations";
    case Errno_ESTDIN:
        return "The standard in stream cannot be opened";
    case Errno_ESTDOUT:
        return "The standard out stream cannot be opened";
    case Errno_ESTDERR:
        return "The standard error stream cannot be opened";
    case Errno_EBADSEEK:
        return "The positioning parameter in fseek is not correct";
    case Errno_EBADNAME:
        return "The object name specified is not correct";
    case Errno_EBADMODE:
        return "The type variable specified on the open function is not correct";
    case Errno_EBADPOS:
        return "The position specifier is not correct";
    case Errno_ENOPOS:
        return "There is no record at the specified position";
    case Errno_ENUMMBRS:
        return "Attempted to use ftell on multiple members";
    case Errno_ENUMRECS:
        return "The current record position is too long for ftell";
    case Errno_EINVAL:
        return "The value specified for the argument is incorrect";
    case Errno_EBADFUNC:
        return "Function parameter in the signal function is not set";
    case Errno_ENOENT:
        return "No such path or directory";
    case Errno_ENOREC:
        return "Record is not found";
    case Errno_EPERM:
        return "The operation is not permitted";
    case Errno_EBADDATA:
        return "Message data is invalid";
    case Errno_EBUSY:
        return "Resource busy";
    case Errno_EBADOPT:
        return "Option specified is not valid";
    case Errno_ENOTUPD:
        return "File is not open for update operations";
    case Errno_ENOTDLT:
        return "File is not open for delete operations";
    case Errno_EPAD:
        return "The number of characters written is shorter than the expected record length";
    case Errno_EBADKEYLN:
        return "A length that was not valid was specified for the key";
    case Errno_EPUTANDGET:
        return "A read operation should not immediately follow a write operation";
    case Errno_EGETANDPUT:
        return "A write operation should not immediately follow a read operation";
    case Errno_EIOERROR:
        return "A nonrecoverable I/O error occurred";
    case Errno_EIORECERR:
        return "A recoverable I/O error occurred";
    case Errno_EACCESS:
        return "Permission denied";
    case Errno_ENOTDIR:
        return "Not a directory";
    case Errno_ENOSPC:
        return "No space is available";
    case Errno_EXDEV:
        return "Improper link";
    case Errno_EAGAIN_OR_WOULDBLOCK:
        return "Operation would have caused the process to be suspended";
    case Errno_EINTR:
        return "Interrupted function call";
    case Errno_EFAULT:
        return "The address used for a function was not correct";
    case Errno_ETIME:
        return "Operation timed out";
    case Errno_ENXIO:
        return "No such device or address";
    case Errno_EAPAR:
        return "Possible APAR condition or hardware failure";
    case Errno_ERECURSE:
        return "Recursive attempt rejected";
    case Errno_EADDRINUSE:
        return "Address already in use";
    case Errno_EADDRNOTAVAIL:
        return "Address is not available";
    case Errno_EAFNOSUPPORT:
        return "The type of socket is not supported in this protocol family";
    case Errno_EALREADY:
        return "The operation is already in progress";
    case Errno_ECONNABORTED:
        return "Connection ended abnormally";
    case Errno_ECONNREFUSED:
        return "A remote host refused an attempted connect operation";
    case Errno_ECONNRESET:
        return "A connection with a remote socket was reset by that socket";
    case Errno_EDESTADDRREQ:
        return "Operation requires a destination address";
    case Errno_EHOSTDOWN:
        return "A remote host is not available";
    case Errno_EHOSTUNREACH:
        return "A route to the remote host is unavailable";
    case Errno_EINPROGRESS:
        return "Operation in progress";
    case Errno_EISCONN:
        return "A connection has already been established";
    case Errno_EMSGSIZE:
        return "Message size is out of range";
    case Errno_ENETDOWN:
        return "The network is not currently available";
    case Errno_ENETRESET:
        return "A socket is connected to a host that is not currently available";
    case Errno_ENETUNREACH:
        return "Cannot reach the destination network";
    case Errno_ENOBUFS:
        return "There is not enough buffer space for the requested operation";
    case Errno_ENOPROTOOPT:
        return "The protocol does not support the specified option";
    case Errno_ENOTCONN:
        return "The requested operation requires a connection";
    case Errno_ENOTSOCK:
        return "The specified descriptor does not reference a socket";
    case Errno_ENOTSUP:
        return "The operation is not supported";
    case Errno_EPFNOSUPPORT:
        return "The procotol family is not supported";
    case Errno_EPROTONOSUPPORT:
        return "No protocol of the specified type and domain exists";
    case Errno_EPROTOTYPE:
        return "The socket type or protocols are not compatible";
    case Errno_ERCVDERR:
        return "An error indication was sent by the peer program";
    case Errno_ESHUTDOWN:
        return "Cannot send data after a shutdown";
    case Errno_ESOCKTNOSUPPORT:
        return "The specified socket type is not supported";
    case Errno_ETIMEDOUT:
        return "A remote host did not respond within the timeout period";
    case Errno_EUNATCH:
        return "The protocol required to support the specified address family is not available at this time";
    case Errno_EBADF:
        return "Descriptor is not valid";
    case Errno_EMFILE:
        return "Too many open files for this process";
    case Errno_ENFILE:
        return "Too many open files in the system";
    case Errno_EPIPE:
        return "Broken pipe";
    case Errno_ECANCEL:
        return "Operation cancelled";
    case Errno_EEXIST:
        return "Object exists";
    case Errno_EDEADLK:
        return "Resource deadlock avoided";
    case Errno_ENOMEM:
        return "Storage allocation request failed";
    case Errno_EOWNERTERM:
        return "The synchronization object no longer exists because the owner is no longer running";
    case Errno_EDESTROYED:
        return "The synchronization object was destroyed, or the object no longer exists";
    case Errno_ETERM:
        return "Operation was terminated";
    case Errno_ENOENT1:
        return "No such file or directory";
    case Errno_ENOEQFLOG:
        return "Object is already linked to a dead directory";
    case Errno_EEMPTYDIR:
        return "Directory is empty";
    case Errno_EMLINK:
        return "Maximum link count for a file was exceeded";
    case Errno_ESPIPE:
        return "Seek request not supported for object";
    case Errno_ENOSYS:
        return "Function not implemented";
    case Errno_EISDIR:
        return "Specified target is a directory";
    case Errno_EROFS:
        return "Read-only file system";
    case Errno_EUNKNOWN:
        return "Unknown system state";
    case Errno_EITERBAD:
        return "Iterator is not valid";
    case Errno_EITERSTE:
        return "Iterator is in wrong state for operation";
    case Errno_EHRICLSBAD:
        return "HRI class is not valid";
    case Errno_EHRICLBAD:
        return "HRI subclass is not valid";
    case Errno_EHRITYPBAD:
        return "HRI type is not valid";
    case Errno_ENOTAPPL:
        return "Data requested is not applicable";
    case Errno_EHRIREQTYP:
        return "HRI request type is not valid";
    case Errno_EHRINAMEBAD:
        return "HRI resource name is not valid";
    case Errno_EDAMAGE:
        return "A damaged object was encountered";
    case Errno_ELOOP:
        return "A loop exists in the symbolic links";
    case Errno_ENAMETOOLONG:
        return "A path name is too long";
    case Errno_ENOLCK:
        return "No locks are available";
    case Errno_ENOTEMPTY:
        return "A directory is not empty";
    case Errno_ENOSYSRSC:
        return "System resources are not available";
    case Errno_ECONVERT:
        return "Conversion error";
    case Errno_E2BIG:
        return "Argument list is too long";
    case Errno_EILSEQ:
        return "Conversion stopped due to input character that does not belong to the input codeset";
    case Errno_ETYPE:
        return "Object type mismatch";
    case Errno_EBADDIR:
        return "Attempted to reference a directory that was not found or was destroyed";
    case Errno_EBADOBJ:
        return "Attempted to reference an object that was not found, was destroyed, or was damaged";
    case Errno_EIDXINVAL:
        return "Data space index used as a directory is not valid";
    case Errno_ESOFTDAMAGE:
        return "Object has soft damage";
    case Errno_ENOTENROLL:
        return "User is not enrolled in system distribution directory";
    case Errno_EOFFLINE:
        return "Object is suspended";
    case Errno_EROOBJ:
        return "Object is read-only";
    case Errno_EEAHDDSI:
        return "Hard damage on extended attribute data space index";
    case Errno_EEASDDSI:
        return "Soft damage on extended attribute data space index";
    case Errno_EEAHDDS:
        return "Hard damage on extended attribute data space";
    case Errno_EEASDDS:
        return "Soft damage on extended attribute data space";
    case Errno_EEADUPRC:
        return "Duplicate extended attribute record";
    case Errno_ELOCKED:
        return "Area being read from or written to is locked";
    case Errno_EFBIG:
        return "Object too large";
    case Errno_EIDRM:
        return "The semaphore, shared memory or message queue identifier is removed from the system";
    case Errno_ENOMSG:
        return "The queue does not contain a message of the desired type and msgflg logically ANDed with IPC_NOWAIT";
    case Errno_EFILECVT:
        return "File ID conversion of a directory failed";
    case Errno_EBADFID:
        return "A file ID could not be assigned when linking a file to a directory";
    case Errno_ESTALE:
        return "File or object handle rejected by server";
    case Errno_ESRCH:
        return "No such process";
    case Errno_ENOTSIGINIT:
        return "Process is not enabled for signals";
    case Errno_ECHILD:
        return "No child process";
    case Errno_EBADH:
        return "Handle is not valid";
    case Errno_ETOOMANYREFS:
        return "The operation would have exceeded the maximum number of references for a descriptor";
    case Errno_ENOTSAFE:
        return "Function is not allowed";
    case Errno_EOVERFLOW:
        return "Object is too large to process";
    case Errno_EJRNDAMAGE:
        return "Journal is damaged";
    case Errno_EJRNINACTIVE:
        return "Journal is inactive";
    case Errno_EJRNRCVSPC:
        return "Journal space or system storage error";
    case Errno_EJRNRMT:
        return "Journal is remote";
    case Errno_ENEWJRNRCV:
        return "New journal receiver is needed";
    case Errno_ENEWJRN:
        return "New journal is needed";
    case Errno_EJOURNALED:
        return "Object already journaled";
    case Errno_EJRNENTTOOLONG:
        return "Journal entry is too large to send";
    case Errno_EDATALINK:
        return "Object is a datalink object";
    case Errno_ENOTAVAIL:
        return "Independent Auxiliary Storage Pool (ASP) is not available";
    case Errno_ENOTTY:
        return "I/O control operation is not appropriate";
    case Errno_EFBIG2:
        return "Attempt to write or truncate file past its sort file size limit";
    case Errno_ETXTBSY:
        return "Text file is busy";
    case Errno_EASPGRPNOTSET:
        return "ASP group not set for thread";
    case Errno_ERESTART:
        return "A system call was interrupted and may be restarted";
    case Errno_ESCANFAILURE:
        return "Object had scan failure";
    default:
        break;
    }
    return "An error unknown to smile occurred";
}

}
