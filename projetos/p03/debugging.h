#ifndef __DEBUGGING__
#define __DEBUGGING__


#define OK "[ \e[0;32m \bOK \e[0;37m \b] "
#define WARNING "[ \e[0;33m \bWARNING \e[0;37m \b] "
#define FAILED "[ \e[0;31m \bFAILED \e[0;37m \b] "
#define DEBUG "[ \e[0;36m \bDEBUG \e[0;37m \b] "

#ifdef SLEEP
#define DEBUG_SLEEP(TIME) sleep(TIME);
#else
#define DEBUG_SLEEP(TIME) ;
#endif
//
// #ifdef DEBUG_ACTIVATE
//
// #include <unistd.h>
// #define DEBUG "[ \e[0;36m \bDEBUG \e[0;37m \b] "
//
// #else
// #define DEBUG ""
// #endif


#endif
