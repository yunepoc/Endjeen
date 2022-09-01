#include <Script.hpp>

#include <App.hpp>
#include "duktape.h"
#include <Game/GameContent.hpp>
#include <Resource.hpp>

namespace ej {

static duk_ret_t native_resource_add(duk_context *ctx) {
  GameContent& content = App::instance().getGameContent();
  if (!duk_is_string(ctx, 0) || !content.resourceHas(duk_to_string(ctx, 0))) {
    WARN("Invalid resource name for native resource_add");
    return 0;
  }
  if (!duk_is_number(ctx, 1)) {
    WARN("Invalid resource value for native resource_add");
    return 0;
  }
  std::string name = duk_to_string(ctx, 0);
  int value = duk_to_int32(ctx, 1);
  if (value < 0)
    content.resourceSub(name, -value);
  else
    content.resourceAdd(name, value);
  return 0;
}

static duk_ret_t native_print(duk_context *ctx) {
  printf("%s\n", duk_to_string(ctx, 0));
  return 0;  /* no return value (= undefined) */
}

void Script::createScript(std::string src, ResScript &script) {
  duk_context* ctx = duk_create_heap_default();
  script.source = src;
  script.handle = static_cast<void*>(ctx);
}

void Script::load(ResScript& script) {
  duk_context* ctx = static_cast<duk_context*>(script.handle);
  // Setup std lib functions
  duk_push_c_function(ctx, native_print, 1 /*nargs*/);
  duk_put_global_string(ctx, "print");
  duk_push_c_function(ctx, native_resource_add, 2 /*nargs*/);
  duk_put_global_string(ctx, "resource_add");
  // Eval script
  duk_eval_string(ctx, script.source.c_str());
  // Call load function
  duk_get_global_string(ctx, "load");
  duk_call(ctx, 0);
  duk_pop(ctx);
}

void Script::update(ResScript& script, double delta) {
  duk_context* ctx = static_cast<duk_context*>(script.handle);
  // Call update function
  duk_get_global_string(ctx, "update");
  duk_push_number(ctx, delta);
  duk_call(ctx, 1);
  duk_pop(ctx);
}

}
