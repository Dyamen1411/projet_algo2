#include "context.h"
#include "hashtable.h"
#include "holdall.h"
#include "lang.h"
#include "linked_list.h"
#include "opt.h"
#include "types.h"

#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR(x) #x
#define XSTR(x) STR(x)

static return_type opt__help(wsctx_parameters_t *p, const char *arg);
static return_type opt__initial(wsctx_parameters_t *p, const char *arg);
static return_type opt__punctuation_like_space(wsctx_parameters_t *p,
    const char *arg);
static return_type opt__same_numbers(wsctx_parameters_t *p, const char *arg);
static return_type opt__top(wsctx_parameters_t *p, const char *arg);
static return_type opt__uppercasing(wsctx_parameters_t *p, const char *arg);
static return_type opt__man(wsctx_parameters_t *p, const char *arg);
static return_type opt__usage(wsctx_parameters_t *p, const char *arg);
static return_type opt__version(wsctx_parameters_t *p, const char *arg);

static opt_t options[] = {
  {
    .long_name = "initial",
    .short_name = 'i',
    .need_argument = true,
    .process = opt__initial,
    .description
      = "Set the maximal number of significant initial letters for words to "
        "VALUE. 0 means without limitation. Default is "
        XSTR(WS_CTX_DEFAULT_OPTION_VALUE__INITIAL) "."
  },
  {
    .long_name = "punctuation-like-space",
    .short_name = 'p',
    .need_argument = false,
    .process = opt__punctuation_like_space,
    .description
      = "Make the punctuation characters play the same role as space "
        "characters in the meaning of words."
  },
  {
    .long_name = "same-numbers",
    .short_name = 's',
    .need_argument = false,
    .process = opt__same_numbers,
    .description = "Print more words than the limit in case of same numbers."
  },
  {
    .long_name = "top",
    .short_name = 't',
    .need_argument = true,
    .process = opt__top,
    .description
      = "Set the maximal number of words to print to VALUE. 0 "
        "means all the words. Default is "
        XSTR(WS_CTX_DEFAULT_OPTION_VALUE__TOP) "."
  },
  {
    .long_name = "uppercasing",
    .short_name = 'u',
    .need_argument = false,
    .process = opt__uppercasing,
    .description
      = "Convert each lowercase letter of words to the "
        "corresponding uppercase letter."
  },
  {
    .long_name = "help",
    .short_name = '?',
    .need_argument = false,
    .process = opt__help,
    .description = "Print this help message and exit."
  },
  {
    .long_name = "man",
    .short_name = '\0',
    .need_argument = false,
    .process = opt__man,
    .description = "Print this help message following man-style and exit."
  },
  {
    .long_name = "usage",
    .short_name = '\0',
    .need_argument = false,
    .process = opt__usage,
    .description = "Print a short usage message and exit."
  },
  {
    .long_name = "version",
    .short_name = '\0',
    .need_argument = false,
    .process = opt__version,
    .description = "Print version information."
  },
};

static return_type parse_arguments(int argc, char **argv,
    wsctx_parameters_t *parameters, linked_list_t *files, int *last_index);

int main(int argc, char **argv) {
  // initializations
  int r = EXIT_SUCCESS;
  wsctx_parameters_t parameters;
  wsctx_t *ctx = NULL;
  linked_list_t *files = linked_list_empty();
  if (files == NULL) {
    goto error_capacity;
  }
  // context parameters initialization
  {
    parameters.exec_name = argv[0] + (strncmp(argv[0], "./", 2) == 0 ? 2 : 0);
    int last_index;
    wsctx_parameters_default_initialization(&parameters);
    switch (parse_arguments(argc, argv, &parameters, files, &last_index)) {
      case RETURN_NONE:
        break;
      case RETURN_ERROR_OPT_UNKNOWN:
        fprintf(stderr, "%s: " LANG_MESSAGE_ERROR__UNKNOWN_OPTION " '%s'.\n",
            argv[0], argv[last_index]);
        goto error;
      case RETURN_ERROR_OPT_MISSING_ARG:
        fprintf(stderr, "%s: " LANG_MESSAGE_ERROR__MISSING_ARGUMENT " '%s'.\n",
            argv[0], argv[last_index]);
        goto error;
      case RETURN_ERROR_CAPACITY:
        goto error_capacity;
      case RETURN_ERROR_OPT_ARGUMENT: // TODO: ??
      case RETURN_EXIT:
        goto dispose;
      default:
        goto error_unexpected;
    }
#ifdef DEBUG
    printf("%30s: %zu\n", "initial", parameters.initial);
    printf("%30s: %s\n", "punctuation like space",
        parameters.punctuation_like_spaces ? "yes" : "no");
    printf("%30s: %s\n", "same numbers", parameters.same_number ? "yes"
        : "no");
    printf("%30s: %zu\n", "top", parameters.top);
    printf("%30s: %s\n", "uppercasing", parameters.uppercasing ? "yes"
        : "no");
#endif
  }
  // check if there is at least 2 files
  const size_t file_count = linked_list_size(files);
  if (file_count < 2) {
    goto error_not_enough_files;
  }
  // context initialization
  {
    char *file_names[file_count];
    for (size_t i = 0; i < file_count; ++i) {
      file_names[i] = (char *) linked_list_remove_head(files);
    }
    linked_list_dispose(&files);
    ctx = wsctx_initialize(&parameters, (const char **) file_names, file_count);
    if (ctx == NULL) {
      goto error_capacity;
    }
  }
  // parsing files
  {
    switch (wsctx_parse_files(ctx)) {
      case RETURN_NONE:
        break;
      case RETURN_ERROR_UNACCESSIBLE_FILE:
        goto error_unaccessible_file;
      case RETURN_ERROR_IO:
        goto error_io;
      case RETURN_ERROR_CAPACITY:
        goto error_capacity;
      default:
        goto error_unexpected;
    }
  }
  {
    wsctx_sort_data(ctx);
    wsctx_output_data(ctx);
  }
  goto dispose;
error_not_enough_files:
  fprintf(stderr, "%s: " LANG_MESSAGE_ERROR__FILE_COUNT ".\n", argv[0]);
  goto error;
error_unaccessible_file:
  fprintf(stderr, "%s: " LANG_MESSAGE_ERROR__UNACCESSIBLE_FILE " '%s'.\n",
      argv[0], wsctx_get_error_message(ctx));
  goto error;
error_io:
  fprintf(stderr, "%s: " LANG_MESSAGE_ERROR__IO " '%s'.\n", argv[0],
      wsctx_get_error_message(ctx));
  goto error;
error_capacity:
  fprintf(stderr, "%s: " LANG_MESSAGE_ERROR__CAPACITY "\n", argv[0]);
  goto error;
error_unexpected:
  fprintf(stderr, "%s: " LANG_MESSAGE_ERROR__UNEXPECTED "\n", argv[0]);
  goto error;
error:
  r = EXIT_FAILURE;
  goto dispose;
dispose:
  linked_list_dispose(&files);
  wsctx_dispose(&ctx);
  return r;
}

static void print_usage(const char *exec_name) {
  printf(LANG_OPT__USAGE__USAGE ": %s [" LANG_OPT__USAGE__OPTION "]... "
      LANG_OPT__USAGE__FILES "\n", exec_name);
}

static void print_short_description(const char *exec_name) {
  printf("%s - Print a list of words shared by text files\n", exec_name);
}

static void print_limits() {
  printf(
      "The number of FILES that can be passed must be at least 2. "
      "The counters of the total number of occurences of the words have "
      "a maximum value of %zu.\n", sizeof(size_t));
}

static void print_version(const char *exec_name) {
  printf("%s - 2022/01/06\n", exec_name);
}

static void print_copyright() {
  printf("This is freeware: you can redistribute it. There is NO WARRANTY.");
}

static bool is_digit(char c) {
  return '0' <= c && c <= '9';
}

return_type parse_arguments(int argc, char **argv,
    wsctx_parameters_t *parameters, linked_list_t *files, int *last_index) {
  for (*last_index = 1; *last_index < argc; ++*last_index) {
    int res = parse_opt(argc, argv, last_index, parameters, files, options,
        sizeof(options) / sizeof(opt_t));
    if (res != RETURN_NONE) {
      return res;
    }
  }
  return RETURN_NONE;
}

//TODO: pass whole argv to opt function
return_type opt__initial(wsctx_parameters_t *p, const char *arg) {
  if (*arg == '\0') {
    return RETURN_ERROR_OPT_ARGUMENT;
  }
  size_t n = 0;
  if (*arg == '-') {
    return RETURN_ERROR_OPT_ARGUMENT;
  }
  while (is_digit(*arg) && *arg != '\0') {
    n *= 10;
    n += (size_t) (*arg - '0');
    ++arg;
  }
  if (*arg != '\0') {
    return RETURN_ERROR_OPT_ARGUMENT;
  }
  p->initial = n;
  return RETURN_NONE;
}

return_type opt__punctuation_like_space(wsctx_parameters_t *p,
    __attribute__((unused)) const char *arg) {
  p->punctuation_like_spaces = true;
  return RETURN_NONE;
}

return_type opt__same_numbers(wsctx_parameters_t *p,
    __attribute__((unused)) const char *arg) {
  p->same_number = true;
  return RETURN_NONE;
}

return_type opt__top(wsctx_parameters_t *p, const char *arg) {
  if (*arg == '\0') {
    return RETURN_ERROR_OPT_ARGUMENT;
  }
  size_t n = 0;
  if (*arg == '-') {
    return RETURN_ERROR_OPT_ARGUMENT;
  }
  while (is_digit(*arg) && *arg != '\0') {
    n *= 10;
    n += (size_t) (*arg - '0');
    ++arg;
  }
  if (*arg != '\0') {
    return RETURN_ERROR_OPT_ARGUMENT;
  }
  p->top = n;
  return RETURN_NONE;
}

return_type opt__uppercasing(wsctx_parameters_t *p,
    __attribute__((unused)) const char *arg) {
  p->uppercasing = true;
  return RETURN_NONE;
}

return_type opt__help(
    __attribute__((unused)) wsctx_parameters_t *p,
    __attribute__((unused)) const char *arg) {
  printf("Usage : ");
  print_usage("ws");
  printf("\n");
  print_short_description("ws");
  printf("\n");
  printf(
      "Mandatory arguments to long options are mandatory for short options "
      "too.\n");
  printf("\n");
  char short_opt[4];
  for (size_t i = 0; i < sizeof(options) / sizeof(opt_t); ++i) {
    opt_t opt = options[i];
    if (opt.short_name == '\0') {
      sprintf(short_opt, "   ");
    } else {
      sprintf(short_opt, "-%c,", opt.short_name);
    }
    printf("  %s --%s%s\t%s\n\n",
        short_opt,
        opt.long_name,
        opt.need_argument ? "=VALUE" : "", opt.description);
  }
  print_limits();
  return RETURN_EXIT;
}

return_type opt__man(
    __attribute__((unused)) wsctx_parameters_t *p,
    __attribute__((unused)) const char *arg) {
  printf("\033[1mNAME\033[0m\n\t");
  print_short_description("ws");
  printf("\n");
  // synopsys
  printf("\033[1mSYNOPSYS\033[0m\n\t");
  print_usage("ws");
  printf("\n");
  // description
  printf("\033[1mDESCRIPTION\033[0m\n\n\t");
  // print_long_description("ws");
  printf("\n");
  // options
  printf("\033[1mOPTIONS\033[0m\n\n\t");
  printf("\n");
  // limits
  printf("\033[1mLIMITS\033[0m\n\n\t");
  printf("\n");
  // author
  printf("\033[1mAUTHOR\033[0m\n\n\t");
  printf("\n");
  // copyright
  printf("\033[1mCOPYRIGHT\033[0m\n\n\t");
  printf("\n");
  return RETURN_EXIT;
}

return_type opt__usage(
    __attribute__((unused)) wsctx_parameters_t *p,
    __attribute__((unused))const char *arg) {
  print_usage(p->exec_name);
  return RETURN_EXIT;
}

return_type opt__version(
    __attribute__((unused)) wsctx_parameters_t *p,
    __attribute__((unused)) const char *arg) {
  print_version("ws");
  print_copyright();
  return RETURN_EXIT;
}
