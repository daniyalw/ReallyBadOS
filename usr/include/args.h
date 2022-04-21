#pragma once

#include "_header.h"

#ifdef __cplusplus
class ArgParse {
    char _argv_[64][64];
    int _argc_ = 0;

    bool there_is_more(char *command, int z)
    {
        for (int b = z; b < strlen(command); b++)
            if (command[b] != ' ')
                return true;

        return false;
    }

public:
    void parse_args(char *command)
    {
        for (int z = 0; z < 64; z++)
            for (int b = 0; b < 64; b++)
                argv[z][b] = 0;

        int _argc = 1;
        const int length = strlen(command);
        char _argv[64][64];
        int charcount = 0;
        bool found_char = false;

        // set argv to 0
        for (int z = 0; z < 64; z++)
            for (int b = 0; b < 64; b++)
                _argv[z][b] = 0;

        for (int z = 0; z < length; z++)
        {
            if (command[z] == ' ')
            {
                if (!found_char) continue;

                if (there_is_more(command, z))
                    _argc++;

                charcount = 0;
            }
            else
            {
                found_char = true;
                _argv[_argc-1][charcount] = command[z];
                charcount++;
            }
        }

        for (int z = 0; z < argc; z++)
            for (int b = 0; b < 64; b++)
                argv[z][b] = _argv[z][b];

        argc = _argc;
    }

    char **argv() {
        return _argv_;
    }

    int argc() {
        return _argc_;
    }
};
#endif
