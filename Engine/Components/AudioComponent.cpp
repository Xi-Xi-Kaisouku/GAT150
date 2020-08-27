#include "pch.h"
#include "AudioComponent.h"
#include "Audio/Sound.h"
#include "Audio/AudioSystem.h"

bool nc::AudioComponent::Create(void* data)
{
	m_owner = static_cast<GameObject*>(data);
	return true;
}

void nc::AudioComponent::Destroy()
{
	m_channel.Stop();
}

void nc::AudioComponent::Read(const rapidjson::Value& value)
{
	json::Get(value, "sound", m_soundName);
	json::Get(value, "loop", m_loop);
	json::Get(value, "playOnAwake", m_playOnAwake);

	if (m_playOnAwake)
	{
		Play();
	}
}

void nc::AudioComponent::Update()
{

}

void nc::AudioComponent::Play()
{
	m_channel.Stop();

	Sound* sound = m_owner->m_engine->GetSystem<nc::ResourceManager>()->Get<nc::Sound>(m_soundName, m_owner->m_engine->GetSystem<nc::AudioSystem>());
	ASSERT_MSG((sound), "Could not find sound: " + m_soundName);
	m_channel = sound->Play(m_loop);

}

void nc::AudioComponent::Stop()
{
	m_channel.Stop();
}
