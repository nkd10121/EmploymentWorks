#include "CharacterBase.h"

CharacterBase::CharacterBase(Priority priority, GameObjectTag tag) :
	Collidable(priority, tag),
	m_status(),
	m_currentAnimNo(-1),
	m_prevAnimNo(-1),
	m_animBlendRate(1.0f),
	m_animSpeed(0.5f),
	m_isAnimationFinish(false)
{
}

CharacterBase::~CharacterBase()
{
}

bool CharacterBase::UpdateAnim(int attachNo, float startTime)
{
	return false;
}

void CharacterBase::ChangeAnim(int animIndex, float animSpeed)
{
}
