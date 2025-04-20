/**
@author		Shun Moriya

能力
*/

#include "Ability.h"

void FAbility::Reset()
{
#if 0
	Strength = 0;
	Wisdom = 0;
	Charisma = 0;
#else
	Strength = std::rand();
	Wisdom = std::rand();
	Charisma = std::rand();
#endif
}
