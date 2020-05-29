#include "GE/Core/Maths/vec.hpp"

using namespace Engine::Core::Maths;

Vec2 Vec2::zero ({0.f, 0.f});
Vec2 Vec2::one ({1.f, 1.f});

Vec3 Vec3::zero({0.f, 0.f, 0.f});
Vec3 Vec3::one({1.f, 1.f, 1.f});
Vec3 Vec3::up({0.f, 1.f, 0.f});
Vec3 Vec3::down({0.f, -1.f, 0.f});
Vec3 Vec3::right({1.f, 0.f, 0.f});
Vec3 Vec3::left({-1.f, 0.f, 0.f});
Vec3 Vec3::forward({0.f, 0.f, 1.f});
Vec3 Vec3::backward({0.f, 0.f, -1.f});

float lerpf(float a, float b, float t)
{
    return a + t * (b - a);
}

Vec2 Vec2::lerp		(const Vec2& vec1, const Vec2& vec2, float t) 
{ 
    return {lerpf(vec1.x, vec2.x, t),
            lerpf(vec1.y, vec2.y, t)};
}	

Vec3 Vec3::lerp		(const Vec3& vec1, const Vec3& vec2, float t) 
{ 
    return {lerpf(vec1.x, vec2.x, t),
            lerpf(vec1.y, vec2.y, t), 
            lerpf(vec1.z, vec2.z, t)};
}	

Vec4 Vec4::lerp		(const Vec4& vec1, const Vec4& vec2, float t) 
{ 
    return {lerpf(vec1.x, vec2.x, t),
            lerpf(vec1.y, vec2.y, t), 
            lerpf(vec1.z, vec2.z, t),
            lerpf(vec1.w, vec2.w, t)};
}	
