#ifndef DEFINES_H
#define DEFINES_H

#ifndef DEBUG_MSG
#define DEBUG_MSG//     printf
#endif

#ifndef DEBUG_FUNC_NAME
#ifdef _MSC_VER
#define DEBUG_FUNC_NAME	DEBUG_MSG("%s: %s (%d)\n", __FILE__, __FUNCTION__, __LINE__);
#else
#define DEBUG_FUNC_NAME	DEBUG_MSG("%s: %s (%d)\n", __FILE__, __func__, __LINE__);
#endif
#endif

#ifndef ARR_SIZE
#define ARR_SIZE(x) sizeof(x)/sizeof(x[0])
#endif

#endif
