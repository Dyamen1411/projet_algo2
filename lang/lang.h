#ifndef __LANG__H__
#define __LANG__H__

#define EN 0
#define FR 1
#define PL 2
#define IT 3

#if !defined(LANG) || (1111 - LANG - 1111 == 2222)
#undef LANG
#define LANG EN
#endif

#define STR(x) #x
#define XSTR(x) STR(x)

#define EXEC_NAME_FORMAT "%s"
#define _LONG_MAX 9223372036854775807

#if (LANG == EN)
#include "lang_en.h"
#elif (LANG == FR)
#include "lang_fr.h"
#elif (LANG == PL)
#include "lang_pl.h"
#elif (LANG == IT)
#include "lang_it.h"
#endif

#undef XSTR
#undef STR

#undef IT
#undef PL
#undef FR
#undef EN
#undef LANG

#endif
