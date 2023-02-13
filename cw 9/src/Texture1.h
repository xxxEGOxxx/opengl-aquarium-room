#pragma once

#include "glm.hpp"

class Rectangle {
public:
	Rectangle(const glm::vec3& min, const glm::vec3& max);

	bool Contains(const glm::vec3& point) const;

private:
	glm::vec3 m_min;
	glm::vec3 m_max;
};