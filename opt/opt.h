#ifndef __OPT_H__
#define __OPT_H__

#include "context.h"
#include "linked_list.h"
#include "types.h"

#include <stdbool.h>

#define OPT__SHORT  "-"
#define OPT__LONG   "--"

#define OPT_LONG_NO_NAME ""
#define OPT_SHORT_NO_NAME '\0'

#define OPT_HAS_SHORT_NAME(_opt) ((_opt).short_name != OPT_SHORT_NO_NAME)
#define OPT_HAS_LONG_NAME(_opt) (*(_opt).long_name != *OPT_LONG_NO_NAME)

typedef struct opt_t {
  const char *long_name;
  const char short_name;
  bool need_argument;
  return_type (*process) (wsctx_parameters_t *p, const char *arg);
  const char *description;
  unsigned int category;
} opt_t;

return_type parse_opt(const int argc, char **argv, int *arg_pos,
    wsctx_parameters_t *parameters, linked_list_t *files,
    const opt_t *opts, const size_t opt_count);

#endif
