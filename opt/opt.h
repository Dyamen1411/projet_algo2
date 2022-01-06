#ifndef __OPT_H__
#define __OPT_H__

#include "context.h"
#include "linked_list.h"
#include "types.h"

#include <stdbool.h>

#define OPT__SHORT  "-"
#define OPT_HELP_S  "h"
#define OPT_INITIAL_S  "i"
#define IS_SHORT_OPT(arg) \
  (strncmp((arg), OPT__SHORT, sizeof(OPT__SHORT) - 1) == 0)

#define OPT__LONG   "--"
#define OPT_HELP_L  "help"
#define OPT_INITIAL_L  "initial"
#define IS_LONG_OPT(arg) \
  (strncmp((arg), OPT__LONG, sizeof(OPT__LONG) - 1) == 0)

typedef struct opt_t {
  const char *long_name;
  const char short_name;
  bool need_argument;
  return_type (*process) (wsctx_parameters_t *p, const char *arg);
  const char *description;
} opt_t;

return_type parse_opt(const int argc, char **argv, int *arg_pos,
    wsctx_parameters_t *parameters, linked_list_t *files,
    const opt_t *opts, const size_t opt_count);

#endif
