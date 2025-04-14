#include "ImageManager.h"

#include "Image.h"

void ImageManager::Init()
{

}

void ImageManager::Release()
{
    Image* image = nullptr;
    map<string, Image*>::iterator it = mapImages.begin();
    while (it != mapImages.end())
    {
        image = it->second;
        image->Release();
        delete image;

        it++;
    }

    Singleton::ReleaseInstance();
}

Image* ImageManager::AddImage(const string& key, const wchar_t* filePath, int width, int height, bool isTransparent, COLORREF transColor)
{
    Image* image = FindImage(key);
    if (nullptr != image)
    {
        return image;
    }

    image = new Image();

    if (S_OK == image->Init(filePath, width, height, isTransparent, transColor))
    {
        if (this->mapImages[key] != nullptr)
        {
            this->mapImages[key]->Release();
            delete this->mapImages[key];
        }
        this->mapImages[key] = image;

        return image;
    }
    
    delete image;
    return nullptr;
}

Image* ImageManager::AddImage(const string& key, const wchar_t* filePath, 
    int width, int height, int maxFrameX, int maxFrameY, bool isTransparent, COLORREF transColor)
{
    Image* image = FindImage(key);
    if (nullptr != image)
    {
        return image;
    }

    image = new Image();
    if (S_OK == image->Init(filePath, width, height, maxFrameX, maxFrameY, isTransparent, transColor))
    {
        if (this->mapImages[key] != nullptr)
        {
            this->mapImages[key]->Release();
            delete this->mapImages[key];
        }
        this->mapImages[key] = image;

        return image;
    }

    delete image;
    return nullptr;

}

void ImageManager::DeleteImage(const string& key)
{
    map<string, Image*>::iterator it = mapImages.find(key);
    if (it == mapImages.end())
    {
        return;
    }
    it->second->Release();
    delete it->second;
    mapImages.erase(key);

}

Image* ImageManager::FindImage(const string& key)
{
    map<string, Image*>::iterator it = mapImages.find(key);
    if (it == mapImages.end())
    {
        return nullptr;
    }
    return it->second;
}

Image* ImageManager::AddImage(EImageKey key, const wchar_t* filePath, int width, int height, bool isTransparent, COLORREF transColor)
{
    string keyString = std::to_string((int)key);
    return this->AddImage(keyString, filePath, width, height, isTransparent, transColor);
}

Image* ImageManager::AddImage(EImageKey key, const wchar_t* filePath, int width, int height, int maxFrameX, int maxFrameY, bool isTransparent, COLORREF transColor)
{
    string keyString = std::to_string((int)key);
    return this->AddImage(keyString, filePath, width, height, maxFrameX, maxFrameY, isTransparent, transColor);
}

void ImageManager::DeleteImage(EImageKey key)
{
    string keyString = std::to_string((int)key);
    this->DeleteImage(keyString);
}

Image* ImageManager::FindImage(EImageKey key)
{
    string keyString = std::to_string((int)key);
    return this->FindImage(keyString);
}
