#pragma once

#define EPERM 1               /* Not super-user */
#define ENOENT 2              /* No such file or directory */
#define ESRCH 3               /* No such process */
#define EINTR 4               /* Interrupted system call */
#define EIO 5                 /* I/O error */
#define ENXIO 6               /* No such device or address */
#define E2BIG 7               /* Arg list too long */
#define ENOEXEC 8             /* Exec format error */
#define EBADF 9               /* Bad file number */
#define ECHILD 10             /* No children */
#define EAGAIN 11             /* No more processes */
#define ENOMEM 12             /* Not enough core */
#define EACCES 13             /* Permission denied */
#define EFAULT 14             /* Bad address */
#define ENOTBLK 15            /* Block device required */
#define EBUSY 16              /* Mount device busy */
#define EEXIST 17             /* File exists */
#define EXDEV 18              /* Cross-device link */
#define ENODEV 19             /* No such device */
#define ENOTDIR 20            /* Not a directory */
#define EISDIR 21             /* Is a directory */
#define EINVAL 22             /* Invalid argument */
#define ENFILE 23             /* Too many open files in system */
#define EMFILE 24             /* Too many open files */
#define ENOTTY 25             /* Not a typewriter */
#define ETXTBSY 26            /* Text file busy */
#define EFBIG 27              /* File too large */
#define ENOSPC 28             /* No space left on device */
#define ESPIPE 29             /* Illegal seek */
#define EROFS 30              /* Read only file system */
#define EMLINK 31             /* Too many links */
#define EPIPE 32              /* Broken pipe */
#define EDOM 33               /* Math arg out of domain of func */
#define ERANGE 34             /* Math result not representable */
#define ENOMSG 35             /* No message of desired type */
#define EIDRM 36              /* Identifier removed */
#define ECHRNG 37             /* Channel number out of range */
#define EL2NSYNC 38           /* Level 2 not synchronized */
#define EL3HLT 39             /* Level 3 halted */
#define EL3RST 40             /* Level 3 reset */
#define ELNRNG 41             /* Link number out of range */
#define EUNATCH 42            /* Protocol driver not attached */
#define ENOCSI 43             /* No CSI structure available */
#define EL2HLT 44             /* Level 2 halted */
#define EDEADLK 45            /* Deadlock condition */
#define ENOLCK 46             /* No record locks available */
#define EBADE 50              /* Invalid exchange */
#define EBADR 51              /* Invalid request descriptor */
#define EXFULL 52             /* Exchange full */
#define ENOANO 53             /* No anode */
#define EBADRQC 54            /* Invalid request code */
#define EBADSLT 55            /* Invalid slot */
#define EDEADLOCK 56          /* File locking deadlock error */
#define EBFONT 57             /* Bad font file fmt */
#define ENOSTR 60             /* Device not a stream */
#define ENODATA 61            /* No data (for no delay io) */
#define ETIME 62              /* Timer expired */
#define ENOSR 63              /* Out of streams resources */
#define ENONET 64             /* Machine is not on the network */
#define ENOPKG 65             /* Package not installed */
#define EREMOTE 66            /* The object is remote */
#define ENOLINK 67            /* The link has been severed */
#define EADV 68               /* Advertise error */
#define ESRMNT 69             /* Srmount error */
#define ECOMM 70              /* Communication error on send */
#define EPROTO 71             /* Protocol error */
#define EMULTIHOP 74          /* Multihop attempted */
#define ELBIN 75              /* Inode is remote (not really error) */
#define EDOTDOT 76            /* Cross mount point (not really error) */
#define EBADMSG 77            /* Trying to read unreadable message */
#define EFTYPE 79             /* Inappropriate file type or format */
#define ENOTUNIQ 80           /* Given log. name not unique */
#define EBADFD 81             /* f.d. invalid for this operation */
#define EREMCHG 82            /* Remote address changed */
#define ELIBACC 83            /* Can't access a needed shared lib */
#define ELIBBAD 84            /* Accessing a corrupted shared lib */
#define ELIBSCN 85            /* .lib section in a.out corrupted */
#define ELIBMAX 86            /* Attempting to link in too many libs */
#define ELIBEXEC 87           /* Attempting to exec a shared library */
#define ENOSYS 88             /* Function not implemented */
#define ENOTEMPTY 90          /* Directory not empty */
#define ENAMETOOLONG 91       /* File or path name too long */
#define ELOOP 92              /* Too many symbolic links */
#define EOPNOTSUPP 95         /* Operation not supported on transport endpoint */
#define EPFNOSUPPORT 96       /* Protocol family not supported */
#define ECONNRESET 104        /* Connection reset by peer */
#define ENOBUFS 105           /* No buffer space available */
#define EAFNOSUPPORT 106      /* Address family not supported by protocol family */
#define EPROTOTYPE 107        /* Protocol wrong type for socket */
#define ENOTSOCK 108          /* Socket operation on non-socket */
#define ENOPROTOOPT 109       /* Protocol not available */
#define ESHUTDOWN 110         /* Can't send after socket shutdown */
#define ECONNREFUSED 111      /* Connection refused */
#define EADDRINUSE 112        /* Address already in use */
#define ECONNABORTED 113      /* Connection aborted */
#define ENETUNREACH 114       /* Network is unreachable */
#define ENETDOWN 115          /* Network interface is not configured */
#define ETIMEDOUT 116         /* Connection timed out */
#define EHOSTDOWN 117         /* Host is down */
#define EHOSTUNREACH 118      /* Host is unreachable */
#define EINPROGRESS 119       /* Connection already in progress */
#define EALREADY 120          /* Socket already connected */
#define EDESTADDRREQ 121      /* Destination address required */
#define EMSGSIZE 122          /* Message too long */
#define EPROTONOSUPPORT 123   /* Unknown protocol */
#define ESOCKTNOSUPPORT 124   /* Socket type not supported */
#define EADDRNOTAVAIL 125     /* Address not available */
#define ENETRESET 126
#define EISCONN 127           /* Socket is already connected */
#define ENOTCONN 128          /* Socket is not connected */
#define ETOOMANYREFS 129
#define EPROCLIM 130
#define EUSERS 131
#define EDQUOT 132
#define ESTALE 133
#define ENOTSUP 134           /* Not supported */
#define EILSEQ 138
#define EOVERFLOW 139         /* Value too large for defined data type */
#define ECANCELED 140         /* Operation canceled */
#define ENOTRECOVERABLE 141   /* State not recoverable */
#define EOWNERDEAD 142        /* Previous owner died */
#define ESTRPIPE 143          /* Streams pipe error */
#define EWOULDBLOCK EAGAIN    /* Operation would block */

char *error_strings[] = {
	"Not super-user",
	"No such file or directory",
	"No such process",
	"Interrupted system call",
	"I/O error",
	"No such device or address",
	"Arg list too long",
	"Exec format error",
	"Bad file number",
	"No children",
	"No more processes",
	"Not enough core",
	"Permission denied",
	"Bad address",
	"Block device required",
	"Mount device busy",
	"File exists",
	"Cross-device link",
	"No such device",
	"Not a directory",
	"Is a directory",
	"Invalid argument",
	"Too many open files in system",
	"Too many open files",
	"Not a typewriter",
	"Text file busy",
	"File too large",
	"No space left on device",
	"Illegal seek",
	"Read only file system",
	"Too many links",
	"Broken pipe",
	"Math arg out of domain of func",
	"Math result not representable",
	"No message of desired type",
	"Identifier removed",
	"Channel number out of range",
	"Level 2 not synchronized",
	"Level 3 halted",
	"Level 3 reset",
	"Link number out of range",
	"Protocol driver not attached",
	"No CSI structure available",
	"Level 2 halted",
	"Deadlock condition",
	"No record locks available",
	"Invalid exchange",
	"Invalid request descriptor",
	"Exchange full",
	"No anode",
	"Invalid request code",
	"Invalid slot",
	"File locking deadlock error",
	"Bad font file fmt",
	"Device not a stream",
	"No data (for no delay io)",
	"Timer expired",
	"Out of streams resources",
	"Machine is not on the network",
	"Package not installed",
	"The object is remote",
	"The link has been severed",
	"Advertise error",
	"Srmount error",
	"Communication error on send",
	"Protocol error",
	"Multihop attempted",
	"Inode is remote (not really error)",
	"Cross mount point (not really error)",
	"Trying to read unreadable message",
	"Inappropriate file type or format",
	"Given log. name not unique",
	"f.d. invalid for this operation",
	"Remote address changed",
	"Can't access a needed shared lib",
	"Accessing a corrupted shared lib",
	".lib section in a.out corrupted",
	"Attempting to link in too many libs",
	"Attempting to exec a shared library",
	"Function not implemented",
	"Directory not empty",
	"File or path name too long",
	"Too many symbolic links",
	"Operation not supported on transport endpoint",
	"Protocol family not supported",
	"Connection reset by peer",
	"No buffer space available",
	"Address family not supported by protocol family",
	"Protocol wrong type for socket",
	"Socket operation on non-socket",
	"Protocol not available",
	"Can't send after socket shutdown",
	"Connection refused",
	"Address already in use",
	"Connection aborted",
	"Network is unreachable",
	"Network interface is not configured",
	"Connection timed out",
	"Host is down",
	"Host is unreachable",
	"Connection already in progress",
	"Socket already connected",
	"Destination address required",
	"Message too long",
	"Unknown protocol",
	"Socket type not supported",
	"Address not available",
	"",
	"Socket is already connected",
	"Socket is not connected",
	"",
	"",
	"",
	"",
	"",
	"Not supported",
	"",
	"Value too large for defined data type",
	"Operation canceled",
	"State not recoverable",
	"Previous owner died",
	"Streams pipe error",
	"Operation would block",
};
