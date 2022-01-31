#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int sec, min, h, d, m, y, wd;
    bool pm;
} time_t;

extern time_t get_time();

#ifdef __cplusplus
}
#endif
