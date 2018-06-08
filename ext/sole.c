#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <ruby.h>
#include <ruby/encoding.h>
#include "sole.h"

/**
 *  Encodes a single character into a C string.
 */
void sole_encode(char *result, const unsigned char c)
{
  result[0] = ALPHA(c/0x10);
  result[1] = ALPHA(c);
}

/**
 *  Encodes characters into a C string.
 */
void sole_encode_update(char *result, size_t size, const char *c)
{
  for (size_t i = 0; i < size; i++, result += 2)
    sole_encode(result, c[i]);
}

/**
 *  Encodes characters into a Ruby string.
 */
VALUE sole_encode_update_rb(VALUE string, size_t size, const char *c)
{
  size_t encoded_length = SOLE_ENCODE_LENGTH(size);
  char *encoded = malloc(encoded_length);

  sole_encode_update(encoded, size, c);
  string = rb_str_buf_cat(string, encoded, encoded_length);
  free(encoded);

  return string;
}

/**
 * Initializes the decoder state.
 */
void sole_decode_init(struct sole_decode_ctx *ctx)
{
  ctx->word = 0;
  ctx->left = 0;
}

long sole_decode_long(char *in)
{
  return (decode_map[in[0]] << 4) | decode_map[in[1]];
}

/**
 * Decodes a single byte from the input. Returns number of decoded characters
 * or -1 in case of unexpected input.
 */
int sole_decode_single(struct sole_decode_ctx *ctx, char *out, uint8_t in)
{
  int digit;
  if (in >= 0x80) return -1;
  digit = decode_map[in];
  switch (digit)
  {
    case -1:
      return -1;
    case -2:
      return 0;
    default:
      assert(digit >= 0);
      assert(digit < 0x10);
      if (ctx->left)
      {
        *out = (ctx->word << 4) | digit;
        ctx->left = 0;
        return 1;
      } else {
        ctx->word = digit;
        ctx->left = 1;
        return 0;
      }
  }
}

/**
 * Decodes a character array of input. Returns 0 in case of failure and 1 in
 * case of success.
 */
int sole_decode_update(
  struct sole_decode_ctx *ctx,
  size_t *out_length,
  char *out,
  size_t in_length,
  const char *in
)
{
  size_t decoded_length;
  size_t i;

  for (i = decoded_length = 0; i < in_length; i++) {
    switch(sole_decode_single(ctx, out + decoded_length, in[i]))
    {
      case -1:
        return 0;
      case 1:
        decoded_length++;
      case 0:
        break;
      default:
        abort();
    }
  }

  assert(decoded_length <= SOLE_ENCODE_LENGTH(in_length));

  *out_length = decoded_length;
  return 1;
}

/**
 * Set context to a final state regardless of it current state.
 */
int sole_decode_finalize(struct sole_decode_ctx *ctx)
{
  return ctx->left == 0;
}

VALUE sole_decode_rb(VALUE string)
{
  VALUE unpacked;
  struct sole_decode_ctx ctx;
  size_t decoded_length;
  size_t encoded_length = RSTRING_LEN(string);
  char *decoded = malloc(SOLE_DECODE_LENGTH(encoded_length));

  sole_decode_init(&ctx);
  sole_decode_update(&ctx, &decoded_length, decoded, RSTRING_LEN(string), StringValueCStr(string));
  sole_decode_finalize(&ctx);

  unpacked = rb_str_new(decoded, decoded_length);
  free(decoded);
  rb_enc_associate(unpacked, rb_enc_find("UTF-8"));
  return unpacked;
}

/*
 *  call-seq:
 *    Sole.generate
 *
 *  Generates a random string with a limited alphabet.
 *
 */
static VALUE sole_generate(VALUE self)
{
  time_t data = time(NULL) - SOLE_EPOCH;
  uint64_t rdata;

  static char result[16] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

  result[0] = ALPHA(data);
  result[1] = ALPHA(data >> 0x4);
  result[2] = ALPHA(data >> 0x8);
  result[3] = ALPHA(data >> 0x12);
  result[4] = ALPHA(data >> 0x16);
  result[5] = ALPHA(data >> 0x20);
  result[6] = ALPHA(data >> 0x28);
  result[7] = ALPHA(data >> 0x32);

  arc4random_buf(&rdata, 8);

  result[8] = alphabet[rdata & 0xf];
  result[9] =  ALPHA(rdata >> 0x4);
  result[10] = ALPHA(rdata >> 0x8);
  result[11] = ALPHA(rdata >> 0x12);
  result[12] = ALPHA(rdata >> 0x16);
  result[13] = ALPHA(rdata >> 0x20);
  result[14] = ALPHA(rdata >> 0x28);
  result[15] = ALPHA(rdata >> 0x32);

  return rb_str_new(result, 16);
}

/*
 *  call-seq:
 *    Sole.unpack(string)
 *
 *  Returns Sole data with the data packed into the string.
 *
 *    Sole.pack(43, 'Bob') #=> "X4W4FCCCPCWGJH2MJGP9P"
 */
static VALUE sole_pack(int argc, VALUE* argv, VALUE self)
{
  VALUE args;
  VALUE string;
  VALUE result;
  rb_scan_args(argc, argv, "*", &args);

  /* Start by generating the random component. */
  result = sole_generate(self);

  /* Iterate over all arguments given to the method. */
  for (int i = 0; i < argc; i++) {
    string = rb_funcall(rb_ary_entry(args, i), rb_intern("to_s"), 0, Qnil);

    /* Encode and add the current argument. */
    result = sole_encode_update_rb(result, RSTRING_LEN(string), StringValuePtr(string));

    /* Add a separator unless this is the last value in the array */
    if (i != (argc - 1)) {
      result = sole_encode_update_rb(result, 1, sole_separator);
    }
  }

  return result;
}

/*
 *  call-seq:
 *    Sole.pack(string)
 *
 *  Returns metadata stored in the Sole data.
 *
 *    Sole.unpack("2GM4FCCCMQ4MXGPM") #=> ['43', 'Bob']
 */
static VALUE sole_unpack(VALUE self, VALUE string)
{
  char *t_data;
  time_t time = SOLE_EPOCH;
  size_t encoded_length = RSTRING_LEN(string);
  char *decoded = malloc(SOLE_DECODE_LENGTH(encoded_length));

  VALUE r_packed_time = rb_str_substr(string, 0, 8);
  char *packed_time = StringValuePtr(r_packed_time);

  for (int i = 0; i < 4; i++) {
    time += sole_decode_long(packed_time += 2);
  }

  char buffer[32];
  strftime(buffer, 32, "%a, %d.%m.%Y %H:%M:%S", localtime(&time));
  printf("TIME: %s\n", buffer);

  VALUE record = sole_decode_rb(rb_str_substr(string, 16, encoded_length));
  VALUE unpacked = rb_str_split(record, sole_separator);
  rb_funcall(unpacked, rb_intern("unshift"), 1, rb_time_new(time, 0));

  return unpacked;
}

void Init_sole()
{ 
  VALUE cSole;
  cSole = rb_define_class("Sole", rb_cObject);
  rb_define_singleton_method(cSole, "generate", sole_generate, 0);
  rb_define_singleton_method(cSole, "pack", sole_pack, -1);
  rb_define_singleton_method(cSole, "unpack", sole_unpack, 1);
}
