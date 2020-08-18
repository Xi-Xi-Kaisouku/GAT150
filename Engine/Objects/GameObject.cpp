#include "pch.h"
#include "GameObject.h"
#include "Components/Component.h"
#include "Components/RenderComponent.h"

bool nc::GameObject::Create(void* data)
{
    m_engine = static_cast<Engine*>(data);

    return false;
}

void nc::GameObject::Destroy()
{
    RemoveAllComponents();
}

void nc::GameObject::Read(const rapidjson::Value& value)
{
    nc::json::Get(value, "position", m_transform.position);
    nc::json::Get(value, "scale", m_transform.scale);
    nc::json::Get(value, "angle", m_transform.angle);
}

void nc::GameObject::Update()
{
    for (auto component : m_components)
    {
        component->Update();
    }
}

void nc::GameObject::Draw()
{
    RenderComponent* component = GetComponent<RenderComponent>();
    if (component)
    {
        component->Draw();
    }
}

void nc::GameObject::AddComponent(Component* component)
{
    m_components.push_back(component);
}

void nc::GameObject::RemoveComponent(Component* component)
{
    auto iter = std::find(m_components.begin(), m_components.end(), component);
    if (iter != m_components.end())
    {
        (*iter)->Destroy();
        delete (*iter);
    }
}

void nc::GameObject::RemoveAllComponents()
{
    for (auto component : m_components)
    {
        component->Destroy();
        delete component;
    }
    m_components.clear();
}

