#pragma once

#include "glm.hpp"

class Rectangle {
public:
	Rectangle(const glm::vec3& min = glm::vec3(0), const glm::vec3& max = glm::vec3(1));

	bool Contains(const glm::vec3& point) const;

	glm::vec3 m_min;
	glm::vec3 m_max;
};