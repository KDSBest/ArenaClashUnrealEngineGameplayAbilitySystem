#include "ArenaGameplayTags.h"

namespace ArenaTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Dead, "State.Dead",
		"The character has no health left.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Debuff_Stun, "State.Debuff.Stun",
		"Cannot move or use abilities.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Debuff_Slow, "State.Debuff.Slow",
		"Moves more slowly.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Debuff_Burning, "State.Debuff.Burning",
		"Takes fire damage over time.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Buff_Shield, "State.Buff.Shield",
		"Damage is absorbed by a shield.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage_Type_Physical, "Damage.Type.Physical",
		"Physical damage, reduced by armor.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage_Type_Fire, "Damage.Type.Fire",
		"Fire damage.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Data_Damage, "Data.Damage",
		"Set By Caller: base damage amount.");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Event_Montage_Hit, "Event.Montage.Hit",
		"Sent by an animation when an attack connects.");
}
