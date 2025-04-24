#pragma once

#include "config.h"
#include "Singleton.h"
#include <map>
#include "fmod.hpp"
#include "fmod_errors.h"

#pragma comment(lib, "fmodL_vc.lib")
#pragma comment(lib, "fmod_vc.lib")

enum class ESoundKey
{
	NONE,

	INTRO,
	MAINMENU,

	// Bgm
	LOBBY,

	ZONE1_1,
	ZONE1_2,
	ZONE1_3,

	ZONE2_1,
	ZONE2_2,
	ZONE2_3,

	// BgmShopkeeper
	ZONE1_1_SHOPKEEPER_M,
	ZONE1_1_SHOPKEEPER,
	ZONE1_2_SHOPKEEPER_M,
	ZONE1_2_SHOPKEEPER,
	ZONE1_3_SHOPKEEPER_M,
	ZONE1_3_SHOPKEEPER,

	ZONE2_1_SHOPKEEPER_M,
	ZONE2_1_SHOPKEEPER,
	ZONE2_2_SHOPKEEPER_M,
	ZONE2_2_SHOPKEEPER,
	ZONE2_3_SHOPKEEPER_M,
	ZONE2_3_SHOPKEEPER,

	// Effect
	MOV_DIG_FAIL,
};

extern map<ESoundKey, string> bgmPath;

class SoundManager : public Singleton<SoundManager>
{
private:
	FMOD::System* system;
	map<ESoundKey, FMOD::Sound*> mapSounds;

	FMOD::Channel* channelBgm;
	FMOD::Channel* channelBgmShopkeeper;
	FMOD::ChannelGroup* channelGroupEffect;
	float bgmVolume;
	float effectVolume;

	ESoundKey keyBgm;
	ESoundKey keyBgmShopkeeper;

	float tempo;
	float elapsedTime;
	float maxTempoChangeTime;

public:
	void Init();
	void Release();
	void Update();

	void LoadSound();

	FMOD::Sound* AddSound(ESoundKey key, const char* filePath, bool loop=false);
	FMOD::Sound* FindSound(ESoundKey key);

	void PlaySoundEffect(ESoundKey effect);
	void PlaySoundBgm(ESoundKey mainBgm, ESoundKey shopkeeperBgm=ESoundKey::NONE);
	void ChangeSoundBgmShopkeeper();

	// volume 0.0 ~ 1.0
	void ChangeVolumeBgm(float volume);
	void ChangeVolumeEffect(float volume);

	ESoundKey GetCurrentKeyBgm();
	unsigned int GetBgmPosition();
	bool IsBgmEnd();

	void SetTempo(float _tempo);
	void RecoverTempo();
	float GetTempo() { return tempo; }
};