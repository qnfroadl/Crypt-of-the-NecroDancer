#pragma once

template <typename T>
class Singleton
{
private:
	// 복사 생성자와 대입 연산자를 private으로 선언하여 외부에서 복사할 수 없도록 함.
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

protected:
	static T* instance;
	
	Singleton(){};
	~Singleton(){};
	

public:
	static T* GetInstance();
	void ReleaseInstance();

};

/*
	링크 - 컴파일 한 목적파일과 외부 파일들 연결해서 최종적인 실행파일을 만드는 과정인데.
	이 링크 과정에서 중복 정의를 방지, 각 템플릿 인스턴스에 대한 스태틱 변수가 고유하게 존재하도록 보장.
*/
template<typename T>
T* Singleton<T>::instance = nullptr;

template<typename T>
inline T* Singleton<T>::GetInstance()
{
	if (nullptr == instance)
	{
		instance = new T();
	}
	return instance;
}

template<typename T>
inline void Singleton<T>::ReleaseInstance()
{
	if (nullptr != instance)
	{
		delete instance;
		instance = nullptr;
	}
}
