/**
@author		Shun Moriya



所持品
*/

#include "Inventory.h"

void FInventoryPair::Reset(const bool reset)
{
	Count = reset ? 0 : 1;
	Skill = 0;
}

void FInventory::Reset(const bool reset)
{
	for (auto& pair : Item)
		pair.Reset(reset);
	for (auto& pair : Weapon)
		pair.Reset(reset);
}
