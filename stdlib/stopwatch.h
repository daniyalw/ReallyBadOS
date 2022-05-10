#include <errno.h>
#include <sys/time/timer.h>

namespace Utils {
    class Stopwatch {
        int _now = 0;
        int _length = 0;
        int _lap = 0;
        int allocated = 0;
        int _paused = 0;
        int _last = 0;
        int *_laps = NULL;

        int allocate_lap_space() {
            allocated += 10;
            uint32_t loc = realloc(_laps, allocated);

            if (!loc) {
                errno = ENOMEM;
                return 1;
            }

            _laps = (int *)loc;
            return 0;
        }

    public:
        int start() {
            _now = get_uptime();
            _last = _now;

            printf("UPTIME: %d\n", _now);

            if (_laps) {
                free(_laps);
            }

            allocated = 10;
            uint32_t loc = malloc(allocated);

            if (!loc) {
                errno = ENOMEM;
                return 1;
            }

            _laps = (int *)loc;

            return 0;
        }

        void calculate_length() {
            auto uptime = get_uptime();

            printf("UPTIME: %d\n", uptime);

            if (!_paused) {
                _length += uptime - _last;
                _last = uptime;
            } else {
                _last = uptime;
            }
        }

        int now() {
            calculate_length();
            return _length;
        }

        void pause() {
            calculate_length();
            _paused = 1;
        }

        void unpause() {
            calculate_length();
            _paused = 0;
        }

        void add_lap() {
            int _now = now();
            _laps[_lap] = _now;
            _lap++;

            if (_lap >= allocated) {
                allocate_lap_space();
            }

            _length = 0;
            _now = get_uptime();
            _last = _now;
        }

        void reset() {
            _now = 0;
            _length = 0;
            _lap = 0;
            allocated = 0;
            _paused = 0;
            _last = 0;

            if (_laps) {
                free(_laps);
                _laps = NULL;
            }
        }

        int lap(int z) {
            if (z < 0 || z >= _lap) {
                return -1;
            }

            return _laps[z];
        }
    };
}
