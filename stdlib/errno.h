#pragma once

// these are not sorted by number, rather by alphabetical order

#define E2BIG 7 /* Arg list too long */
#define EACCES 13 /* Permission denied */
#define EADDRINUSE 112 /* Address already in use */
#define EADDRNOTAVAIL 125 /* Address not available */
#define EADV 68 /* Advertise error */
#define EAFNOSUPPORT 106 /* Address family not supported by protocol family */
#define EAGAIN 11 /* No more processes */
#define EALREADY 120 /* Socket already connected */
#define EBADE 50 /* Invalid exchange */
#define EBADF 9 /* Bad file number */
#define EBADFD 81 /* f.d. invalid for this operation */
#define EBADMSG 77 /* Trying to read unreadable message */
#define EBADR 51 /* Invalid request descriptor */
#define EBADRQC 54 /* Invalid request code */
#define EBADSLT 55 /* Invalid slot */
#define EBFONT 57 /* Bad font file fmt */
#define EBUSY 16 /* Mount device busy */
#define ECANCELED 140 /* Operation canceled */
#define ECHILD 10 /* No children */
#define ECHRNG 37 /* Channel number out of range */
#define ECOMM 70 /* Communication error on send */
#define ECONNABORTED 113 /* Connection aborted */
#define ECONNREFUSED 111 /* Connection refused */
#define ECONNRESET 104 /* Connection reset by peer */
#define EDEADLK 45 /* Deadlock condition */
#define EDEADLOCK 56 /* File locking deadlock error */
#define EDESTADDRREQ 121 /* Destination address required */
#define EDOM 33 /* Math arg out of domain of func */
#define EDOTDOT 76 /* Cross mount point (not really error) */
#define EDQUOT 132
#define EEXIST 17 /* File exists */
#define EFAULT 14 /* Bad address */
#define EFBIG 27 /* File too large */
#define EFTYPE 79 /* Inappropriate file type or format */
#define EHOSTDOWN 117 /* Host is down */
#define EHOSTUNREACH 118 /* Host is unreachable */
#define EIDRM 36 /* Identifier removed */
#define EILSEQ 138
#define EINPROGRESS 119 /* Connection already in progress */
#define EINTR 4 /* Interrupted system call */
#define EINVAL 22 /* Invalid argument */
#define EIO 5 /* IO error */
#define EISCONN 127 /* Socket is already connected */
#define EISDIR 21 /* Is a directory */
#define EL2HLT 44 /* Level 2 halted */
#define EL2NSYNC 38 /* Level 2 not synchronized */
#define EL3HLT 39 /* Level 3 halted */
#define EL3RST 40 /* Level 3 reset */
#define ELBIN 75 /* Inode is remote (not really error) */
#define ELIBACC 83 /* Can't access a needed shared lib */
#define ELIBBAD 84 /* Accessing a corrupted shared lib */
#define ELIBEXEC 87 /* Attempting to exec a shared library */
#define ELIBMAX 86 /* Attempting to link in too many libs */
#define ELIBSCN 85 /* .lib section in a.out corrupted */
#define ELNRNG 41 /* Link number out of range */
#define ELOOP 92 /* Too many symbolic links */
#define EMFILE 24 /* Too many open files */
#define EMLINK 31 /* Too many links */
#define EMSGSIZE 122 /* Message too long */
#define EMULTIHOP 74 /* Multihop attempted */
#define ENAMETOOLONG 91 /* File or path name too long */
#define ENETDOWN 115 /* Network interface is not configured */
#define ENETRESET 126
#define ENETUNREACH 114 /* Network is unreachable */
#define ENFILE 23 /* Too many open files in system */
#define ENOANO 53 /* No anode */
#define ENOBUFS 105 /* No buffer space available */
#define ENOCSI 43 /* No CSI structure available */
#define ENODATA 61 /* No data (for no delay io) */
#define ENODEV 19 /* No such device */
#define ENOENT 2 /* No such file or directory */
#define ENOEXEC 8 /* Exec format error */
#define ENOLCK 46 /* No record locks available */
#define ENOLINK 67 /* The link has been severed */
#define ENOMEM 12 /* Not enough core */
#define ENOMSG 35 /* No message of desired type */
#define ENONET 64 /* Machine is not on the network */
#define ENOPKG 65 /* Package not installed */
#define ENOPROTOOPT 109 /* Protocol not available */
#define ENOSPC 28 /* No space left on device */
#define ENOSR 63 /* Out of streams resources */
#define ENOSTR 60 /* Device not a stream */
#define ENOSYS 88 /* Function not implemented */
#define ENOTBLK 15 /* Block device required */
#define ENOTCONN 128 /* Socket is not connected */
#define ENOTDIR 20 /* Not a directory */
#define ENOTEMPTY 90 /* Directory not empty */
#define ENOTRECOVERABLE 141 /* State not recoverable */
#define ENOTSOCK 108 /* Socket operation on non-socket */
#define ENOTSUP 134 /* Not supported */
#define ENOTTY 25 /* Not a typewriter */
#define ENOTUNIQ 80 /* Given log. name not unique */
#define ENXIO 6 /* No such device or address */
#define EOPNOTSUPP 95 /* Operation not supported on transport endpoint */
#define EOVERFLOW 139 /* Value too large for data type */
#define EOWNERDEAD 142 /* Previous owner died */
#define EPERM 1 /* Operation not permitted */
#define EPFNOSUPPORT 96 /* Protocol family not supported */
#define EPIPE 32 /* Broken pipe */
#define EPROCLIM 130
#define EPROTO 71 /* Protocol error */
#define EPROTONOSUPPORT 123 /* Unknown protocol */
#define EPROTOTYPE 107 /* Protocol wrong type for socket */
#define ERANGE 34 /* Math result not representable */
#define EREMCHG 82 /* Remote address changed */
#define EREMOTE 66 /* The object is remote */
#define EROFS 30 /* Read only file system */
#define ESHUTDOWN 110 /* Can't send after socket shutdown */
#define ESOCKTNOSUPPORT 124 /* Socket type not supported */
#define ESPIPE 29 /* Illegal seek */
#define ESRCH 3 /* No such process */
#define ESRMNT 69 /* Srmount error */
#define ESTALE 133
#define ESTRPIPE 143 /* Streams pipe error */
#define ETIME 62 /* Timer expired */
#define ETIMEDOUT 116 /* Connection timed out */
#define ETOOMANYREFS 129
#define ETXTBSY 26 /* Text file busy */
#define EUNATCH 42 /* Protocol driver not attached */
#define EUSERS 131
#define EWOULDBLOCK EAGAIN /* Operation would block */
#define EXDEV 18 /* Cross-device link */
#define EXFULL 52 /* Exchange full */
