#include <stdio.h>
#include <stdint.h>

int errno = -1;

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

char *strerror(int err) {
    if (err < 0) {
        return NULL;
    }

    return error_strings[err - 1];
}

void perror(char *func) {
    printf("%s: %s\n", func, strerror(errno));
}
