#include "Physics.h"
#include <float.h> // FLT_MAX
#include <iostream>



glm::vec2 TripleProduct(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c)
{
    float z = a.x * b.y - a.y * b.x;
    return glm::vec2{ -c.y * z, c.x * z };
}

// line case
bool Simplex2(std::vector<glm::vec2>& simplex, glm::vec2& dir)
{
    glm::vec2 a = simplex[1];
    glm::vec2 b = simplex[0];

    glm::vec2 ab = b - a;
    glm::vec2 ao = -a;

    if (glm::dot(ab, ao) > 0) {
        dir = TripleProduct(ab, ao, ab);
    } else {
        simplex = { a };
        dir = ao;
    }

    return false;
}

// triangle case
bool Simplex3(std::vector<glm::vec2>& simplex, glm::vec2& dir)
{
    glm::vec2 a = simplex[2];
    glm::vec2 b = simplex[1];
    glm::vec2 c = simplex[0];

    glm::vec2 ab = b - a;
    glm::vec2 ac = c - a;
    glm::vec2 abPerp = TripleProduct(ac, ab, ab);
    glm::vec2 acPerp = TripleProduct(ab, ac, ac);

    glm::vec2 ao = -a;

    if (glm::dot(abPerp, ao) > 0) {
        simplex = { a, b };
        dir = abPerp;
    } else if (glm::dot(acPerp, ao) > 0) {
        simplex = { a, c };
        dir = acPerp;
    } else {
        return true;
    }

    return false;
}

bool Gjk(Shape& shapeA, Shape& shapeB)
{
    // get an initial search direction
    glm::vec2 dir = shapeB.Center() - shapeA.Center();

    // get first support point on the minkowski sum
    glm::vec2 support = shapeA.Support(dir) - shapeB.Support(-dir);

    // create the simplex
    std::vector<glm::vec2> simplex = { support };

    // direction towards the origin
    dir = -support;

    while (true) {
        glm::vec2 a = shapeA.Support(dir) - shapeB.Support(-dir);

        if (glm::dot(a, dir) < 0) { // no intersection case
            return false;
        }

        // push the point onto the simplex
        simplex.push_back(a);

        // check if the origin is within the simplex
        if (simplex.size() == 2) {
            if (Simplex2(simplex, dir)) {
                return true;
            }
        } else if (simplex.size() == 3) {
            if (Simplex3(simplex, dir)) {
                return true;
            }
        }
    }
}