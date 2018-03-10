#include <stdlib.h>
#include <time.h>
#include <ruby.h>
#include "sole.h"

static VALUE sole_generate(VALUE self)
{
  time_t data = time(NULL) - 1520680705;
  uint64_t rdata;

  static char result[16] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

  result[0] = alphabet[data & 0xf];
  result[1] = alphabet[(data >> 0x4) & 0xf];
  result[2] = alphabet[(data >> 0x8) & 0xf];
  result[3] = alphabet[(data >> 0x12) & 0xf];
  result[4] = alphabet[(data >> 0x16) & 0xf];
  result[5] = alphabet[(data >> 0x20) & 0xf];
  result[6] = alphabet[(data >> 0x28) & 0xf];
  result[7] = alphabet[(data >> 0x32) & 0xf];

  arc4random_buf(&rdata, 8);

  result[8] = alphabet[rdata & 0xf];
  result[9] = alphabet[(rdata >> 0x4) & 0xf];
  result[10] = alphabet[(rdata >> 0x8) & 0xf];
  result[11] = alphabet[(rdata >> 0x12) & 0xf];
  result[12] = alphabet[(rdata >> 0x16) & 0xf];
  result[13] = alphabet[(rdata >> 0x20) & 0xf];
  result[14] = alphabet[(rdata >> 0x28) & 0xf];
  result[15] = alphabet[(rdata >> 0x32) & 0xf];

  return rb_str_new(result, 16);
}

void Init_sole()
{ 
  VALUE cSole;
  cSole = rb_define_class("Sole", rb_cObject);
  rb_define_singleton_method(cSole, "generate", sole_generate, 0);
}
