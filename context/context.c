#include "context.h"
#include "holdall.h"
#include "hashtable.h"
#include "lang.h"
#include "linked_list.h"

#include <ctype.h>
#include <limits.h>
#include <string.h>

#define DEFAULT_WORD_CAPACITY 8
#define BITS_IN_BYTE 8
#define DEFAULT_PATTERN_CELL_0 1

//==============================================================================
//  Structures
//==============================================================================

// TODO@Dyamen1411 TODO@KarolinaK25 :
//  Add comments to explain structures

typedef struct word_t {
  char *word;
  size_t count;
  char *pattern;
} word_t;

struct wsctx_t {
  holdall *dictionary;
  hashtable *table;

  struct words_data_holder {
    word_t **list;
    size_t count;
    size_t capacity;
    holdall *h;
  } words;

  struct files_data_holder {
    size_t count;
    size_t current;
    size_t pattern_size;
    char **names;
    char *current_pattern;
  } files;

  wsctx_parameters_t parameters;
  const char *error_message;
};

typedef int (*getc_fun)(FILE *);

//==============================================================================
//  Prototypes
//==============================================================================

//  wsctx_parse_next_file : lit et stocke les mots contenus dans un fichier
//    en lecture, en prenant compte des options, et les place dans un buffer.
//    Vérifie si le mot a été coupé, si c'est le cas, il y a message d'erreur,
//    sion ne change rien.

static return_type wsctx_parse_next_file(wsctx_t *ctx);

//  skip_spaces : passe tous les characteres consideres comme un espace dans le
//    flot controle par stream et incremente *line_number si un saut de ligne
//    est repere. Revoie RETURN_ERROR_IO en cas d'erreur sur le flot controle
//    par stream, RETURN_EXIT si le dernier charactere lu est EOF, RETURN_NONE
//    sinon
static return_type skip_spaces(FILE *stream, getc_fun _getc,
    size_t *line_number, bool punctuation_like_spaces);

//  get_next_word : lis le prochain MOT de stream dans *word_buffer et la taille
//    de ce mot sera enregistree dans *word_length. Retourne
//    RETURN_ERROR_CAPACITY en cas de depassement de capacites, RETURN_ERROR_IO
//    en cas d'erreur sur le flot controle par stream , RETURN_NONE si non.
static return_type get_next_word(FILE *stream, getc_fun _getc,
    char **word_buffer, size_t *word_length, size_t default_capacity,
    bool uppercasing, bool punctuation_like_spaces);

//  wsctx_add_word_to_dictionnary : ajoute la chaine de charactere pointee par
//    word au dictionnaire. Retourne NULL en cas de passement de capacites, un
//    pointeur sur une structure word_t associee a la chaine de charactere
//    pointee par word sinon
static word_t *wsctx_add_word(wsctx_t *ctx, char *word);

//  wsctx_prepare_next_file : prepare le motif et l'index du prochain fichier
//    dans la liste des fichiers du context pointe par ctx
static void wsctx_prepare_next_file(wsctx_t *ctx);

//  strhash : renvoie un hash de la chaine de caractere pointee par s.
static size_t strhash(const char *s);

//  getc_stream : renvoie le caractère à la position courante du flux.
//    Après lecture dudit caractère, par le biais de la fonction fgetc(), la
//    lecture associée au flux passe au caractère suivant.
static int getc_stream(FILE *stream);

//  getc_stdin : lecture du caractère se trouvant à la position courante
//    du flux de l'entrée standard
static int getc_stdin(__attribute__((unused)) FILE *stream);

//  fclose_stream : ferme le flux de caractère pointé par STREAM,
//    préalablement ouvert.
static int fclose_stream(FILE *stream);

//  fclose_stdin : renvoie 0. Utilisé en cas de fermeture réalisée de
//    flux de caractères.
static int fclose_stdin(__attribute__((unused)) FILE *stream);

//  is_space : indique si le charactere c est considere comme un espace ou non.
static bool is_space(int c, bool punctuation_like_spaces);

static size_t word_file_occurence_count(const word_t *word,
    size_t pattern_size);

//  word_compar_file_appearances : compare les motifs de deux mots
//    passés en paramètres. Coma=pare les apparitions dans les fichiers.
static int word_compar_file_appearances(const word_t **word1,
    const word_t **word2);

//  word_compar_pattern : compare le motif de la chaîne de caractères
//    pointée par word1 à celui de la chaîne de caractères pointée par
//    word2.
static int word_compar_pattern(const word_t **word1, const word_t **word2);

//  word_compar_count : compare le nombre total d'occurrence associé à
//    la chaîne de caractères pointée par word1 à celui de la chaine de
//    caractères pointée par word2.
static int word_compar_count(const word_t **word1, const word_t **word2);

//  word_compar_word : compare les deux chaines de caractères pointées
//    respectivement par word1 et word2.
static int word_compar_word(const word_t **word1, const word_t **word2);

//  min_size_t : renvoie le minimum des tailles a et b
static size_t min__size_t(size_t a, size_t b);

//  print_word ; renvoie le motif, le nombre d'occurrence ainsi que la
//    chaine de caractère prise en compte pour chaque mot.
static void print_word(wsctx_t *ctx, word_t *word);

//  r_free : libere les ressources allouees a ptr et renvoie 0.
static int r_free(void *ptr);

//  r_free_word : libere les ressources allouees a word et renvoie 0.
static int r_free_word(word_t *word);

//==============================================================================
// Variables globales
//==============================================================================
static size_t pattern_size;

//==============================================================================
//  Fonctions de context.h
//==============================================================================

//  wsctx_parameters_default_initialization : attribue à chaque option
//    une valeur par défaut
void wsctx_parameters_default_initialization(
    wsctx_parameters_t *parameters) {
  parameters->initial = WS_CTX_DEFAULT_OPTION_VALUE__INITIAL;
  parameters->punctuation_like_spaces
    = WS_CTX_DEFAULT_OPTION_VALUE__PUNCTUATION_LIKE_SPACE;
  parameters->same_number = WS_CTX_DEFAULT_OPTION_VALUE__SAME_NUMBER;
  parameters->top = WS_CTX_DEFAULT_OPTION_VALUE__TOP;
  parameters->uppercasing = WS_CTX_DEFAULT_OPTION_VALUE__UPPERCASING;
}

// TODO to complete
//  wsctx_initialize : initialise les paramètres
wsctx_t *wsctx_initialize(const wsctx_parameters_t *parameters,
    const char **file_names, size_t file_count) {
  wsctx_t *ctx = (wsctx_t *) malloc(sizeof(wsctx_t));
  if (ctx == NULL) {
    return NULL;
  }
  ctx->dictionary = NULL;
  ctx->table = NULL;
  ctx->words.h = NULL;
  ctx->words.list = NULL;
  ctx->files.names = NULL;
  ctx->files.current_pattern = NULL;
  ctx->dictionary = holdall_empty();
  if (ctx->dictionary == NULL) {
    goto error;
  }
  ctx->table = hashtable_empty(
      (int (*) (const void *, const void *)) strcmp,
      (size_t (*) (const void *)) strhash);
  if (ctx->table == NULL) {
    goto error;
  }
  ctx->words.h = holdall_empty();
  if (ctx->words.h == NULL) {
    goto error;
  }
  ctx->words.list
    = (word_t **) malloc(DEFAULT_WORD_CAPACITY * sizeof(word_t *));
  if (ctx->words.list == NULL) {
    goto error;
  }
  ctx->files.pattern_size
    = 1 + ((file_count + BITS_IN_BYTE - 1) / BITS_IN_BYTE);
  ctx->files.current_pattern = (char *) malloc(ctx->files.pattern_size);
  if (ctx->files.current_pattern == NULL) {
    goto error;
  }
  memset(ctx->files.current_pattern, 0, ctx->files.pattern_size);
  --ctx->files.pattern_size;
  ctx->files.names = (char **) malloc(file_count * sizeof(char *));
  if (ctx->files.names == NULL) {
    goto error;
  }
  for (size_t i = 0; i < file_count; ++i) {
    const char *file_name = file_names[i];
    ctx->files.names[i] = (char *) malloc(strlen(file_name) + 1);
    if (ctx->files.names[i] == NULL) {
      for (; i != (size_t) (-1); --i) {
        free(ctx->files.names[i]);
      }
      goto error;
    }
    strcpy(ctx->files.names[i], file_names[i]);
  }
  ctx->words.count = 0;
  ctx->words.capacity = DEFAULT_WORD_CAPACITY;
  ctx->files.current_pattern[0] = DEFAULT_PATTERN_CELL_0;
  ctx->files.count = file_count;
  ctx->files.current = 0;
  ctx->parameters = *parameters;
  ctx->error_message = NULL;
  return ctx;
error:
  holdall_dispose(&ctx->dictionary);
  hashtable_dispose(&ctx->table);
  holdall_dispose(&ctx->words.h);
  if (ctx->words.list != NULL) {
    free(ctx->words.list);
  }
  if (ctx->files.names != NULL) {
    free(ctx->files.names);
  }
  if (ctx->files.current_pattern != NULL) {
    free(ctx->files.current_pattern);
  }
  free(ctx);
  return NULL;
}

//  wsctx_parse_files : permet de traiter tous les fichiers, rentrés dans la
//    ligne de commande.
return_type wsctx_parse_files(wsctx_t *ctx) {
  return_type res;
  for (size_t i = 0; i < ctx->files.count; ++i) {
    res = wsctx_parse_next_file(ctx);
    if (res != RETURN_NONE) {
      return res;
    }
  }
  ctx->words.list = (word_t **) realloc(ctx->words.list,
      ctx->words.count * sizeof(word_t *));
  return ctx->words.list == NULL
         ? RETURN_ERROR_CAPACITY
         : RETURN_NONE;
}

// TODO
//  wsctx_sort_data :
void wsctx_sort_data(wsctx_t *ctx) {
  // Sort by pattern
  pattern_size = ctx->files.pattern_size;
  qsort(ctx->words.list, ctx->words.count, sizeof(word_t *),
      (int (*) (const void *, const void *)) word_compar_pattern);
  // Sort by count
  word_t **base = ctx->words.list;
  size_t count = 1;
  word_t *previous = ctx->words.list[0];
  for (size_t i = 1; i < ctx->words.count; ++i) {
    if (word_compar_pattern((const word_t **) &previous,
        (const word_t **) &ctx->words.list[i]) != 0) {
      qsort(base, count, sizeof(word_t *),
          (int (*) (const void *, const void *)) word_compar_count);
      base += count;
      previous = ctx->words.list[i];
      count = 1;
      continue;
    }
    ++count;
  }
  qsort(base, count, sizeof(word_t *),
      (int (*) (const void *, const void *)) word_compar_count);
  // Sort by lexical reverse order
  base = ctx->words.list;
  count = 1;
  previous = ctx->words.list[0];
  for (size_t i = 1; i < ctx->words.count; ++i) {
    if (word_compar_file_appearances((const word_t **) &previous,
        (const word_t **) &ctx->words.list[i]) != 0
        || word_compar_count((const word_t **) &previous,
        (const word_t **) &ctx->words.list[i]) != 0) {
      qsort(base, count, sizeof(word_t *),
          (int (*) (const void *, const void *)) word_compar_word);
      base += count;
      previous = ctx->words.list[i];
      count = 1;
      continue;
    }
    ++count;
  }
  qsort(base, count, sizeof(word_t *),
      (int (*) (const void *, const void *)) word_compar_word);
}

//  TODO
//  wsctx_output_data :
void wsctx_output_data(wsctx_t *ctx) {
  const size_t limit = ctx->parameters.top == 0
      ? ctx->words.count
      : min__size_t(ctx->words.count, ctx->parameters.top);
  for (size_t i = 0; i < limit; ++i) {
    word_t *word = ctx->words.list[ctx->words.count - i - 1];
    if (word_file_occurence_count(word, ctx->files.pattern_size) < 2) {
      return;
    }
    print_word(ctx, word);
  }
  if (!ctx->parameters.same_number || ctx->parameters.top == 0) {
    return;
  }
  word_t *model = ctx->words.list[ctx->words.count - limit - 1];
  for (size_t i = limit; i < ctx->words.count; ++i) {
    word_t *word = ctx->words.list[ctx->words.count - i - 1];
    if (word_file_occurence_count(word, ctx->files.pattern_size) < 2) {
      return;
    }
    if (word_compar_file_appearances((const word_t **) &model,
        (const word_t **) word) != 0
        || word_compar_count((const word_t **) &model,
        (const word_t **) word) != 0) {
      break;
    }
    print_word(ctx, ctx->words.list[ctx->words.count - i - 1]);
  }
}

//  wsctx_get_error_message : renvoie le message d'erreur
//    pour un éément donné du context.
const char *wsctx_get_error_message(wsctx_t *ctx) {
  return ctx->error_message;
}

//  wsctx_dispose : libère l'espace mémoire occupé les éléments du
//    context
void wsctx_dispose(wsctx_t **ctx) {
  if (*ctx == NULL) {
    return;
  }
  holdall_apply((*ctx)->dictionary, r_free);
  holdall_apply((*ctx)->words.h, (int (*) (void *)) r_free_word);
  holdall_dispose(&(*ctx)->dictionary);
  holdall_dispose(&(*ctx)->words.h);
  hashtable_dispose(&(*ctx)->table);
  free((*ctx)->words.list);
  for (size_t i = 0; i < (*ctx)->files.count; ++i) {
    free((*ctx)->files.names[i]);
  }
  free((*ctx)->files.names);
  free((*ctx)->files.current_pattern);
  free(*ctx);
  *ctx = NULL;
}

//==============================================================================
//  Corps des prototypes declares dans context.c
//==============================================================================

return_type wsctx_parse_next_file(wsctx_t *ctx) {
  const char *file_name = ctx->files.names[ctx->files.current];
  const bool is_file = *file_name != '\0';
  return_type res = RETURN_NONE;
  // opening stream
  FILE *stream = is_file
      ? fopen(file_name, "r")
      : stdin;
  if (is_file && stream == NULL) {
    ctx->error_message = file_name;
    return RETURN_ERROR_UNACCESSIBLE_FILE;
  }
  int (*_getc)(FILE *) = is_file
      ? getc_stream
      : getc_stdin;
  int (*_fclose)(FILE *) = is_file
      ? fclose_stream
      : fclose_stdin;
  // searching for words in stream
  size_t line_number = 1;
  while (!feof(stream)) {
    // skip spaces and punctuation if -p is specified
    res = skip_spaces(stream, _getc, &line_number,
        ctx->parameters.punctuation_like_spaces);
    if (res != RETURN_NONE) {
      if (res == RETURN_EXIT) {
        break;
      }
      _fclose(stream);
      ctx->error_message = file_name;
      return RETURN_ERROR_IO;
    }
    // retrieve word into word buffer
    size_t word_length = 0;
    char *word_buffer = NULL;
    res = get_next_word(stream, _getc, &word_buffer, &word_length,
        ctx->parameters.initial, ctx->parameters.uppercasing,
        ctx->parameters.punctuation_like_spaces);
    if (res != RETURN_NONE) {
      _fclose(stream);
      if (res == RETURN_ERROR_CAPACITY) {
        return RETURN_ERROR_CAPACITY;
      }
      ctx->error_message = file_name;
      return RETURN_ERROR_IO;
    }
    // printing warning message if word is sliced
    if (ctx->parameters.initial != 0
        && word_length >= ctx->parameters.initial) {
      word_buffer[ctx->parameters.initial] = '\0';
      fprintf(stderr, "%s: ", ctx->parameters.exec_name);
      if (*file_name == '\0') {
        fprintf(stderr, LANG_MESSAGE_WARNING_WORD_SPLICING__STDIN, line_number);
      } else {
        fprintf(stderr, LANG_MESSAGE_WARNING_WORD_SPLICING__FILE, file_name,
            line_number);
      }
      fprintf(stderr, ": '%s...'.\n", word_buffer);
    }
    // adding word to context
    word_t *word = wsctx_add_word(ctx, word_buffer);
    if (word == NULL) {
      free(word_buffer);
      _fclose(stream);
      return RETURN_ERROR_CAPACITY;
    }
    if (word->count != 0) {
      free(word_buffer);
    }
    ++word->count;
  }
  // Close the stream (if it's not stdin)
  if (_fclose(stream) == EOF) {
    ctx->error_message = file_name;
    return RETURN_ERROR_IO;
  }
  wsctx_prepare_next_file(ctx);
  return RETURN_NONE;
}

//  Making stdin readable again on POSIX compliant systems :
//    http://ostack.cn/?qa=634510/
return_type skip_spaces(FILE *stream, getc_fun _getc, size_t *line_number,
    bool punctuation_like_spaces) {
  int c;
  while (1) {
    c = _getc(stream);
    if (c == EOF || !is_space(c, punctuation_like_spaces)) {
      break;
    }
    *line_number += c == '\n';
  }
  if (c != EOF) {
    return ungetc(c, stream) == EOF
           ? RETURN_ERROR_IO
           : RETURN_NONE;
  }
  if (ferror(stream)) {
    return RETURN_ERROR_IO;
  }
  if (stream == stdin) {
    clearerr(stream);
  }
  return RETURN_EXIT;
}

return_type get_next_word(FILE *stream, getc_fun _getc, char **word_buffer,
    size_t *word_length, size_t default_capacity, bool uppercasing,
    bool punctuation_like_spaces) {
  // Initialization
  const bool word_not_capped = default_capacity == 0;
  size_t word_buffer_capacity = (word_not_capped
      ? WS_CTX_DEFAULT_OPTION_VALUE__INITIAL
      : default_capacity);
  *word_buffer = (char *) malloc(word_buffer_capacity + 1);
  if (*word_buffer == NULL) {
    return RETURN_ERROR_CAPACITY;
  }
  memset(*word_buffer, 0, word_buffer_capacity);
  *word_length = 0;
  //retrieving word
  int c;
  while (1) {
    c = _getc(stream);
    if (c == EOF || is_space(c, punctuation_like_spaces)) {
      break;
    }
    // Extend word buffer capacity if needed (always leave one byte free for
    //  the null terminator)
    if (word_not_capped && *word_length == word_buffer_capacity) {
      word_buffer_capacity = word_buffer_capacity * 2 + 1;
      *word_buffer = (char *) realloc(*word_buffer, word_buffer_capacity + 1);
      if (*word_buffer == NULL) {
        return RETURN_ERROR_CAPACITY;
      }
      memset(*word_buffer + *word_length + 1, 0,
          word_buffer_capacity - *word_length - 1);
    }
    // If word has infinite capacity or space remaining, append char to word
    if (word_not_capped || *word_length <= word_buffer_capacity) {
      (*word_buffer)[*word_length] = (char) (uppercasing ? toupper(c) : c);
      ++*word_length;
    }
  }
  if (ferror(stream)) {
    free(*word_buffer);
    return RETURN_ERROR_IO;
  }
  if (c == EOF && stream == stdin) {
    clearerr(stream);
  }
  *word_buffer = realloc(*word_buffer, *word_length + 1);
  return *word_buffer == NULL
         ? RETURN_ERROR_CAPACITY
         : RETURN_NONE;
}

word_t *wsctx_add_word(wsctx_t *ctx, char *word) {
  word_t *w = (word_t *) hashtable_search(ctx->table, (const void *) word);
  if (w == NULL) {
    // create word
    w = (word_t *) malloc(sizeof(word_t));
    if (w == NULL) {
      return NULL;
    }
    // create empty pattern for word
    w->pattern = (char *) malloc(ctx->files.pattern_size);
    if (w->pattern == NULL) {
      free(w);
      return NULL;
    }
    memset(w->pattern, 0, ctx->files.pattern_size);
    // add word to holder
    if (holdall_put(ctx->words.h, (void *) w) != 0) {
      free(w);
      return NULL;
    }
    // populate word
    w->count = 0;
    w->word = word;
    // enlarge word list if nedded
    if (ctx->words.count == ctx->words.capacity) {
      ctx->words.capacity *= 2;
      ctx->words.list = (word_t **) realloc(ctx->words.list,
          ctx->words.capacity * sizeof(word_t *));
      if (ctx->words.list == NULL) {
        free(w->pattern);
        free(w);
        return NULL;
      }
    }
    // add word to word list
    ctx->words.list[ctx->words.count] = w;
    ++ctx->words.count;
    // save word to dictionnary
    if (holdall_put(ctx->dictionary, (void *) word) != 0) {
      return NULL;
    }
    if (hashtable_add(ctx->table,
        (const void *) word, (const void *) w) == NULL) {
      return NULL;
    }
  }
  const size_t byte_index = ctx->files.current / BITS_IN_BYTE;
  const size_t bit_index = ctx->files.current % BITS_IN_BYTE;
  w->pattern[byte_index] |= (char) (1 << bit_index);
  return w;
}

inline void wsctx_prepare_next_file(wsctx_t *ctx) {
  // evil bithack to update pattern
  short *interesting_byte = (short *) (ctx->files.current_pattern
      + (ctx->files.current / BITS_IN_BYTE));
  *interesting_byte = (short) (*interesting_byte << 1);
  ++ctx->files.current;
}

//==============================================================================
// Helper functions
//==============================================================================

size_t strhash(const char *s) {
  size_t h = 0;
  for (const unsigned char *p = (const unsigned char *) s; *p != '\0'; ++p) {
    h = 37 * h + *p;
  }
  return h;
}

int getc_stream(FILE *stream) {
  return fgetc(stream);
}

int getc_stdin(__attribute__((unused)) FILE *stream) {
  return getchar();
}

int fclose_stream(FILE *stream) {
  return fclose(stream);
}

int fclose_stdin(__attribute__((unused)) FILE *stream) {
  return 0;
}

bool is_space(int c, bool punctuation_like_spaces) {
  return isspace(c) || (punctuation_like_spaces && ispunct(c));
}

size_t word_file_occurence_count(const word_t *word, size_t pattern_size) {
  size_t file_occurence_count = 0;
  for (size_t i = 0; i < pattern_size; ++i) {
    for (size_t j = 0; j < BITS_IN_BYTE; ++j) {
      file_occurence_count += (word->pattern[i] >> j) & 1;
    }
  }
  return file_occurence_count;
}

int word_compar_file_appearances(const word_t **word1, const word_t **word2) {
  const int occurence_count_1
    = (int) word_file_occurence_count(*word1, pattern_size);
  const int occurence_count_2
    = (int) word_file_occurence_count(*word2, pattern_size);
  return (occurence_count_1 > occurence_count_2)
         - (occurence_count_1 < occurence_count_2);
}

int word_compar_pattern(const word_t **word1, const word_t **word2) {
  int res = word_compar_file_appearances(word1, word2);
  if (res != 0) {
    return res;
  }
  return 0;
}

int word_compar_count(const word_t **word1, const word_t **word2) {
  return ((*word1)->count > (*word2)->count)
         - ((*word1)->count < (*word2)->count);
}

int word_compar_word(const word_t **word1, const word_t **word2) {
  return strcmp((*word2)->word, (*word1)->word);
}

size_t min__size_t(size_t a, size_t b) {
  return a < b ? a : b;
}

void print_word(wsctx_t *ctx, word_t *word) {
  for (size_t i = 0; i < ctx->files.count; ++i) {
    printf("%c",
        ((word->pattern[(i / BITS_IN_BYTE)]
        >> (i % BITS_IN_BYTE)) & 1) ? 'x' : '-');
  }
  printf("\t%zu\t%s\n", word->count, word->word);
}

int r_free(void *ptr) {
  free(ptr);
  return 0;
}

int r_free_word(word_t *word) {
  free(word->pattern);
  free(word);
  return 0;
}
