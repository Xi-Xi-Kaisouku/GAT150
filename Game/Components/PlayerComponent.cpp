#include "pch.h"
#include "PlayerComponent.h"
#include "Components/PhysicsComponent.h"
#include "Objects/GameObject.h"

bool nc::PlayerComponent::Create(void* data)
{
    m_owner = static_cast<GameObject*>(data);
    return true;
}

void nc::PlayerComponent::Destroy()
{

}

void nc::PlayerComponent::Update()
{
    if (m_owner->m_engine->GetSystem<nc::InputSystem>()->GetButtonState(SDL_SCANCODE_LEFT) == nc::InputSystem::eButtonState::HELD)
    {
        m_owner->m_transform.angle = m_owner->m_transform.angle - 200.0f * m_owner->m_engine->GetTimer().DeltaTime();
    }
    if (m_owner->m_engine->GetSystem<nc::InputSystem>()->GetButtonState(SDL_SCANCODE_RIGHT) == nc::InputSystem::eButtonState::HELD)
    {
        m_owner->m_transform.angle = m_owner->m_transform.angle + 200.0f * m_owner->m_engine->GetTimer().DeltaTime();
    }

    //set force
    nc::Vector2 force{ 0, 0 };
    if (m_owner->m_engine->GetSystem<nc::InputSystem>()->GetButtonState(SDL_SCANCODE_UP) == nc::InputSystem::eButtonState::HELD)
    {
        force = nc::Vector2::forward * 1000.0f;
    }
    force = nc::Vector2::Rotate(force, nc::DegreesToRadians(m_owner->m_transform.angle));

    //apply force
    PhysicsComponent* component = m_owner->GetComponent<PhysicsComponent>();
    if (component)
    {
        component->ApplyForce(force);
    }
}
