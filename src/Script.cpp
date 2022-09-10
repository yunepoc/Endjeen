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

static duk_ret_t native_pointer_is_building(duk_context *ctx) {
  GameObject* obj = static_cast<GameObject*>(duk_to_pointer(ctx, 0));
  if (!obj) {
    duk_push_false(ctx);
    return 1;
  }

  Building* building = dynamic_cast<Building*>(obj);
  if (building)
    duk_push_true(ctx);
  else
    duk_push_false(ctx);
  return 1;
}

static duk_ret_t native_pointer_is_worker(duk_context *ctx) {
  GameObject* obj = static_cast<GameObject*>(duk_to_pointer(ctx, 0));
  if (!obj) {
    duk_push_false(ctx);
    return 1;
  }

  Worker* worker = dynamic_cast<Worker*>(obj);
  if (worker)
    duk_push_true(ctx);
  else
    duk_push_false(ctx);
  return 1;
}

static duk_ret_t native_building_get_closest(duk_context *ctx) {
  Building* building = static_cast<Building*>(duk_to_pointer(ctx, 0));
  std::string name = duk_to_string(ctx, 1);
  Building* closest = building->closest(name);
  duk_push_pointer(ctx, closest);
  return 1;
}

static duk_ret_t native_worker_get_done(duk_context *ctx) {
  Worker* worker = static_cast<Worker*>(duk_to_pointer(ctx, 0));
  duk_push_boolean(ctx, worker->getDone());
  return 1;
}

static duk_ret_t native_worker_get_available(duk_context *ctx) {
  std::string name = duk_to_string(ctx, 0);
  Worker *worker = App::instance().getGame().getAvailableWorker(name);
  duk_push_pointer(ctx, worker);
  return 1;
}

static duk_ret_t native_worker_get_building(duk_context *ctx) {
  Worker* worker = static_cast<Worker*>(duk_to_pointer(ctx, 0));
  duk_push_pointer(ctx, worker->getBuilding());
  return 1;
}

static duk_ret_t native_worker_is_available(duk_context *ctx) {
  Worker* worker = static_cast<Worker*>(duk_to_pointer(ctx, 0));
  duk_push_boolean(ctx, worker->isAvailable());
  return 1;
}

static duk_ret_t native_worker_has_target(duk_context *ctx) {
  Worker* worker = static_cast<Worker*>(duk_to_pointer(ctx, 0));
  duk_push_boolean(ctx, worker->hasTarget());
  return 1;
}

static duk_ret_t native_worker_set_available(duk_context *ctx) {
  Worker* worker = static_cast<Worker*>(duk_to_pointer(ctx, 0));
  bool available = duk_to_boolean(ctx, 1);
  worker->setAvailable(available);
  return 0;
}

static duk_ret_t native_worker_set_building(duk_context *ctx) {
  Worker* worker = static_cast<Worker*>(duk_to_pointer(ctx, 0));
  Building* building = static_cast<Building*>(duk_to_pointer(ctx, 1));
  worker->setBuilding(building);
  return 0;
}

static duk_ret_t native_worker_set_done(duk_context *ctx) {
  Worker* worker = static_cast<Worker*>(duk_to_pointer(ctx, 0));
  bool done = duk_to_boolean(ctx, 1);
  worker->setDone(done);
  return 0;
}

static duk_ret_t native_worker_set_target(duk_context *ctx) {
  Worker* worker = static_cast<Worker*>(duk_to_pointer(ctx, 0));
  Building* building = static_cast<Building*>(duk_to_pointer(ctx, 1));
  worker->setTarget(building);
  return 0;
}

static duk_ret_t native_worker_set_visible(duk_context *ctx) {
  Worker* worker = static_cast<Worker*>(duk_to_pointer(ctx, 0));
  bool visible = duk_to_boolean(ctx, 1);
  worker->setVisible(visible);
  return 0;
}

void Script::createScript(std::string src, ResScript &script) {
  script.source = src;
  script.handle = nullptr;
}

void Script::load(ResScript& script, GameObject &object) {
  duk_context* ctx = duk_create_heap_default();
  script.handle = static_cast<void*>(ctx);
  // Setup std lib functions
  duk_push_c_function(ctx, native_print, 1);
  duk_put_global_string(ctx, "print");
  duk_push_c_function(ctx, native_resource_add, 2);
  duk_put_global_string(ctx, "_resource_add");
  duk_push_c_function(ctx, native_building_get_closest, 2);
  duk_put_global_string(ctx, "_building_get_closest");
  duk_push_c_function(ctx, native_pointer_is_building, 1);
  duk_put_global_string(ctx, "_pointer_is_building");
  duk_push_c_function(ctx, native_pointer_is_worker, 1);
  duk_put_global_string(ctx, "_pointer_is_worker");
  duk_push_c_function(ctx, native_worker_get_done, 1);
  duk_put_global_string(ctx, "_worker_get_done");
  duk_push_c_function(ctx, native_worker_get_available, 1);
  duk_put_global_string(ctx, "_worker_get_available");
  duk_push_c_function(ctx, native_worker_get_building, 1);
  duk_put_global_string(ctx, "_worker_get_building");
  duk_push_c_function(ctx, native_worker_has_target, 1);
  duk_put_global_string(ctx, "_worker_has_target");
  duk_push_c_function(ctx, native_worker_is_available, 1);
  duk_put_global_string(ctx, "_worker_is_available");
  duk_push_c_function(ctx, native_worker_set_available, 2);
  duk_put_global_string(ctx, "_worker_set_available");
  duk_push_c_function(ctx, native_worker_set_building, 2);
  duk_put_global_string(ctx, "_worker_set_building");
  duk_push_c_function(ctx, native_worker_set_done, 2);
  duk_put_global_string(ctx, "_worker_set_done");
  duk_push_c_function(ctx, native_worker_set_target, 2);
  duk_put_global_string(ctx, "_worker_set_target");
  duk_push_c_function(ctx, native_worker_set_visible, 2);
  duk_put_global_string(ctx, "_worker_set_visible");
  // Eval script
  if (duk_peval_string(ctx, script.source.c_str()) != DUK_EXEC_SUCCESS) {
    std::string error = duk_safe_to_string(ctx, -1);
    WARN("Script execution failed: " << error);
  }
  // Get load function
  duk_get_global_string(ctx, "load");
  // Create JS game object
  duk_get_global_string(ctx, "_createGameObject");
  duk_push_pointer(ctx, &object);
  if (duk_pcall(ctx, 1) != DUK_EXEC_SUCCESS) {
    std::string error = duk_safe_to_string(ctx, -1);
    WARN("Script execution failed: " << error);
  }
  // Call load function
  if (duk_pcall(ctx, 1) != DUK_EXEC_SUCCESS) {
    std::string error = duk_safe_to_string(ctx, -1);
    WARN("Script execution failed: " << error);
  }
  duk_pop(ctx);
}

void Script::update(ResScript& script, double delta) {
  duk_context* ctx = static_cast<duk_context*>(script.handle);
  // Call update function
  duk_get_global_string(ctx, "update");
  duk_push_number(ctx, delta);
  if (duk_pcall(ctx, 1) != DUK_EXEC_SUCCESS) {
    std::string error = duk_safe_to_string(ctx, -1);
    WARN("Script execution failed: " << error);
  }
  duk_pop(ctx);
}

}
