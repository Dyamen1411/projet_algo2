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
#include <sys/ioctl.h>

#define LOCAL_DIRECTORY_PREFIX "./"
#define DEFULT_MAX_TEXT_WIDTH 80

#define OPT_HELP_CHAR_COUNT_TO_LONG_NAME 8
#define OPT_HELP_DEFAULT_PADDING_LENGTH 4
#define OPT_HELP_DEFAULT_INDENT 20
#define OPT_MAN_DEFAULT_INDENT 8

//  Long option names
#define OPT_NAME_LONG__INITIAL "initial"
#define OPT_NAME_LONG__PUNCTUATION_LIKE_SPACE "punctuation-like-space"
#define OPT_NAME_LONG__SAME_NUMBER "same-numbers"
#define OPT_NAME_LONG__TOP "top"
#define OPT_NAME_LONG__UPPERCASING "uppercasing"
#define OPT_NAME_LONG__HELP "help"
#define OPT_NAME_LONG__MAN "man"
#define OPT_NAME_LONG__USAGE "usage"
#define OPT_NAME_LONG__VERSION "version"

//  Short option names
#define OPT_NAME_SHORT__INITIAL 'i'
#define OPT_NAME_SHORT__PUNCTUATION_LIKE_SPACE 'p'
#define OPT_NAME_SHORT__SAME_NUMBER 's'
#define OPT_NAME_SHORT__TOP 't'
#define OPT_NAME_SHORT__UPPERCASING 'u'
#define OPT_NAME_SHORT__HELP '?'
#define OPT_NAME_SHORT__MAN OPT_SHORT_NO_NAME
#define OPT_NAME_SHORT__USAGE OPT_SHORT_NO_NAME
#define OPT_NAME_SHORT__VERSION OPT_SHORT_NO_NAME

#define PRINT_CATEGORY_HELP(_cc, _category) \
  printf(LANG_OPT_CATEGORY_NAME__ ## _category "\n"); \
  for (size_t i = 0; i < sizeof(options) / sizeof(opt_t); ++i) { \
    if (options[i].category != OPT_CATEGORY_ ## _category) { \
      continue; \
    } \
    print_opt__help(&options[i], (_cc)); \
    putchar('\n'); \
  } \
  putchar('\n')

#define PRINT_CATEGORY_MAN(_category, _column_count) \
  printf(MAKE_CATEGORY(_category)); \
  _print_section_man(OPT_CATEGORY_ ## _category, (_column_count))

#define MAKE_SECTION(_title) MAKE_BOLD(LANG_MANN_SECTION__ ## _title) "\n"
#define MAKE_CATEGORY(_title) "    " \
  MAKE_BOLD(LANG_OPT_CATEGORY_NAME__ ## _title) "\n"

#define STR_TO_LOWER(_dest, _src) \
  { \
    strcpy((_dest), (_src)); \
    for (size_t i = 0; (_dest)[i] != '\0'; ++i) { \
      (_dest)[i] = (char) tolower((_src)[i]); \
    } \
  }

#define OPTION(_name, _need_argument, _process_function, _category) \
  { \
    .long_name = OPT_NAME_LONG__ ## _name, \
    .short_name = OPT_NAME_SHORT__ ## _name, \
    .need_argument = (_need_argument), \
    .process = (_process_function), \
    .description = LANG_OPT_DESCRIPTION__ ## _name, \
    .category = OPT_CATEGORY_ ## _category \
  }

typedef enum opt_category {
  OPT_CATEGORY_INFORMATION,
  OPT_CATEGORY_INPUT_CONTROL,
  OPT_CATEGORY_OUTPUT_CONTROL
} opt_category;

typedef enum opt_index {
  OPT_INDEX_INITIAL,
  OPT_INDEX_PUNCTUATION_LIKE_SPACE,
  OPT_INDEX_SAME_NUMBER,
  OPT_INDEX_TOP,
  OPT_INDEX_UPPERCASING,
  OPT_INDEX_HELP,
  OPT_INDEX_MAN,
  OPT_INDEX_USAGE,
  OPT_INDEX_VERSION
} opt_index;

//  print_opt__help : affiche la les noms court et long puis la description de
//    l'option pointee par opt suivi de une indentation de
//    OPT_HELP_DEFAULT_INDENT caracteres.
static void print_opt__help(opt_t *opt, size_t column_count);

//  print_opt__man : affiche la description de l'option pointee par opt avec
//    une indentation de OPT_MAN_DEFAULT_INDENT caracteres dans un format de
//    page de mannuel.
static void print_opt__man(opt_t *opt, size_t column_count);

//  _print_description : affiche le texte pointe par description avec une
//    indentation de indent caracteres. Si match n'est pas nul, souligne toutes
//    les occurences de match dans le text pointe par description.
static void _print_description(size_t column_count, size_t indent,
    size_t offset, const char *description, const char *match);

//  print_description : affiche le texte pointe par description avec une
//    indentation de indent caracteres.
static void print_description(size_t column_count, size_t indent, size_t offset,
    const char *description);

//  _print_section_man : affiches toutes les options (options) qui font partie
//    de la categorie category au format d'une page de mannuel.
static void _print_section_man(opt_category category, size_t column_count);

//  opt__help : affiche de l'aide sut la sortie standard puis renvoie
//    RETURN_EXIT
static return_type opt__help(wsctx_parameters_t *p, const char *arg);

//  opt__initial : si le parametre arg est bien une chaine de caractere qui
//    represente un nombre entier positif, met le champ p->initial a cette
//    valeur puis renvoie RETURN_NONE. Sinon, renvoie RETURN_ERROR_OPT_ARGUMENT.
static return_type opt__initial(wsctx_parameters_t *p, const char *arg);

//  opt__punctuation_like_space : met le champ p->opt__punctuation_like_space
//    a true. puis renvoie RETURN_NONE.
static return_type opt__punctuation_like_space(wsctx_parameters_t *p,
    const char *arg);

//  opt__same_numbers : met le champ p->same_numbers a true puis renvoie
//    RETURN_NONE
static return_type opt__same_numbers(wsctx_parameters_t *p, const char *arg);

//  opt__top : si le parametre arg est bien une chaine de caractere qui
//    represente un nombre entier positif, met le champ p->top a cette valeur
//    puis renvoie RETURN_NONE. Sinon, renvoie RETURN_ERROR_OPT_ARGUMENT.
static return_type opt__top(wsctx_parameters_t *p, const char *arg);

//  opt__uppercasing : met le champ p->uppercasing a true puis renvoie
//    RETURN_NONE.
static return_type opt__uppercasing(wsctx_parameters_t *p, const char *arg);

//  opt__man : affiche la page de manuel du programme puis renvoie RETURN_EXIT.
static return_type opt__man(wsctx_parameters_t *p, const char *arg);

//  opt__usage : affiche l'utilisation du programme puis renvoie RETURN_EXIT.
static return_type opt__usage(wsctx_parameters_t *p, const char *arg);

//  opt_version : affiche la version du programme puis renvoie RETURN_EXIT.
static return_type opt__version(wsctx_parameters_t *p, const char *arg);

//  parse_arguments : traite tout les arguments pointes par argv.
//    Les fichiers sont ajoutes a la liste chainee pointe par files et les
//    options rencontrees sont traitees par leur fonction respective
//    (opt__ ## name). Renvoie RETURN_NONE si tout s'est bien passe,
//    RETURN_ERROR_CAPACITY si un depassement de memoire est survenu,
//    RETURN_ERROR_OPT_ARGUMENT si le parametre d'une option est incorrecte,
//    RETURN_ERROR_OPT_ARGUMENT si un argument n'est pas reconnu,
//    RETURN_ERROR_OPT_MISSING_ARG si le parametre d'une option est manquant.
static return_type parse_arguments(int argc, char **argv,
    wsctx_parameters_t *parameters, linked_list_t *files, int *last_index);

static opt_t options[] = {
  OPTION(INITIAL, true, opt__initial, INPUT_CONTROL),
  OPTION(PUNCTUATION_LIKE_SPACE, false, opt__punctuation_like_space,
      INPUT_CONTROL),
  OPTION(SAME_NUMBER, false, opt__same_numbers, OUTPUT_CONTROL),
  OPTION(TOP, true, opt__top, OUTPUT_CONTROL),
  OPTION(UPPERCASING, false, opt__uppercasing, INPUT_CONTROL),
  OPTION(HELP, false, opt__help, INFORMATION),
  OPTION(MAN, false, opt__man, INFORMATION),
  OPTION(USAGE, false, opt__usage, INFORMATION),
  OPTION(VERSION, false, opt__version, INFORMATION)
};

int main(int argc, char **argv) {
  const char *exec_name = argv[0]
      + (strncmp(argv[0], LOCAL_DIRECTORY_PREFIX,
      sizeof(LOCAL_DIRECTORY_PREFIX) - 1) == 0
      ? sizeof(LOCAL_DIRECTORY_PREFIX) - 1
      : 0);
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
    parameters.exec_name = exec_name;
    int last_index;
    wsctx_parameters_default_initialization(&parameters);
    switch (parse_arguments(argc, argv, &parameters, files, &last_index)) {
      case RETURN_NONE:
        break;
      case RETURN_ERROR_OPT_UNKNOWN:
        fprintf(stderr, "%s: " LANG_MESSAGE_ERROR__UNKNOWN_OPTION " '%s'.\n",
            exec_name, argv[last_index]);
        goto error_user_invalid_input;
      case RETURN_ERROR_OPT_MISSING_ARG:
        fprintf(stderr, "%s: " LANG_MESSAGE_ERROR__MISSING_ARGUMENT " '%s'.\n",
            exec_name, argv[last_index]);
        goto error_user_invalid_input;
      case RETURN_ERROR_OPT_ARGUMENT:
        fprintf(stderr, "%s: " LANG_MESSAGE_ERROR__INVALID_PARAMETER " '%s'.\n",
            exec_name, argv[last_index]);
        goto error_user_invalid_input;
      case RETURN_ERROR_CAPACITY:
        goto error_capacity;
      case RETURN_EXIT:
        goto dispose;
      default:
        goto error_unexpected;
    }
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
error_user_invalid_input:
  fprintf(stderr, LANG_MESSAGE_MORE_INFO "\n", exec_name);
  goto error;
error_not_enough_files:
  fprintf(stderr, "%s: " LANG_MESSAGE_ERROR__FILE_COUNT ".\n", exec_name);
  goto error;
error_unaccessible_file:
  fprintf(stderr, "%s: " LANG_MESSAGE_ERROR__UNACCESSIBLE_FILE " '%s'.\n",
      exec_name, wsctx_get_error_message(ctx));
  goto error;
error_io:
  fprintf(stderr, "%s: " LANG_MESSAGE_ERROR__IO " '%s'.\n", exec_name,
      wsctx_get_error_message(ctx));
  goto error;
error_capacity:
  fprintf(stderr, "%s: " LANG_MESSAGE_ERROR__CAPACITY "\n", exec_name);
  goto error;
error_unexpected:
  fprintf(stderr, "%s: " LANG_MESSAGE_ERROR__UNEXPECTED "\n", exec_name);
  goto error;
error:
  r = EXIT_FAILURE;
  goto dispose;
dispose:
  linked_list_dispose(&files);
  wsctx_dispose(&ctx);
  return r;
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

return_type opt__initial(wsctx_parameters_t *p, const char *arg) {
  size_t n = 0;
  while (isdigit(*arg) && *arg != '\0') {
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
  while (isdigit(*arg) && *arg != '\0') {
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
  printf(LANG_USAGE_MESSAGE "\n", p->exec_name);
  putchar('\n');
  printf(LANG_WS__SHORT_DESCRIPTION "\n");
  putchar('\n');
  printf(LANG_WS__HOW_TO_USE_OPTIONS "\n");
  putchar('\n');
  struct winsize ws;
  if (ioctl(0, TIOCGWINSZ, &ws) == -1) {
    ws.ws_col = DEFULT_MAX_TEXT_WIDTH;
  }
  PRINT_CATEGORY_HELP(ws.ws_col, INFORMATION);
  PRINT_CATEGORY_HELP(ws.ws_col, INPUT_CONTROL);
  PRINT_CATEGORY_HELP(ws.ws_col, OUTPUT_CONTROL);
  print_description(ws.ws_col, 0, 0, LANG_WS__LIMITS);
  putchar('\n');
  return RETURN_EXIT;
}

return_type opt__man(wsctx_parameters_t *p,
    __attribute__((unused)) const char *arg) {
  struct winsize ws;
  if (ioctl(0, TIOCGWINSZ, &ws) == -1) {
    ws.ws_col = DEFULT_MAX_TEXT_WIDTH;
  }
  const size_t exec_name_length = strlen(p->exec_name);
  const size_t offset = (size_t) OPT_MAN_DEFAULT_INDENT + exec_name_length;
  printf(MAKE_SECTION(NAME));
  printf("%*c%s", OPT_MAN_DEFAULT_INDENT, ' ', p->exec_name);
  print_description(ws.ws_col, OPT_MAN_DEFAULT_INDENT, offset,
      " - " LANG_WS__SHORT_DESCRIPTION);
  putchar('\n');
  //  TODO@Dyamen1411: fix synopsis. 'options' & 'files' are in caps but they
  //    should not be
  printf(MAKE_SECTION(SYNOPSIS));
  printf("%*c" MAKE_BOLD(EXEC_NAME_FORMAT), OPT_MAN_DEFAULT_INDENT, ' ',
      p->exec_name);
  print_description(ws.ws_col, OPT_MAN_DEFAULT_INDENT, offset,
      " " LANG_WS__SYNOPSIS);
  putchar('\n');
  printf(MAKE_SECTION(DESCRIPTION));
  printf("\n%*c" MAKE_BOLD(EXEC_NAME_FORMAT), OPT_MAN_DEFAULT_INDENT, ' ',
      p->exec_name);
  _print_description(ws.ws_col, OPT_MAN_DEFAULT_INDENT, offset,
      " " LANG_WS__LONG_DESCRIPTION, LANG_OPT__PARAMETER_FILES);
  putchar('\n');
  printf(MAKE_SECTION(OPTIONS));
  PRINT_CATEGORY_MAN(INFORMATION, ws.ws_col);
  PRINT_CATEGORY_MAN(INPUT_CONTROL, ws.ws_col);
  PRINT_CATEGORY_MAN(OUTPUT_CONTROL, ws.ws_col);
  printf(MAKE_SECTION(LIMITS));
  printf("%*c", OPT_MAN_DEFAULT_INDENT, ' ');
  _print_description(ws.ws_col, OPT_MAN_DEFAULT_INDENT, OPT_MAN_DEFAULT_INDENT,
      LANG_WS__LIMITS, LANG_OPT__PARAMETER_FILES);
  putchar('\n');
  printf(MAKE_SECTION(AUTHORS));
  printf("%*c", OPT_MAN_DEFAULT_INDENT, ' ');
  print_description(ws.ws_col, OPT_MAN_DEFAULT_INDENT, OPT_MAN_DEFAULT_INDENT,
      LANG_WS__AUTHORS);
  putchar('\n');
  printf(MAKE_SECTION(COPYRIGHT));
  printf("%*c%s", OPT_MAN_DEFAULT_INDENT, ' ', p->exec_name);
  print_description(ws.ws_col, OPT_MAN_DEFAULT_INDENT, offset,
      " - " WS_VERSION "\n" LANG_WS__COPYRIHT);
  return RETURN_EXIT;
}

return_type opt__usage(
    __attribute__((unused)) wsctx_parameters_t *p,
    __attribute__((unused))const char *arg) {
  printf(LANG_USAGE_MESSAGE "\n", p->exec_name);
  return RETURN_EXIT;
}

return_type opt__version(
    __attribute__((unused)) wsctx_parameters_t *p,
    __attribute__((unused)) const char *arg) {
  printf("%s - " WS_VERSION "\n" LANG_WS__COPYRIHT "\n", p->exec_name);
  return RETURN_EXIT;
}

void _print_description(size_t column_count, size_t indent, size_t offset,
    const char *description, const char *match) {
  char buffer[column_count + 1];
  memset(buffer, 0, column_count + 1);
  const bool search = match != NULL;
  const size_t match_length = search
      ? strlen(match)
      : 0;
  char param_lower[match_length + 1];
  if (match) {
    STR_TO_LOWER(param_lower, match);
  }
  unsigned int cursor = (unsigned int) offset;
  while (1) {
    while (isspace(*description)) {
      if (cursor >= column_count || *description == '\n') {
        cursor = (unsigned int) indent + 1;
        printf("\n%*c", (unsigned int) indent, ' ');
        ++description;
        break;
      }
      putchar(*description);
      ++cursor;
      ++description;
    }
    while (isspace(*description)) {
      if (*description == '\n') {
        printf("\n%*c", (unsigned int) indent, ' ');
      }
      ++description;
    }
    if (*description == '\0') {
      break;
    }
    size_t i = 0;
    const char *base = description;
    bool underline = search && strncmp(base, match, match_length) == 0;
    while (*description != '\0' && !isspace(*description) && i < column_count) {
      if (cursor >= column_count) {
        cursor = (unsigned int) (indent + i);
        printf("\n%*c", (unsigned int) indent, ' ');
      }
      ++description;
      ++i;
      ++cursor;
    }
    if (i != 0) {
      if (underline) {
        printf(MAKE_UNDERLINED("%s"), param_lower);
        base += match_length;
        i -= match_length;
      }
      memcpy(buffer, base, i);
      printf("%s", buffer);
      memset(buffer, 0, column_count + 1);
    }
  }
  putchar('\n');
}

void print_description(size_t column_count, size_t indent, size_t offset,
    const char *description) {
  _print_description(column_count, indent, offset, description, NULL);
}

void print_opt__help(opt_t *opt, size_t column_count) {
  char short_opt[4];
  const unsigned int prefix_length = (unsigned int) (strlen(opt->long_name)
      + (opt->need_argument ? sizeof(LANG_OPT__PARAMETER_VALUE) : 0));
  const bool overflow = prefix_length
      >= (OPT_HELP_DEFAULT_INDENT - OPT_HELP_CHAR_COUNT_TO_LONG_NAME - 1);
  const size_t offset = OPT_HELP_DEFAULT_INDENT + 1
      + (overflow ? prefix_length - OPT_HELP_CHAR_COUNT_TO_LONG_NAME : 0);
  const size_t padding_length = overflow
      ? OPT_HELP_DEFAULT_PADDING_LENGTH
      : OPT_HELP_DEFAULT_INDENT - OPT_HELP_CHAR_COUNT_TO_LONG_NAME
      - prefix_length;
  const char *long_str = opt->need_argument
      ? "=" LANG_OPT__PARAMETER_VALUE
      : "";
  const char *short_format = opt->short_name == '\0' ? "   " : "-%c,";
  sprintf(short_opt, short_format, opt->short_name);
  printf("  %s --%s%s", short_opt, opt->long_name, long_str);
  printf("%*c", (unsigned int) padding_length, ' ');
  print_description(column_count, OPT_HELP_DEFAULT_INDENT, offset,
      opt->description);
}

void print_opt__man(opt_t *opt, size_t column_count) {
  char lower_case_parameter_value[sizeof(LANG_OPT__PARAMETER_VALUE) + 1];
  STR_TO_LOWER(lower_case_parameter_value, LANG_OPT__PARAMETER_VALUE);
  const bool has_short_name = opt->short_name != '\0';
  const bool has_long_name = *opt->long_name != '\0';
  printf("%*c", OPT_MAN_DEFAULT_INDENT, ' ');
  if (has_short_name) {
    printf(MAKE_BOLD("%c"), opt->short_name);
    if (opt->need_argument) {
      printf(" " MAKE_UNDERLINED("%s"), lower_case_parameter_value);
    }
  }
  if (has_short_name && has_long_name) {
    printf(", ");
  }
  if (has_long_name) {
    printf(MAKE_BOLD("--%s"), opt->long_name);
    if (opt->need_argument) {
      printf("=" MAKE_UNDERLINED("%s"), lower_case_parameter_value);
    }
  }
  putchar('\n');
  printf("%*c", 2 * OPT_MAN_DEFAULT_INDENT, ' ');
  _print_description(column_count, 2 * OPT_MAN_DEFAULT_INDENT,
      2 * OPT_MAN_DEFAULT_INDENT, opt->description, LANG_OPT__PARAMETER_VALUE);
  putchar('\n');
}

void _print_section_man(opt_category category, size_t column_count) {
  for (size_t i = 0; i < sizeof(options) / sizeof(opt_t); ++i) {
    if (options[i].category != category) {
      continue;
    }
    print_opt__man(&options[i], column_count);
  }
}
