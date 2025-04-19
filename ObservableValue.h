#pragma once

#include <functional>

template<typename T>
class ObservableValue 
{
private:
	// T는 기본형이어야 한다.
	T preValue;
	T value;

	std::function<void(const T& preValue, const T& value)> onChanged;

public:
	void Set(const T& v) 
	{
		if (v != value) 
		{
			preValue = value;
			value = v;
			if (onChanged) 
			{
				onChanged(preValue, value);
			}
		}
	}

	T Get() const { return value; }

	void Bind(std::function<void(const T& preValue, const T& value)> f) 
	{ 
		onChanged = f; 
	}

};