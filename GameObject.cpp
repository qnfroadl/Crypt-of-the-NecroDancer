#include "GameObject.h"

HRESULT GameObject::Init()
{
    return S_OK;
}

void GameObject::Release()
{

}

void GameObject::Update()
{
}

void GameObject::Render(HDC hdc)
{
}

void GameObject::SetActive(bool _bActive)
{
    this->bActive = _bActive;
}

bool GameObject::IsActive()
{
    return this->bActive;
}

GameObject::GameObject()
    :bActive(true)
{
}

GameObject::~GameObject()
{
    Release();
}

void GameObject::AddTag(GameTag tag)
{
    this->tags.insert(tag);
}

void GameObject::AddTags(const unordered_set<GameTag>& tags)
{
    for (const auto& tag : tags)
    {
        AddTag(tag);
    }
}

const unordered_set<GameTag>& GameObject::GetTags()
{
    return this->tags;
}

bool GameObject::FindTag(GameTag tag)
{
    if (0 < this->tags.count(tag))
    {
        return true;
    }

    return false;
}
