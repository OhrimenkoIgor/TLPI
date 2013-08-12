/*
 Use thread-specific data to write thread-safe versions of dirname() and basename()
 (Section 18.14).

 Trailing slash characters in pathname are ignored.
 If pathname doesn’t contain a slash, then dirname() returns the string . (dot)
 and basename() returns pathname.
 If pathname consists of just a slash, then both dirname() and basename() return
 the string /. Applying the concatenation rule above to create a pathname from
 these returned strings would yield the string ///. This is a valid pathname.
 Because multiple consecutive slashes are equivalent to a single slash, the pathname
 /// is equivalent to the pathname /.
 If pathname is a NULL pointer or an empty string, then both dirname() and
 basename() return the string . (dot). (Concatenating these strings yields the
 pathname ./., which is equivalent to ., the current directory.)

 */


#include <iostream>

#include "functions.h"

int main() {



	return 0;
}
