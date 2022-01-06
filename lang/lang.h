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