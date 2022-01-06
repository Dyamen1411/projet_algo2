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

#define LANG_OPT__USAGE__OPTION "OPTION"
#define LANG_OPT__USAGE__FILES "FILES"

#define LANG_OPT__USAGE__USAGE "Usage"

#define LANG_OPT_DESC_SHORT "Print a list of words shared by text files"
#define LANG_OPT_DESC_LONG  \
  "prints a list of words that appear in several text files." \
  "Each of the words listed is accompanied by " \
  "a pattern indicating in which files it appears" \
  "and the total number of its occurences in the files.\n\n" \
  "A word is, by default, a maximum length sequence" \
  "of characters that do not belong to the space class." \
  "A word is shared when it appears in at least two files." \
  "Only shared words are  listed." \
  "The  printed list is sorted in descending order" \
  "of the number of files in which the words appear," \
  "primary key, in descending order of the total number" \
  "of occurrences, secondary key, and in" \
  "ascending lexicographical order of words," \
  "tertiary key. The word comparison is understood in the sense" \
  "of the standard function strcmp. One line of text" \
  "is produced per  shared  word.  The content" \
  "of  a  line  consists  of  three  fields:" \
  "the pattern with 'x's or '-'s depending on" \
  "wether the word appears in the files or not;" \
  "the total number of occurrences; the" \
  "word.The tab character is used as field separator." \
  "Read the standard input for any of the files" \
  " which is " - "."
