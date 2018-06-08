#ifndef SOLE_H
#define SOLE_H

/**
 * Restrict the alphabet to characters which are in ASCII and easily
 * distinguishable to allow developers to easily differentiate them
 * while debugging.
 */
static const char alphabet[16] = "CFGHJMPQRVWX2479";

/**
 * Maps ASCII values to an alphabet index. An index of -1 indicates and invalid
 * input character and -2 indicates a character which may be ignored (i.e.
 * whitespace).
 */
static const signed char decode_map[0x80] = {
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -2, -2, -1, -1, -2, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, 12, -1, 13, -1, -1, 14, -1, 15, -1, -1, -1, -1, -1, -1,
  -1, -1, -1,  0, -1, -1,  1,  2,  3, -1,  4, -1, -1,  5, -1, -1,
   6,  7,  8, -1, -1, -1,  9, 10, 11, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1,  0, -1, -1,  1,  2,  3, -1,  4, -1, -1,  5, -1, -1,
   6,  7,  8, -1, -1, -1,  9, 10, 11, -1, -1, -1, -1, -1, -1, -1,
};

/**
 * Packed values have to be separated so we chose a character this is not
 * likely used in data. In this case the 'record separator' from ASCII.
 */
static const char *sole_separator = "\x1e";

/**
 * Keeps the state of the decoder.
 */
struct sole_decode_ctx
{
  char word; /* Leftover data from previous byte */
  char left; /* 1 when the word still has usable data and 0 otherwise */
};

/**
 * In order to reduce storage requirements for the time we start our EPOCH
 * at an unusual date.
 */
#define SOLE_EPOCH 1520680705

/**
 * Convert an unsigned char value to its Base16 encoded alternative.
 */
#define ALPHA(x) (alphabet[(x) & 0xf])

/**
 * Length of the character data after encoding.
 */
#define SOLE_ENCODE_LENGTH(length) ((length) * 2)

/**
 * Length of the character data after decoding.
 */
#define SOLE_DECODE_LENGTH(length) ((length) / 2)

#endif
