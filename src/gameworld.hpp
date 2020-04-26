#pragma once
#include "esm/reader.hpp"
#include "esm/records.hpp"
#include "esm/types.hpp"
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
    GameData<ESM::Projectile> projectiles;
    // GameData<ESM::LeveledItem>
    // GameData<ESM::Weather>
    // GameData<ESM::Climate>
    // GameData<ESM::Region>
    // GameData<ESM::NavigationMeshInfoMap>
    // GameData<ESM::DialogueTopic>
    // GameData<ESM::Quest>
    // GameData<ESM::IdleAnimation>
    // GameData<ESM::Package>
    // GameData<ESM::CombatStyle>
    // GameData<ESM::LoadScreen>
    // GameData<ESM::AnimatedObject>
    // GameData<ESM::Water>
    // GameData<ESM::EffectShader>
    // GameData<ESM::Explosion>
    // GameData<ESM::Debris>
    // GameData<ESM::ImageSpace>
    // GameData<ESM::ImageSpaceAdapter>
    // GameData<ESM::FormIdList>
    // GameData<ESM::Perk>
    // GameData<ESM::BodyPartData>
    // GameData<ESM::AddonNode>
    // GameData<ESM::ActorValueInformation>
    // GameData<ESM::RadiationStage>
    // GameData<ESM::CameraShot>
    // GameData<ESM::CameraPath>
    // GameData<ESM::VoiceType>
    // GameData<ESM::Impact>
    // GameData<ESM::ImpactDataset>
    // GameData<ESM::ArmorAddon>
    // GameData<ESM::EncounterZone>
    // GameData<ESM::Message>
    // GameData<ESM::Ragdoll>
    // GameData<ESM::DefaultObjectManager>
    // GameData<ESM::LightingTemplate>
    // GameData<ESM::MusicType>
    // GameData<ESM::ItemMod>
    // GameData<ESM::Reputation>
    // GameData<ESM::Recipe>
    // GameData<ESM::RecipeCategory>
    // GameData<ESM::CasinoChip>
    // GameData<ESM::LoadScreenType>
    // GameData<ESM::MediaSet>
    // GameData<ESM::MediaLocationController>
    // GameData<ESM::Challenge>
    // GameData<ESM::AmmoEffect>
    // GameData<ESM::CaravanCard>
    // GameData<ESM::CaravanMoney>
    // GameData<ESM::CaravanDeck>
    // GameData<ESM::DehydrationStage>
    // GameData<ESM::HungerStage>
    // GameData<ESM::SleepDeprivationStage>
    // GameData<ESM::Cell>
    // GameData<ESM::World>

    std::unordered_map<uint32_t, GameDataBase*> dataStores;
    GameDataBase*                               getDataStore(uint32_t type);

    void initDataStoreMap();

public:
    GameWorld()
    {
        initDataStoreMap();
    }

    void load(ESM::ESMReader& reader);
};

};
