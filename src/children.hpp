#pragma once

#include "gamedata.hpp"
#include "esm/records.hpp"
#include "esm/reader.hpp"

namespace ESM {

class CellChildren {

	GameData<ESM::PlacedObject> placedObjects; 		//REFR
	GameData<ESM::PlacedCreature> placedCreatures;  //ACRE
	GameData<ESM::PlacedGrenade> placedGrenades; 	//PGRE
	GameData<ESM::PlacedMissile> placedMissiles; 	//PMIS
	GameData<ESM::PlacedNPC> placedNPCs; 			//ACHR
	GameData<ESM::NavigationMesh> navigationMeshes; //NAVM

	std::unordered_map<uint32_t, GameDataBase*> typeMap;

public:
	CellChildren();
	void load(ESMReader& reader);
}

};