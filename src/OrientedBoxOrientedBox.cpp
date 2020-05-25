#include "GE/Core/Maths/ShapeRelation/OrientedBoxOrientedBox.hpp"
#include "GE/Core/Maths/ShapeRelation/AabbAabb.hpp"
#include "GE/Core/Maths/vec.hpp"

#include <limits>

using namespace Engine::Core::Maths;
using namespace Engine::Core::Maths::Shape3D;
using namespace Engine::Core::Maths::ShapeRelation;

static void affectMinMaxValue(float& min, float& max, float value)
{
    min = std::min(min, value);
    max = std::max(max, value);
}


static bool isSATFoundedOnAxe(const OrientedBox& box1, const OrientedBox& box2, const Vec3& axe)
{
    float minBox1 = std::numeric_limits<float>::max(), maxBox1 = std::numeric_limits<float>::min(), minBox2 = std::numeric_limits<float>::max(), maxBox2 = std::numeric_limits<float>::min();

    affectMinMaxValue(minBox1, maxBox1, Vec3::dot(axe, box1.ptForwardTopLeft()));
    affectMinMaxValue(minBox1, maxBox1, Vec3::dot(axe, box1.ptForwardTopRight()));
    affectMinMaxValue(minBox1, maxBox1, Vec3::dot(axe, box1.ptForwardBottomLeft()));
    affectMinMaxValue(minBox1, maxBox1, Vec3::dot(axe, box1.ptForwardBottomRight()));
    affectMinMaxValue(minBox1, maxBox1, Vec3::dot(axe, box1.ptBackTopLeft()));
    affectMinMaxValue(minBox1, maxBox1, Vec3::dot(axe, box1.ptBackTopRight()));
    affectMinMaxValue(minBox1, maxBox1, Vec3::dot(axe, box1.ptBackBottomLeft()));
    affectMinMaxValue(minBox1, maxBox1, Vec3::dot(axe, box1.ptBackBottomRight()));

    affectMinMaxValue(minBox2, maxBox2, Vec3::dot(axe, box2.ptForwardTopLeft()));
    affectMinMaxValue(minBox2, maxBox2, Vec3::dot(axe, box2.ptForwardTopRight()));
    affectMinMaxValue(minBox2, maxBox2, Vec3::dot(axe, box2.ptForwardBottomLeft()));
    affectMinMaxValue(minBox2, maxBox2, Vec3::dot(axe, box2.ptForwardBottomRight()));
    affectMinMaxValue(minBox2, maxBox2, Vec3::dot(axe, box2.ptBackTopLeft()));
    affectMinMaxValue(minBox2, maxBox2, Vec3::dot(axe, box2.ptBackTopRight()));
    affectMinMaxValue(minBox2, maxBox2, Vec3::dot(axe, box2.ptBackBottomLeft()));
    affectMinMaxValue(minBox2, maxBox2, Vec3::dot(axe, box2.ptBackBottomRight()));

    //try seg1 on seg2 or seg2 on seg1
    return ((minBox2 <= maxBox1 && minBox2 >= minBox1) ||
            (maxBox2 <= maxBox1 && maxBox2 >= minBox1)) ||
            ((minBox1 <= maxBox2 && minBox1 >= minBox2) ||
            (maxBox1 <= maxBox2 && maxBox1 >= minBox2));
}

bool OrientedBoxOrientedBox::isBothOrientedBoxCollided(const OrientedBox& box1, const OrientedBox& box2)
{
    /*Exclude the majority case of no collision*/
    if (!AabbAabb::isBothAABBCollided(box1.getAABB(), box2.getAABB()))
    {
        return false;
    }

    /*Use SAT to know if collision existe*/
    if (!isSATFoundedOnAxe(box1, box2, box1.getReferential().unitI)) return false;
    if (!isSATFoundedOnAxe(box1, box2, box1.getReferential().unitJ)) return false;
    if (!isSATFoundedOnAxe(box1, box2, box1.getReferential().unitK)) return false;

    if (!isSATFoundedOnAxe(box1, box2, box2.getReferential().unitI)) return false;
    if (!isSATFoundedOnAxe(box1, box2, box2.getReferential().unitJ)) return false;
    if (!isSATFoundedOnAxe(box1, box2, box2.getReferential().unitK)) return false;

    if (!isSATFoundedOnAxe(box1, box2, Vec3::cross(box1.getReferential().unitI, box2.getReferential().unitI))) return false;
    if (!isSATFoundedOnAxe(box1, box2, Vec3::cross(box1.getReferential().unitI, box2.getReferential().unitJ))) return false;
    if (!isSATFoundedOnAxe(box1, box2, Vec3::cross(box1.getReferential().unitI, box2.getReferential().unitK))) return false;

    if (!isSATFoundedOnAxe(box1, box2, Vec3::cross(box1.getReferential().unitJ, box2.getReferential().unitI))) return false;
    if (!isSATFoundedOnAxe(box1, box2, Vec3::cross(box1.getReferential().unitJ, box2.getReferential().unitJ))) return false;
    if (!isSATFoundedOnAxe(box1, box2, Vec3::cross(box1.getReferential().unitJ, box2.getReferential().unitK))) return false;

    if (!isSATFoundedOnAxe(box1, box2, Vec3::cross(box1.getReferential().unitK, box2.getReferential().unitI))) return false;
    if (!isSATFoundedOnAxe(box1, box2, Vec3::cross(box1.getReferential().unitK, box2.getReferential().unitJ))) return false;
    if (!isSATFoundedOnAxe(box1, box2, Vec3::cross(box1.getReferential().unitK, box2.getReferential().unitK))) return false;

    return true;
}