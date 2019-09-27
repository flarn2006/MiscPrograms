#ifndef __SPELLNUM_H__
#define __SPELLNUM_H__

typedef long long int number;

size_t spellnum_len(number n);
char *spellnum(number n, char *ptr);
const char *spellnum_alloc(number n);
void spellnum_free(const char *ptr);

#endif
