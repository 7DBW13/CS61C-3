/**
 * The contents of this file are only a suggestion.
 * Feel free to change anything.
 */

#include "matcher.h"


/**
 * Returns true if partial_line matches pattern, starting from
 * the first char of partial_line.
 */
int matches_leading(char *partial_line, char *pattern, char *original) {
  // You can use this recommended helper function, or not.
  // TODO put your code here.
  if (*pattern == '\0') {
    return 1;
  } else if (*partial_line == '\0') {
    return 0;
  } else {
    char * curr = pattern;
    char * next = pattern + 1;
    if (*curr == '\\') {
      char * next2 = next + 1;
      if (*next2 != '+' && *next2 != '?') {
        if (*partial_line == *next) {
          return matches_leading(partial_line + 1, pattern + 2, original);
        } else {
          return matches_leading(partial_line + 1, original, original);
        }
      } else if (*next2 == '+') {
        if (*partial_line == *next) {
          return matches_leading(partial_line + 1, pattern + 3, original) ||
            matches_leading(partial_line + 1, pattern, original);
        } else {
          return matches_leading(partial_line + 1, original, original);
        }
      } else {
        if (*partial_line == *next) {
          return matches_leading(partial_line + 1, pattern + 3, original) ||
            matches_leading(partial_line, pattern + 3, original);
        } else {
          return matches_leading(partial_line + 1, pattern + 3, original) ||
            matches_leading(partial_line, pattern + 3, original);
        }
      }
    } else if (*next == '+') {
      if (*partial_line == *curr || *curr == '.') {
        return matches_leading(partial_line + 1, pattern + 2, original) ||
          matches_leading(partial_line + 1, pattern, original);
      } else {
        return matches_leading(partial_line + 1, original, original);
      }
    } else if (*next == '?') {
      if (*partial_line == *curr || *curr == '.') {
        return matches_leading(partial_line + 1, pattern + 2, original) ||
          matches_leading(partial_line, pattern + 2, original);
      } else {
        return matches_leading(partial_line + 1, original, original) ||
          matches_leading(partial_line, pattern + 2, original);
      }
    } else {
      if (*partial_line == *curr || *curr == '.') {
        return matches_leading(partial_line + 1, pattern + 1, original);
      } else {
        return matches_leading(partial_line + 1, original, original);
      }
    }
  }
}

/**
 * Implementation of your matcher function, which
 * will be called by the main program.
 *
 * You may assume that both line and pattern point
 * to reasonably short, null-terminated strings.
 */
int rgrep_matches(char *line, char *pattern) {

    //
    // TODO put your code here.
    //

    return matches_leading(line, pattern, pattern);
}
