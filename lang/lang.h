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
#define TEXT_FW_BOLD "\033[1m"
#define TEXT_FW_UNDERLINED "\033[4m"
#define TEXT_FW_NONE "\033[0m"

#define MAKE_BOLD(text) TEXT_FW_BOLD text TEXT_FW_NONE
#define MAKE_UNDERLINED(text) TEXT_FW_UNDERLINED text TEXT_FW_NONE
#define _MAKE_UNDERLINED(text) text

#if (LANG == EN)
#include "lang_en.h"
#elif (LANG == FR)
#include "lang_fr.h"
#elif (LANG == PL)
#include "lang_pl.h"
#elif (LANG == IT)
#include "lang_it.h"
#endif

#undef IT
#undef PL
#undef FR
#undef EN
#undef LANG

#endif
