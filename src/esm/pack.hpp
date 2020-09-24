#pragma once
#include "reader.hpp"
#include "record.hpp"
#include "subrecords/scriptdata.hpp"
#include "structs.hpp"

namespace ESM {

enum class PackageGeneralFlags : uint32_t {
	OFFERS_SERVICES = 0x00000001,
	MUST_REACH_LOCATION = 0x00000002,
	MUST_COMPLETE = 0x00000004,
	LOCK_DOORS_AT_PACKAGE_START = 0x00000008,
	LOCK_DOORS_AT_PACKAGE_END = 0x00000010,
	LOCK_DOORS_AT_LOCATION = 0x00000020,
	UNLOCK_DOORS_AT_PACKAGE_START = 0x00000040,
	UNLOCK_DOORS_AT_PACKAGE_END = 0x00000080,
	UNLOCK_DOORS_AT_LOCATION = 0x00000100,
	CONTINUE_IF_PC_NEAR = 0x00000200,
	ONCE_PER_DAY = 0x00000400,
	UNKNOWN_1 = 0x00000800,
	SKIP_FALLOUT_BEHAVIOR = 0x00001000,
	ALWAYS_RUN = 0x00002000,
	UNKNOWN_2 = 0x00004000,
	UNKNOWN_3 = 0x00008000,
	UNKNOWN_4 = 0x00010000,
	ALWAYS_SNEAK = 0x00020000,
	ALLOW_SWIMMING = 0x00040000,
	ALLOW_FALLS = 0x00080000,
	HEAD_TRACKING_OFF = 0x00100000,
	WEAPONS_UNEQUIPPED = 0x00200000,
	DEFENSIVE_COMBAT = 0x00400000,
	WEAPON_DRAWN = 0x00800000,
	NO_IDLE_ANIMS = 0x01000000,
	PRETEND_IN_COMBAT = 0x02000000,
	CONTINUE_DURING_COMBAT = 0x04000000,
	NO_COMBAT_ALERT = 0x08000000,
	NO_WARN_ATTACK_BEHAVIOR = 0x10000000
};

enum class PackageType : uint8_t {
	FIND,
	FOLLOW,
	ESCORT,
	EAT,
	SLEEP,
	WANDER,
	TRAVEL,
	ACCOMPANY,
	USE_ITEM_AT,
	AMBUSH,
	FLEE_NOT_COMBAT,
	UNKNOWN,
	SANDBOX,
	PATROL,
	GUARD,
	DIALOGUE,
	USE_WEAPON
};

enum class FalloutBehaviorFlags : uint16_t{
	HELLOS_TO_PLAYER = 0x0001,
	RANDOM_CONVERSATIONS = 0x0002,
	OBSERVE_COMBAT_BEHAVIOR = 0x0004,
	UNKNOWN = 0x0008,
	REACTION_TO_PLAYER_ACTIONS = 0x0010,
	FRIENDLY_FIRE_COMMENTS = 0x0020,
	AGGRO_RADIUS_BEHAVIOR = 0x0040,
	ALLOW_IDLE_CHATTER = 0x0080,
	AVOID_RADIATION = 0x0100
};

struct PackageData {
	PackageGeneralFlags flags;
	PackageType			type;
	uint8_t				unused;
	FalloutBehaviorFlags falloutBehaviorFlags;
	uint16_t            typeSpecificFlags; //type field determines flag meaning, some types have no flags
	uint8_t				padding[2];
};

enum class dow : int8_t {
	Any = -1,
	Sunday = 0,
	Monday = 1,
	Tuesday = 2,
	Wednesday = 3,
	Thursday = 4,
	Friday = 5,
	Saturday = 6,
	Weekdays = 7,
	Weekends = 8,
	MonWedFri = 9,
	TueThu = 10
};
  
struct PackageSchedule {
	int8_t month;
	dow    dayOfWeek;
	uint8_t date;
	int8_t time;
	int32_t duration;
};

enum class PackageTargetType : int32_t {
	SpecificReference = 0, //formid
	ObjectId = 1, //formid
	ObjectType = 2, //ObjectType
	LinkedReference = 3
};

enum class ObjectType : uint32_t {
	NONE,
	ACTIVATORS,
	ARMOR,
	BOOKS,
	CLOTHING,
	CONTAINERS,
	DOORS,
	INGREDIENTS,
	LIGHTS,
	MISC,
	FLORA,
	FURNITURE,
	WEAPONS_ANY,
	AMMO,
	NPCS,
	CREATURES,
	KEYS,
	ALCHEMY,
	FOOD,
	ALL_COMBAT_WEARABLE,
	ALL_WEARABLE,
	WEAPONS_RANGED,
	WEAPONS_MELEE,
	WEAPONS_NONE,
	ACTOR_EFFECTS_ANY,
	ACTOR_EFFECTS_RANGE_TARGET,
	ACTOR_EFFECTS_RANGE_TOUCH,
	ACTOR_EFFECTS_RANGE_SELF,
	UNKNOWN,
	ACTORS_ANY
};

struct PackageTarget {
	PackageTargetType type;
	uint32_t 		  target;
	int32_t			  count_distance;
	float 			  unknown;
};

enum PackageIdleAnimationFlags : uint8_t {
	RUN_IN_SEQUENCE = 0x01,
	DO_ONCE = 0x04
};

enum class UseWeaponFlags : uint32_t {
	ALWAYS_HIT = 0x00000001,
	DO_NO_DAMAGE = 0x00000100,
	CROUCH_TO_RELOAD = 0x00010000,
	HOLD_FIRE_WHEN_BLOCKED = 0x01000000
};

enum class FireRate : uint8_t {
	AutoFire = 0,
	VolleyFire = 1
};

enum class FireCount : uint8_t {
	NumberOfBursts = 0,
	RepeatFire = 1
};

struct UseWeaponData {
	UseWeaponFlags flags;
	FireRate       fireRate;
	FireCount      fireCount;
	uint16_t 	   numOfBursts;
	uint16_t       minShotsPerVolley;
	uint16_t       maxShotsPerVolley;
	float          minPauseBetweenVolleys;
	float          maxPauseBetweenVolleys;
	uint8_t		   padding[4];
};

enum class PackageDialogueFlags : uint32_t {
	NO_HEADTRACKING = 0x00000001,
	DONT_CONTROL_TARGET_MOVEMENT = 0x00000100
};

enum class PackageDialogueType : uint32_t {
	SayTosation = 0, //what the fuck
	SayTo = 1
};

struct PackageDialogue {
	float fov;
	formid topic; //DIAL
	PackageDialogueFlags flags;
	uint8_t  padding[4];
	PackageDialogueType type;
	uint32_t unknown;
};

enum class PackageLocationType : uint32_t {
	NearReference = 0,
	InCell = 1,
	NearCurrentLocation = 2,
	NearEditorLocation = 3,
	ObjectId = 4,
	ObjectType = 5,
	NearLinkedReference = 6,
	AtPackageLocation = 7
};

struct PackageLocation { 
	PackageLocationType type;
	uint32_t 			location;
	int32_t 			radius;
};

struct Package : public Record {

	PackageData               data;
	PackageLocation 		  location_1;
	PackageLocation 		  location_2; //navodno moze biti na 2 razlicita mjesta u recordu
	PackageSchedule           schedule;
	PackageTarget   		  target_1;
	std::vector<Condition>    conditions;
	PackageIdleAnimationFlags idleAnimationFlags;
	uint32_t  				  idleAnimationCount; //4 bytes in file, only first byte used
	float 					  idleTimerSetting;
	std::vector<formid>		  idleAnimations; //IDLE
	uint32_t				  unused;
	formid					  combatStyle; //CSTY
	//eat marker
	bool 					  eat;
	uint32_t				  escortDistance;
	float 					  followTriggerRadius;
	uint16_t				  patrolFlags;
	UseWeaponData			  useWeaponData;
	PackageTarget 			  target_2;
	//use item marker
	bool useItem;
	//ambush marker
	bool ambush;
	PackageDialogue  		  dialogue;
	//onbegin marker
	formid 				      onBeginIdle; //IDLE
	ScriptData 				  onBeginScript;
	formid					  onBeginTopic; //DIAL
	//onendmarker
	formid 				      onEndIdle; //IDLE
	ScriptData 				  onEndScript;
	formid					  onEndTopic; //DIAL
	//onchangemarker
	formid 				      onChangeIdle; //IDLE
	ScriptData 				  onChangeScript;
	formid					  onChangeTopic; //DIAL

	Package(ESMReader& reader);

private:
	static void loadIdleScriptTopicGroup(ESMReader& reader, formid& idle, ScriptData& script, formid& topic);
};

};