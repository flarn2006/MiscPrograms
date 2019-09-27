#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "spellnum.h"

#ifdef __SIZEOF_LONG_LONG__
#define SIZEOF_NUMBER __SIZEOF_LONG_LONG__
#else
#define SIZEOF_NUMBER 8
#endif

#if SIZEOF_NUMBER > 8
#error sizeof number > 8 -- not prepared for that many potential digits!
#elif SIZEOF_NUMBER > 4
#define MAX_DIGITS 20
#elif SIZEOF_NUMBER > 2
#define MAX_DIGITS 10
#elif SIZEOF_NUMBER > 1
#define MAX_DIGITS 5
#else
#define MAX_DIGITS 3
#endif

#define MAX_GROUPS ((MAX_DIGITS + 2) / 3)

const char *zero = "zero";
const char *ones[] = {
	"one", "two", "three", "four", "five", "six", "seven", "eight", "nine",
	"ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"
};
const char *tens[] = { "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety" };

const char *suffixes[] = {
#if MAX_DIGITS > 3
	"thousand"
#if MAX_DIGITS > 6
	, "million"
#if MAX_DIGITS > 9
	, "billion"
#if MAX_DIGITS > 12
	, "trillion"
#if MAX_DIGITS > 15
	, "quadrillion"
#if MAX_DIGITS > 18
	, "quintillion"
#endif /* MAX_DIGITS > 18 */
#endif /* MAX_DIGITS > 15 */
#endif /* MAX_DIGITS > 12 */
#endif /* MAX_DIGITS > 9 */
#endif /* MAX_DIGITS > 6 */
#else
	NULL
#endif /* MAX_DIGITS > 3 */
};

static size_t len_19(number n)
{
	assert(0 <= n && n <= 19);
	if (n == 0)
		return 0;
	else
		return strlen(ones[n-1]) + 1;
}

static size_t len_99(number n)
{
	assert(0 <= n && n <= 99);
	if (n < 20) {
		return len_19(n);
	} else {
		number ones_place = n % 10;
		size_t result = strlen(tens[n/10-2]) + 1;
		if (ones_place > 0) 
			result += len_19(ones_place);
		return result;
	}
}

static size_t len_999(number n)
{
	assert(0 <= n && n <= 999);
	if (n < 100) {
		return len_99(n);
	} else {
		number place_99 = n % 100;
		size_t result = strlen(ones[n/100-1]) + 9;  /* add room for " hundred " */
		if (place_99 > 0)
			result += len_99(place_99);
		return result;
	}
}

static char *str_19(number n, char *ptr)
{
	assert(0 <= n && n <= 19);
	if (n == 0) {
		return ptr;
	} else {
		size_t len = len_19(n);
		memcpy(ptr, ones[n-1], len-1);
		ptr[len-1] = ' ';
		return ptr + len;
	}
}

static char *str_99(number n, char *ptr)
{
	assert(0 <= n && n <= 99);
	if (n == 0) {
		return ptr;
	} else if (n < 20) {
		return str_19(n, ptr);
	} else {
		const char *str = tens[n/10-2];
		size_t len = strlen(str);
		memcpy(ptr, str, len);
		number ones_place = n % 10;
		if (ones_place > 0) {
			ptr[len] = '-';
			return str_19(ones_place, ptr+len+1);
		} else {
			ptr[len] = ' ';
			return ptr+len+1;
		}
	}
}

static char *str_999(number n, char *ptr)
{
	assert(0 <= n && n <= 999);
	if (n == 0) {
		return ptr;
	} else if (n < 100) {
		return str_99(n, ptr);
	} else {
		const char *str = ones[n/100-1];
		size_t len = strlen(str);
		memcpy(ptr, str, len);
		memcpy(ptr + len, " hundred ", 9);
		return str_99(n % 100, ptr+len+9);
	}
}

size_t spellnum_len(number n)
{
	if (n == 0) {
		return 4; /* "zero" */
	} else {
		size_t result;

		if (n < 0) {
			result = 9; /* "negative " */
			n = -n;
		} else {
			result = 0;
		}

		int i; for (i=0; n>0; ++i) {
			number group = n % 1000;
			n /= 1000;
			if (group > 0) {
				result += len_999(group);
				if (i > 0)
					result += strlen(suffixes[i-1]) + 1;
			}
		}

		return result - 1;
	}
}

char *spellnum(number n, char *ptr)
{
	if (n == 0) {
		strcpy(ptr, zero);
		return ptr + 4;
	} else {
		if (n < 0) {
			strcpy(ptr, "negative ");
			ptr += 9;
			n = -n;
		}
		number groups[MAX_GROUPS];
		int i; for (i=0; n>0; ++i) {
			groups[i] = n % 1000;
			n /= 1000;
		}
		for (--i; i>=0; --i) {
			if (groups[i] > 0) {
				ptr = str_999(groups[i], ptr);
				if (i > 0) {
					size_t len = strlen(suffixes[i-1]);
					memcpy(ptr, suffixes[i-1], len);
					ptr[len] = ' ';
					ptr += len + 1;
				}
			}
		}

		*(--ptr) = '\0';
		return ptr;
	}
}

const char *spellnum_alloc(number n)
{
	if (n == 0) {
		return zero;
	} else {
		size_t len = spellnum_len(n);
		char *ptr = malloc(len + 1);
		if (ptr) {
			spellnum(n, ptr);
			assert(strlen(ptr) == len);
		}
		return ptr;
	}
}

void spellnum_free(const char *ptr)
{
	if (ptr != zero)
		free((char*)ptr);
}

#ifdef SPELLNUM_STANDALONE

int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "Usage: %s NUMBER\n", argv[0]);
		return 255;
	}

	number n = (number)atoll(argv[1]);
	const char *str = spellnum_alloc(n);
	printf("%s\n", str);
	spellnum_free(str);
	return 0;
}

#endif
