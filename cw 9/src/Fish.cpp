#include "Fish.h"
#include "Texture1.h"
#include "Rectangle.h"
#include "glm.hpp"
#include "glew.h"
#include <vector>

Fish::Fish(const glm::vec3& position, const glm::vec3& velocity, Texture* texture)
    : m_position(position), m_velocity(velocity), m_texture(texture), m_bounds() {}



void Fish::Update(const std::vector<Fish>& neighbors, const glm::vec3& target, float dt) {
    // W tej funkcji nale¿y zaimplementowaæ algorytm boid.
    // neighbors jest wektorem s¹siaduj¹cych ryb.
    // target jest pozycj¹ drapie¿nika, dt jest ró¿nic¹ czasu miêdzy aktualnym i poprzednim klatkami.

    // Pozycja ryby jest aktualizowana na podstawie algorytmu boid.

    glm::vec3 average_position(0.0f, 0.0f, 0.0f);
    glm::vec3 average_velocity(0.0f, 0.0f, 0.0f);
    int count = 0;

    for (const Fish& neighbor : neighbors) {
        if (&neighbor == this) {
            continue;
        }

        average_position += neighbor.m_position;
        average_velocity += neighbor.m_velocity;
        ++count;
    }

    if (count > 0) {
        average_position /= count;
        average_velocity /= count;
    }

    glm::vec3 center = m_bounds.m_min + (m_bounds.m_max - m_bounds.m_min) * 0.5f;
    glm::vec3 avoid = m_position - center;

    m_velocity += (average_position - m_position) * 0.05f +
        (average_velocity - m_velocity) * 0.05f +
        avoid * 0.05f;

    if (target != glm::vec3(0.0f, 0.0f, 0.0f)) {
        m_velocity += (target - m_position) * 0.05f;
    }

    m_velocity = glm::normalize(m_velocity);
    m_position += m_velocity * dt;

    // Ograniczenie pozycji ryby do prostok¹ta.
    m_position.x = glm::clamp(m_position.x, m_bounds.m_min.x, m_bounds.m_max.x);
    m_position.y = glm::clamp(m_position.y, m_bounds.m_min.y, m_bounds.m_max.y); 
    m_position.z = glm::clamp(m_position.z, m_bounds.m_min.z, m_bounds.m_max.z);
}

void Fish::Render() const { // W tej funkcji nale¿y zrenderowaæ teksturê ryby w odpowiedniej pozycji. // Mo¿na to zrobiæ na przyk³ad za pomoc¹ biblioteki OpenGL.

    glm::mat4 model(1.0f); model = glm::translate(model, m_position);

    GLuint model_location = glGetUniformLocation(m_shader_program, "model"); glUniformMatrix4fv(model_location, 1, GL_FALSE, &model[0][0]);

    GLuint texture_location = glGetUniformLocation(m_shader_program, "texture_sampler"); glUniform1i(texture_location, 0);

    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, m_texture.m_id);

    glBindVertexArray(m_vao); glDrawArrays(GL_TRIANGLES, 0, m_vertex_count); glBindVertexArray(0);
}