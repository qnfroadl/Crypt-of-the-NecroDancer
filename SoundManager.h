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

	// Bgm
	LOBBY,

	ZONE_1_1,
	ZONE_1_2,
	ZONE_1_3,

	ZONE_2_1,
	ZONE_2_2,
	ZONE_2_3,

	// Effect

};

class SoundManager : public Singleton<SoundManager>
{
private:
	FMOD::System* system;
	map<ESoundKey, FMOD::Sound*> mapSounds;

	FMOD::Channel* channelBgm;
	FMOD::Channel* channelBgmShopkeeper;
	FMOD::ChannelGroup* channelGroupEffect;

public:
	void Init();
	void Release();
	void Update();

	FMOD::Sound* AddSound(ESoundKey key, const char* filePath, bool loop=false);
	FMOD::Sound* FindSound(ESoundKey key);

	void PlaySoundEffect(ESoundKey effect);
	void PlaySoundBgm(ESoundKey mainBgm, ESoundKey shopkeeperBgm=ESoundKey::NONE);

	// volume 0.0 ~ 1.0
	void ChangeVolumeBgm(float volume);
	void ChangeVolumeEffect(float volume);
};