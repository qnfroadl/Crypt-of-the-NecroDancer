#include "SoundManager.h"

void SoundManager::Init()
{
	FMOD::System_Create(&system);
	system->init(512, FMOD_INIT_NORMAL, nullptr);
	system->createChannelGroup("channelGroupEffect", &channelGroupEffect);

	AddSound(ESoundKey::LOBBY, "Sound/Bgm/lobby.ogg", true);

	AddSound(ESoundKey::ZONE_1_1, "Sound/Bgm/zone1_1.ogg");
	AddSound(ESoundKey::ZONE_1_2, "Sound/Bgm/zone1_2.ogg");
	AddSound(ESoundKey::ZONE_1_3, "Sound/Bgm/zone1_3.ogg");

	AddSound(ESoundKey::ZONE_2_1, "Sound/Bgm/zone2_1.ogg");
	AddSound(ESoundKey::ZONE_2_2, "Sound/Bgm/zone2_2.ogg");
	AddSound(ESoundKey::ZONE_2_3, "Sound/Bgm/zone2_3.ogg");
}

void SoundManager::Release()
{
	for (auto& iter : mapSounds)
	{
		if (iter.second)
		{
			iter.second->release();

		}
	}

	system->release();

	ReleaseInstance();
}

void SoundManager::Update()
{
	system->update();
}

FMOD::Sound* SoundManager::AddSound(ESoundKey key, const char* filePath, bool loop)
{
	FMOD::Sound* sound = FindSound(key);
	if (nullptr != sound)
	{
		return sound;
	}

	FMOD_MODE mode = loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;

	system->createSound(filePath, mode, nullptr, &sound);
	if (mapSounds[key] != nullptr)
	{
		mapSounds[key]->release();
	}
	mapSounds[key] = sound;

}

FMOD::Sound* SoundManager::FindSound(ESoundKey key)
{
	map<ESoundKey, FMOD::Sound*>::iterator it = mapSounds.find(key);
	if (it == mapSounds.end())
	{
		return nullptr;
	}
	return it->second;
}

void SoundManager::PlaySoundEffect(ESoundKey effect)
{
	FMOD::Sound* sound = FindSound(effect);
	if (sound)
	{
		FMOD::Channel* channelEffects;
		system->playSound(sound, channelGroupEffect, false, &channelEffects);
	}
}

void SoundManager::PlaySoundBgm(ESoundKey mainBgm, ESoundKey shopkeeperBgm)
{
	if (channelBgm)
	{
		channelBgm->stop();
	}

	if (channelBgmShopkeeper)
	{
		channelBgmShopkeeper->stop();
	}

	FMOD::Sound* sound = FindSound(mainBgm);
	if (sound)
	{
		system->playSound(sound, nullptr, false, &channelBgm);
	}

	sound = FindSound(shopkeeperBgm);
	if (sound)
	{
		system->playSound(sound, nullptr, false, &channelBgmShopkeeper);
	}
}

void SoundManager::ChangeVolumeBgm(float volume)
{
	channelBgm->setVolume(volume);
	channelBgmShopkeeper->setVolume(volume);
}

void SoundManager::ChangeVolumeEffect(float volume)
{
	channelGroupEffect->setVolume(volume);
}
