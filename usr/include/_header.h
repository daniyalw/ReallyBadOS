#pragma once

#ifdef __cplusplus
    #define C_HEADER extern "C" {
    #define C_HEADER_END }
#else
    #define C_HEADER
    #define C_HEADER_END
#endif
