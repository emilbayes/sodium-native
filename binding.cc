#include <node.h>
#include <node_buffer.h>
#include <nan.h>
#include "deps/libsodium/src/libsodium/include/sodium.h"

using namespace node;
using namespace v8;

// As per Libsodium install docs
#define SODIUM_STATIC

#define CDATA(buf) (unsigned char *) node::Buffer::Data(buf)
#define CLENGTH(buf) (unsigned long long) node::Buffer::Length(buf)
#define LOCAL_STRING(str) Nan::New<String>(str).ToLocalChecked()
#define LOCAL_FUNCTION(fn) Nan::GetFunction(Nan::New<FunctionTemplate>(fn)).ToLocalChecked()
#define EXPORT_NUMBER(name) Nan::Set(target, LOCAL_STRING(#name), Nan::New<Number>(name));
#define EXPORT_STRING(name) Nan::Set(target, LOCAL_STRING(#name), LOCAL_STRING(name));
#define EXPORT_FUNCTION(name) Nan::Set(target, LOCAL_STRING(#name), LOCAL_FUNCTION(name));

// crypto_sign.c

NAN_METHOD(crypto_sign_seed_keypair) {
  Local<Object> public_key = info[0]->ToObject();
  Local<Object> secret_key = info[1]->ToObject();
  Local<Object> seed = info[2]->ToObject();

  int ret = crypto_sign_seed_keypair(CDATA(public_key), CDATA(secret_key), CDATA(seed));
  info.GetReturnValue().Set(Nan::New(ret));
}

NAN_METHOD(crypto_sign_keypair) {
  Local<Object> public_key = info[0]->ToObject();
  Local<Object> secret_key = info[1]->ToObject();

  int ret = crypto_sign_keypair(CDATA(public_key), CDATA(secret_key));
  info.GetReturnValue().Set(Nan::New(ret));
}

NAN_METHOD(crypto_sign) {
  Local<Object> signed_message = info[0]->ToObject();
  Local<Object> message = info[1]->ToObject();
  Local<Object> secret_key = info[2]->ToObject();

  unsigned long long signed_message_length;

  int ret = crypto_sign(CDATA(signed_message), &signed_message_length, CDATA(message), CLENGTH(message), CDATA(secret_key));
  info.GetReturnValue().Set(Nan::New(ret));
}

NAN_METHOD(crypto_sign_open) {
  Local<Object> message = info[0]->ToObject();
  Local<Object> signed_message = info[1]->ToObject();
  Local<Object> public_key = info[2]->ToObject();

  unsigned long long message_length;

  int ret = crypto_sign_open(CDATA(message), &message_length, CDATA(signed_message), CLENGTH(signed_message), CDATA(public_key));
  info.GetReturnValue().Set(Nan::New(ret));
}

NAN_METHOD(crypto_sign_detached) {
  Local<Object> signature = info[0]->ToObject();
  Local<Object> message = info[1]->ToObject();
  Local<Object> secret_key = info[2]->ToObject();

  unsigned long long signature_length;

  int ret = crypto_sign_detached(CDATA(signature), &signature_length, CDATA(message), CLENGTH(message), CDATA(secret_key));
  info.GetReturnValue().Set(Nan::New(ret));
}

NAN_METHOD(crypto_sign_verify_detached) {
  Local<Object> signature = info[0]->ToObject();
  Local<Object> message = info[1]->ToObject();
  Local<Object> public_key = info[2]->ToObject();

  int ret = crypto_sign_verify_detached(CDATA(signature), CDATA(message), CLENGTH(message), CDATA(public_key));
  info.GetReturnValue().Set(Nan::New(ret));
}

// crypto_generichash.c

NAN_METHOD(crypto_generichash_statebytes) {
  int ret = crypto_generichash_statebytes();
  info.GetReturnValue().Set(Nan::New(ret));
}

NAN_METHOD(crypto_generichash) {
  Local<Object> out = info[0]->ToObject();
  Local<Object> in = info[1]->ToObject();

  unsigned char *key = NULL;
  unsigned long long key_len = 0;

  if (info[2]->IsObject()) {
    key = CDATA(info[2]);
    key_len = CLENGTH(info[2]);
  }

  int ret = crypto_generichash(CDATA(out), CLENGTH(out), CDATA(in), CLENGTH(in), key, key_len);
  info.GetReturnValue().Set(Nan::New(ret));
}

NAN_MODULE_INIT(InitAll) {
  if (sodium_init() == -1) return Nan::ThrowError("sodium_init() failed");

  // crypto_sign

  EXPORT_NUMBER(crypto_sign_SEEDBYTES)
  EXPORT_NUMBER(crypto_sign_PUBLICKEYBYTES)
  EXPORT_NUMBER(crypto_sign_SECRETKEYBYTES)
  EXPORT_NUMBER(crypto_sign_BYTES)

  EXPORT_FUNCTION(crypto_sign_seed_keypair)
  EXPORT_FUNCTION(crypto_sign_keypair)
  EXPORT_FUNCTION(crypto_sign)
  EXPORT_FUNCTION(crypto_sign_open)
  EXPORT_FUNCTION(crypto_sign_detached)
  EXPORT_FUNCTION(crypto_sign_verify_detached)

<<<<<<< HEAD
  // crypto_generic_hash

  EXPORT_STRING(crypto_generichash_PRIMITIVE)

=======
  EXPORT_NUMBER(crypto_generichash_BYTES_MIN)
  EXPORT_NUMBER(crypto_generichash_BYTES_MAX)
  EXPORT_NUMBER(crypto_generichash_BYTES)
  EXPORT_NUMBER(crypto_generichash_KEYBYTES_MIN)
  EXPORT_NUMBER(crypto_generichash_KEYBYTES_MAX)
  EXPORT_NUMBER(crypto_generichash_KEYBYTES)
  EXPORT_STRING(crypto_generichash_PRIMITIVE)

  EXPORT_FUNCTION(crypto_generichash)

>>>>>>> Incomplete crypto_generichash bindings
  #undef EXPORT_FUNCTION
  #undef EXPORT_NUMBER
  #undef EXPORT_STRING
  #undef LOCAL_FUNCTION
  #undef LOCAL_STRING
  #undef CDATA
  #undef CLENGTH
}

NODE_MODULE(sodium, InitAll)
