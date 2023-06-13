#pragma once
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

namespace glm 
{
    template <typename T, precision P>
    bool operator<(const tvec2<T, P>& a, const tvec2<T, P>& b)
    {
        return (a.x < b.x || (a.x == b.x && a.y < b.y));
    }

    template <typename T, precision P>
    bool operator<(const tvec3<T, P>& a, const tvec3<T, P>& b)
    {
        return (a.x < b.x || (a.x == b.x && a.y < b.y) || (a.x == b.x && a.y == b.y && a.z < b.z));
    }
};