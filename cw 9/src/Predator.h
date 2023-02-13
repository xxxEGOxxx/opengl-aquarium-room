#pragma once

#include "glm.hpp"

class Predator {
public:
	Predator(const glm::vec3& position);

	void Render();

private:
	glm::vec3 m_position;
};