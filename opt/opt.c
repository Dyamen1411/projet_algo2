#include "opt.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define STDIN_NAME "-"

#define IS_SHORT_OPT(arg) \
  (strncmp((arg), OPT__SHORT, sizeof(OPT__SHORT) - 1) == 0)

#define IS_LONG_OPT(arg) \
  (strncmp((arg), OPT__LONG, sizeof(OPT__LONG) - 1) == 0)

#define IS_STDIN(arg) \
  (strcmp((arg), STDIN_NAME) == 0)

static return_type parse_long_opt(const int argc, char **argv,
    int *arg_pos, wsctx_parameters_t *parameters, const opt_t *opts,
    const size_t opt_count) {
  const char *opt_name = argv[*arg_pos] + 2;
  for (size_t i = 0; i < opt_count; ++i) {
    opt_t opt = opts[i];
    const size_t opt_name_length = strlen(opt.long_name);
    if (strncmp(opt_name, opt.long_name, opt_name_length) != 0) {
      continue;
    }
    const char *arg = NULL;
    if (opt.need_argument) {
      if (*(opt_name + opt_name_length) == '=') {
        arg = opt_name + opt_name_length + 1;
      } else {
        if (*(opt_name + opt_name_length) != '\0') {
          return RETURN_ERROR_OPT_UNKNOWN;
        }
        if (*arg_pos + 1 == argc) {
          return RETURN_ERROR_OPT_MISSING_ARG;
        }
        ++*arg_pos;
        arg = argv[*arg_pos];
      }
    }
    return opt.process(parameters, arg);
  }
  return RETURN_ERROR_OPT_UNKNOWN;
}

static return_type parse_short_opt(const int argc, char **argv,
    int *arg_pos, wsctx_parameters_t *parameters, const opt_t *opts,
    const size_t opt_count) {
  const char *opt_name = argv[*arg_pos] + 1;
  for (size_t i = 0; i < opt_count; ++i) {
    opt_t opt = opts[i];
    if (opt_name[0] != opt.short_name) {
      continue;
    }
    const char *arg = NULL;
    if (opt.need_argument) {
      if (*(opt_name + 1) == '=') {
        arg = opt_name + 2;
      } else {
        if (*(opt_name + 1) != '\0') {
          return RETURN_ERROR_OPT_UNKNOWN;
        }
        if (*arg_pos + 1 == argc) {
          return RETURN_ERROR_OPT_MISSING_ARG;
        }
        ++*arg_pos;
        arg = argv[*arg_pos];
      }
    }
    return opt.process(parameters, arg);
  }
  return RETURN_ERROR_OPT_UNKNOWN;
}

return_type parse_opt(const int argc, char **argv, int *arg_pos,
    wsctx_parameters_t *parameters, linked_list_t *files,
    const opt_t *opts, const size_t opt_count) {
  bool is_stdin = IS_STDIN(argv[*arg_pos]);
  const char *file_name = argv[*arg_pos + is_stdin];
  if (!is_stdin) {
    if (IS_LONG_OPT(file_name)) {
      return parse_long_opt(argc, argv, arg_pos, parameters, opts, opt_count);
    }
    if (IS_SHORT_OPT(file_name)) {
      return parse_short_opt(argc, argv, arg_pos, parameters, opts, opt_count);
    }
  }
  return linked_list_add_tail(files, file_name) == NULL
         ? RETURN_ERROR_CAPACITY
         : RETURN_NONE;
}
