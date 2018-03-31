AC_INIT([Smile], [0:0:0])
AC_REVISION([$Revision$])
AC_CONFIG_MACRO_DIR([m4])

# Our supported options
AC_ARG_WITH([g++], [AS_HELP_STRING([--with-g++], [use g++])], SMILE_COMPILER=g++)
AC_ARG_WITH([sun-cc], [AS_HELP_STRING([--with-sun-cc], [use the Sun C++ compiler CC])], SMILE_COMPILER=sun-cc)
AC_ARG_WITH([xlc], [AS_HELP_STRING([--with-xlc], [use the IBM C++ compiler xlC_r])], SMILE_COMPILER=xlc)
AC_ARG_ENABLE([debug], [AS_HELP_STRING([--enable-debug], [compile with debugging information, assertions, and no optimizations])], SMILE_DEBUG=yes)
AC_ARG_ENABLE([bits], [AS_HELP_STRING([--enable-bits], [build for 32-bit or 64-bit support, must be 32 or 64])], SMILE_BITS="${enableval}")

# Our host
AC_CANONICAL_HOST

# Now the host operating system
MACHINE=[`echo ${host} | sed -e 's/^\([^-]*\)-.*$/\1/' -e 's/[0-9]86/x86/'`]
#AC_SUBST([MACHINE])
#AC_DEFINE_UNQUOTED(SMILE_`echo ${MACHINE} | sed 's/[-\.]/_/g'`)
OS=[`echo ${host} | sed -e 's/^[^-]*-[^-]*-\(.*\)$/\1/' -e 's/^\([^0-9]*\).*$/\1/'`]
AC_DEFINE_UNQUOTED([SMILE_OS], [${OS}], [the name of this operating system])
#AC_SUBST([OS])
#AC_DEFINE_UNQUOTED(SMILE_`echo ${OS} | sed 's/[-\.]/_/g'`)
OS_VERSION=[`echo ${host} | sed -e 's/^[^-]*-[^-]*-[^0-9]*\([0-9]*.*\)$/\1/' -e 's/[^0-9]//g'`]
#AC_SUBST([OS_VERSION])
#AC_DEFINE_UNQUOTED([SMILE_OS_VERSION], ${OS_VERSION})

# Take care of compilers
if test -z "${SMILE_COMPILER}" ; then
    case ${host_os} in
        *solaris*)
            SMILE_COMPILER=sun-cc
            if test -z "${SMILE_BITS}" && test "${MACHINE}" = sparc ; then
                SMILE_BITS=64
            fi
            ;;
        *aix*)
            SMILE_COMPILER=xlc
            if test -z "${SMILE_BITS}" ; then
                SMILE_BITS=64
            fi
            ;;
        *)
            SMILE_COMPILER=g++
            if test -z "${SMILE_BITS}" ; then
                SMILE_BITS=32
            fi
            ;;
    esac
fi

# Make sure the bits are okay
if test "${SMILE_BITS}" = "32" || test "${SMILE_BITS}" = "64" ; then
    AC_MSG_NOTICE([building for ${SMILE_BITS} bits])
else
    AC_MSG_ERROR([The bitness of the build could not be determined. Please set --enable-bits=32 or --enable-bits=64.])
fi

# -g for debug works on all supported compilers
if test -z "${CDBGFLAGS}" ; then
    CDBGFLAGS="-g"
fi

case "${SMILE_COMPILER}" in
    g++)
        AC_MSG_NOTICE([configuring for g++])
        CXX=g++
        AC_CHECK_PROG([SMILE_COMPILER_FOUND], [${CXX}], [yes])
        if test "${SMILE_COMPILER_FOUND}" != "yes" ; then
            AC_MSG_ERROR([the compiler ${CXX} was not found in PATH])
        fi
        if test -z "${COPTFLAGS}" ; then
            COPTFLAGS="-O2"
        fi
        CXXFLAGS="${CXXFLAGS} -pthread -Wall"
        CXXFLAGS=[`echo ${CXXFLAGS} | sed -e 's/-O[0-9]//g' -e 's/-g//g'`]
        if test "${SMILE_BITS}" = "32" ; then
            CXXFLAGS="${CXXFLAGS} -m32"
        else
            CXXFLAGS="${CXXFLAGS} -m64"
        fi
        DEBUGGER=gdb
        SMILE_WSTR_BASE="L"
        SMILE_WSTR_UCHAR_CAST="s"
        ;;
    sun-cc)
        AC_MSG_NOTICE([configuring for the Sun C++ compiler, CC])
        CXX=CC
        CC=cc
        AC_CHECK_PROG([SMILE_COMPILER_FOUND], [${CXX}], [yes])
        if test "${SMILE_COMPILER_FOUND}" != "yes" ; then
            AC_MSG_ERROR([the compiler ${CXX} was not found in PATH])
        fi
        if ! CC -V 2>&1 | grep "Sun C" 2>&1 > /dev/null ; then
            AC_MSG_ERROR([CC is not actually a compiler from Sun])
        fi
        if test -z "${COPTFLAG}" ; then
            COPTFLAGS="-fast"
        fi
        CXXFLAGS=[`echo ${CXXFLAGS} | sed -e 's/-g//g'`]
        CXXFLAGS="${CXXFLAGS} -xtarget=generic -mt -xustr=ascii_utf16_ushort"
        if test "${SMILE_BITS}" = "32" ; then
            CXXFLAGS="${CXXFLAGS} -m32"
        else
            CXXFLAGS="${CXXFLAGS} -m64"
        fi
        DEBUGGER=dbx
        SMILE_WSTR_BASE="U"
        SMILE_WSTR_UCHAR_CAST="s"
        ;;
    xlc)
        AC_MSG_NOTICE([configuring for the IBM C++ compiler, xlC_r])
        CXX=xlC_r
        CC=xlc_r
        AC_CHECK_PROG([SMILE_COMPILER_FOUND], [${CXX}], [yes])
        if test "${SMILE_COMPILER_FOUND}" != "yes" ; then
            AC_MSG_ERROR([the compiler ${CXX} was not found in PATH])
        fi
        if ! xlC_r -qversion 2>&1 | grep -e 'C for AIX' -e 'IBM XL C/C++' 2>&1 > /dev/null ; then
            AC_MSG_ERROR([xlC_r is not actually a compiler from IBM])
        fi
        if test -z "${COPTFLAG}" ; then
            COPTFLAG="-qoptimize"
        fi
        CXXFLAGS=[`echo ${CXXFLAGS} | sed -e 's/-g//g'`]
        CXXFLAGS="${CXXFLAGS} -qrtti -qthreaded -qsuppress=1540-1103 -qmaxmem=-1 -qutf"
        if test "${SMILE_BITS}" = "32" ; then
            CXXFLAGS="${CXXFLAGS} -q32"
        else
            CXXFLAGS="${CXXFLAGS} -q64"
        fi
        DEBUGGER=dbx
        SMILE_WSTR_BASE="u"
        SMILE_WSTR_UCHAR_CAST="reinterpret_cast<const UChar*>(s)"
        ;;
esac
AC_SUBST([DEBUGGER])
AC_SUBST([SMILE_WSTR_BASE])
AC_SUBST([SMILE_WSTR_UCHAR_CAST])

# Set up debugging
if test "${SMILE_DEBUG}" = "yes" ; then
    CXXFLAGS="${CXXFLAGS} ${CDBGFLAGS}"
else
    CPPFLAGS="${CPPFLAGS} -DNDEBUG"
    CXXFLAGS="${CXXFLAGS} ${COPTFLAGS}"
fi

# Do automake
AM_INIT_AUTOMAKE([foreign])
AM_MAINTAINER_MODE

# Now get C++
AC_LANG([C++])

# Libtool junk
LT_INIT
LT_LANG([C++])

# Set up options specific to different operating systems
# Note that AIX is assumed to be the operating system whenever
# IBM's C++ compiler is being used, so all options for AIX are
# handled above in the compiler section.
case "${OS}" in
linux-gnu*)
    LIBS="${LIBS} -lpthread -ldl"
    if test "${SMILE_COMPILER}" = "sun-cc" ; then
        CPPFLAGS="${CPPFLAGS} -D_XOPEN_SOURCE=600"
    fi
    ;;
solaris)
    LIBS="${LIBS} -lsocket -lnsl"
    TEST_LIBS="-lrt"
    CPPFLAGS="${CPPFLAGS} -D_POSIX_PTHREAD_SEMANTICS"
    ;;
darwin)
    CPPGUIFLAGS="-FCocoa"
    LDGUIFLAGS="-framework Cocoa -lobjc"
    CXXGUI="gcc -x objective-c++"
    LDGUI="g++"
    INGREDIENTS="cocoa"
    ;;
aix)
    AR="${AR} -X ${SMILE_BITS}"
    NM="${NM} -X ${SMILE_BITS}"
    ;;
esac
AC_SUBST([CPPGUIFLAGS])
AC_SUBST([LDGUIFLAGS])
AC_SUBST([CXXGUI])
AC_SUBST([LDGUI])
AC_SUBST([INGREDIENTS])
AC_SUBST([TEST_LIBS])

# Run checks with C++
AC_PROG_CXX

# Check endianness
AC_C_BIGENDIAN(AC_DEFINE([SMILE_BIG_ENDIAN], [1], [big endian]))

# Get the name demangler going
if test "${ac_compiler_gnu}" = "yes" ; then
    AC_CHECK_HEADERS([cxxabi.h], HAVE_CXXABI_H=yes, AC_MSG_WARN([Type name demangling will not be performed]))
    if test "${HAVE_CXXABI_H}" = "yes"; then
        AC_MSG_CHECKING([abi::__cxa_demangle])
    	AC_TRY_COMPILE([#include <cxxabi.h>
using abi::__cxa_demangle;], [], SMILE_CXA_NAMESPACE=abi)
    	if test -z "${SMILE_CXA_NAMESPACE}"; then
    	    AC_MSG_RESULT([no])
    	    AC_MSG_CHECKING([__cxxabiv1::__cxa_demangle])
                AC_TRY_COMPILE([#include <cxxabi.h>
    using __cxxabiv1::__cxa_demangle;], [], SMILE_CXA_NAMESPACE=__cxxabiv1)
    	fi
    	if test -n "${SMILE_CXA_NAMESPACE}"; then
    	    AC_MSG_RESULT([yes])
    	    AC_DEFINE_UNQUOTED([SMILE_CXA_NAMESPACE], [${SMILE_CXA_NAMESPACE}], [namespace for cxa])
    	    AC_DEFINE_UNQUOTED([HOMINID_CXA_NAMESPACE], [${SMILE_CXA_NAMESPACE}], [namespace for cxa])
    	    AC_DEFINE([SMILE_GCC_DEMANGLING], [1], [gcc style demangling])
    	    AC_DEFINE([HOMINID_GCC_DEMANGLING], [1], [gcc style demangling])
    	else
    	    AC_MSG_RESULT([no])
    	    AC_MSG_WARN([Type name demangling will not be performed])
    	fi
    fi
fi

# Check for ICU
AC_PATH_PROG([ICU_CONFIG], [icu-config])
if test -z "${ICU_CONFIG}" || ! ${ICU_CONFIG} --exists ; then
    AC_CHECK_HEADERS([unicode/ucnv.h unicode/ustring.h unicode/uchar.h unicode/ucal.h unicode/uloc.h unicode/urename.h unicode/udat.h unicode/ustdio.h unicode/uregex.h], , AC_MSG_ERROR([ICU headers are required]))
    LIBS="${LIBS} -licuio -licui18n -licuuc -licudata"
    DESIRED_ICU_FUNCTIONS="ucnv_open ucnv_openCCSID ucnv_close ucnv_getName ucnv_toUChars ucnv_fromUChars ucnv_getCCSID u_strCompare u_strCaseCompare u_strncpy u_strToUpper u_isWhitespace ucal_open ucal_close ucal_setDateTime ucal_getMillis uloc_getDefault uloc_getName u_isdigit u_memset u_memchr u_memrchr udat_open udat_close udat_format u_strlen u_strtok_r ucal_getNow u_getIntPropertyValue u_strchr u_fopen u_fclose u_fgets uregex_openC uregex_close uregex_setText uregex_matches uregex_group u_isblank uregex_groupCount u_strFromWCS"
    AC_CHECK_FUNCS([${DESIRED_ICU_FUNCTIONS}], , NO_ICU_FUNCS="yes"; break)
    if test "${NO_ICU_FUNCS}" = "yes" ; then
        URENAME_H=`echo "#include <unicode/urename.h>" | cpp -H 2>&1 | awk '$1 == "." && $2 ~ /urename/ { print $2 }'`
        ICU_FUNCTIONS=""
        for FUNC in ${DESIRED_ICU_FUNCTIONS}
        do
            ICU_FUNCTIONS="${ICU_FUNCTIONS} `awk '$2 == "'${FUNC}'" { print $3 }' ${URENAME_H}`"
        done
        AC_CHECK_FUNCS([${ICU_FUNCTIONS}], , AC_MSG_ERROR([ICU functions are required]))
    fi
else
    CPPFLAGS="${CPPFLAGS} `${ICU_CONFIG} --cppflags-searchpath`"
    LIBS="${LIBS} `${ICU_CONFIG} --ldflags-searchpath` `${ICU_CONFIG} --ldflags-icuio` `${ICU_CONFIG} --ldflags-libsonly`"
fi

# Check for OpenSSL
OPENSSL_HEADERS="openssl/rand.h openssl/evp.h openssl/err.h openssl/bio.h openssl/ssl.h"
for FILE in ${OPENSSL_HEADERS}
do
    AC_CHECK_FILE([/usr/local/ssl/include/${FILE}], , NO_DEFAULT_SSL=true)
    if test "${NO_DEFAULT_SSL}" = "true" ; then
        break
    fi
done
if test "${NO_DEFAULT_SSL}" != "true" ; then
    CPPFLAGS="${CPPFLAGS} -I/usr/local/ssl/include"
    LDFLAGS="${LDFLAGS} -L/usr/local/ssl/lib"
fi
AC_CHECK_HEADERS([${OPENSSL_HEADERS}], , AC_MSG_ERROR([OpenSSL headers are required]))
LIBS="${LIBS} -lssl -lcrypto"
AC_CHECK_FUNCS([RAND_bytes EVP_sha1 EVP_DigestInit EVP_DigestUpdate EVP_DigestFinal EVP_MD_CTX_cleanup ERR_get_error ERR_error_string EVP_CIPHER_CTX_init EVP_CIPHER_CTX_cleanup EVP_CIPHER_CTX_set_padding EVP_des_ecb EVP_bf_ecb EVP_EncryptInit_ex EVP_EncryptUpdate EVP_EncryptFinal_ex SSL_load_error_strings ERR_load_BIO_strings BIO_new_connect BIO_read BIO_write SSL_CTX_new SSL_CTX_free SSLv23_client_method], , AC_MSG_ERROR([OpenSSL functions are required]))
AC_CHECK_DECLS([EVP_CIPHER_key_length,EVP_CIPHER_block_size,OpenSSL_add_all_algorithms,BIO_get_fd,BIO_do_connect,BIO_get_ssl,SSL_set_mode,BIO_set_conn_hostname], , AC_MSG_ERROR([OpenSSL macros are required]), [#include <openssl/evp.h>
#include <openssl/ssl.h>])

# Check for doxygen
AC_PATH_PROG([DOXYGEN], [doxygen], [], [/Applications/Doxygen.app/Contents/Resources:${PATH}])
AC_PATH_PROG([DOT], [dot], [], [/Applications/Doxygen.app/Contents/Resources:${PATH}])
AC_PATH_PROG([MSCGEN], [mscgen], [], [/Applications/Doxygen.app/Contents/Resources:${PATH}])
DOT=`echo ${DOT} | sed -e 's/dot$//'`
MSCGEN=`echo ${MSCGEN} | sed -e 's/mscgen$//'`
if test -z "${DOT}"; then
    HAVE_DOT=NO
else
    HAVE_DOT=YES
fi
AC_SUBST([HAVE_DOT])

# Get ready to create output
AC_CONFIG_FILES([Makefile
                 tools/Makefile
                 tools/hominid/Makefile
                 test/Makefile
                 smile/TextMacros.hpp
                 doc-config/Doxyfile
                 tools/debug-test])
AC_CONFIG_HEADERS([Config.hpp
                   tools/hominid/Config.hpp])
AC_OUTPUT
