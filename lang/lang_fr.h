//==============================================================================
//= Error messages
//==============================================================================

//  Option processing
#define LANG_MESSAGE_ERROR__MISSING_ARGUMENT "Argument manquant"
#define LANG_MESSAGE_ERROR__UNKNOWN_OPTION "Option non reconnue"

//  Processing error
#define LANG_MESSAGE_ERROR__FILE_COUNT "Au moins deux fichiers sont attendus"
#define LANG_MESSAGE_ERROR__UNACCESSIBLE_FILE "Le fichier ne peut etre " \
  "ouvert a la lecture"
#define LANG_MESSAGE_ERROR__IO "Une erreur est survenue lors du traitement " \
  "du fichier"
#define LANG_MESSAGE_ERROR__CAPACITY "Pas assez de memoire"
#define LANG_MESSAGE_ERROR__UNEXPECTED "Une erreur inatendue est survenue"
#define LANG_MESSAGE_ERROR__INVALID_PARAMETER "Parametre invalide"

//  Information
#define LANG_MESSAGE_MORE_INFO "Essayez '" EXEC_NAME_FORMAT " " OPT__LONG  \
  OPT_NAME_LONG__HELP "' pour plus d'informations."

//==============================================================================
//= Warning messages
//==============================================================================

//  Splicing word
#define LANG_MESSAGE_WARNING_WORD_SPLICING__FILE "Un mot de '%s' a la ligne " \
  "%zu a ete coupe"
#define LANG_MESSAGE_WARNING_WORD_SPLICING__STDIN "Un mot de l'entree " \
  "standard a la ligne %zu a ete coupe"

//==============================================================================
//= Options
//==============================================================================

//  Generic parameter names
#define LANG_OPT__PARAMETER_OPTION "OPTION"
#define LANG_OPT__PARAMETER_FILES "FICHIERS"
#define LANG_OPT__PARAMETER_VALUE "VALEUR"

#define LANG_OPT_DESCRIPTION__INITIAL "Definit le nombre de caracteres " \
  "significatif d'un mot a " LANG_OPT__PARAMETER_VALUE ". 0 signifie qu'il " \
  "n'y a pas de limite. La nombre par defaut est " \
  XSTR(WS_CTX_DEFAULT_OPTION_VALUE__INITIAL) "."
#define LANG_OPT_DESCRIPTION__PUNCTUATION_LIKE_SPACE "Les caracteres de " \
  "ponctuation seront consideres comme est caracteres d'espacement."
#define LANG_OPT_DESCRIPTION__SAME_NUMBER "Affiche plus de mots que la " \
  "limite en cas de meme nombre."
#define LANG_OPT_DESCRIPTION__TOP "Definit le nombre maximal de mots a " \
  "afficher a " LANG_OPT__PARAMETER_VALUE ". 0 veut dire que tout les mots " \
  "seront affiches. Le nombre par defaut est " \
  XSTR(WS_CTX_DEFAULT_OPTION_VALUE__TOP) "."
#define LANG_OPT_DESCRIPTION__UPPERCASING "Convertis les caracetres " \
  "minuscules en majuscules."

#define LANG_OPT_DESCRIPTION__HELP "Affiche ce message d'aide."
#define LANG_OPT_DESCRIPTION__MAN "Affiche ce message suivant le style man " \
  "puis quite."
#define LANG_OPT_DESCRIPTION__USAGE "Affiche un message d'utilisation puis " \
  "quitte."
#define LANG_OPT_DESCRIPTION__VERSION "Affiche la version"

//==============================================================================

#define LANG_MANN_SECTION__NAME "NOM"
#define LANG_MANN_SECTION__SYNOPSIS "SYNOPSIS"
#define LANG_MANN_SECTION__DESCRIPTION "DESCRIPTION"
#define LANG_MANN_SECTION__OPTIONS "OPTIONS"
#define LANG_MANN_SECTION__LIMITS "LIMITATIONS"
#define LANG_MANN_SECTION__AUTHORS "AUTEURS"
#define LANG_MANN_SECTION__COPYRIGHT "DROITS D'AUTEUR"

//==============================================================================

#define LANG_OPT_CATEGORY_NAME__INFORMATION "Informations"
#define LANG_OPT_CATEGORY_NAME__INPUT_CONTROL "Controle de l'entree"
#define LANG_OPT_CATEGORY_NAME__OUTPUT_CONTROL "Controle de la sortie"

//==============================================================================

#define LANG_USAGE_MESSAGE "Utilisation: " EXEC_NAME_FORMAT " [" \
  LANG_OPT__PARAMETER_OPTION "]... " LANG_OPT__PARAMETER_FILES

#define LANG_WS__SHORT_DESCRIPTION "Affiche une liste de mots partages par " \
  "des fichiers. "

#define LANG_WS__SYNOPSIS "[" MAKE_UNDERLINED(LANG_OPT__PARAMETER_OPTION) \
  "]... " MAKE_UNDERLINED(LANG_OPT__PARAMETER_FILES)

#define LANG_WS__HOW_TO_USE_OPTIONS "Les arguments exiges par les options " \
  "longues sont exgiges par les options courtes aussi."

//  TODO@Dyamen1411@KarolinaK25:
//    translate
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

#define LANG_WS__LIMITS "Le nombre de " LANG_OPT__PARAMETER_FILES " qui " \
  "doivent etre passes doit etre au moins 2. les conteurs des nombres totaux " \
  "d'occurence des mots a une valeur maximale de " XSTR(_LONG_MAX) "."

#define LANG_WS__AUTHORS "Ecrit par " WS_AUTHOR_NAMES "."

#define LANG_WS__COPYRIHT "Ceci est un logiciel libre: vous pouvez le " \
  "redistribuer a condition de nous citer. Il n'y a AUCUNE GARANTIE."
