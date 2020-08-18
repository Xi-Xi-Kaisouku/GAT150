#pragma once
#include <SDL.h>
#include <string>
#include "Math/Vector2.h"
#include "Resources/Resource.h"

namespace nc
{
	class Texture : public Resource
	{
	public:
		bool Create(const std::string& name, void* renderer) override;
		void Destroy() override;

		void Draw(const Vector2& position, const Vector2& scale, float angle);
		void Draw(const SDL_Rect& source, const Vector2& position, const Vector2& scale, float angle);
		Vector2 GetSize();


	protected:
		SDL_Texture* m_texture{ nullptr };
		SDL_Renderer* m_renderer{ nullptr };
	};
}