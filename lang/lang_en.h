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
#define LANG_MESSAGE_WARNING_WORD_SPLICING__WORD_FROM "Word from"
#define LANG_MESSAGE_WARNING_WORD_SPLICING__STDIN "standard input"
#define LANG_MESSAGE_WARNING_WORD_SPLICING__AT_LINE "at line"
#define LANG_MESSAGE_WARNING_WORD_SPLICING__CUT "cut"

//==============================================================================
//= Options
//==============================================================================

//  Generic parameter names
#define LANG_OPT__PARAMETER_OPTION "OPTION"
#define LANG_OPT__PARAMETER_FILES "FILES"
#define LANG_OPT__PARAMETER_VALUE "VALUE"

//  Long option names
#define LANG_OPT_NAME_LONG__INITIAL "initial"
#define LANG_OPT_NAME_LONG__PUNCTUATION_LIKE_SPACE "punctuation-like-space"
#define LANG_OPT_NAME_LONG__SAME_NUMBER "same-number"
#define LANG_OPT_NAME_LONG__TOP "top"
#define LANG_OPT_NAME_LONG__USAGE "usage"

//  Short option names
#define LANG_OPT_NAME_SHORT__INITIAL 'i'
#define LANG_OPT_NAME_SHORT__PUNCTUATION_LIKE_SPACE 'p'
#define LANG_OPT_NAME_SHORT__SAME_NUMBER 's'
#define LANG_OPT_NAME_SHORT__TOP 't'
#define LANG_OPT_NAME_SHORT__USAGE '\0'

//  Option descriptions
//  TODO@Dyamen1411 :
//    Change external vales by format instead of direct.
//    To do so, might add new field to opt_t to pass such values
#define LANG_OPT_DESCRIPTION__INITIAL "Set the maximal number of significant " \
  "initial letters for words to VALUE. 0 means without limitation. Default " \
  "is " XSTR(WS_CTX_DEFAULT_OPTION_VALUE__INITIAL) "."
#define LANG_OPT_DESCRIPTION__PUNCTUATION_LIKE_SPACE "Make the punctuation " \
  "characters play the same role as space characters in the meaning of words."
#define LANG_OPT_DESCRIPTION__SAME_NUMBER "Print more words than the limit " \
  "in case of same numbers."
#define LANG_OPT_DESCRIPTION__TOP "Set the maximal number of words to print " \
  "to VALUE. 0 means all the words. Default is " \
  XSTR(WS_CTX_DEFAULT_OPTION_VALUE__TOP) "."
#define LANG_OPT_DESCRIPTION__USAGE "Print a short usage message and exit."

//  Usage
#define LANG_OPT__USAGE__USAGE "Usage"

//==============================================================================

#define LANG_WS__SHORT_DESCRIPTION \
  "Print a list of words shared by text files"

#define LANG_WS__HOW_TO_USE_OPTIONS \
  "Mandatory arguments to long options are mandatory for short options too"

#define LANG_WS__LONG_DESCRIPTION \
  "prints a list of words that appear in several text files. Each of the " \
  "words listed is accompanied by a pattern indicating in which files it " \
  "appears and  the  total  number  of  its occurrences in the files.\n\n\t" \
  "A word is, by default, a maximum length sequence of characters that" \
  "do not belong to the space class. A word is shared when it appears" \
  "in at least two files. Only shared words are listed." \
  "The printed list is sorted in descending order of the number of files" \
  "in which the words appear, primary key, in descending order of the total" \
  "number of occurrences, secondary key, and in ascending lexicographical" \
  "order of words, tertiary key. The word comparison is understood in" \
  "the sense of the standard function strcmp. One line of text is" \
  "produced per shared word. The content of a line consists" \
  "of  three  fields: the pattern with 'x's or '-'s" \
  "depending on wether the word appears in the files or not; " \
  "the total number of occurrences; the word.The tab" \
  "character is used as field separator.\n\n\t" \
  "Read the standard input for any of the files which is \" - \"."

#define LANG_WS__MAN_LIMITS \
  "The number of FILES that can be passed must be at least 2. " \
  "The counters of the total number of occurences of the words have " \
  "a maximum value of"

#define LANG_WS__COPYRIHT \
  "This is freeware: you can redistribute it. There is NO WARRANTY."

#define LANG_WS__AUTHOR_WRITTEN "Written by"

#define LANG_WS__AUTHOR_NAME "A. MASSIAS & K.KLAK"
