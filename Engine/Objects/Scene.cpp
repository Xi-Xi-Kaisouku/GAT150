#include "pch.h"
#include "Scene.h"
#include "GameObject.h"
#include "ObjectFactory.h"


bool nc::Scene::Create(void* data)
{
    m_engine = static_cast<Engine*>(data);
	return true;
}

void nc::Scene::Destroy()
{
	RemoveAllGameObjects();
}

void nc::Scene::Read(const rapidjson::Value& value)
{
	if (value.HasMember("Prototypes"))
	{
		const rapidjson::Value& objectsValue = value["Prototypes"];
		if (objectsValue.IsArray())
		{
			ReadPrototypes(objectsValue);
		}
	}

	if (value.HasMember("GameObjects"))
	{
		const rapidjson::Value& objectsValue = value["GameObjects"];
		if (objectsValue.IsArray())
		{
			ReadGameObjects(objectsValue);
		}
	}
}

void nc::Scene::ReadGameObjects(const rapidjson::Value& value)
{
	for (rapidjson::SizeType i = 0; i < value.Size(); i++)
	{
		const rapidjson::Value& objectValue = value[i];
		if (objectValue.IsObject())
		{
			std::string typeName;
			nc::json::Get(objectValue, "type", typeName);
			GameObject* gameObject = nc::ObjectFactory::Instance().Create<nc::GameObject>(typeName);
				
			if (gameObject)
			{
					gameObject->Create(m_engine);
					gameObject->Read(objectValue);
					AddGameObject(gameObject);
			}
		}
	}
}

void nc::Scene::ReadPrototypes(const rapidjson::Value& value)
{
	for (rapidjson::SizeType i = 0; i < value.Size(); i++)
	{
		const rapidjson::Value& objectValue = value[i];
		if (objectValue.IsObject())
		{
			std::string typeName;
			nc::json::Get(objectValue, "type", typeName);
			GameObject* gameObject = nc::ObjectFactory::Instance().Create<nc::GameObject>(typeName);

			if (gameObject)
			{
				gameObject->Create(m_engine);
				gameObject->Read(objectValue);

				
				ObjectFactory::Instance().Register(gameObject->m_name, new Prototype<Object>(gameObject));
			}
		}
	}
}

void nc::Scene::Update()
{
	for (auto gameObject : m_gameObjects)
	{
		gameObject->Update();
	}

	auto iter = m_gameObjects.begin();
	while (iter != m_gameObjects.end())
	{
		if ((*iter)->m_flags[GameObject::eFlags::DESTROY])
		{
			(*iter)->Destroy();
			delete (*iter);
			iter = m_gameObjects.erase(iter);
		}
		else 
		{
			iter++;

		}
	}
}

void nc::Scene::Draw()
{
	for (auto gameObject : m_gameObjects)
	{
		gameObject->Draw();
	}
}

nc::GameObject* nc::Scene::Find(const std::string& name)
{
	for (auto gameObject : m_gameObjects)
	{
		if (gameObject->m_name == name)
		{
			return gameObject;
		}
	}

	return nullptr;


}

std::vector<nc::GameObject*> nc::Scene::FindGameObjectsWithTag(const std::string& tag)
{
	std::vector<GameObject*> gameObjects;
	for (auto gameObject : m_gameObjects)
	{
		if (gameObject->m_tag == tag)
		{
			gameObjects.push_back(gameObject);
		}
	}

	return gameObjects;
}

void nc::Scene::AddGameObject(GameObject* gameObject)
{
	m_gameObjects.push_back(gameObject);
}

void nc::Scene::RemoveGameObject(GameObject* gameObject)
{
	auto iter = std::find(m_gameObjects.begin(), m_gameObjects.end(), gameObject);
	if (iter != m_gameObjects.end())
	{
		(*iter)->Destroy();
		delete* iter;
		m_gameObjects.erase(iter);
	}

}

void nc::Scene::RemoveAllGameObjects()
{
	for (GameObject* gameObject : m_gameObjects)
	{
		gameObject->Destroy();
		delete gameObject;
	}

	m_gameObjects.clear();

}
