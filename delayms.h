/* delayms.h - Defines delayms(milliseconds) according to platform. */
/* If Windows, the Win32 Sleep() API call is used. */
/* Otherwise, usleep() is used. */

#ifndef delayms
#ifdef _WIN32
#include <windows.h>
#define delayms(ms) Sleep(ms)
#else
#include <unistd.h>
#define delayms(ms) usleep(1000*(ms))
#endif /*def _WIN32*/
#endif /*ndef delayms*/
