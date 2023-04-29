#ifndef PHYSICS_H_
#define PHYSICS_H_

#include <glm/glm.hpp>
#include <vector>

struct GjkCollider
{
    virtual glm::vec2 Support(const glm::vec2& dir) const = 0;
};

struct BoxCollider : GjkCollider
{
    glm::vec2 min;
    glm::vec2 max;

    glm::vec2 Support(const glm::vec2& dir) const override {
        glm::vec2 result;
        result.x = (dir.x >= 0) ? max.x : min.x;
        result.y = (dir.y >= 0) ? max.y : min.y;
        return result;
    }
};

struct CircleCollider : GjkCollider
{
    glm::vec2 center;
    float radius;

    glm::vec2 Support(const glm::vec2& dir) const override {
        return center + radius * glm::normalize(dir);
    }
};

glm::vec2 TripleProduct(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c);
bool Simplex2(std::vector<glm::vec2>& simplex, glm::vec2& dir);
bool Simplex3(std::vector<glm::vec2>& simplex, glm::vec2& dir);
bool Gjk(const GjkCollider& colliderA, const GjkCollider& colliderB);

#endif // PHYSICS_H_