#include <signal.h>
#include <ruby.h>

static VALUE sole_bytes(VALUE self)
{
  return Qnil;
}

void Init_sole()
{
  VALUE cSole;
  cSole = rb_define_class("Sole", rb_cObject);
  rb_define_singleton_method(cSole, "bytes", sole_bytes, 0);
}
