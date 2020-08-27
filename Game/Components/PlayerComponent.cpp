#include "pch.h"
#include "PlayerComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Objects/GameObject.h"
#include "Components/AudioComponent.h"

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
    auto contacts = m_owner->GetContactsWithTag("Floor");
    bool onGround = !contacts.empty();

    nc::Vector2 force{ 0, 0 };
    if (m_owner->m_engine->GetSystem<nc::InputSystem>()->GetButtonState(SDL_SCANCODE_LEFT) == nc::InputSystem::eButtonState::HELD)
    {
        force.x = -100;
    }
    if (m_owner->m_engine->GetSystem<nc::InputSystem>()->GetButtonState(SDL_SCANCODE_RIGHT) == nc::InputSystem::eButtonState::HELD)
    {
        force.x = 100;
    }

    if (onGround && m_owner->m_engine->GetSystem<nc::InputSystem>()->GetButtonState(SDL_SCANCODE_SPACE) == nc::InputSystem::eButtonState::HELD)
    {
        force.y = -600;
        AudioComponent* audioComponent = m_owner->GetComponent<AudioComponent>();
        if (audioComponent)
        {
            audioComponent->Play();
        }

    }

    //apply force
    RigidBodyComponent* component = m_owner->GetComponent<RigidBodyComponent>();
    if (component)
    {
        component->ApplyForce(force);
    }

    //check for coin contact
    auto coinContacts = m_owner->GetContactsWithTag("Coin");
    for (GameObject* contact : coinContacts)
    {
        contact->m_flags[GameObject::eFlags::DESTROY] = true;
    }

}
