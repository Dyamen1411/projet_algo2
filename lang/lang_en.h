//==============================================================================
//= Error messages
//==============================================================================

//  Option processing
#define LANG_MESSAGE_ERROR__MISSING_ARGUMENT "Missing argument"
#define LANG_MESSAGE_ERROR__UNKNOWN_OPTION "Unknown option"

//  Processing error
#define LANG_MESSAGE_ERROR__FILE_COUNT "At least 2 files are expected"
#define LANG_MESSAGE_ERROR__UNACCESSIBLE_FILE "Can't open for reading file"
#define LANG_MESSAGE_ERROR__IO "An error occured while processing file"
#define LANG_MESSAGE_ERROR__CAPACITY "Not enough memory"
#define LANG_MESSAGE_ERROR__UNEXPECTED "An unexpected error occured."

//==============================================================================
//= Warning messages
//==============================================================================

//  Splicing word
#define LANG_MESSAGE_WARNING_WORD_SPLICING__FILE "Word from '%s' at line %zu " \
  "cut"
#define LANG_MESSAGE_WARNING_WORD_SPLICING__STDIN "Word from standard input " \
  "at line %zu cut"

//==============================================================================
//= Options
//==============================================================================

//  Generic parameter names
#define LANG_OPT__PARAMETER_OPTION "OPTION"
#define LANG_OPT__PARAMETER_FILES "FILES"
#define LANG_OPT__PARAMETER_VALUE "VALUE"

#define LANG_OPT_DESCRIPTION__INITIAL "Set the maximal number of significant " \
  "initial letters for words to " LANG_OPT__PARAMETER_VALUE ". 0 means " \
  "without limitation. Default is " XSTR(WS_CTX_DEFAULT_OPTION_VALUE__INITIAL) \
  "."
#define LANG_OPT_DESCRIPTION__PUNCTUATION_LIKE_SPACE "Make the punctuation " \
  "characters play the same role as space characters in the meaning of words."
#define LANG_OPT_DESCRIPTION__SAME_NUMBER "Print more words than the limit " \
  "in case of same numbers."
#define LANG_OPT_DESCRIPTION__TOP "Set the maximal number of words to print " \
  "to " LANG_OPT__PARAMETER_VALUE ". 0 means all the words. Default is " \
  XSTR(WS_CTX_DEFAULT_OPTION_VALUE__TOP) "."
#define LANG_OPT_DESCRIPTION__UPPERCASING "Convert each lowercase letter of " \
  "words to the corresponding uppercase letter."
#define LANG_OPT_DESCRIPTION__HELP "Print this help message and exit."
#define LANG_OPT_DESCRIPTION__MAN "Print this help message following " \
  "man-style and exit."
#define LANG_OPT_DESCRIPTION__USAGE "Print a short usage message and exit."
#define LANG_OPT_DESCRIPTION__VERSION "Print version information."

//==============================================================================

#define LANG_MANN_SECTION__NAME "NAME"
#define LANG_MANN_SECTION__SYNOPSIS "SYNOPSIS"
#define LANG_MANN_SECTION__DESCRIPTION "DESCRIPTION"
#define LANG_MANN_SECTION__OPTIONS "OPTIONS"
#define LANG_MANN_SECTION__LIMITS "LIMITS"
#define LANG_MANN_SECTION__AUTHORS "AUTHORS"
#define LANG_MANN_SECTION__COPYRIGHT "COPYRIGHT"

//==============================================================================

#define LANG_OPT_CATEGORY_NAME__INFORMATION "Program Information"
#define LANG_OPT_CATEGORY_NAME__INPUT_CONTROL "Input Control"
#define LANG_OPT_CATEGORY_NAME__OUTPUT_CONTROL "Output Control"

//==============================================================================

#define LANG_USAGE_MESSAGE "Usage: " EXEC_NAME_FORMAT " [" \
  LANG_OPT__PARAMETER_OPTION "]... " LANG_OPT__PARAMETER_FILES

#define LANG_WS__SHORT_DESCRIPTION "Print a list of words shared by text files."

#define LANG_WS__SYNOPSIS "[" MAKE_UNDERLINED(LANG_OPT__PARAMETER_OPTION) \
  "]... " MAKE_UNDERLINED(LANG_OPT__PARAMETER_FILES)

#define LANG_WS__HOW_TO_USE_OPTIONS "Mandatory arguments to long options are " \
  "mandatory for short options too."

#define LANG_WS__LONG_DESCRIPTION "prints a list of words that appear in " \
  "several text " LANG_OPT__PARAMETER_FILES ". Each of the words listed is " \
  "accompanied by a pattern indicating in which " LANG_OPT__PARAMETER_FILES \
  " it appears and the total number of its occurrences in the " \
  LANG_OPT__PARAMETER_FILES ".\n\nA word is, by default, a maximum length " \
  "sequence of characters that do not belong to the space class. A word is " \
  "shared when it appears in at least two " LANG_OPT__PARAMETER_FILES ". " \
  "Only shared words are listed. The printed list is sorted in descending " \
  "order of the number of " LANG_OPT__PARAMETER_FILES " in which the words " \
  "appear, primary key, in descending order of the total number of " \
  "occurrences, secondary key, and in ascending lexicographical order of " \
  "words, tertiary key. The word comparison is understood in the sense of " \
  "the standard function strcmp. One line of text is produced per shared " \
  "word. The content of a line consists of three fields: the pattern with " \
  "'x's or '-' depending on wether the word appears in the " \
  LANG_OPT__PARAMETER_FILES " or not; the total number of occurrences; the " \
  "word.The tab character is used as field separator.\n\nRead the standard " \
  "input for any of the " LANG_OPT__PARAMETER_FILES " which is \"-\"."

#define LANG_WS__LIMITS "The number of " LANG_OPT__PARAMETER_FILES " that " \
  "can be passed must be at least 2. The counters of the total number of " \
  "occurences of the words have a maximum value of " XSTR(_LONG_MAX) "."

#define LANG_WS__AUTHORS "Written by " WS_AUTHOR_NAMES "."

#define LANG_WS__COPYRIHT "This is freeware: you can redistribute it. There " \
  "is NO WARRANTY."
