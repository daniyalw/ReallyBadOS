#define DEBUG_ON 0

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <ctype.h>

int handle_comment(char *line, char **buf, int sz);
void debug(char *text, ...);

#if DEBUG_ON == 1
#  define DEBUG(text, ...)
#else
#  define DEBUG debug
#endif

#define VVARNF 1 /* variable not found */
#define VVARF 2 /* variable exists */
#define VCMDNF 3 /* command not full */
#define VTPNF 4 /* type not found */
#define VWRTP 5 /* wrong type */

void print_error();
void seterr(int _err);
int geterr();
char **get_errors();

int op_add(char *line, char **buf, int ret);
int op_subtract(char *line, char **buf, int ret);
int op_multiply(char *line, char **buf, int ret);
int op_division(char *line, char **buf, int ret);

int print(char *line, char **buf, int ret);

int tokenize(char * str, char * d, char **buf);

int startswith(char *str, char *begin);
int contains_char(char *string, char ch);

#define T_INT 1
#define T_CHAR 2
#define T_STR 3

#define S_INT "ir"
#define S_CHAR "cr"
#define S_STR "sr"

typedef struct
{
    char *name;
    char *val;
    int type;
} var_t;

typedef struct
{
    char *name;
    int (*command)(char *, char **, int);
} command_t;

int total_commands = 8;

void check_variables_mem();
void variable_down(int index);

void free_variable(int index);
void free_all_variables();
int index_var(char *name);

int add_variable(char *name, char *val, int type);
int create_variable(char * line, char **buf, int ret);
int show_var(char *line, char **buf, int ret);

void init_variables();

void print_vars();

var_t *get_all_vars();
int get_var_count();

command_t commands[] = {
    {"pr", print},
    {"va", create_variable},
    {"sw", show_var},
    {"pl", op_add},
    {"sb", op_subtract},
    {"ml", op_multiply},
    {"dv", op_division},
    {"cm", handle_comment}
};

int handle_comment(char *line, char **buf, int sz)
{
    // since it is a comment, just skip
    return 0;
}

void debug(char *text, ...)
{
    va_list va;
    va_start(va, text);
    char out[strlen(text) + 100];
    memset(out, 0, strlen(text) + 100);
    vsprintf(out, text, va);
    va_end(va);

    printf("DEBUG: %s\n", text);
}

int err = 0;

char *estrs[] = {
    "",
    "variable doesn't exist",
    "variable already exists",
    "command not complete",
    "type not found",
    "wrong type"
};

void print_error()
{
    if (err)
    {
        printf("ERROR: %s\n", estrs[err]);
    }
}

void seterr(int _err)
{
    err = _err;
}

int geterr() {
    return err;
}

char **get_errors() {
    return estrs;
}

int op_add(char *line, char **buf, int ret)
{
    if (ret < 3)
    {
        seterr(VCMDNF);
        return 1;
    }

    char target[100];
    memset(target, 0, 100);
    strcpy(target, &buf[1][1]);

    int index = index_var(target);

    if (index < 0)
    {
        seterr(VVARNF);
        return 1;
    }

    var_t var = get_all_vars()[index];

    if (var.type == T_INT)
    {
        for (int z = 0; z < strlen(buf[2]); z++)
        {
            if (!isdigit(buf[2][z]))
            {
                seterr(VWRTP);
                return 1;
            }
        }

        int orig = atoi(var.val);
        int num = atoi(buf[2]);
        sprintf(var.val, "%d", orig + num);
    }
    else if (var.type == T_CHAR)
    {
        seterr(VWRTP);
        return 1;
    }
    else if (var.type == T_STR)
    {
        int a = 0;
        int pos = -1;

        for (int z = 0; z < strlen(line); z++)
        {
            if (line[z] == ' ')
            {
                if (a)
                {
                    pos = z + 1;
                    break;
                }

                a = 1;
            }
        }

        sprintf(var.val, "%s%s", var.val, &line[pos]);
    }
    else
    {
        seterr(VTPNF);
        return 1;
    }

    get_all_vars()[index] = var;

    return 0;
}

int op_subtract(char *line, char **buf, int ret)
{
    if (ret < 3)
    {
        seterr(VCMDNF);
        return 1;
    }

    char target[100];
    memset(target, 0, 100);
    strcpy(target, &buf[1][1]);

    int index = index_var(target);

    if (index < 0)
    {
        seterr(VVARNF);
        return 1;
    }

    var_t var = get_all_vars()[index];

    if (var.type == T_INT)
    {
        for (int z = 0; z < strlen(buf[2]); z++)
        {
            if (!isdigit(buf[2][z]))
            {
                seterr(VWRTP);
                return 1;
            }
        }

        int orig = atoi(var.val);
        int num = atoi(buf[2]);
        sprintf(var.val, "%d", orig - num);
    }
    else if (var.type == T_CHAR)
    {
        seterr(VWRTP);
        return 1;
    }
    else if (var.type == T_STR)
    {
        int size = atoi(buf[2]);
        int length = strlen(var.val);

        if (length >= size)
        {
            var.val[length - size] = 0;
        }
    }
    else
    {
        seterr(VTPNF);
        return 1;
    }

    get_all_vars()[index] = var;

    return 0;
}

int op_multiply(char *line, char **buf, int ret)
{
    if (ret < 3)
    {
        seterr(VCMDNF);
        return 1;
    }

    char target[100];
    memset(target, 0, 100);
    strcpy(target, &buf[1][1]);

    int index = index_var(target);

    if (index < 0)
    {
        seterr(VVARNF);
        return 1;
    }

    var_t var = get_all_vars()[index];

    if (var.type == T_INT)
    {
        for (int z = 0; z < strlen(buf[2]); z++)
        {
            if (!isdigit(buf[2][z]))
            {
                seterr(VWRTP);
                return 1;
            }
        }

        int orig = atoi(var.val);
        int num = atoi(buf[2]);
        sprintf(var.val, "%d", orig * num);
    }
    else if (var.type == T_CHAR)
    {
        seterr(VWRTP);
        return 1;
    }
    else if (var.type == T_STR)
    {
        seterr(VWRTP);
        return 1;
    }
    else
    {
        seterr(VTPNF);
        return 1;
    }

    get_all_vars()[index] = var;

    return 0;
}

int op_division(char *line, char **buf, int ret)
{
    if (ret < 3)
    {
        seterr(VCMDNF);
        return 1;
    }

    char target[100];
    memset(target, 0, 100);
    strcpy(target, &buf[1][1]);

    int index = index_var(target);

    if (index < 0)
    {
        seterr(VVARNF);
        return 1;
    }

    var_t var = get_all_vars()[index];

    if (var.type == T_INT)
    {
        for (int z = 0; z < strlen(buf[2]); z++)
        {
            if (!isdigit(buf[2][z]))
            {
                seterr(VWRTP);
                return 1;
            }
        }

        int orig = atoi(var.val);
        int num = atoi(buf[2]);
        sprintf(var.val, "%d", orig/num);
    }
    else if (var.type == T_CHAR)
    {
        seterr(VWRTP);
        return 1;
    }
    else if (var.type == T_STR)
    {
        seterr(VWRTP);
        return 1;
    }
    else
    {
        seterr(VTPNF);
        return 1;
    }

    get_all_vars()[index] = var;

    return 0;
}

int handle_string(char *str)
{
    for (int z = 0; z < strlen(str); z++)
    {
        if (str[z] == '\"')
            return -1;

        putchar(str[z]);
    }

    putchar(' ');

    return 0;
}

int print(char *line, char **buf, int ret)
{
    int is_in_str = 0;

    for (int z = 1; z < ret; z++)
    {
        if (is_in_str)
        {
            int ret = handle_string(buf[z]);

            if (ret == 1)
                return 1;
            else if (ret == -1)
                is_in_str = 0;

            continue;
        }

        if (startswith(buf[z], "$"))
        {
            char target[100];
            memset(target, 0, 100);
            sscanf(buf[z], "$%s", (char *)&target);

            int index = index_var(target);

            if (index == -1)
            {
                seterr(VVARNF);
                return 1;
            }

            printf("%s ", (char *)get_all_vars()[index].val);
        }
        else if (startswith(buf[z], "\""))
        {
            is_in_str = 1;
            int ret = handle_string(&buf[z][1]);

            if (ret == 1)
                return 1;
            else if (ret == -1)
                is_in_str = 0;
        }
        else if (strcmp(buf[z], "+") == 0)
        {
            continue;
        }
        else if (strcmp(buf[z], "NL") == 0)
        {
            putchar('\n');
            continue;
        }
    }

    putchar('\n');

    return 0;
}

int tokenize(char * str, char * d, char **buf)
{
	char *p;
	char *s;
	int args = 0;

	p = strtok_r(str, d, &s);

	if (!p)
        return 0;

	while (p != NULL)
    {
		buf[args] = (char *)p;
		args++;
		p = strtok_r(NULL, d, &s);
	}

	buf[args] = NULL;
	return args;
}

int startswith(char *str, char *begin)
{
    const int first = strlen(str);
    const int max = strlen(begin);

    if (max == 0 || first < max)
        return 0;

    for (int z = 0; z < max; z++)
    {
        if (str[z] != begin[z])
            return 0;
    }

    return 1;
}

int contains_char(char *string, char ch)
{
    for (int z = 0; z < strlen(string); z++)
        if (string[z] == ch)
            return 1;

    return 0;
}

var_t all_variables[100];
int variable_count = 0;
int allocated = 0;

void check_variables_mem()
{
}

var_t *get_all_vars() {
    return all_variables;
}

int get_var_count() {
    return variable_count;
}

int add_variable(char *name, char *val, int type)
{
    // make sure the name doesn't already exist
    if (index_var(name) != -1)
    {
        seterr(VVARF);
        return 1;
    }

    var_t var;
    var.name = name;
    var.val = val;
    var.type = type;

    all_variables[variable_count] = var;
    variable_count++;

    check_variables_mem();

    return 0;
}

int edit_variable(char *name, char *newval)
{
    int index;

    if ((index = index_var(name)) == -1)
    {
        seterr(VVARNF);
        return 1;
    }

    var_t var = all_variables[index];
    var.val = newval;

    all_variables[index] = var;

    return 0;
}

void print_vars()
{
    for (int z = 0; z < variable_count; z++)
    {
        printf("VARIABLE %s of value %s\n", all_variables[z].name, (char *)all_variables[z].val);
    }
}

void init_variables()
{
}

void variable_down(int index)
{
    for (int b = index; b < variable_count; b++)
    {
        all_variables[b] = all_variables[b + 1];
    }

    variable_count--;
}

void free_variable(int index)
{
    free(all_variables[index].name);
    variable_down(index);
}

void free_all_variables()
{
    for (int z = 0; z < variable_count; z++)
    {
        free(all_variables[z].name);
        free(all_variables[z].val);
    }
}

int index_var(char *name)
{
    for (int z = 0; z < variable_count; z++)
    {
        if (strcmp(all_variables[z].name, name) == 0)
            return z;
    }

    return -1;
}

int create_variable(char *line, char **buf, int ret)
{
    if (ret <= 2)
    {
        seterr(VCMDNF);
        return 1;
    }

    const int length = strlen(line);
    int type = -1;

    if (strcmp(buf[2], S_INT) == 0)
    {
        type = T_INT;
    }
    else if (strcmp(buf[2], S_CHAR) == 0)
    {
        type = T_CHAR;
    }
    else if (strcmp(buf[2], S_STR) == 0)
    {
        type = T_STR;
    }
    else
    {
        seterr(VTPNF);
        return 1;
    }

    char *target = (char *)malloc(100);
    memset(target, 0, 100);

    for (int z = 1; z < strlen(buf[1]); z++) {
        target[z - 1] = buf[1][z];
    }

    char *val = (char *)malloc(length);
    int x = 0;
    int began = 0;
    int done = 0;

    if (buf[3][0] == '$')
    {
        int index = index_var(&buf[3][1]);

        if (index < 0)
        {
            seterr(VVARNF);
            return 1;
        }

        strcpy(val, all_variables[index].val);
        return add_variable(target, val, type);
    }

    if (type == T_STR)
    {
        for (int z = 3; z < ret; z++)
        {
            for (int c = 0; c < strlen(buf[z]); c++)
            {
                if (buf[z][c] == '"')
                {
                    if (!began)
                        began = 1;
                    else
                        done = 1;

                    continue;
                }

                if (began && !done)
                {
                    val[x] = buf[z][c];
                    x++;
                }
            }

            if (began && !done)
            {
                val[x] = ' ';
                x++;
            }
        }
    }
    else if (type == T_INT)
    {
        strcpy(val, buf[3]);

        for (int z = 0; z < strlen(val); z++)
        {
            if (!isdigit(val[z]))
            {
                free(val);
                free(target);
                seterr(VWRTP);
                return 1;
            }
        }
    }
    else if (type == T_CHAR)
    {
        val[0] = buf[3][0];
    }


    int r = add_variable(target, val, type);

    return r;
}

int show_var(char *line, char **buf, int ret)
{
    if (ret < 2)
    {
        seterr(VCMDNF);
        return 1;
    }

    char target[100];
    memset(target, 0, 100);
    sscanf(buf[1], "$%s", (char *)&target);

    int index = index_var(target);

    if (index == -1)
    {
        seterr(VVARNF);
        return 1;
    }

    printf("Value: %s\n", (char *)all_variables[index].val);

    return 0;
}

#define MAX_SIZE 100000 // yes, one hundred thousand

int parse_line(char *line)
{
    char copy[strlen(line) + 1];
    memset(copy, 0, strlen(line) + 1);
    strcpy(copy, line);

    char **buf = (char **)malloc(64 * 64);
    int ret = tokenize(copy, " ", buf);
    int code = -1;

    for (int z = 0; z < total_commands; z++)
    {
        if (strcmp(commands[z].name, buf[0]) == 0)
        {
            code = commands[z].command(line, buf, ret);
        }
    }

    free(buf);

    return code;
}

int parse(char *text)
{
    char **line_breaker = (char **)malloc(64 * 64);
    int c = tokenize(text, "\n", line_breaker);

    for (int z = 0; z < c; z++)
    {
        int code = parse_line(line_breaker[z]);

        if (code == -1)
        {
            printf("Command not found: %s\n", line_breaker[z]);
            free(line_breaker);
            return 1;
        }

        if (code == 1)
        {
            printf("Error (line %d):\n\tIn call to '%s':\n\t", z + 1, line_breaker[z]);
            print_error();
            free(line_breaker);
            return 1;
        }
    }

    free(line_breaker);

    return 0;
}

void print_usage()
{
    printf("Usage: ./lang.o <file>\n");
}

void clean()
{
}

int main(int argc, char *argv[])
{
    init_variables();

    if (argc < 2)
    {
        print_usage();
        clean();
        return 1;
    }

    FILE *file = fopen(argv[1], "r");

    if (!file)
    {
        printf("Error: file '%s' not valid.\n", argv[1]);
        clean();
        return 1;
    }

    long size = fsize(file);

    if (size > MAX_SIZE)
    {
        printf("Error: file too big!\n");
        clean();
        return 1;
    }

    char content[size + 1];
    memset(content, 0, size + 1);
    fread(content, 1, size, file);

    int ret = parse(content);

    clean();
    fclose(file);

    return ret;
}
