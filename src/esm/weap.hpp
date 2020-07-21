#pragma once
#include "record.hpp"
#include "structs.hpp"

#include <cstdint>
#include <string>
#include <sstream>

namespace ESM {

#pragma pack(1)
struct WeaponData {
	int32_t value;
	int32_t health;
	float weight;
	int16_t baseDamage;
	uint8_t magazineSize;
};
#pragma options align=reset

enum class WeaponAnimationType {
	HAND_TO_HAND,
	MELEE_1_HAND,
	MELEE_2_HAND,
	PISTOL_BALLISTIC_1_HAND,
	PISTOL_ENERGY_1_HAND,
	RIFLE_BALLISTIC_2_HAND,
	RIFLE_AUTOMATIC_2_HAND,
	RIFLE_ENERGY_2_HAND,
	HANDLE_2_HAND,
	LAUNCHER_2_HAND,
	GRENADE_THROW_1_HAND,
	LAND_MINE_1_HAND,
	MINE_DROP_1_HAND,
	THROWN_1_HAND
};	

enum class WeaponFlags : uint8_t {
	IGNORES_RESISTANCE = 0x01,
	IS_AUTOMATIC = 0x02,
	HAS_SCOPE = 0x04,
	CANT_DROP = 0x08,
	HIDE_BACKPACK = 0x10,
	EMBEDDED_WEAPON = 0x20,
	DONT_USE_1ST_PERSON_IS_ANIMATIONS = 0x40,
	NON_PLAYABLE = 0x80
};

enum class GripAnimation : uint8_t {
	HANDGRIP_1 = 230,
	HANDGRIP_2 = 231,
	HANDGRIP_3 = 232,
	HANDGRIP_4 = 233,
	HANDGRIP_5 = 234,
	HANDGRIP_6 = 235,
	DEFAULT = 255,
};

enum class ReloadAnimation : uint8_t {
  A,
  B,
  C,
  D,
  E,
  F,
  G,
  H,
  I,
  J,
  K,
  L,
  M,
  N,
  O,
  P,
  Q,
  R,
  S,
  W,
  X,
  Y,
  Z
};

enum class WeaponAttackAnimation : uint8_t {
	ATTACK_LEFT = 26,
	ATTACK_RIGHT = 32,
	ATTACK_3 = 38,
	ATTACK_4 = 44,
	ATTACK_5 = 50,
	ATTACK_6 = 56,
	ATTACK_7 = 62,
	ATTACK_8 = 68,
	ATTACK_LOOP = 74,
	ATTACK_SPIN_1 = 80,
	ATTACK_SPIN_2 = 86,
	PLACE_MINE_1 = 102,
	PLACE_MINE_2 = 108,
	ATTACK_THROW_1 = 114,
	ATTACK_THROW_2 = 120,
	ATTACK_THROW_3 = 126,
	ATTACK_THROW_4 = 132,
	ATTACK_THROW_5 = 138,
	ATTACK_9 = 144,
	ATTACK_THROW_6 = 150,
	ATTACK_THROW_7 = 156,
	ATTACK_THROW_8 = 162,
	DEFAULT = 255
};

enum class EmbeddedWeaponActorValue : uint8_t {
	PERCEPTION,
	ENDURANCE,
	LEFT_ATTACK,
	RIGHT_ATTACK,
	LEFT_MOBILITY,
	RIGHT_MOBILITY,
	BRAIN
};

enum class OnHit : uint32_t {
	NORMAL,
	DISMEMBER_ONLY,
	EXPLODE_ONLY,
	NO_DISMEMBER_OR_EXPLODE
};

enum class WeaponAttackFlags : uint32_t {
	PLAYER_ONLY = 0x1,
	NPC_USE_AMMO = 0x2,
	NO_JAM_AFTER_RELOAD = 0x4,
	OVERRIDE_ACTION_POINTS = 0x8,
	MINOR_CRIME = 0x10,
	RANGE_FIXED = 0x20,
	NOT_USED_IN_NORMAL_COMBAT = 0x40,
	OVERRIDE_DAMAGE_TO_WEAPON_MULT = 0x80,
	DONT_USE_3RD_PERSON_IS_ANIMATIONS = 0x100,
	SHORT_BURST = 0x200,
	RUMBLE_ALTERNATE = 0x400,
	LONG_BURST = 0x800,
	SCOPE_HAS_NIGHT_VISION = 0x1000,
	SCOPE_FROM_MOD = 0x2000
};

enum class RumblePattern : uint32_t {
	CONSTANT,
	SQUARE,
	TRIANGLE,
	SAWTOOTH
};

enum class WeaponModEffect : uint32_t {
	NONE,
	INCREASE_DAMAGE,
	INCREASE_MAGAZINE_CAPACITY,
	DECREASE_SPREAD,
	DECREASE_WEIGHT,
	REGENERATE_AMMO_SHOTS,
	REGENERATE_AMMO_SECONDS,
	DECREASE_EQUIP_TIME,
	INCREASE_RATE_OF_FIRE,
	INCREASE_PROJECTILE_SPEED,
	INCREASE_MAX_CONDITION,
	SILENCE,
	SPLIT_BEAM,
	VATS_BONUS,
	INCREASE_ZOOM,
	DECREASE_EQUIP_TIME_2,
	SUPPRESSOR	
};

enum class PowerAttackAnimationOverride : uint32_t {
	UNKNOWN,
	ATTACK_CUSTOM_1 = 97,
	ATTACK_CUSTOM_2 = 98,
	ATTACK_CUSTOM_3 = 99,
	ATTACK_CUSTOM_4 = 100,
	ATTACK_CUSTOM_5 = 101,
	DEFAULT = 255
};


struct WeaponAttributes {
	WeaponAnimationType AnimationType;
	float animationMultiplier;
	float reach;
	WeaponFlags flags_1;
	GripAnimation gripAnimation;
	uint8_t ammoUse;
	ReloadAnimation reloadAnimation;
	float minSpread;
	float spread;
	uint8_t unknown[4];
	float sightFov;
	float unknown_2;
	formid projectile;
	uint8_t baseVATSToHitChance;
	WeaponAttackAnimation attackAnimation;
	uint8_t projectileCount;
	EmbeddedWeaponActorValue embeddedWeaponActorValue;
	float minRange;
	float maxRange;
	OnHit onHit;
	WeaponAttackFlags flags_2;
	float animationAttackMultiplier;
	float fireRate;
	float overrideActionPoints;
	float rumbleLeftMotorStrength;
	float rumbleRightMotorStrength;
	float rumbleDuration;
	float overrideDamageToWeaponMult;
	float attackShotsPerSecond;
	float reloadTime;
	float jamTime;
	float aimArc;
	int32_t skill;
	RumblePattern rumblePattern;
	float rumbleWavelength;
	float limbDamageMultiplier;
	int32_t resistanceType;
	float sightUsage;
	float semiAutoFireDelayMin;
	float semiAutoFireDelayMax;
	float unknown_3;
	WeaponModEffect effectMod1;
	WeaponModEffect effectMod2;
	WeaponModEffect effectMod3;
	float valueAMod1;
	float valueAMod2;
	float valueAMod3;
	PowerAttackAnimationOverride powerAttackAnimationOverride;
	uint32_t strengthRequirement;
	uint8_t unknown_4;
	ReloadAnimation reloadAnimationMod;
	uint8_t unknown_5[2];
	float regenRate;
	float killImpulse;
	float valueBMod1;
	float valueBMod2;
	float valueBMod3;
	float impulseDist;
	uint32_t skillRequirement;
};

enum class CriticalFlags : uint8_t {
	ON_DEATH = 1
};

struct CriticalData {
	struct CriticalDataNullEffect {
		uint16_t criticalDamage;
		uint8_t padding[2];
		float criticalPercentageMultiplier;
		CriticalFlags flags;
		uint8_t padding_2[3];
	};

	uint16_t criticalDamage;
	uint8_t padding[2];
	float criticalPercentageMultiplier;
	CriticalFlags flags;
	uint8_t padding_2[3];
	formid effect;

	static void load(ESMReader& reader, CriticalData& critData);
};

struct VATSData {
	struct VATSDataNullEffect {
		float skill;
		float damageMultiplier;
		float AP;
		uint8_t silent; //0 - no, 1 - yes
		uint8_t modRequired; //0 - no, 1 - yes
		uint8_t padding[2];
	};

	formid effect;
	float skill;
	float damageMultiplier;
	float AP;
	uint8_t silent; //0 - no, 1 - yes
	uint8_t modRequired; //0 - no, 1 - yes
	uint8_t padding[2];

	static void load(ESMReader& reader, VATSData& vats);
};


struct Weapon : public Record {

	ObjectBounds objectBounds;
	std::string name;
	ModelData modelData;
	std::string largeIconFilename;
	std::string smallIconFilename;
	formid script;
	formid objectEffect;
	int16_t enchantmentChargeAmount;
	formid ammo;
	DestructionData destructionData;
	
	formid repairList;
	int32_t equipmentType;
	formid bipedModelList;
	formid soundPickUp;
	formid soundDrop;
	
	ModelData shellCasingModelData;
	ModelData scopeModelData;
	
	formid scopeEffect;
	ModelData scopeEffectModelData;
	

	std::string modelWithMod1;
	std::string modelWithMod2;
	std::string modelWithMod12;
	std::string modelWithMod3;
	std::string modelWithMod13;
	std::string modelWithMod23;
	std::string modelWithMod123;
	

	std::string vatsAttackName;
	std::string embeddedWeaponNode;
	formid impactDataset;
	

	formid firstPersonModel;
	formid firstPersonModelWithMod1;
	formid firstPersonModelWithMod2;
	formid firstPersonModelWithMod12;
	formid firstPersonModelWithMod3;
	formid firstPersonModelWithMod13;
	formid firstPersonModelWithMod23;
	formid firstPersonModelWithMod123;
	

	formid weaponMod1;
	formid weaponMod2;
	formid weaponMod3;
	

	formid soundGunShoot3D;
	formid soundGunShootDist;
    formid soundGunShoot2D;
   	formid soundGunShoot3DLooping;
	
	formid soundMeleeSwing; //used for gun with no ammo
	formid soundBlock;
	formid soundIdle;
	formid soundEquip;
	formid soundUnequip;
	
	formid soundMod1Shoot3D;
	formid soundMod1ShootDist; //doesn't exist
    formid soundMod1Shoot2D;

    WeaponData data;
    WeaponAttributes attributes;
    CriticalData criticalData;
    VATSData vats;
    SoundLevel soundLevel;

	Weapon(ESMReader& reader);

};

};