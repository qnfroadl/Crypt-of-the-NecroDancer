#pragma once
#include <random>
#include "Singleton.h"
#include "GameActor.h"
#include "config.h"

class Player;
class Camera : public Singleton<Camera>, public GameActor
{
private:
    weak_ptr<Player> wptTarget;    // 추적할 대상의 좌표 (ex. 플레이어 위치 포인터)
    SIZE  viewSize;                     // 화면 크기 (보이는 영역)

	float shakeDuration; // 흔들림 지속 시간
	int shakeIntensity; // 흔들림 강도

    default_random_engine randEngine;
    uniform_real_distribution<float> randOffset{ -1.0f, 1.0f };

public:
    Camera();
    Camera(int width, int height);
    ~Camera();

    void SetTarget(weak_ptr<Player> _target);     // 타겟 설정
    void SetSize(SIZE size);
    SIZE GetSize();
    void Update();                      // 타겟 위치 따라 카메라 갱신
    RECT GetViewRect();          // 현재 뷰 영역 (카메라 기준 사각형)

	FPOINT GetScreenPos(const FPOINT& worldPos); // 카메라 기준 좌표

	void Shake(float duration, int intensity); // 시간, 강도 카메라 흔들기
};
