#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef long long int number;

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
		strcpy(ptr + len, " hundred ");
		return str_99(n % 100, ptr+len+9);
	}
}

#ifdef SPELLNUM_STANDALONE

int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "Usage: %s NUMBER\n", argv[0]);
		return 255;
	}

	number n = (number)atoll(argv[1]);
	char *buf = malloc(len_999(n));
	char *end = str_999(n, buf);
	*(end-1) = '\0';
	printf("%s\n", buf);
	free(buf);
	return 0;
}

#endif