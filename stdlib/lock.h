#pragma once

namespace std {

class Lock {
    bool is_locked = false;

public:
    void lock() {
        is_locked = true;
    }

    void unlock() {
        is_locked = false;
    }

    bool get() {
        return is_locked;
    }
};

}
