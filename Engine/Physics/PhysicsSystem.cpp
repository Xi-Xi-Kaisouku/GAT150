#include "pch.h"
#include "PhysicsSystem.h"
#include "ContactListener.h"

bool nc::PhysicsSystem::Startup()
{
    b2Vec2 gravity{ 0, 10 };
    m_world = new b2World{ gravity };

    m_contatctListener = new ContactListener;
    m_world->SetContactListener(m_contatctListener);

    return true;
}

void nc::PhysicsSystem::Shutdown()
{
    delete m_world;
    m_world = nullptr;
}

void nc::PhysicsSystem::Update()
{
    float timeStep{ 1.0f / 60.0f };
    m_world->Step(timeStep, 8, 3);
}

b2Body* nc::PhysicsSystem::CreateBody(const Vector2& position, const Vector2& size, float density, bool isDynamic)
{
    b2BodyDef bodyDef;
    bodyDef.type = (isDynamic) ? b2_dynamicBody : b2_staticBody;
    bodyDef.position.Set(position.x, position.y);
    b2Body* body = m_world->CreateBody(&bodyDef);

    b2PolygonShape shape;
    shape.SetAsBox(size.x, size.y);

    body->CreateFixture(&shape, density);

    return body;
}

b2Body* nc::PhysicsSystem::CreateBody(const Vector2& position, float angle, const RigidBodyData& data, GameObject* gameObject)
{
    b2BodyDef bodyDef;

    Vector2 world = PhysicsSystem::ScreenToWorld(position);

    bodyDef.type = (data.isDynamic) ? b2_dynamicBody : b2_staticBody;
    bodyDef.position.Set(world.x, world.y);
    bodyDef.angle = nc::DegreesToRadians(angle);
    bodyDef.fixedRotation = data.lockAngle;
    b2Body* body = m_world->CreateBody(&bodyDef);

    b2PolygonShape shape;
    Vector2 worldSize = PhysicsSystem::ScreenToWorld(data.size);
    shape.SetAsBox(worldSize.x, worldSize.y);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = data.density;
    fixtureDef.friction = data.friction;
    fixtureDef.restitution = data.restitution;
    fixtureDef.isSensor = data.isSensor;
    fixtureDef.userData = gameObject;

    body->CreateFixture(&fixtureDef);

    return body;
}

void nc::PhysicsSystem::DestroyBody(b2Body* body)
{
    m_world->DestroyBody(body);
}
