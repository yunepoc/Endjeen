#include <Physics.hpp>

#include <App.hpp>
#include <btBulletDynamicsCommon.h>
#include <Debug.hpp>
#include <Renderer.hpp>
#include <Resource.hpp>

namespace ej {

class PhysicsDebugDraw: public btIDebugDraw {
    public:
        PhysicsDebugDraw();
        virtual void draw3dText(const btVector3& location,const char* textString);
        virtual void drawLine(const btVector3& from,const btVector3& to,const btVector3& color);
        virtual void drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color);
        virtual void reportErrorWarning(const char* warningString);
        virtual void setDebugMode(int debugMode);
        virtual int getDebugMode() const;
        void begin();
        void end();
        void render();
    private:
        std::vector<float> meshData;
        Material material;
        ResRenderable renderable;
};

PhysicsDebugDraw::PhysicsDebugDraw() {
  ResShader &shader = App::instance().getResourceMgr().get<ResShader>("default.shader");
  material.setShader(shader);
}

void PhysicsDebugDraw::draw3dText(const btVector3& location,const char* textString) {}

void PhysicsDebugDraw::drawLine(const btVector3& from,const btVector3& to,const btVector3& color) {
  meshData.push_back(from.getX());
  meshData.push_back(from.getY());
  meshData.push_back(from.getZ());
  meshData.push_back(to.getX());
  meshData.push_back(to.getY());
  meshData.push_back(to.getZ());
}

void PhysicsDebugDraw::drawContactPoint(
    const btVector3& PointOnB,
    const btVector3& normalOnB,
    btScalar distance,
    int lifeTime,
    const btVector3& color) {}

void PhysicsDebugDraw::reportErrorWarning(const char* warningString) {
  WARN(warningString);
}

void PhysicsDebugDraw::setDebugMode(int debugMode) {}
int  PhysicsDebugDraw::getDebugMode() const { return 3; }; // DBG_DrawWireframe | DBG_DrawAabb

void PhysicsDebugDraw::begin() {
  meshData.clear();
  Renderer::deleteRenderable(renderable);
}

void PhysicsDebugDraw::end() {
  Renderer::createLinesRenderable(meshData, renderable);
}

void PhysicsDebugDraw::render() {
  Renderer& renderer = App::instance().getRenderer();
  Transform transform;
  renderer.render(renderable, material, transform);
}

// -----------------------------------------------------------------------------

struct Physics::Handle {
  Handle();
  btDefaultCollisionConfiguration collisionConfiguration;
  btCollisionDispatcher dispatcher;
  btDbvtBroadphase broadphase;
  btSequentialImpulseConstraintSolver solver;
  btDiscreteDynamicsWorld dynamicsWorld;
  PhysicsDebugDraw debugDraw;
};

// -----------------------------------------------------------------------------

PhysicsBox::~PhysicsBox() {
  // Id the physics box is not initialized, there is nothing to destroy
  if (!handle)
    return;
  btRigidBody* body = static_cast<btRigidBody*>(handle);
  App::instance().getPhysics().handle->dynamicsWorld.removeRigidBody(body);
  btCollisionShape* shape = body->getCollisionShape();
  btMotionState* motionState = body->getMotionState();
  delete motionState;
  delete shape;
  delete body;
}

void PhysicsBox::setPosition(glm::vec3 position) {
  btRigidBody* body = static_cast<btRigidBody*>(handle);
  btTransform transform;
  transform.setIdentity();
  transform.setOrigin({position.x, position.y, position.z});

  body->setWorldTransform(transform);
  body->getMotionState()->setWorldTransform(transform);
}

void* PhysicsBox::getUserData() {
  return userData;
}

void PhysicsBox::setUserData(void* data) {
  userData = data;
}

// -----------------------------------------------------------------------------

Physics::Handle::Handle():
  dispatcher(&collisionConfiguration), dynamicsWorld(&dispatcher, &broadphase, &solver, &collisionConfiguration) {}

Physics::~Physics() {
  delete handle;
}

void Physics::createPhysicsBox(PhysicsBox &box, glm::vec3 size) {
  btVector3 bsize(size.x / 2.0, size.y / 2.0, size.z/ 2.0);
  btCollisionShape* shape = new btBoxShape(bsize);
  btTransform transform;

  btVector3 localInertia(0, 0, 0);
  btDefaultMotionState* motionState = new btDefaultMotionState(transform);
  btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0, motionState, shape, localInertia);
  btRigidBody* body = new btRigidBody(rbInfo);

  body->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));
  body->setFriction(0.0);
  body->setActivationState(DISABLE_DEACTIVATION);
  body->setUserPointer(&box);
  handle->dynamicsWorld.addRigidBody(body, 1, 1);
  box.handle = static_cast<void*>(body);
}

std::vector<PhysicsBox*> Physics::currentColliders(PhysicsBox &box) {
  std::vector<PhysicsBox*> colliders;
  int numManifolds = handle->dispatcher.getNumManifolds();
  for (int i=0; i<numManifolds; i++) {
    btPersistentManifold* contactManifold = handle->dynamicsWorld.getDispatcher()->getManifoldByIndexInternal(i);
    if (contactManifold->getNumContacts() == 0)
      continue;
    btCollisionObject* obA = const_cast<btCollisionObject*>(contactManifold->getBody0());
    btCollisionObject* obB = const_cast<btCollisionObject*>(contactManifold->getBody1());
    PhysicsBox* pbA = static_cast<PhysicsBox*>(obA->getUserPointer());
    PhysicsBox* pbB = static_cast<PhysicsBox*>(obB->getUserPointer());
    if (pbA == &box)
      colliders.push_back(pbB);
    else if (pbB == &box)
      colliders.push_back(pbA);
  }
  return colliders;
}

void Physics::load() {
  handle = new Handle();
  handle->dynamicsWorld.setDebugDrawer(nullptr);
  App::instance().getUI().addDebugBoolean("Debug physics", &debug);
}

std::pair<PhysicsBox*, glm::vec3> Physics::raycast(glm::vec3 point, glm::vec3 direction) {
  glm::vec3 to = point + direction * 1000.0f;
  btCollisionWorld::ClosestRayResultCallback RayCallback(
    btVector3(point.x, point.y, point.z),
    btVector3(to.x, to.y, to.z)
  );
  handle->dynamicsWorld.rayTest(
    btVector3(point.x, point.y, point.z),
    btVector3(to.x, to.y, to.z),
    RayCallback
  );
  if(RayCallback.hasHit()) {
    PhysicsBox* pb = (PhysicsBox*)RayCallback.m_collisionObject->getUserPointer();
    if (!pb)
      return {nullptr, {0.0, 0.0, 0.0}};
    auto p = RayCallback.m_hitPointWorld;
    glm::vec3 point = { p.getX(), p.getY(), p.getZ() };
    return {pb, point};
  }
  return {nullptr, {0.0, 0.0, 0.0}};
}

void Physics::render() {
  handle->dynamicsWorld.setDebugDrawer(debug ? &handle->debugDraw : nullptr);
  if (debug)
    handle->debugDraw.render();
}

void Physics::shutdown() {}

void Physics::update(float delta) {
  handle->dynamicsWorld.stepSimulation(1/60.f);
  if (debug) {
    handle->debugDraw.begin();
    handle->dynamicsWorld.debugDrawWorld();
    handle->debugDraw.end();
  }
}

}
