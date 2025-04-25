#include "SoundManager.h"
#include "TimerManager.h"

#include <mutex>

map<ESoundKey, string> bgmPath = {
	{ESoundKey::LOBBY, "Sound/Bgm/lobby"},
	{ESoundKey::ZONE1_1, "Sound/Bgm/zone1_1"},
	{ESoundKey::ZONE1_2, "Sound/Bgm/zone1_2"},
	{ESoundKey::ZONE1_3, "Sound/Bgm/zone1_3"},
	{ESoundKey::ZONE2_1, "Sound/Bgm/zone2_1"},
	{ESoundKey::ZONE2_2, "Sound/Bgm/zone2_2"},
	{ESoundKey::ZONE2_3, "Sound/Bgm/zone2_3"},
};

void SoundManager::Init()
{
	FMOD::System_Create(&system);
	system->init(512, FMOD_INIT_NORMAL, nullptr);
	system->createChannelGroup("channelGroupEffect", &channelGroupEffect);

	bgmVolume = 1.f;
	effectVolume = 1.f;

	tempo = 1.f;
	elapsedTime = 0.f;
	maxTempoChangeTime = 5.f;
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

	channelGroupEffect->release();

	system->release();

	ReleaseInstance();
}

void SoundManager::Update()
{
	system->update();

	elapsedTime += TimerManager::GetInstance()->GetDeltaTime();
	if (elapsedTime > maxTempoChangeTime)
	{
		RecoverTempo();
	}
}

void SoundManager::LoadSound()
{
	std::mutex sound_mutex;
	std::lock_guard<std::mutex> lock(sound_mutex);

	AddSound(ESoundKey::LOBBY, "Sound/Bgm/lobby.ogg", true);

	AddSound(ESoundKey::ZONE1_1, "Sound/Bgm/zone1_1.ogg");
	AddSound(ESoundKey::ZONE1_2, "Sound/Bgm/zone1_2.ogg");
	AddSound(ESoundKey::ZONE1_3, "Sound/Bgm/zone1_3.ogg");

	//AddSound(ESoundKey::ZONE2_1, "Sound/Bgm/zone2_1.ogg");
	//AddSound(ESoundKey::ZONE2_2, "Sound/Bgm/zone2_2.ogg");
	//AddSound(ESoundKey::ZONE2_3, "Sound/Bgm/zone2_3.ogg");

	AddSound(ESoundKey::ZONE1_1_SHOPKEEPER_M, "Sound/Bgm/ShopKeeperVocal/zone1_1_shopkeeper_m.ogg");
	AddSound(ESoundKey::ZONE1_2_SHOPKEEPER_M, "Sound/Bgm/ShopKeeperVocal/zone1_2_shopkeeper_m.ogg");
	AddSound(ESoundKey::ZONE1_3_SHOPKEEPER_M, "Sound/Bgm/ShopKeeperVocal/zone1_3_shopkeeper_m.ogg");
	AddSound(ESoundKey::ZONE1_1_SHOPKEEPER, "Sound/Bgm/ShopKeeperVocal/zone1_1_shopkeeper.ogg");
	AddSound(ESoundKey::ZONE1_2_SHOPKEEPER, "Sound/Bgm/ShopKeeperVocal/zone1_2_shopkeeper.ogg");
	AddSound(ESoundKey::ZONE1_3_SHOPKEEPER, "Sound/Bgm/ShopKeeperVocal/zone1_3_shopkeeper.ogg");

	//AddSound(ESoundKey::ZONE2_1_SHOPKEEPER_M, "Sound/Bgm/ShopKeeperVocal/zone2_1_shopkeeper_m.ogg");
	//AddSound(ESoundKey::ZONE2_2_SHOPKEEPER_M, "Sound/Bgm/ShopKeeperVocal/zone2_2_shopkeeper_m.ogg");
	//AddSound(ESoundKey::ZONE2_3_SHOPKEEPER_M, "Sound/Bgm/ShopKeeperVocal/zone2_3_shopkeeper_m.ogg");
	//AddSound(ESoundKey::ZONE2_1_SHOPKEEPER, "Sound/Bgm/ShopKeeperVocal/zone2_1_shopkeeper.ogg");
	//AddSound(ESoundKey::ZONE2_2_SHOPKEEPER, "Sound/Bgm/ShopKeeperVocal/zone2_2_shopkeeper.ogg");
	//AddSound(ESoundKey::ZONE2_3_SHOPKEEPER, "Sound/Bgm/ShopKeeperVocal/zone2_3_shopkeeper.ogg");

	AddSound(ESoundKey::MOV_DIG_FAIL, "Sound/Effect/mov_dig_fail.ogg");
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
		system->playSound(sound, channelGroupEffect, false, nullptr);
	}

	ChangeVolumeEffect(effectVolume);
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
		keyBgm = mainBgm;
	}

	sound = FindSound(shopkeeperBgm);
	if (sound)
	{
		system->playSound(sound, nullptr, false, &channelBgmShopkeeper);
		keyBgmShopkeeper = shopkeeperBgm;
	}

	ChangeVolumeBgm(bgmVolume);
}

void SoundManager::ChangeSoundBgmShopkeeper()
{
	if (channelBgmShopkeeper)
	{
		bool isPlaying{};
		channelBgmShopkeeper->isPlaying(&isPlaying);
		if (isPlaying)
		{
			unsigned int position;
			channelBgmShopkeeper->getPosition(&position, FMOD_TIMEUNIT_MS);
			channelBgmShopkeeper->stop();
			ESoundKey nextKey = ESoundKey(static_cast<int>(keyBgmShopkeeper) + 1);
			FMOD::Sound* sound = FindSound(nextKey);
			if (sound)
			{
				system->playSound(sound, nullptr, false, &channelBgmShopkeeper);
				channelBgmShopkeeper->setPosition(position, FMOD_TIMEUNIT_MS);
				
				ChangeVolumeBgm(bgmVolume);
			}
		}
	}
}

void SoundManager::ChangeVolumeBgm(float volume)
{
	bgmVolume = volume;
	channelBgm->setVolume(bgmVolume);
	channelBgmShopkeeper->setVolume(bgmVolume);
}

void SoundManager::ChangeVolumeEffect(float volume)
{
	effectVolume = volume;
	channelGroupEffect->setVolume(effectVolume);
}

ESoundKey SoundManager::GetCurrentKeyBgm()
{
	return keyBgm;
}

unsigned int SoundManager::GetBgmPosition()
{
	if (channelBgm && !IsBgmEnd())
	{
		unsigned int position{};
		channelBgm->getPosition(&position, FMOD_TIMEUNIT_MS);
		return position;
	}
	return 0;
}

bool SoundManager::IsBgmEnd()
{
	if (channelBgm)
	{
		bool isPlaying{};
		channelBgm->isPlaying(&isPlaying);
		return !isPlaying;
	}
	return true;
}

void SoundManager::SetTempo(float _tempo)
{
	tempo = _tempo;
	elapsedTime = 0.f;

	if (channelBgm)
	{
		channelBgm->setPitch(tempo);
	}

	if (channelBgmShopkeeper)
	{
		channelBgmShopkeeper->setPitch(tempo);
	}
}

void SoundManager::RecoverTempo()
{
	tempo = 1.f;

	if (channelBgm)
	{
		channelBgm->setPitch(tempo);
	}

	if (channelBgmShopkeeper)
	{
		channelBgmShopkeeper->setPitch(tempo);
	}
}
