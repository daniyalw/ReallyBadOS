class string
{
    char * words;
    int length;
public:
    string(char * str);
    int size();
    char * get();
    void set(char * str);
    void append(char * other);
    int startswith(char * start);
};

void itoa(int n, char str[]);
