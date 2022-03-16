#include "file.h"
#include "node.h"

int create_file(char *name, char *path, int permission)
{
    fs_node_t *node = create_node(name, path, FS_FILE, permission);

    if (node == NULL)
        return 1;

    return 0;
}

int create_file(char *name, char *path)
{
    return create_file(name, path, USER_PERMISSION);
}

int create_file(char *name, char *path, char *contents, int permission)
{
    fs_node_t *node = create_node(name, path, FS_FILE, permission);

    if (node == NULL)
        return 1;

    node->size = strlen(contents);
    node->contents = contents;

    nodes[node->id] = node;

    return 0;
}

int create_file(char *name, char *path, char *contents)
{
    return create_file(name, path, contents, USER_PERMISSION);
}

int kcreate_file(char *name, char *path, char *contents)
{
    return create_file(name, path, contents, ROOT_PERMISSION);
}

int create_file(char *name, char *path, __read read, __write write, int permission)
{
    fs_node_t *node = create_node(name, path, FS_FILE, permission);

    if (node == NULL)
        return 1;

    node->write = write;
    node->read = read;

    nodes[node->id] = node;

    return 0;
}

int create_file(char *name, char *path, __read read, __write write)
{
    return create_file(name, path, read, write, USER_PERMISSION);
}

int kcreate_file(char *name, char *path, __read read, __write write)
{
    return create_file(name, path, read, write, ROOT_PERMISSION);
}

FILE *fopen(char *path, int permission)
{
    fs_node_t *node = find_node(path);

    if (node == NULL)
        return NULL;

    if (node->permission < permission)
        return NULL;

    FILE *file = new FILE();

    file->ptr = 0;
    file->eof = 0;
    file->node = node;

    return file;
}

FILE *fopen(char *path)
{
    return fopen(path, USER_PERMISSION);
}

FILE *kopen(char *path)
{
    return fopen(path, ROOT_PERMISSION);
}

void fclose(FILE *file)
{
    free(file);
}

int fwrite(FILE *file, int size, int n, char *buffer)
{
    int ret = write_node(file->node, file->ptr, size * n, buffer);

    return ret;
}

int fgetc(FILE *file)
{
    char buf[1];
    fread(file, 1, 1, buf);
    file->ptr++;

    if (file->ptr == file->node->size)
        file->eof = EOF;

    return buf[0];
}

int fread(FILE *file, int size, int n, char *buffer)
{
    int ret = read_node(file->node, file->ptr, size * n, buffer);

    file->ptr += size * n;

    if (file->ptr >= file->node->size)
        file->ptr = EOF;

    return ret;
}

int feof(FILE *file)
{
    return file->eof < 0;
}

char *fgets(char *str, int n, FILE *file)
{
    fread(file, 1, n, str);
    return str;
}

int fgetpos(FILE *file, fpos_t *pos)
{
    *pos = file->ptr;

    return *pos;
}

int fseek(FILE *file, int offset, int whence)
{
    int w = 0;

    switch (whence)
    {
        case SEEK_SET:
            w = 0;
            break;
        case SEEK_CUR:
            w = file->ptr;
            break;
        case SEEK_END:
            w = file->node->size;
            break;
    }

    if (file == NULL)
        return 1;

    file->ptr = offset + w;
    return 0;
}

int ftell(FILE *file)
{
    int offset[1];
    offset[0] = fgetpos(file, offset);
    return offset[0];
}

int fsize(FILE *file)
{
    fseek(file, 0, SEEK_END);
    int ret = ftell(file);
    fseek(file, 0, 0);
    return ret;
}

void rewind(FILE *file)
{
    fseek(file, 0, 0);
}

int fsetpos(FILE *file, fpos_t *pos)
{
    file->ptr = *pos;
    return file->ptr;
}

int fvsscanf(FILE *file, char *fmt, va_list va)
{
	int cz = 0;
    char *str = (char *)malloc(file->node->size - file->ptr + 1);
    fread(file, 1, file->node->size - file->ptr, str);

	while (*fmt)
	{
		if (*fmt == ' ')
		{
			while (*str && isspace(*str))
            {
				str++;
                file->ptr++;
            }
		}
		else if (*fmt == '%')
		{
			fmt++;
			int _long = 0;

			if (*fmt == 'l')
			{
				fmt++;

				if (*fmt == 'l')
				{
					_long = 1;
					fmt++;
				}
			}

			if (*fmt == 'd')
			{
				int z = 0;
				int negative = 1;

				while (isspace(*str))
                {
                    str++;
                    file->ptr++;
                }

                // see if the first is a minus sign
				if (*str == '-')
				{
					negative = -1;
					str++;
                    file->ptr++;
				}

				while (*str && *str >= '0' && *str <= '9')
				{
					z = z * 10 + *str - '0';
					str++;
                    file->ptr++;
				}

				int * out = (int *)va_arg(va, int*);
				cz++;
				*out = z * negative;
			}
			else if (*fmt == 'u')
			{
				uint32_t z = 0;

				while (isspace(*str))
                {
                    str++;
                    file->ptr++;
                }

				while (*str && *str >= '0' && *str <= '9')
				{
					z = z * 10 + *str - '0';
					str++;
                    file->ptr++;
				}

				uint32_t *out = (uint32_t *)va_arg(va, uint32_t*);
				cz++;
				*out = z;
			}
            else if (*fmt == 's')
            {
                char **out = (char **)va_arg(va, char**);
                char *s = (char *)&out[0];
                int sz = 0;

                while (isspace(*str))
                {
                    str++;
                    file->ptr++;
                }

                while (isalpha(*str))
                {
                    s[sz] = *str;
                    *str++;
                    file->ptr++;
                    sz++;
                }

                s[sz] = 0;
                cz++;

                if (strisempty(s))
                {
                    s = NULL;
                    cz--;
                }
            }
		}
		else
		{
			if (*str == *fmt)
            {
				str++;
                file->ptr++;
            }
			else
				break;
		}

		fmt++;
	}

	if (cz <= 0)
        return EOF;
    else
        return cz;
}

int fscanf(FILE *file, char *fmt, ...)
{
    va_list va;

    va_start(va, fmt);
    int ret = fvsscanf(file, fmt, va);
    va_end(va);

    return ret;
}
