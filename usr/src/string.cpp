#include <string.h>
#include <stdint.h>
#include <mem.h>
#include <stdarg.h>
#include <ctype.h>

extern "C" void strcpy(char *dest, const char *orig)
{
    for (int z = 0; z < strlen(orig); z++)
        dest[z] = orig[z];
}

extern "C" void memset(void *dest, char val, int length)
{
    unsigned char *d = (unsigned char *)dest;

    for (int z = 0; z < length; z++)
        d[z] = val;
}

extern "C" void memcpy(void *dest, const void *orig, int length)
{
    uint8_t *d = (uint8_t *)dest;
    uint8_t *s = (uint8_t *)orig;

    for (int z = 0; z < length; z++)
        d[z] = s[z];
}

extern "C" int strcmp(const char * str1, const char * str2)
{
    int total = strlen(str1);
    int sec = strlen(str2);

    if (total != sec) return 1;

    for (int z = 0; z < total; z++)
        if (str1[z] != str2[z]) return 1;

    return 0;
}

extern "C" char *strdup(const char *str) {
    int len = strlen(str);
    char *s = (char *)malloc(len + 1);
    memset(s, 0, len + 1);
    strcpy(s, str);
    return s;
}

static int is_valid(int base, char c) {
	if (c < '0') return 0;
	if (base <= 10) {
		return c < ('0' + base);
	}

	if (c >= 'a' && c < 'a' + (base - 10)) return 1;
	if (c >= 'A' && c < 'A' + (base - 10)) return 1;
	if (c >= '0' && c <= '9') return 1;
	return 0;
}

static int convert_digit(char c) {
	if (c >= '0' && c <= '9') {
		return c - '0';
	}
	if (c >= 'a' && c <= 'z') {
		return c - 'a' + 0xa;
	}
	if (c >= 'A' && c <= 'Z') {
		return c - 'A' + 0xa;
	}
	return 0;
}

#define strtox(max, type) \
	if (base < 0 || base == 1 || base > 36) { \
		errno = EINVAL; \
		return max; \
	} \
	while (*nptr && isspace(*nptr)) nptr++; \
	int sign = 1; \
	if (*nptr == '-') { \
		sign = -1; \
		nptr++; \
	} else if (*nptr == '+') { \
		nptr++; \
	} \
	if (base == 16) { \
		if (*nptr == '0') { \
			nptr++; \
			if (*nptr == 'x') { \
				nptr++; \
			} \
		} \
	} \
	if (base == 0) { \
		if (*nptr == '0') { \
			base = 8; \
			nptr++; \
			if (*nptr == 'x') { \
				base = 16; \
				nptr++; \
			} \
		} else { \
			base = 10; \
		} \
	} \
	type val = 0; \
	while (is_valid(base, *nptr)) { \
		val *= base; \
		val += convert_digit(*nptr); \
		nptr++; \
	} \
	if (endptr) { \
		*endptr = (char *)nptr; \
	} \
	if (sign == -1) { \
		return -val; \
	} else { \
		return val; \
	}

extern "C" int strtol(char *nptr, char **endptr, int base) {
	strtox(0, int);
}


extern "C" char * strchr(char * s, int c) {
    while (*s != (char) c) {
        if (!*s++) {
            return NULL;
        }
    }
    return s;
}

extern "C" int vsscanf(char *str, char *fmt, va_list va) {
	int cz = 0;

	while (*fmt) {
		if (*fmt == ' ') {
			while (*str && isspace(*str))
				str++;
		} else if (*fmt == '%') {
			fmt++;

			if (*fmt == 'd') {
				int z = 0;
				int negative = 1;

				while (isspace(*str)) str++;

                // see if the first is a minus sign
				if (*str == '-') {
					negative = -1;
					str++;
				}

				while (*str && *str >= '0' && *str <= '9') {
					z = z * 10 + *str - '0';
					str++;
				}

				int * out = (int *)va_arg(va, int*);
				cz++;
				*out = z * negative;
			} else if (*fmt == 'u') {
				uint32_t z = 0;

				while (isspace(*str)) str++;

				while (*str && *str >= '0' && *str <= '9') {
					z = z * 10 + *str - '0';
					str++;
				}

				uint32_t *out = (uint32_t *)va_arg(va, uint32_t*);
				cz++;
				*out = z;
			} else if (*fmt == 'x') {
                uint32_t z = 0;

                while (isspace(*str)) str++;

                z = strtol(str, NULL, 16);
                int *out = (int *)va_arg(va, int*);
                cz++;
                *out = z;
            } else if (*fmt == 's') {
                char **out = (char **)va_arg(va, char**);
                char *s = (char *)&out[0];
                int sz = 0;

                while (isspace(*str)) str++;

                while (isalpha(*str)) {
                    s[sz] = *str;
                    *str++;
                    sz++;
                }

                s[sz] = 0;
                cz++;
            }
		} else {
			if (*str == *fmt)
				str++;
			else
				break;
		}

		fmt++;
	}

	return cz;
}

extern "C" int sscanf(char *s, char *fmt, ...) {
	va_list va;
    int out;

	va_start(va, fmt);
	out = vsscanf(s, fmt, va);
	va_end(va);

	return out;
}

extern "C" char *strstr(char *s, char *d) {
	for (int i = 0; s[i]; i++) {
		bool found = false;

		for (int j = 0; d[j]; j++) {
			if (!d[j] || s[i + j] == d[j])
				continue;

			found = true;
			break;
		}

		if (!found)
			return (char *)&s[i];
	}

	return NULL;
}

extern "C" char *strtok_r(char *s, char *del, char **m) {
	char *tok;

	if (s)
		tok = s;
	else if (*m)
		tok = *m;
	else
		return NULL;

	while (*tok && strchr(del, *tok))
		tok++;

	auto p = tok;

	while (*p && !strchr(del, *p))
		p++;

	if (*p) {
		*p = 0;
		*m = p + 1;
	} else {
		*m = NULL;
	}

	if (p == tok)
		return NULL;

	return tok;
}

extern "C" char *strtok(char *s, char *del) {
        static char *saved;
        return strtok_r(s, del, &saved);
}

namespace text {
    int len(const char *str) {
        return strlen(str);
    }

    void copy(void *d, const void *s, int length = -1) {
        if (length == -1) {
            length = text::len((const char *)s);
        }

        memcpy(d, s, length);
    }

    int compare(const char *s1, const char *s2) {
        return strcmp(s1, s2);
    }

    char *duplicate(const char *s) {
        return strdup(s);
    }

    void set(void *dest, char val, int length) {
        memset(dest, val, length);
    }
}
