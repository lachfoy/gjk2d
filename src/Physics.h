#ifndef PHYSICS_H_
#define PHYSICS_H_

#include <glm/glm.hpp>
#include <vector>
#include "Renderer.h"
#include "TextRenderer.h"

struct Shape
{
    std::vector<glm::vec2> points;

    glm::vec2 Support(const glm::vec2& dir) const
    {
        glm::vec2 maxPoint;
		float maxDistance = -FLT_MAX;
 
		for (glm::vec2 p : points) {
			float distance = glm::dot(p, dir);
			if (distance > maxDistance) {
				maxDistance = distance;
				maxPoint = p;
			}
		}
 
		return maxPoint;
    }

    glm::vec2 Center() const
    {
        glm::vec2 center = glm::vec2(0.0f);
        for (glm::vec2 p : points) {
			center += p;
		}

        center /= points.size();

        return center;
    }

    void Draw(Renderer& renderer, const glm::vec3& color)
    {
        for (size_t i = 0; i < points.size() - 1; i++) {

			renderer.DrawLine(points[i], points[i + 1], color);
		}

        renderer.DrawLine(points[points.size() - 1], points[0], color);
    }
};

glm::vec2 TripleProduct(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c);
bool Simplex2(std::vector<glm::vec2>& simplex, glm::vec2& dir);
bool Simplex3(std::vector<glm::vec2>& simplex, glm::vec2& dir);
bool Gjk(Shape& shapeA, Shape& shapeB);

#endif // PHYSICS_H_