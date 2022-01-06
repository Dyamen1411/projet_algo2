#ifndef __WS_CONTEXT_H__
#define __WS_CONTEXT_H__

#include "holdall.h"
#include "hashtable.h"
#include "linked_list.h"
#include "types.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define WS_CTX_DEFAULT_OPTION_VALUE__INITIAL 63
#define WS_CTX_DEFAULT_OPTION_VALUE__PUNCTUATION_LIKE_SPACE 0
#define WS_CTX_DEFAULT_OPTION_VALUE__SAME_NUMBER 0
#define WS_CTX_DEFAULT_OPTION_VALUE__TOP 10
#define WS_CTX_DEFAULT_OPTION_VALUE__UPPERCASING 0

typedef struct wsctx_parameters_t {
  size_t initial;               // -i
  bool punctuation_like_spaces; // -p
  bool same_number;             // -s
  size_t top;                   // -t
  bool uppercasing;             // -u
  const char *exec_name;
} wsctx_parameters_t;

typedef struct wsctx_t wsctx_t;

//  wsctx_parameters_default_initialization : initialize les parametres pointes
//    par parameters avec les valeurs par defaut
extern void wsctx_parameters_default_initialization(
  wsctx_parameters_t *parameters);

//  wsctx_initialize : cree puis initialise un contexte ws. Renvoie NULL en cas
//    de depassement memoire ou si file_count vaut 0, un pointeur sur le
//    contexte si non.
extern wsctx_t *wsctx_initialize(const wsctx_parameters_t *parameters,
    const char **file_names, size_t file_count);

//  wsctx_parse_files :
//  TODO: add test to tell wether user is allowed to use function
//    (already used ?) or not.
extern return_type wsctx_parse_files(wsctx_t *ctx);

//  wsctx_sort_data :
extern void wsctx_sort_data(wsctx_t *ctx);

extern void wsctx_output_data(wsctx_t *ctx);

//  wsctx_get_error_message : retourne le message d'erreur le plus recent.
extern const char *wsctx_get_error_message(wsctx_t *ctx);

//  wsctx_dispose : libere les ressources allouees a *ctx
extern void wsctx_dispose(wsctx_t **ctx);

#endif
