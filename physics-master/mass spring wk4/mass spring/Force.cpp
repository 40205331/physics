#include <iostream >
#include <cmath >
#include "Force.h"
#include "Body.h"
#include "glm/ext.hpp"

glm::vec3 Force::apply(float mass, const glm::vec3 &pos, const glm::vec3 &vel) {
	return glm::vec3(0.0f);

}

/*
   ** GRAVITY
   */
glm::vec3 Gravity::apply(float mass, const glm::vec3 &pos, const glm::vec3 &vel) {
	// complete. Should return the acceleration resulting from gravity
	glm::vec3 adg = mass*m_gravity;

	return adg;
}

/*
   ** DRAG
   */
glm::vec3 Drag::apply(float mass, const glm::vec3 &pos, const glm::vec3 &vel) {
	// complete. Should return the acceleration resulting from aerodynamic drag
	glm::vec3 Drag = 0.5 * mass * glm::dot(vel, vel) * vel / std::sqrt(glm::dot(vel, vel)) * 1.15 * 1 * 0.1;
	return Drag;
}
/*
  ** HOOK'S LAW
  */
glm::vec3 Hook::apply(float mass, const glm::vec3 &pos, const glm::vec3 &vel) {
	// complete
	float displacement = glm::distance(m_b2->getPos(), m_b1->getPos());
	float stiffness = -m_ks * (m_rest - displacement);
	
	glm::vec3 Unit = (m_b2->getPos() - m_b1->getPos()) / displacement;

	//glm::vec3 v1 = Unit * m_b1->getVel();
	//glm::vec3 v2 = Unit * m_b2->getVel();

	float dampness = -m_kd * (glm::dot(m_b1->getVel(), Unit) - glm::dot(m_b2->getVel(), Unit));

	glm::vec3 Hook = (stiffness + dampness) * Unit;
	return Hook;
}