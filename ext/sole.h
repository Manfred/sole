#ifndef SOLE_H
#define SOLE_H

/**
 * We have 128 bits to work with.
 */
typedef __uint128_t SOLE;

/**
 * Restrict the alphabet to characters which are in ASCII and easily
 * distinguishable.
 */
static const char alphabet[16] = "CFGHJMPQRVWX2479";

#endif