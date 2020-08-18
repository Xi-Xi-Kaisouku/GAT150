#include "pch.h"
#include "Graphics/Texture.h"
#include "Engine.h"
#include "Objects/GameObject.h"
#include "Components/PlayerComponent.h"
#include "Core/Json.h"
#include "Core/Factory.h"
#include "Objects/ObjectFactory.h"

nc::Engine engine;
nc::GameObject player;


int main(int, char**)
{
    //nc::json::Load("json.txt", document);

    //nc::Timer timer;
    ////profile
    //for (size_t i = 0; i < 100; i++) { std::sqrt(rand() % 100); }
    //std::cout << timer.ElapsedSeconds() << std::endl;

    engine.Startup();

    nc::ObjectFactory::Instance().Initialize();
    nc::ObjectFactory::Instance().Register("PlayerComponent", nc::Object::Instantiate<nc::PlayerComponent>);

    nc::GameObject* player = nc::ObjectFactory::Instance().Create<nc::GameObject>("GameObject");

    player->Create(&engine);

    rapidjson::Document document;
    nc::json::Load("player.txt", document);
    player->Read(document);
    
    nc::Component* component;
    component = nc::ObjectFactory::Instance().Create<nc::Component>("PhysicsComponent");
    component->Create(player);
    player->AddComponent(component);
    
    component = nc::ObjectFactory::Instance().Create<nc::Component>("SpriteComponent");
    component->Create(player);
    nc::json::Load("sprite.txt", document);
    component->Read(document);
    player->AddComponent(component);

    component = nc::ObjectFactory::Instance().Create<nc::Component>("PlayerComponent");
    component->Create(player);
    player->AddComponent(component);


    //Texture
    nc::Texture* background = engine.GetSystem<nc::ResourceManager>()->Get<nc::Texture>("background.png", engine.GetSystem<nc::Renderer>());

    SDL_Event event;
    bool quit = false;
    while (!quit)
    {
        SDL_PollEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
            quit = true;
            break;
        }


        //update
        engine.Update();
        player->Update();


        if (engine.GetSystem<nc::InputSystem>()->GetButtonState(SDL_SCANCODE_ESCAPE) == nc::InputSystem::eButtonState::PRESSED)
        {
            quit = true;
        }




        //draw
        engine.GetSystem<nc::Renderer>()->BeginFrame();

        background->Draw({ 0,0 }, { 1.0f, 1.0f }, 0);

        player->Draw();
        
        engine.GetSystem<nc::Renderer>()->EndFrame();


    }

    engine.Shutdown();

    return 0;
}

