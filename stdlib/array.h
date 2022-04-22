namespace data {

class array : public std::vector<char *> {
public:
    void add(auto data) {
        push_back((char *)data);
    }
};

}
