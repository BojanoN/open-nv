#pragma once
#include "esm/reader.hpp"
#include "esm/records.hpp"
#include "esm/types.hpp"

#include "cellblock.hpp"
#include "gamedata.hpp"

#include <iostream>
#include <sstream>
#include <unordered_map>
#include <utility>

namespace GameWorld {

class GameWorld {
    // TODO: sve ostalo
    GameData<ESM::GameSetting>      gameSettings;
    GameData<ESM::TextureSet>       textureSets;
    GameData<ESM::MenuIcon>         menuIcons;
    GameData<ESM::GlobalVariable>   globalVariables;
    GameData<ESM::Class>            classes;
    GameData<ESM::Faction>          factions;
    GameData<ESM::HeadPart>         headParts;
    GameData<ESM::Hair>             hairs;
    GameData<ESM::Eyes>             eyes;
    GameData<ESM::Race>             races;
    GameData<ESM::Sound>            sounds;
    GameData<ESM::AcousticSpace>    acousticSpaces;
    GameData<ESM::MagicEffect>      magicEffects;
    GameData<ESM::Script>           scripts;
    GameData<ESM::LandscapeTexture> landscapeTextures;
    GameData<ESM::ObjectEffect>     objectEffects;
    GameData<ESM::ActorEffect>      actorEffects;
    GameData<ESM::Activator>        activators;
    GameData<ESM::TalkingActivator> talkingActivators;
    GameData<ESM::Terminal>         terminals;
    GameData<ESM::Armor>            armors;
    GameData<ESM::Book>             books;
    GameData<ESM::Container>        containers;
    GameData<ESM::Door>             doors;
    GameData<ESM::Ingredient>       ingredients;
    GameData<ESM::Light>            lights;
    GameData<ESM::MoveableStatic>   moveableStatics;
    GameData<ESM::PlaceableWater>   placeableWaters;
    GameData<ESM::Tree>             trees;
    //GameData<ESM::Weapon>           weapons;
    GameData<ESM::Item>             miscItems;
    GameData<ESM::Static>           statics;
    GameData<ESM::StaticCollection> staticCollections;
    GameData<ESM::Grass>            grasses;
    GameData<ESM::Furniture>        furnitures;
    GameData<ESM::Ammo>             ammunitions;
    GameData<ESM::NPC>              NPCs;
    GameData<ESM::Creature>         creatures;
    GameData<ESM::LeveledActor>     leveledCreatures;
    GameData<ESM::LeveledActor>     leveledNPCs;
    GameData<ESM::Item>             keys;
    GameData<ESM::Ingestible>       ingestibles;
    GameData<ESM::IdleMarker>       idleMarkers;
    GameData<ESM::Note>             notes;
    // COBJ is nonexistent
    // GameData<ESM::ConstructibleObject>
    GameData<ESM::Projectile>  projectiles;
    GameData<ESM::LeveledItem> leveledItems;
    GameData<ESM::Weather>     weather;
    GameData<ESM::Climate>     climates;
    GameData<ESM::Region>      regions;
    // GameData<ESM::NavigationMeshInfoMap>
    GameData<ESM::Worldspace> worldspaces;

    GameData<ESM::DialogueTopic>           dialogueTopics;
    GameData<ESM::Landscape>               landscapes;
    GameData<ESM::Quest>                   quests;
    GameData<ESM::IdleAnimation>           idleAnimations;
    GameData<ESM::Package>                 packages;
    GameData<ESM::AnimatedObject>          animatedObjects;
    GameData<ESM::CombatStyle>             combatStyles;
    GameData<ESM::LoadingScreen>           loadingScreens;
    GameData<ESM::EffectShader>            effectShaders;
    GameData<ESM::Explosion>               explosions;
    GameData<ESM::Water>                   waters;
    GameData<ESM::Debris>                  debris;
    GameData<ESM::ImageSpace>              imageSpaces;
    GameData<ESM::ImageSpaceAdapter>       imageSpaceAdapters;
    GameData<ESM::BodyPartData>            bodyParts;
    GameData<ESM::AddonNode>               addonNodes;
    GameData<ESM::ActorValueInformation>   actorValueInformations;
    GameData<ESM::RadiationStage>          radiationStages;
    GameData<ESM::CameraShot>              cameraShots;
    GameData<ESM::CameraPath>              cameraPaths;
    GameData<ESM::VoiceType>               voiceTypes;
    GameData<ESM::Impact>                  impacts;
    GameData<ESM::ImpactDataset>           impactDatasets;
    GameData<ESM::ArmorAddon>              armorAddons;
    GameData<ESM::EncounterZone>           encounterZones;
    GameData<ESM::Message>                 messages;
    GameData<ESM::Ragdoll>                 ragdolls;
    GameData<ESM::DefaultObjectManager>    defaultObjectManagers;
    GameData<ESM::LightingTemplate>        lightingTemplates;
    GameData<ESM::MusicType>               musicTypes;
    GameData<ESM::ItemMod>                 itemMods;
    GameData<ESM::Reputation>              reputations;
    GameData<ESM::Recipe>                  recipes;
    GameData<ESM::RecipeCategory>          recipeCategories;
    GameData<ESM::CasinoChip>              casinoChips;
    GameData<ESM::FormIDList>              formIDlists;
    GameData<ESM::Perk>                    perks;
    GameData<ESM::Casino>                  casinos;
    GameData<ESM::LoadingScreenType>       loadingScreenTypes;
    GameData<ESM::MediaSet>                mediaSets;
    GameData<ESM::MediaLocationController> mediaLocationControllers;
    GameData<ESM::Challenge>               challenges;
    GameData<ESM::AmmoEffect>              ammoEffects;
    GameData<ESM::CaravanCard>             caravanCards;
    GameData<ESM::CaravanMoney>            caravanMoney;
    GameData<ESM::CaravanDeck>             caravanDecks;
    GameData<ESM::DehydrationStage>        dehydrationStages;
    // GameData<ESM::HungerStage>
    // GameData<ESM::SleepDeprivationStage>
    // GameData<ESM::World>

    GameData<ESM::Cell> interiorCells;
    GameData<ESM::Cell> exteriorCells;

    //std::unordered_map<uint32_t, std::unordered_set<formid>> interiorCellBlocks;
    //std::unordered_map<uint32_t, std::unordered_set<formid>> interiorCellSubBlocks;
    std::unordered_map<uint32_t, CellBlock> interiorCellBlocks;

    //std::unordered_map<uint32_t, std::unordered_map<formid, CellChildren*>*>         cellChildrenTypeMap;
    //std::unordered_map<formid, CellChildren*> persistentChildren;
    //std::unordered_map<formid, CellChildren*> temporaryChildren;
    //std::unordered_map<formid, CellChildren*> visibleDistantChildren;

    //std::unordered_map<formid, WorldChildren*> worldChildren;

    std::unordered_map<uint32_t, GameDataBase*> dataStores;
    GameDataBase*                               getDataStore(uint32_t type);
    //std::unordered_map<formid, CellChildren*>&  getCellChildrenMap(uint32_t groupType);

    void initDataStoreMap();
    void parseCellGroup(ESM::ESMReader& reader);
    //void loadCellChildren(ESM::ESMReader& reader, formid cellId, uint32_t& recordsLoaded, uint32_t& recordsSkipped);
    void loadCellChildren(ESM::ESMReader& reader, ESM::Cell& cellId, uint32_t& recordsLoaded, uint32_t& recordsSkipped);

    void parseWorldspaceGroup(ESM::ESMReader& reader);

public:
    GameWorld()
    {
        initDataStoreMap();
    }

    void load(ESM::ESMReader& reader);
};

};
