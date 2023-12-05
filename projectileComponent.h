#pragma once
#include "entityCS.h"
#include "components.h"
#include "vector2D.h"

class ProjectileComponent : public Component
{
public:

	ProjectileComponent(int rng, int sp, Vector2D velocity) : range(rng), speed(sp)
	{}
	~ProjectileComponent()
	{}


	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		transform->velocity = velocity;
	}

	void update() override
	{
		distance += speed;

		if (distance > range)
		{
			entity->destroy();
		}
		else if (transform->position.x > Game::camera.x + Game::camera.w || 
				transform->position.x < Game::camera.x ||
				transform->position.y > Game::camera.y + Game::camera.h ||
				transform->position.y < Game::camera.y)
					{
					entity->destroy();
					}
		
	}


private:

	TransformComponent* transform;

	int range;
	int speed;
	int distance;
	Vector2D velocity;

};