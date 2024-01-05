#include "ColliderComponent.h"
#include "Actor.h"

ColliderComponent::ColliderComponent(class Actor* actor)
    :Component(actor)
    , mRadius(0.0f)
{
}

// ’†SÀ•W‚ğæ“¾
const Vector2& ColliderComponent::GetCenter() const
{
    return mActor->GetPosition();
}

// ƒXƒP[ƒ‹‚ğl—¶‚µ‚½”¼Œa‚ğ•Ô‹p
float ColliderComponent::GetRadius() const
{
    return mActor->GetScale() * mRadius;
}

// Õ“Ëˆ—
bool Intersect(const ColliderComponent& a, const ColliderComponent& b)
{
    // ŒvZ•‰‰×ŒyŒ¸‚Ì‚½‚ßsqrt‚¹‚¸‚É”äŠr
    // Õ“Ë‚µ‚½•¨‘Ì‚Æ‚Ì‹——£
    Vector2 diff = a.GetCenter() - b.GetCenter();
    float distSq = diff.LengthSq();
    // Õ“Ë‰~‚Ì”¼Œa‚Ì‡ŒvˆÈ‰º‚È‚çÕ“Ë
    float radDiff = a.GetRadius() + b.GetRadius();
    return distSq <= radDiff * radDiff;
}