#include <filesystem/node.h>
#include <filesystem/file.h>
#include <ctype.h>
#include <string.h>

FILE *fopen(char *path)
{
    int id = find_id(path);

    if (id == -1)
        return NULL;

    fs_node node = find_node(id);

    if (node.flags != FS_NODE_FILE)
        return NULL;

    FILE *file = (FILE *)malloc(sizeof(FILE *));

    file->node = node;
    file->null = node.null;
    file->eof = NULL;
    file->ptr = NULL;

    strcpy(file->name, node.name);

    return file;
}

void fclose(FILE *file)
{
    free(file);
}

int create_file(char *path, char *folder, char *contents)
{
    folder = format_folder(folder);
    int id = find_id(folder);

    if (id < 0)
        return 1;

    fs_node node = create_node(path, id, FS_NODE_FILE);

    if (node.null)
        return 1;

    node_write_basic(node.id, contents);

    return 0;
}

int create_file(char *path, char *folder, __read read, __write write)
{
    folder = format_folder(folder);
    int parent_id = find_id(folder);

    if (parent_id < 0)
        return 1;

    fs_node node = create_node(path, parent_id, FS_NODE_FILE);

    if (node.null)
        return 1;

    node.write = write;
    node.read = read;

    nodes[node.id] = node;

    return 0;
}

void fprintf(FILE *file, char *data)
{
    fprintf(file->node, data);
}

void fprintf(FILE file, char *data)
{
    fprintf(file.node, data);
}

void fprintf(fs_node node, char *data)
{
    if (node.write != NULL)
    {
        node.write(node, 0, node.size, data);
    }
    else
    {
        node_write_basic(node.id, data);
        node = nodes[node.id];
    }
}

int fwrite(char *buf, int offset, int size, FILE *file)
{
    if (file == NULL) return 1;

    if (file->node.write)
        file->node.write(file->node, offset, size, buf);
    else
        node_write_basic(file->node.id, buf);

    file->node = find_node(file->node.id);

    return 0;
}

char * fread(char *buf, int offset, int size, FILE *file)
{
    if (file->node.read)
        buf = file->node.read(file->node, offset, size, buf);
    else
    {
        char *tmp = node_read_basic(file->node.id);
        strcpy(buf, &tmp[offset]);
        buf[size] = 0;
    }
    return buf;
}

char *fread(char *buf, FILE *file)
{
    return fread(buf, 0, file->node.size, file);
}

int fgetc(FILE *file)
{
    char *str = fread(str, file->ptr, 1, file);
    file->ptr++;

    if (file->ptr == file->node.size)
        file->eof = EOF;

    return str[0];
}

int feof(FILE *file)
{
    return file->eof < 0;
}

char *fgets(char *str, int n, FILE *file)
{
    char c;
    int z = 0;

    while (true)
    {
        if (z == n || feof(file))
            break;

        c = fgetc(file);

        str[z] = c;
        z++;
    }

    str[z] = NULL;

    if (z == 0)
        return NULL;

    return str;
}

int fgetpos(FILE *file, fpos_t *pos)
{
    *pos = file->ptr;

    return *pos;
}

int fseek(FILE *file, int offset, int w)
{
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
    char *str = file->read("", file->ptr, file->node.size);

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
