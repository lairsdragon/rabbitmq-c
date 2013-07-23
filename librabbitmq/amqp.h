/* vim:set ft=c ts=2 sw=2 sts=2 et cindent: */
/** \file */
/*
 * ***** BEGIN LICENSE BLOCK *****
 * Version: MIT
 *
 * Portions created by Alan Antonuk are Copyright (c) 2012-2013
 * Alan Antonuk. All Rights Reserved.
 *
 * Portions created by VMware are Copyright (c) 2007-2012 VMware, Inc.
 * All Rights Reserved.
 *
 * Portions created by Tony Garnock-Jones are Copyright (c) 2009-2010
 * VMware, Inc. and Tony Garnock-Jones. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * ***** END LICENSE BLOCK *****
 */

#ifndef AMQP_H
#define AMQP_H

/** \cond HIDE_FROM_DOXYGEN */

#ifdef __cplusplus
#define AMQP_BEGIN_DECLS extern "C" {
#define AMQP_END_DECLS }
#else
#define AMQP_BEGIN_DECLS
#define AMQP_END_DECLS
#endif



/*
 * \internal
 * Important API decorators:
 *  AMQP_PUBLIC_FUNCTION - a public API function
 *  AMQP_PUBLIC_VARIABLE - a public API external variable
 *  AMQP_CALL - calling convension (used on Win32)
 */

#if defined(_WIN32) && defined(_MSC_VER)
# if defined(AMQP_BUILD) && !defined(AMQP_STATIC)
#  define AMQP_PUBLIC_FUNCTION __declspec(dllexport)
#  define AMQP_PUBLIC_VARIABLE __declspec(dllexport) extern
# else
#  define AMQP_PUBLIC_FUNCTION
#  if !defined(AMQP_STATIC)
#   define AMQP_PUBLIC_VARIABLE __declspec(dllimport) extern
#  else
#   define AMQP_PUBLIC_VARIABLE extern
#  endif
# endif
# define AMQP_CALL __cdecl

#elif defined(_WIN32) && defined(__BORLANDC__)
# if defined(AMQP_BUILD) && !defined(AMQP_STATIC)
#  define AMQP_PUBLIC_FUNCTION __declspec(dllexport)
#  define AMQP_PUBLIC_VARIABLE __declspec(dllexport) extern
# else
#  define AMQP_PUBLIC_FUNCTION
#  if !defined(AMQP_STATIC)
#   define AMQP_PUBLIC_VARIABLE __declspec(dllimport) extern
#  else
#   define AMQP_PUBLIC_VARIABLE extern
#  endif
# endif
# define AMQP_CALL __cdecl

#elif defined(_WIN32) && defined(__MINGW32__)
# if defined(AMQP_BUILD) && !defined(AMQP_STATIC)
#  define AMQP_PUBLIC_FUNCTION __declspec(dllexport)
#  define AMQP_PUBLIC_VARIABLE __declspec(dllexport) extern
# else
#  define AMQP_PUBLIC_FUNCTION
#  if !defined(AMQP_STATIC)
#   define AMQP_PUBLIC_VARIABLE __declspec(dllimport) extern
#  else
#   define AMQP_PUBLIC_VARIABLE extern
#  endif
# endif
# define AMQP_CALL __cdecl

#elif defined(_WIN32) && defined(__CYGWIN__)
# if defined(AMQP_BUILD) && !defined(AMQP_STATIC)
#  define AMQP_PUBLIC_FUNCTION __declspec(dllexport)
#  define AMQP_PUBLIC_VARIABLE __declspec(dllexport)
# else
#  define AMQP_PUBLIC_FUNCTION
#  if !defined(AMQP_STATIC)
#   define AMQP_PUBLIC_VARIABLE __declspec(dllimport) extern
#  else
#   define AMQP_PUBLIC_VARIABLE extern
#  endif
# endif
# define AMQP_CALL __cdecl

#elif defined(__GNUC__) && __GNUC__ >= 4
# include <sys/uio.h>
# define AMQP_PUBLIC_FUNCTION \
  __attribute__ ((visibility ("default")))
# define AMQP_PUBLIC_VARIABLE \
  __attribute__ ((visibility ("default"))) extern
# define AMQP_CALL
#else
# define AMQP_PUBLIC_FUNCTION
# define AMQP_PUBLIC_VARIABLE extern
# define AMQP_CALL
#endif

#if __GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1)
# define AMQP_DEPRECATED(function) \
  function __attribute__ ((__deprecated__))
#elif defined(_MSC_VER)
# define AMQP_DEPRECATED(function) \
  __declspec(deprecated) function
#else
# define AMQP_DEPRECATED(function)
#endif

/* Define ssize_t on Win32/64 platforms
   See: http://lists.cs.uiuc.edu/pipermail/llvmdev/2010-April/030649.html for details
   */
#if !defined(_W64)
#if !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && _MSC_VER >= 1300
#define _W64 __w64
#else
#define _W64
#endif
#endif

#ifdef _MSC_VER
#ifdef _WIN64
typedef __int64 ssize_t;
#else
typedef _W64 int ssize_t;
#endif
#endif

/** \endcond */

#include <stddef.h>
#include <stdint.h>

struct timeval;

AMQP_BEGIN_DECLS

/**
 * \def AMQP_VERSION_MAJOR
 *
 * Major library version number compile-time constant
 *
 * The major version is incremented when backwards incompatible API changes
 * are made.
 *
 * \sa AMQP_VERSION, AMQP_VERSION_STRING
 *
 * \since v0.4.0
 */

/**
 * \def AMQP_VERSION_MINOR
 *
 * Minor library version number compile-time constant
 *
 * The minor version is incremented when new APIs are added. Existing APIs
 * are left alone.
 *
 * \sa AMQP_VERSION, AMQP_VERSION_STRING
 *
 * \since v0.4.0
 */

/**
 * \def AMQP_VERSION_PATCH
 *
 * Patch library version number compile-time constant
 *
 * The patch version is incremented when library code changes, but the API
 * is not changed.
 *
 * \sa AMQP_VERSION, AMQP_VERSION_STRING
 *
 * \since v0.4.0
 */

/**
 * \def AMQP_VERSION_IS_RELEASE
 *
 * Version constant set to 1 for tagged release, 0 otherwise
 *
 * NOTE: versions that are not tagged releases are not guaranteed to be API/ABI
 * compatible with older releases, and may change commit-to-commit.
 *
 * \sa AMQP_VERSION, AMQP_VERSION_STRING
 *
 * \since v0.4.0
 */
/*
 * Developer note: when changing these, be sure to update SOVERSION constants
 *  in CMakeLists.txt and configure.ac
 */

#define AMQP_VERSION_MAJOR 0
#define AMQP_VERSION_MINOR 4
#define AMQP_VERSION_PATCH 0
#define AMQP_VERSION_IS_RELEASE 0


/**
 * \def AMQP_VERSION
 *
 * Packed version number
 *
 * AMQP_VERSION is a 4-byte unsigned integer with the most significant byte
 * set to AMQP_VERSION_MAJOR, the second most significant byte set to
 * AMQP_VERSION_MINOR, third most significant byte set to AMQP_VERSION_PATCH,
 * and the lowest byte set to AMQP_VERSION_IS_RELEASE.
 *
 * For example version 2.3.4 which is released version would be encoded as
 * 0x02030401
 *
 * \sa amqp_version_number() AMQP_VERSION_MAJOR, AMQP_VERSION_MINOR,
 *     AMQP_VERSION_PATCH, AMQP_VERSION_IS_RELEASE
 *
 * \since v0.4.0
 */
#define AMQP_VERSION ((AMQP_VERSION_MAJOR << 24) | \
                      (AMQP_VERSION_MINOR << 16) | \
                      (AMQP_VERSION_PATCH << 8)  | \
                      (AMQP_VERSION_IS_RELEASE))

/** \cond HIDE_FROM_DOXYGEN */
#define AMQ_STRINGIFY(s) AMQ_STRINGIFY_HELPER(s)
#define AMQ_STRINGIFY_HELPER(s) #s

#define AMQ_VERSION_STRING  AMQ_STRINGIFY(AMQP_VERSION_MAJOR) "." \
                            AMQ_STRINGIFY(AMQP_VERSION_MINOR) "." \
                            AMQ_STRINGIFY(AMQP_VERSION_PATCH)
/** \endcond */

/**
 * \def AMQP_VERSION_STRING
 *
 * Version string compile-time constant
 *
 * Non-released versions of the library will have "-pre" appended to the
 * version string
 *
 * \sa amqp_version()
 *
 * \since v0.4.0
 */
#if AMQP_VERSION_IS_RELEASE
# define AMQP_VERSION_STRING AMQ_VERSION_STRING
#else
# define AMQP_VERSION_STRING AMQ_VERSION_STRING "-pre"
#endif


/**
 * Returns the rabbitmq-c version as a packed integer.
 *
 * See \ref AMQP_VERSION
 *
 * \return packed 32-bit integer representing version of library at runtime
 *
 * \sa AMQP_VERSION, amqp_version()
 *
 * \since v0.4.0
 */
AMQP_PUBLIC_FUNCTION
uint32_t
AMQP_CALL amqp_version_number(void);

/**
 * Returns the rabbitmq-c version as a string.
 *
 * See \ref AMQP_VERSION_STRING
 *
 * \return a statically allocated string describing the version of rabbitmq-c.
 *
 * \sa amqp_version_number(), AMQP_VERSION_STRING, AMQP_VERSION
 *
 * \since v0.1
 */
AMQP_PUBLIC_FUNCTION
char const *
AMQP_CALL amqp_version(void);

/**
 * \def AMQP_DEFAULT_FRAME_SIZE
 *
 * Default frame size (128Kb)
 *
 * \sa amqp_login(), amqp_login_with_properties()
 *
 * \since v0.4.0
 */
#define AMQP_DEFAULT_FRAME_SIZE 131072

/**
 * \def AMQP_DEFAULT_MAX_CHANNELS
 *
 * Default maximum number of channels (0, no limit)
 *
 * \sa amqp_login(), amqp_login_with_properties()
 *
 * \since v0.4.0
 */
#define AMQP_DEFAULT_MAX_CHANNELS 0

/**
 * \def AMQP_DEFAULT_HEARTBEAT
 *
 * Default heartbeat interval (0, heartbeat disabled)
 *
 * \sa amqp_login(), amqp_login_with_properties()
 *
 * \since v0.4.0
 */
#define AMQP_DEFAULT_HEARTBEAT 0

/**
 * boolean type 0 = false, true otherwise
 *
 * \since v0.1
 */
typedef int amqp_boolean_t;

/**
 * Method number
 *
 * \since v0.1
 */
typedef uint32_t amqp_method_number_t;

/**
 * Bitmask for flags
 *
 * \since v0.1
 */
typedef uint32_t amqp_flags_t;

/**
 * Channel type
 *
 * \since v0.1
 */
typedef uint16_t amqp_channel_t;

/**
 * Buffer descriptor
 *
 * \since v0.1
 */
typedef struct amqp_bytes_t_ {
  size_t len;   /**< length of the buffer in bytes */
  void *bytes;  /**< pointer to the beginning of the buffer */
} amqp_bytes_t;

/**
 * Decimal data type
 *
 * \since v0.1
 */
typedef struct amqp_decimal_t_ {
  uint8_t decimals;   /**< the location of the decimal point */
  uint32_t value;     /**< the value before the decimal point is applied */
} amqp_decimal_t;

/**
 * AMQP field table
 *
 * An AMQP field table is a set of key-value pairs.
 * A key is a UTF-8 encoded string up to 128 bytes long, and are not null
 * terminated.
 * A value can be one of several different datatypes. \sa amqp_field_value_kind_t
 *
 * \sa amqp_table_entry_t
 *
 * \since v0.1
 */
typedef struct amqp_table_t_ {
  int num_entries;                      /**< length of entries array */
  struct amqp_table_entry_t_ *entries;  /**< an array of table entries */
} amqp_table_t;

/**
 * An AMQP Field Array
 *
 * A repeated set of field values, all must be of the same type
 *
 * \since v0.1
 */
typedef struct amqp_array_t_ {
  int num_entries;                      /**< Number of entries in the table */
  struct amqp_field_value_t_ *entries;  /**< linked list of field values */
} amqp_array_t;

/*
  0-9   0-9-1   Qpid/Rabbit  Type               Remarks
---------------------------------------------------------------------------
        t       t            Boolean
        b       b            Signed 8-bit
        B                    Unsigned 8-bit
        U       s            Signed 16-bit      (A1)
        u                    Unsigned 16-bit
  I     I       I            Signed 32-bit
        i                    Unsigned 32-bit
        L       l            Signed 64-bit      (B)
        l                    Unsigned 64-bit
        f       f            32-bit float
        d       d            64-bit float
  D     D       D            Decimal
        s                    Short string       (A2)
  S     S       S            Long string
        A                    Nested Array
  T     T       T            Timestamp (u64)
  F     F       F            Nested Table
  V     V       V            Void
                x            Byte array

Remarks:

 A1, A2: Notice how the types **CONFLICT** here. In Qpid and Rabbit,
         's' means a signed 16-bit integer; in 0-9-1, it means a
          short string.

 B: Notice how the signednesses **CONFLICT** here. In Qpid and Rabbit,
    'l' means a signed 64-bit integer; in 0-9-1, it means an unsigned
    64-bit integer.

I'm going with the Qpid/Rabbit types, where there's a conflict, and
the 0-9-1 types otherwise. 0-8 is a subset of 0-9, which is a subset
of the other two, so this will work for both 0-8 and 0-9-1 branches of
the code.
*/

/**
 * A field table value
 *
 * \since v0.1
 */
typedef struct amqp_field_value_t_ {
  uint8_t kind;             /**< the type of the entry /sa amqp_field_value_kind_t */
  union {
    amqp_boolean_t boolean;   /**< boolean type AMQP_FIELD_KIND_BOOLEAN */
    int8_t i8;                /**< int8_t type AMQP_FIELD_KIND_I8 */
    uint8_t u8;               /**< uint8_t type AMQP_FIELD_KIND_U8 */
    int16_t i16;              /**< int16_t type AMQP_FIELD_KIND_I16 */
    uint16_t u16;             /**< uint16_t type AMQP_FIELD_KIND_U16 */
    int32_t i32;              /**< int32_t type AMQP_FIELD_KIND_I32 */
    uint32_t u32;             /**< uint32_t type AMQP_FIELD_KIND_U32 */
    int64_t i64;              /**< int64_t type AMQP_FIELD_KIND_I64 */
    uint64_t u64;             /**< uint64_t type AMQP_FIELD_KIND_U64, AMQP_FIELD_KIND_TIMESTAMP */
    float f32;                /**< float type AMQP_FIELD_KIND_F32 */
    double f64;               /**< double type AMQP_FIELD_KIND_F64 */
    amqp_decimal_t decimal;   /**< amqp_decimal_t AMQP_FIELD_KIND_DECIMAL */
    amqp_bytes_t bytes;       /**< amqp_bytes_t type AMQP_FIELD_KIND_UTF8, AMQP_FIELD_KIND_BYTES */
    amqp_table_t table;       /**< amqp_table_t type AMQP_FIELD_KIND_TABLE */
    amqp_array_t array;       /**< amqp_array_t type AMQP_FIELD_KIND_ARRAY */
  } value;              /**< a union of the value */
} amqp_field_value_t;

/**
 * An entry in a field-table
 *
 * \sa amqp_table_encode(), amqp_table_decode(), amqp_table_clone()
 *
 * \since v0.1
 */
typedef struct amqp_table_entry_t_ {
  amqp_bytes_t key;           /**< the table entry key. Its a null-terminated UTF-8 string,
                               * with a maximum size of 128 bytes */
  amqp_field_value_t value;   /**< the table entry values */
} amqp_table_entry_t;

/**
 * Field value types
 *
 * \since v0.1
 */
typedef enum {
  AMQP_FIELD_KIND_BOOLEAN = 't',  /**< boolean type. 0 = false, 1 = true @see amqp_boolean_t */
  AMQP_FIELD_KIND_I8 = 'b',       /**< 8-bit signed integer, datatype: int8_t */
  AMQP_FIELD_KIND_U8 = 'B',       /**< 8-bit unsigned integer, datatype: uint8_t */
  AMQP_FIELD_KIND_I16 = 's',      /**< 16-bit signed integer, datatype: int16_t */
  AMQP_FIELD_KIND_U16 = 'u',      /**< 16-bit unsigned integer, datatype: uint16_t */
  AMQP_FIELD_KIND_I32 = 'I',      /**< 32-bit signed integer, datatype: int32_t */
  AMQP_FIELD_KIND_U32 = 'i',      /**< 32-bit unsigned integer, datatype: uint32_t */
  AMQP_FIELD_KIND_I64 = 'l',      /**< 64-bit signed integer, datatype: int64_t */
  AMQP_FIELD_KIND_U64 = 'L',      /**< 64-bit unsigned integer, datatype: uint64_t */
  AMQP_FIELD_KIND_F32 = 'f',      /**< single-precision floating point value, datatype: float */
  AMQP_FIELD_KIND_F64 = 'd',      /**< double-precision floating point value, datatype: double */
  AMQP_FIELD_KIND_DECIMAL = 'D',  /**< amqp-decimal value, datatype: amqp_decimal_t */
  AMQP_FIELD_KIND_UTF8 = 'S',     /**< UTF-8 null-terminated character string, datatype: amqp_bytes_t */
  AMQP_FIELD_KIND_ARRAY = 'A',    /**< field array (repeated values of another datatype. datatype: amqp_array_t */
  AMQP_FIELD_KIND_TIMESTAMP = 'T',/**< 64-bit timestamp. datatype uint64_t */
  AMQP_FIELD_KIND_TABLE = 'F',    /**< field table. encapsulates a table inside a table entry. datatype: amqp_table_t */
  AMQP_FIELD_KIND_VOID = 'V',     /**< empty entry */
  AMQP_FIELD_KIND_BYTES = 'x'     /**< unformatted byte string, datatype: amqp_bytes_t */
} amqp_field_value_kind_t;

/**
 * A list of allocation blocks
 *
 * \since v0.1
 */
typedef struct amqp_pool_blocklist_t_ {
  int num_blocks;     /**< Number of blocks in the block list */
  void **blocklist;   /**< Array of memory blocks */
} amqp_pool_blocklist_t;

/**
 * A memory pool
 *
 * \since v0.1
 */
typedef struct amqp_pool_t_ {
  size_t pagesize;            /**< the size of the page in bytes.
                               *  allocations less than or equal to this size are
                               *    allocated in the pages block list
                               *  allocations greater than this are allocated in their
                               *   own block in the large_blocks block list */

  amqp_pool_blocklist_t pages;        /**< blocks that are the size of pagesize */
  amqp_pool_blocklist_t large_blocks; /**< allocations larger than the pagesize */

  int next_page;      /**< an index to the next unused page block */
  char *alloc_block;  /**< pointer to the current allocation block */
  size_t alloc_used;  /**< number of bytes in the current allocation block that has been used */
} amqp_pool_t;

/**
 * An amqp method
 *
 * \since v0.1
 */
typedef struct amqp_method_t_ {
  amqp_method_number_t id;      /**< the method id number */
  void *decoded;                /**< pointer to the decoded method,
                                 *    cast to the appropriate type to use */
} amqp_method_t;

/**
 * An AMQP frame
 *
 * \since v0.1
 */
typedef struct amqp_frame_t_ {
  uint8_t frame_type;       /**< frame type. The types:
                             * - AMQP_FRAME_METHOD - use the method union member
                             * - AMQP_FRAME_HEADER - use the properties union member
                             * - AMQP_FRAME_BODY - use the body_fragment union member
                             */
  amqp_channel_t channel;   /**< the channel the frame was received on */
  union {
    amqp_method_t method;   /**< a method, use if frame_type == AMQP_FRAME_METHOD */
    struct {
      uint16_t class_id;    /**< the class for the properties */
      uint64_t body_size;   /**< size of the body in bytes */
      void *decoded;        /**< the decoded properties */
      amqp_bytes_t raw;     /**< amqp-encoded properties structure */
    } properties;           /**< message header, a.k.a., properties,
                                  use if frame_type == AMQP_FRAME_HEADER */
    amqp_bytes_t body_fragment; /**< a body fragment, use if frame_type == AMQP_FRAME_BODY */
    struct {
      uint8_t transport_high;           /**< @internal first byte of handshake */
      uint8_t transport_low;            /**< @internal second byte of handshake */
      uint8_t protocol_version_major;   /**< @internal third byte of handshake */
      uint8_t protocol_version_minor;   /**< @internal fourth byte of handshake */
    } protocol_header;    /**< Used only when doing the initial handshake with the broker,
                                don't use otherwise */
  } payload;              /**< the payload of the frame */
} amqp_frame_t;

/**
 * Response type
 *
 * \since v0.1
 */
typedef enum amqp_response_type_enum_ {
  AMQP_RESPONSE_NONE = 0,         /**< the library got an EOF from the socket */
  AMQP_RESPONSE_NORMAL,           /**< response normal, the RPC completed successfully */
  AMQP_RESPONSE_LIBRARY_EXCEPTION,/**< library error, an error occurred in the library, examine the library_error */
  AMQP_RESPONSE_SERVER_EXCEPTION  /**< server exception, the broker returned an error, check replay */
} amqp_response_type_enum;

/**
 * Reply from a RPC method on the broker
 *
 * \since v0.1
 */
typedef struct amqp_rpc_reply_t_ {
  amqp_response_type_enum reply_type;   /**< the reply type:
                                         * - AMQP_RESPONSE_NORMAL - the RPC completed successfully
                                         * - AMQP_RESPONSE_SERVER_EXCEPTION - the broker returned
                                         *     an exception, check the reply field
                                         * - AMQP_RESPONSE_LIBRARY_EXCEPTION - the library
                                         *    encountered an error, check the library_error field
                                         */
  amqp_method_t reply;                  /**< in case of AMQP_RESPONSE_SERVER_EXCEPTION this
                                         * field will be set to the method returned from the broker */
  int library_error;                    /**< in case of AMQP_RESPONSE_LIBRARY_EXCEPTION this
                                         *    field will be set to an error code. An error
                                         *     string can be retrieved using amqp_error_string */
} amqp_rpc_reply_t;

/**
 * SASL method type
 *
 * \since v0.1
 */
typedef enum amqp_sasl_method_enum_ {
  AMQP_SASL_METHOD_PLAIN = 0      /**< the PLAIN SASL method for authentication to the broker */
} amqp_sasl_method_enum;

/**
 * connection state object
 *
 * \since v0.1
 */
typedef struct amqp_connection_state_t_ *amqp_connection_state_t;

/**
 * Socket object
 *
 * \since v0.4.0
 */
typedef struct amqp_socket_t_ amqp_socket_t;

/**
 * Status codes
 *
 * \since v0.4.0
 */
typedef enum amqp_status_enum_
{
  AMQP_STATUS_OK =                         0x0,     /**< The operation was
                                                      successful */
  AMQP_STATUS_NO_MEMORY =                 -0x0001,  /**< Memory allocation
                                                      failed */
  AMQP_STATUS_BAD_AMQP_DATA =             -0x0002,
  AMQP_STATUS_UNKNOWN_CLASS =             -0x0003,
  AMQP_STATUS_UNKNOWN_METHOD =            -0x0004,
  AMQP_STATUS_HOSTNAME_RESOLUTION_FAILED= -0x0005,
  AMQP_STATUS_INCOMPATIBLE_AMQP_VERSION = -0x0006,
  AMQP_STATUS_CONNECTION_CLOSED =         -0x0007,
  AMQP_STATUS_BAD_URL =                   -0x0008,
  AMQP_STATUS_SOCKET_ERROR =              -0x0009,
  AMQP_STATUS_INVALID_PARAMETER =         -0x000A,
  AMQP_STATUS_TABLE_TOO_BIG =             -0x000B,
  AMQP_STATUS_WRONG_METHOD =              -0x000C,
  AMQP_STATUS_TIMEOUT =                   -0x000D,
  AMQP_STATUS_TIMER_FAILURE =             -0x000E,
  AMQP_STATUS_HEARTBEAT_TIMEOUT =         -0x000F,
  AMQP_STATUS_UNEXPECTED_STATE =          -0x0010,

  AMQP_STATUS_TCP_ERROR =                 -0x0100,
  AMQP_STATUS_TCP_SOCKETLIB_INIT_ERROR =  -0x0101,

  AMQP_STATUS_SSL_ERROR =                 -0x0200,
  AMQP_STATUS_SSL_HOSTNAME_VERIFY_FAILED= -0x0201,
  AMQP_STATUS_SSL_PEER_VERIFY_FAILED =    -0x0202,
  AMQP_STATUS_SSL_CONNECTION_FAILED =     -0x0203
} amqp_status_enum;

AMQP_END_DECLS

#include <amqp_framing.h>

AMQP_BEGIN_DECLS

/**
 * Empty bytes structure
 *
 * \since v0.2
 */
AMQP_PUBLIC_VARIABLE const amqp_bytes_t amqp_empty_bytes;

/**
 * Empty table structure
 *
 * \since v0.2
 */
AMQP_PUBLIC_VARIABLE const amqp_table_t amqp_empty_table;

/**
 * Empty table array structure
 *
 * \since v0.2
 */
AMQP_PUBLIC_VARIABLE const amqp_array_t amqp_empty_array;

/* Compatibility macros for the above, to avoid the need to update
   code written against earlier versions of librabbitmq. */

/**
 * \def AMQP_EMPTY_BYTES
 *
 * Deprecated, use \ref amqp_empty_bytes instead
 *
 * \deprecated use \ref amqp_empty_bytes instead
 *
 * \since v0.1
 */
#define AMQP_EMPTY_BYTES amqp_empty_bytes

/**
 * \def AMQP_EMPTY_TABLE
 *
 * Deprecated, use \ref amqp_empty_table instead
 *
 * \deprecated use \ref amqp_empty_table instead
 *
 * \since v0.1
 */
#define AMQP_EMPTY_TABLE amqp_empty_table

/**
 * \def AMQP_EMPTY_ARRAY
 *
 * Deprecated, use \ref amqp_empty_array instead
 *
 * \deprecated use \ref amqp_empty_array instead
 *
 * \since v0.1
 */
#define AMQP_EMPTY_ARRAY amqp_empty_array

/**
 * Initializes an amqp_pool_t memory allocation pool for use
 *
 * Readies an allocation pool for use. An amqp_pool_t
 * must be initialized before use
 *
 * \param [in] pool the amqp_pool_t structure to initialize.
 *              Calling this function on a pool a pool that has
 *              already been initialized will result in undefined
 *              behavior
 * \param [in] pagesize the unit size that the pool will allocate
 *              memory chunks in. Anything allocated against the pool
 *              with a requested size will be carved out of a block
 *              this size. Allocations larger than this will be
 *              allocated individually
 *
 * \sa recycle_amqp_pool(), empty_amqp_pool(), amqp_pool_alloc(),
 *     amqp_pool_alloc_bytes(), amqp_pool_t
 *
 * \since v0.1
 */
AMQP_PUBLIC_FUNCTION
void
AMQP_CALL init_amqp_pool(amqp_pool_t *pool, size_t pagesize);

/**
 * Recycles an amqp_pool_t memory allocation pool
 *
 * Recycles the space allocate by the pool
 *
 * This invalidates all allocations made against the pool before this call is
 * made, any use of any allocations made before recycle_amqp_pool() is called
 * will result in undefined behavior.
 *
 * Note: this may or may not release memory, to force memory to be released
 * call empty_amqp_pool().
 *
 * \param [in] pool the amqp_pool_t to recycle
 *
 * \sa recycle_amqp_pool(), empty_amqp_pool(), amqp_pool_alloc(),
 *      amqp_pool_alloc_bytes()
 *
 * \since v0.1
 *
 */
AMQP_PUBLIC_FUNCTION
void
AMQP_CALL recycle_amqp_pool(amqp_pool_t *pool);

/**
 * Empties an amqp memory pool
 *
 * Releases all memory associated with an allocation pool
 *
 * \param [in] pool the amqp_pool_t to empty
 *
 * \since v0.1
 */
AMQP_PUBLIC_FUNCTION
void
AMQP_CALL empty_amqp_pool(amqp_pool_t *pool);

/**
 * Allocates a block of memory from an amqp_pool_t memory pool
 *
 * Memory will be aligned on a 8-byte boundary. If a 0-length allocation is
 * requested, a NULL pointer will be returned.
 *
 * \param [in] pool the allocation pool to allocate the memory from
 * \param [in] amount the size of the allocation in bytes.
 * \return a pointer to the memory block, or NULL if the allocation cannot
 *          be satisfied.
 *
 * \sa init_amqp_pool(), recycle_amqp_pool(), empty_amqp_pool(),
 *     amqp_pool_alloc_bytes()
 *
 * \since v0.1
 */
AMQP_PUBLIC_FUNCTION
void *
AMQP_CALL amqp_pool_alloc(amqp_pool_t *pool, size_t amount);

/**
 * Allocates a block of memory from an amqp_pool_t to an amqp_bytes_t
 *
 * Memory will be aligned on a 8-byte boundary. If a 0-length allocation is
 * requested, output.bytes = NULL.
 *
 * \param [in] pool the allocation pool to allocate the memory from
 * \param [in] amount the size of the allocation in bytes
 * \param [in] output the location to store the pointer. On success
 *              output.bytes will be set to the beginning of the buffer
 *              output.len will be set to amount
 *              On error output.bytes will be set to NULL and output.len
 *              set to 0
 *
 * \sa init_amqp_pool(), recycle_amqp_pool(), empty_amqp_pool(),
 *     amqp_pool_alloc()
 *
 * \since v0.1
 */
AMQP_PUBLIC_FUNCTION
void
AMQP_CALL amqp_pool_alloc_bytes(amqp_pool_t *pool, size_t amount, amqp_bytes_t *output);

/**
 * Wraps a c string in an amqp_bytes_t
 *
 * Takes a string, calculates its length and creates an
 * amqp_bytes_t that points to it. The string is not duplicated.
 *
 * For a given input cstr, The amqp_bytes_t output.bytes is the
 * same as cstr, output.len is the length of the string not including
 * the \0 terminator
 *
 * This function uses strlen() internally so cstr must be properly
 * terminated
 *
 * \param [in] cstr the c string to wrap
 * \return an amqp_bytes_t that describes the string
 *
 * \since v0.1
 */
AMQP_PUBLIC_FUNCTION
amqp_bytes_t
AMQP_CALL amqp_cstring_bytes(char const *cstr);

/**
 * Duplicates an amqp_bytes_t buffer.
 *
 * The buffer is cloned and the contents copied.
 *
 * The memory associated with the output is allocated
 * with amqp_bytes_malloc() and should be freed with
 * amqp_bytes_free()
 *
 * \param [in] src
 * \return a clone of the src
 *
 * \sa amqp_bytes_free(), amqp_bytes_malloc()
 *
 * \since v0.1
 */
AMQP_PUBLIC_FUNCTION
amqp_bytes_t
AMQP_CALL amqp_bytes_malloc_dup(amqp_bytes_t src);

/**
 * Allocates a amqp_bytes_t buffer
 *
 * Creates an amqp_bytes_t buffer of the specified amount, the buffer should be
 * freed using amqp_bytes_free()
 *
 * \param [in] amount the size of the buffer in bytes
 * \returns an amqp_bytes_t with amount bytes allocated.
 *           output.bytes will be set to NULL on error
 *
 * \sa amqp_bytes_free(), amqp_bytes_malloc_dup()
 *
 * \since v0.1
 */
AMQP_PUBLIC_FUNCTION
amqp_bytes_t
AMQP_CALL amqp_bytes_malloc(size_t amount);

/**
 * Frees an amqp_bytes_t buffer
 *
 * Frees a buffer allocated with amqp_bytes_malloc() or amqp_bytes_malloc_dup()
 *
 * Calling amqp_bytes_free on buffers not allocated with one
 * of those two functions will result in undefined behavior
 *
 * \param [in] bytes the buffer to free
 *
 * \sa amqp_bytes_malloc(), amqp_bytes_malloc_dup()
 *
 * \since v0.1
 */
AMQP_PUBLIC_FUNCTION
void
AMQP_CALL amqp_bytes_free(amqp_bytes_t bytes);

/**
 * Creates a new amqp_connection_state_t object
 *
 * amqp_connection_state_t objects created with this function
 * should be freed with amqp_destroy_connection
 *
 * \sa amqp_destroy_connection()
 *
 * \since v0.1
 */
AMQP_PUBLIC_FUNCTION
amqp_connection_state_t
AMQP_CALL amqp_new_connection(void);

/**
 * Get the underlying socket descriptor for the connection
 *
 * \param [in] state the connection object
 * \returns the socket descriptor if one has been set, -1 otherwise
 *
 * \sa amqp_tcp_socket_new(), amqp_ssl_socket_new(), amqp_socket_open()
 *
 * \since v0.1
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL amqp_get_sockfd(amqp_connection_state_t state);


/**
 * Deprecated, use amqp_tcp_socket_new() or amqp_ssl_socket_new()
 *
 * \deprecated Use amqp_tcp_socket_new() or amqp_ssl_socket_new()
 *
 * Sets the socket descriptor associated with the connection. The socket
 * should be connected to a broker, and should not be read to or written from
 * before calling this function.  A socket descriptor can be created and opened
 * using amqp_open_socket()
 *
 * \param [in] state the connection object
 * \param [in] sockfd the socket
 *
 * \sa amqp_open_socket(), amqp_tcp_socket_new(), amqp_ssl_socket_new()
 *
 * \since v0.1
 */
AMQP_DEPRECATED(
  AMQP_PUBLIC_FUNCTION
  void
  AMQP_CALL amqp_set_sockfd(amqp_connection_state_t state, int sockfd)
);


/**
 * Tune client side parameters
 *
 * \warning This function may call abort() if the connection is in a certain
 *  state. As such it should probably not be called code outside the library.
 *  connection parameters should be specified when calling amqp_login() or
 *  amqp_login_with_properties()
 *
 * This function changes channel_max, frame_max, and heartbeat parameters, on
 * the client side only. It does not try to renegotiate these parameters with
 * the broker. Using this function will lead to unexpected results.
 *
 * \param [in] state the connection object
 * \param [in] channel_max the maximum number of channels.
 *              The largest this can be is 65535
 * \param [in] frame_max the maximum size of an frame.
 *              The smallest this can be is 4096
 *              The largest this can be is 2147483647
 *              Unless you know what you're doing the recommended
 *              size is 131072 or 128KB
 * \param [in] heartbeat the number of seconds between heartbeats
 *
 * \return AMQP_STATUS_OK on success, amqp_status_enum value otherwise
 *
 * \sa amqp_login(), amqp_login_with_properties()
 *
 * \since v0.1
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL amqp_tune_connection(amqp_connection_state_t state,
                               int channel_max,
                               int frame_max,
                               int heartbeat);

/**
 * Get the maximum number of channels the connection can handle
 *
 * This number can be changed using the amqp_tune_connection function
 *
 * \param [in] state the connection object
 * \return the maximum number of channels. 0 if there is no limit
 *
 * \since v0.1
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL amqp_get_channel_max(amqp_connection_state_t state);

/**
 * Destroys a connection object
 *
 * Destroys a connection object created with amqp_new_connection
 * This function will free all memory and close any sockets
 * associated with the connection
 *
 * \param [in] state the connection object
 *
 * \since v0.1
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL amqp_destroy_connection(amqp_connection_state_t state);

/**
 * Handle input
 *
 * For a given input buffer and connection state potentially decode
 * a frame from it
 *
 * \param [in] state the connection object
 * \param [in] received_data a buffer of the data to be decoded
 * \param [in] decoded_frame the frame
 * \return 0 on success, 0 > on error
 *
 * \since v0.1
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL amqp_handle_input(amqp_connection_state_t state,
                            amqp_bytes_t received_data,
                            amqp_frame_t *decoded_frame);

/**
 * Check to see if the connection is in a state it can release its internal buffers
 *
 * Call this to check before calling amqp_release_buffers.
 *
 * Alternatively call amqp_maybe_release_buffers to do this all in one step
 *
 * \param [in] state the connection object
 * \returns TRUE if the buffers can be released FALSE otherwise
 *
 * \since v0.1
 */
AMQP_PUBLIC_FUNCTION
amqp_boolean_t
AMQP_CALL amqp_release_buffers_ok(amqp_connection_state_t state);

/**
 * Release connect object internal buffers
 *
 * Call amqp_release_buffers_ok before calling this to ensure
 * the connection is in a state that it can release the buffers.
 * failing to do this will cause the program to abort.
 *
 * \param [in] state the connection object
 *
 * \since v0.1
 */
AMQP_PUBLIC_FUNCTION
void
AMQP_CALL amqp_release_buffers(amqp_connection_state_t state);

/**
 * Release buffers if possible
 *
 * Releases interal connection buffers if it is possible
 *
 * \param [in] state the connection object
 *
 * \since v0.1
 */
AMQP_PUBLIC_FUNCTION
void
AMQP_CALL amqp_maybe_release_buffers(amqp_connection_state_t state);

/**
 *
 * \param [in] state
 * \param [in] channel
 *
 * \since v0.4.0
 */
AMQP_PUBLIC_FUNCTION
void
AMQP_CALL amqp_maybe_release_buffers_on_channel(amqp_connection_state_t state, amqp_channel_t channel);

/**
 * Send a frame to the broker
 *
 * \param [in] state the connection object
 * \param [in] frame the frame to send to the broker
 * \return 0 on success, 0 > on error
 *
 * \since v0.1
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL amqp_send_frame(amqp_connection_state_t state, amqp_frame_t const *frame);

/**
 * Compare two table entries
 *
 * Works just like strcmp(), comparing two the table keys, datatype, then values
 *
 * \param [in] entry1 the entry on the left
 * \param [in] entry2 the entry on the right
 * \return 0 if entries are equal, 0 < if left is greater, 0 > if right is greater
 *
 * \since v0.1
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL amqp_table_entry_cmp(void const *entry1, void const *entry2);

/**
 * Attempt to open a socket
 *
 * Attempts to open a socket to hostname on portnumber
 *
 * \param [in] hostname this can be a hostname or IP address.
 *              Both IPv4 and IPv6 are acceptable
 * \param [in] portnumber the port to connect on. RabbitMQ brokers
 *              listen on port 5672, and 5671 for SSL
 * \return the sockfd, or 0 > on error
 *
 * \since v0.1
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL amqp_open_socket(char const *hostname, int portnumber);

/**
 * Send AMQP handshake to broker
 *
 * \param [in] state the connection object
 * \return -1 on error, a positive value on success
 *
 * \since v0.1
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL amqp_send_header(amqp_connection_state_t state);

/**
 * Checks to see if there are any incoming frames ready to be read
 *
 * If this function returns true, amqp_simple_wait_method will
 * return a new frame without potentially entering a blocking read()
 *
 * \param [in] state the connection object
 * \return TRUE if there are frames enqueued, FALSE otherwise
 *
 * \since v0.1
 */
AMQP_PUBLIC_FUNCTION
amqp_boolean_t
AMQP_CALL amqp_frames_enqueued(amqp_connection_state_t state);

/**
 * Waits for a single frame from the broker
 *
 * \param [in] state the connection object
 * \param [out] decoded_frame the frame
 * \return 0 on success, 0 > on error
 *
 * \since v0.1
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL amqp_simple_wait_frame(amqp_connection_state_t state,
                                 amqp_frame_t *decoded_frame);

/**
 * Wait for a frame with a timeout
 *
 * \param [in,out] state
 * \param [out] decoded_frame
 * \param [in] tv
 * \return
 *
 * \sa amqp_simple_wait_frame()
 *
 * \since v0.4.0
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL amqp_simple_wait_frame_noblock(amqp_connection_state_t state,
                                         amqp_frame_t *decoded_frame,
                                         struct timeval *tv);

/**
 * Waits for a specific method from the broker
 *
 * Waits for a single method on a channel from the broker.
 * If a frame is received that does not match expected_channel
 * or expected_method the program will abort
 *
 * \param [in] state the connection object
 * \param [in] expected_channel the channel that the method should be delivered on
 * \param [in] expected_method the method to wait for
 * \param [out] output the method
 * \returns 0 on success, 0 < on failure
 *
 * \since v0.1
 */

AMQP_PUBLIC_FUNCTION
int
AMQP_CALL amqp_simple_wait_method(amqp_connection_state_t state,
                                  amqp_channel_t expected_channel,
                                  amqp_method_number_t expected_method,
                                  amqp_method_t *output);

/**
 * Sends a method to the broker
 *
 * \param [in] state the connection object
 * \param [in] channel the channel object
 * \param [in] id the method number
 * \param [in] decoded the method object
 * \returns 0 on success, 0 < on failure
 *
 * \since v0.1
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL amqp_send_method(amqp_connection_state_t state,
                           amqp_channel_t channel,
                           amqp_method_number_t id,
                           void *decoded);

/**
 * Sends a method to the broker and waits for a method response
 *
 * \param [in] state the connection object
 * \param [in] channel the channel object
 * \param [in] request_id the method number of the request
 * \param [in] expected_reply_ids a 0 terminated array of expected response method numbers
 * \param [in] decoded_request_method the method to be sent to the broker
 * \return a amqp_rpc_reply_t which contains the broker response
 *
 * \since v0.1
 */
AMQP_PUBLIC_FUNCTION
amqp_rpc_reply_t
AMQP_CALL amqp_simple_rpc(amqp_connection_state_t state,
                          amqp_channel_t channel,
                          amqp_method_number_t request_id,
                          amqp_method_number_t *expected_reply_ids,
                          void *decoded_request_method);

/**
 * Sends a method to the broker and waits for a method response
 *
 * \param [in] state the connection object
 * \param [in] channel the channel object
 * \param [in] request_id the method number of the request
 * \param [in] reply_id the method number expected in response
 * \param [in] decoded_request_method the request method
 * \return a pointer to the method, or NULL if an error occurs
 *
 * \since v0.1
 */
AMQP_PUBLIC_FUNCTION
void *
AMQP_CALL amqp_simple_rpc_decoded(amqp_connection_state_t state,
                                  amqp_channel_t channel,
                                  amqp_method_number_t request_id,
                                  amqp_method_number_t reply_id,
                                  void *decoded_request_method);

/**
 * Get the last global amqp_rpc_reply
 *
 * The API methods corresponding to most synchronous AMQP methods
 * return a pointer to the decoded method result.  Upon error, they
 * return NULL, and we need some way of discovering what, if anything,
 * went wrong. amqp_get_rpc_reply() returns the most recent
 * amqp_rpc_reply_t instance corresponding to such an API operation
 * for the given connection.
 *
 * Only use it for operations that do not themselves return
 * amqp_rpc_reply_t; operations that do return amqp_rpc_reply_t
 * generally do NOT update this per-connection-global amqp_rpc_reply_t
 * instance.
 *
 * \param [in] state the connection object
 * \return the most recent amqp_rpc_reply_t
 *
 * \since v0.1
 */
AMQP_PUBLIC_FUNCTION
amqp_rpc_reply_t
AMQP_CALL amqp_get_rpc_reply(amqp_connection_state_t state);

/**
 * Login to the broker
 *
 * After using amqp_open_socket and amqp_set_sockfd, call
 * amqp_login to complete connecting to the broker
 *
 * \param [in] state the connection object
 * \param [in] vhost the virtual host on the broker. The default virtual
 *              host on most brokers is "/"
 * \param [in] channel_max the maximum number of channels to request of
 *              the broker for this connection. 0 means no limit, this is a
 *              good default.
 * \param [in] frame_max the maximum size of an AMQP frame on the wire to
 *              request of the broker for this connection. 4096 is the minimum
 *              size, 2^31-1 is the maximum, a good default is 131072 (128KB).
 * \param [in] heartbeat the number of seconds between heartbeat frames to
 *              request of the broker. A value of 0 disables heartbeats.
 *              NOTE: rabbitmq-c does not support heartbeats, your best bet
 *              is to disable it by leaving this at 0
 * \param [in] sasl_method the SASL method to authenticate with the broker.
 *              followed by the authentication information.
 *              For AMQP_SASL_METHOD_PLAIN, the AMQP_SASL_METHOD_PLAIN
 *              should be followed by two arguments in this order:
 *              const char* username, and const char* password.
 * \return amqp_rpc_reply_t indicating success or failure.
 *
 * \since v0.1
 */
AMQP_PUBLIC_FUNCTION
amqp_rpc_reply_t
AMQP_CALL amqp_login(amqp_connection_state_t state, char const *vhost,
                     int channel_max, int frame_max, int heartbeat,
                     amqp_sasl_method_enum sasl_method, ...);

/**
 * Login to the broker passing a properties table
 *
 * \param [in] state
 * \param [in] vhost
 * \param [in] channel_max
 * \param [in] frame_max
 * \param [in] heartbeat
 * \param [in] properties
 * \param [in] sasl_method
 * \return val
 *
 * \since v0.4.0
 */
AMQP_PUBLIC_FUNCTION
amqp_rpc_reply_t
AMQP_CALL amqp_login_with_properties(amqp_connection_state_t state, char const *vhost,
                                     int channel_max, int frame_max, int heartbeat,
                                     const amqp_table_t *properties, amqp_sasl_method_enum sasl_method, ...);

struct amqp_basic_properties_t_;

/**
 * Publish a message to the broker
 *
 * \param [in] state the connection object
 * \param [in] channel the channel identifier
 * \param [in] exchange the exchange on the broker to publish to
 * \param [in] routing_key the routing key to use when publishing the message
 * \param [in] mandatory indicate to the broker that the message MUST be routed
 *              to a queue. If the broker cannot do this it should respond with
 *              a basic.reject method.
 * \param [in] immediate indicate to the broker that the message MUST be delivered
 *              to a consumer immediately. If the broker cannot do this it should
 *              response with a basic.reject method.
 * \param [in] properties the properties associated with the message
 * \param [in] body the message body
 * \return 0 on success, 0 > on failure to transmit the message to the broker.
 *          since basic.publish is an async method, this return value will not
 *          indicate whether the broker encountered an error, such as a non-existant
 *          exchange, or a failure in processing due to a mandatory or immediate flag.
 *
 * \since v0.1
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL amqp_basic_publish(amqp_connection_state_t state, amqp_channel_t channel,
                             amqp_bytes_t exchange, amqp_bytes_t routing_key,
                             amqp_boolean_t mandatory, amqp_boolean_t immediate,
                             struct amqp_basic_properties_t_ const *properties,
                             amqp_bytes_t body);

/**
 * Closes an channel
 *
 * \param [in] state the connection object
 * \param [in] channel the channel identifier
 * \param [in] code the reason for closing the channel, AMQP_REPLY_SUCCESS is a good default
 * \return amqp_rpc_reply_t indicating success or failure
 *
 * \since v0.1
 */
AMQP_PUBLIC_FUNCTION
amqp_rpc_reply_t
AMQP_CALL amqp_channel_close(amqp_connection_state_t state, amqp_channel_t channel,
                             int code);

/**
 * Closes the entire connection
 *
 * Implicitly closes all channels and informs the broker the connection
 * is being closed, after receiving acknowldgement from the broker it closes
 * the socket.
 *
 * \param [in] state the connection object
 * \param [in] code the reason code for closing the connection. AMQP_REPLY_SUCCESS is a good default.
 * \return amqp_rpc_reply_t indicating the result
 *
 * \since v0.1
 */
AMQP_PUBLIC_FUNCTION
amqp_rpc_reply_t
AMQP_CALL amqp_connection_close(amqp_connection_state_t state, int code);

/**
 * Acknowledges a message
 *
 * Does a basic.ack on a received message
 *
 * \param [in] state the connection object
 * \param [in] channel the channel identifier
 * \param [in] delivery_tag the delivery take of the message to be ack'd
 * \param [in] multiple if true, ack all messages up to this delivery tag, if
 *              false ack only this delivery tag
 * \return 0 on success,  0 > on failing to send the ack to the broker.
 *            this will not indicate failure if something goes wrong on the broker
 *
 * \since v0.1
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL amqp_basic_ack(amqp_connection_state_t state, amqp_channel_t channel,
                         uint64_t delivery_tag, amqp_boolean_t multiple);

/**
 * Do a basic.get
 *
 * Synchonously polls the broker for a message in a queue, and
 * retrieves the message if a message is in the queue.
 *
 * \param [in] state the connection object
 * \param [in] channel the channel identifier to use
 * \param [in] queue the queue name to retrieve from
 * \param [in] no_ack if true the message is automatically ack'ed
 *              if false amqp_basic_ack should be called once the message
 *              retrieved has been processed
 * \return amqp_rpc_reply indicating success or failure
 *
 * \since v0.1
 */
AMQP_PUBLIC_FUNCTION
amqp_rpc_reply_t
AMQP_CALL amqp_basic_get(amqp_connection_state_t state, amqp_channel_t channel,
                         amqp_bytes_t queue, amqp_boolean_t no_ack);

/**
 * Do a basic.reject
 *
 * Actively reject a message that has been delivered
 *
 * \param [in] state the connection object
 * \param [in] channel the channel identifier
 * \param [in] delivery_tag the delivery tag of the message to reject
 * \param [in] requeue indicate to the broker whether it should requeue the
 *              message or just discard it.
 * \return 0 on success, 0 > on failing to send the reject method to the broker.
 *          This will not indicate failure if something goes wrong on the broker.
 *
 * \since v0.1
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL amqp_basic_reject(amqp_connection_state_t state, amqp_channel_t channel,
                            uint64_t delivery_tag, amqp_boolean_t requeue);

/**
 * Check to see if there is data left in the receive buffer
 *
 * Can be used to see if there is data still in the buffer, if so
 * calling amqp_simple_wait_frame will not immediately enter a
 * blocking read.
 *
 * \param [in] state the connection object
 * \return true if there is data in the recieve buffer, false otherwise
 *
 * \since v0.1
 */
AMQP_PUBLIC_FUNCTION
amqp_boolean_t
AMQP_CALL amqp_data_in_buffer(amqp_connection_state_t state);

/**
 * Get the error string for the given error code.
 *
 * \deprecated This function has been deprecated in favor of
 *  \ref amqp_error_string2() which returns statically allocated
 *  string which do not need to be freed by the caller.
 *
 * The returned string resides on the heap; the caller is responsible
 * for freeing it.
 *
 * \param [in] err return error code
 * \return the error string
 *
 * \since v0.1
 */
AMQP_DEPRECATED(
  AMQP_PUBLIC_FUNCTION
  char *
  AMQP_CALL amqp_error_string(int err)
);


/**
 * Get the error string for the given error code.
 *
 * Get an error string associated with an error code. The string is statically
 * allocated and does not need to be freed
 *
 * \param [in] err
 * \return the error string
 *
 * \since v0.4.0
 */
AMQP_PUBLIC_FUNCTION
const char *
AMQP_CALL amqp_error_string2(int err);


/**
 * Deserializes an amqp_table_t from AMQP wireformat
 *
 * This is an internal function and is not typically used by
 * client applications
 *
 * \param [in] encoded the buffer containing the serialized data
 * \param [in] pool memory pool used to allocate the table entries from
 * \param [in] output the amqp_table_t structure to fill in. Any existing
 *             entries will be erased
 * \param [in,out] offset The offset into the encoded buffer to start
 *                 reading the serialized table. It will be updated
 *                 by this function to end of the table
 * \return 0 on success, > 1 on error
 *
 * \since v0.1
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL amqp_decode_table(amqp_bytes_t encoded, amqp_pool_t *pool,
                            amqp_table_t *output, size_t *offset);

/**
 * Serializes an amqp_table_t to the AMQP wireformat
 *
 * This is an internal function and is not typically used by
 * client applications
 *
 * \param [in] encoded the buffer where to serialize the table to
 * \param [in] input the amqp_table_t to serialize
 * \param [in,out] offset The offset into the encoded buffer to start
 *                 writing the serialized table. It will be updated
 *                 by this function to where writing left off
 * \return 0 on success, > 1 on error
 *
 * \since v0.1
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL amqp_encode_table(amqp_bytes_t encoded, amqp_table_t *input, size_t *offset);


/**
 * Create a deep-copy of an amqp_table_t object
 *
 * Creates a deep-copy of an amqp_table_t object, using the provided pool
 * object to allocate the necessary memory. This memory can be freed later by
 * call recycle_amqp_pool(), or empty_amqp_pool()
 *
 * \param [in] original
 * \param [in,out] clone
 * \param [in] pool
 * \return
 *
 * \since v0.4.0
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL amqp_table_clone(amqp_table_t *original, amqp_table_t *clone, amqp_pool_t *pool);

/**
 * A message object
 *
 * \since v0.4.0
 */
typedef struct amqp_message_t_ {
  amqp_basic_properties_t properties; /**< message properties */
  amqp_bytes_t body;                  /**< message body */
  amqp_pool_t pool;                   /**< pool used to allocate properties */
} amqp_message_t;

/**
 * Reads the next message on a channel
 *
 * Reads a complete message (header + body) on a specified channel. This
 * function is intended to be used with amqp_basic_get() or when an
 * AMQP_BASIC_DELIVERY_METHOD method is received.
 *
 * \param [in,out] state the connection object
 * \param [in] channel the channel on which to read the message from
 * \param [in,out] message a pointer to a amqp_message_t object. Caller should
 *                 call amqp_message_destroy() when it is done using the
 *                 fields in the message object.  The caller is responsible for
 *                 allocating/destroying the amqp_message_t object itself.
 * \param [in] flags pass in 0. Currently unused.
 * \returns a amqp_rpc_reply_t object. ret.reply_type == AMQP_RESPONSE_NORMAL on success.
 *
 * \since v0.4.0
 */
AMQP_PUBLIC_FUNCTION
amqp_rpc_reply_t
AMQP_CALL amqp_read_message(amqp_connection_state_t state,
                            amqp_channel_t channel,
                            amqp_message_t *message, int flags);

/**
 * Frees memory associated with a amqp_message_t allocated in amqp_read_message
 *
 * \param [in] message
 *
 * \since v0.4.0
 */
AMQP_PUBLIC_FUNCTION
void
AMQP_CALL amqp_destroy_message(amqp_message_t *message);

/**
 * Envelope object
 *
 * \since v0.4.0
 */
typedef struct amqp_envelope_t_ {
  amqp_channel_t channel;           /**< channel message was delivered on */
  amqp_bytes_t consumer_tag;        /**< the consumer tag the message was delivered to */
  uint64_t delivery_tag;            /**< the messages delivery tag */
  amqp_boolean_t redelivered;       /**< flag indicating whether this message is being redelivered */
  amqp_bytes_t exchange;            /**< exchange this message was published to */
  amqp_bytes_t routing_key;         /**< the routing key this message was published with */
  amqp_message_t message;           /**< the message */
} amqp_envelope_t;

/**
 * Wait for and consume a message
 *
 * Waits for a basic.deliver method on any channel, upon receipt of
 * basic.deliver it reads that message, and returns. If any other method is
 * received before basic.deliver, this function will return an amqp_rpc_reply_t
 * with ret.reply_type == AMQP_RESPONSE_LIBRARY_EXCEPTION, and
 * ret.library_error == AMQP_STATUS_UNEXPECTED_FRAME. The caller should then
 * call amqp_simple_wait_frame() to read this frame and take appropriate action.
 *
 * This function should be used after starting a consumer with the
 * amqp_basic_consume() funtion
 *
 * \param [in,out] state the connection object
 * \param [in,out] envelope a pointer to a amqp_envelope_t object. Caller
 *                 should call amqp_envelope_destroy() when it is done using
 *                 the fields in the envelope object. The caller is responsible
 *                 for allocating/destroying the amqp_envelope_t object itself.
 * \param [in] timeout a timeout to wait for a message delivery. Passing in
 *             NULL will result in blocking behavior.
 * \param [in] flags pass in 0. Currently unused.
 * \returns a amqp_rpc_reply_t object.  ret.reply_type == AMQP_RESPONSE_NORMAL
 *          on success. If ret.reply_type == AMQP_RESPONSE_LIBRARY_EXCEPTION, and
 *          ret.library_error == AMQP_STATUS_UNEXPECTED_FRAME, a frame other
 *          than AMQP_BASIC_DELIVER_METHOD was received, the caller should call
 *          amqp_simple_wait_frame() to read this frame and take appropriate
 *          action.
 *
 * \since v0.4.0
 */
AMQP_PUBLIC_FUNCTION
amqp_rpc_reply_t
AMQP_CALL amqp_consume_message(amqp_connection_state_t state,
                               amqp_envelope_t *envelope,
                               struct timeval *timeout, int flags);

/**
 * Frees memory associated iwth a amqp_envelope_t allocated in amqp_consume_message
 *
 * \param [in] envelope
 *
 * \since v0.4.0
 */
AMQP_PUBLIC_FUNCTION
void
AMQP_CALL amqp_destroy_envelope(amqp_envelope_t *envelope);


/**
 * Parameters used to connect to the RabbitMQ broker
 *
 * \since v0.1
 */
struct amqp_connection_info {
  char *user;                 /**< the username to authenticate with the broker, default on most broker is 'guest' */
  char *password;             /**< the password to authenticate with the broker, default on most brokers is 'guest' */
  char *host;                 /**< the hostname of the broker */
  char *vhost;                /**< the virtual host on the broker to connect to, a good default is "/" */
  int port;                   /**< the port that the broker is listening on, default on most brokers is 5672 */
  amqp_boolean_t ssl;
};

/**
 * Initialze an amqp_connection_info to default values
 *
 * The default values are:
 * - user: "guest"
 * - password: "guest"
 * - host: "localhost"
 * - vhost: "/"
 * - port: 5672
 *
 * \param [out] parsed the connection info to set defaults on
 *
 * \since v0.1
 */
AMQP_PUBLIC_FUNCTION
void
AMQP_CALL amqp_default_connection_info(struct amqp_connection_info *parsed);

/**
 * Parse a connection URL
 *
 * An amqp connection url takes the form:
 *
 * amqp://[$USERNAME[:$PASSWORD]\@]$HOST[:$PORT]/[$VHOST]
 *
 * Examples:
 *  amqp://guest:guest\@localhost:5672//
 *  amqp://guest:guest\@localhost/myvhost
 *
 * \param [in] url URI to parse
 * \param [out] parsed the connection info gleaned from the URI
 * \returns 0 on success, anything else indicates failure
 *
 * \since v0.1
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL amqp_parse_url(char *url, struct amqp_connection_info *parsed);

/* socket API */

/**
 * Open a socket connection.
 *
 * This function opens a socket connection returned from amqp_tcp_socket_new()
 * or amqp_ssl_socket_new(). This function should be called after setting
 * socket options and prior to assigning the socket to an AMQP connection with
 * amqp_set_socket().
 *
 * \param [in,out] self A socket object.
 * \param [in] host Connect to this host.
 * \param [in] port Connect on this remote port.
 *
 * \return Zero upon success, non-zero otherwise.
 *
 * \since v0.4.0
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL
amqp_socket_open(amqp_socket_t *self, const char *host, int port);

/**
 * Open a socket connection.
 *
 * This function opens a socket connection returned from amqp_tcp_socket_new()
 * or amqp_ssl_socket_new(). This function should be called after setting
 * socket options and prior to assigning the socket to an AMQP connection with
 * amqp_set_socket().
 *
 * \param [in,out] self A socket object.
 * \param [in] host Connect to this host.
 * \param [in] port Connect on this remote port.
 * \param [in] timeout Max allowed time to spent on opening. If NULL - run in blocking mode
 *
 * \return Zero upon success, non-zero otherwise.
 *
 * \since v0.4.0
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL
amqp_socket_open_noblock(amqp_socket_t *self, const char *host, int port, struct timeval *timeout);

/**
 * Get the socket descriptor in use by a socket object.
 *
 * Retrieve the underlying socket descriptor. This function can be used to
 * perform low-level socket operations that aren't supported by the socket
 * interface. Use with caution!
 *
 * \param [in,out] self A socket object.
 *
 * \return The underlying socket descriptor.
 *
 * \since v0.4.0
 */
AMQP_PUBLIC_FUNCTION
int
AMQP_CALL
amqp_socket_get_sockfd(amqp_socket_t *self);

/**
 * Get the socket object associated with a amqp_connection_state_t
 *
 * \param [in] state the connection object to get the socket from
 * \return a pointer to the socket object, or NULL if one has not been assigned
 *
 * \since v0.4.0
 */
AMQP_PUBLIC_FUNCTION
amqp_socket_t *
amqp_get_socket(amqp_connection_state_t state);

AMQP_END_DECLS


#endif /* AMQP_H */
