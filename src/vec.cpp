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