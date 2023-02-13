#pragma once

#include "glm.hpp"
#include <vector>

class Texture;

class Fish {
public:
	Fish::Fish(const glm::vec3& position, const glm::vec3& velocity, Texture* texture);


	void Update(const std::vector<Fish>& neighbors, const glm::vec3& target, float dt);
	void Render();

private:
	glm::vec3 m_position;
	glm::vec3 m_velocity;
	Texture* m_texture;
	Rectangle m_bounds;
};