## ▶️ 게임 개요

- **게임 제목**: 크립트 오브 더 네크로댄서
- **장르**: 리듬 로그라이크 액션
- **플랫폼**: Windows (WinAPI 기반 C++)
- **그래픽 방식**: 2D 타일 기반
- **주요 특징**:
    - 음악 비트에 맞춘 플레이
    - 랜덤 던전 생성
    - 시야에 따른 타일/몬스터 표시
    - 코인 획득 → 상점 소비

---

## 🧩 게임 선정 이유

- **비트 기반 게임 플레이 구현**
    
    → 리듬에 맞춘 입력 및 타이밍 기반 시스템 설계 필요
    
- **2D 타일 기반 탑뷰 시점 구성**
    
    → 시야 처리 및 충돌, 타일 단위 맵 제어 구현 중심
    
- **라이팅 시스템 통한 시야 제한/표현**
    
    → 조명 범위 외 투명도 처리로 몰입감 증가
    
- **프리팹 기반 동적 맵 생성 시스템**
    
    → 시드 기반의 구조적 랜덤 던전 구성 학습
    
- **기존 프로젝트에서 다뤄보지 않았던 기능 다수 포함**
    
    → 개발 난이도 및 재미 요소 모두 고려하여 선정
    

---

## ⚙️ 게임 시스템

### 🔽 1. 기본 시스템

- 이동 방식: 4방향, 한 칸 단위, 비트 동기화
- 공격 시스템: 근접/원거리 무기, 비트 타이밍
- 콤보 시스템: 연속 성공 시 코인 획득 배수 증가
- 시야 처리: 광원 범위 밖 타일/적 반투명

### 🔽 2. 리듬 시스템

- BPM 분석: 음원파일 PCM 변환 후 분석
- 음원 재생속도 변환 / 템포 조절
- 사용자 음악 지원: wav, (mp3)

### 🔽 3. 맵 시스템

- 랜덤 맵 생성 (시드 기반)
- 프리팹 블록 조합 방식
- 에디터로 타일 / 벽 / 함정 배치 가능
- 상점, 보물방, 출구방 등 룸 타입 구분

### 🔽 4. 몬스터/AI

- AI 경로탐색: BFS / A*
- 업데이트 조건: 카메라 시야 내에서만 처리
- 특수 패턴: 공격 대기, 범위 공격, 방패

### 🔽 5. UI/UX

- 체력 / 콤보 / 코인 UI
- 리듬 타일 강조 (색상 / 애니메이션)
- 아이템 설명 오버레이 (투명 배경)

---

📅 개발 일정표 (2주 단기 프로젝트)

| 기간 | 작업 항목 | 비고 |
| --- | --- | --- |
| **1주차** |  |  |
| 화 ~ 수 | - 기획서 마무리 및 전체 구조 설계
- 공통 리소스 및 프로젝트 세팅 | 협업 준비 및 기획, 클래스 설계 |
| 수 ~ 2주차 수 | - 각 담당자 기능 구현. | 자세한 일정은 엑셀에 표기 |
| 목 | - 최종 테스트 및 릴리즈
- 발표 자료 준비  | 게임플레이 완성,
시연, 영상 or 문서 정리 |
| 금 (마감) | - 발표 준비 |  |

---

## 👥 역할 분담 (4인 기준)

### 🔽 김미현 - 음원 분석 / 비트 기반 동작 시스템 / 메뉴,레벨 UI 구현

- 음원 파일 BPM 분석
- 타이밍 동기화 및 Beat 시스템
- 메뉴, 레벨 UI 구현 (1인, 2인 코옵)
- 이미지 투명화 기능

### 🔽 문경원 - 캐릭터 행동 / 라이팅 시스템

- 이동 / 공격 / 아이템 획득 로직
- 카메라 기능 구현 (1인, 2인 코옵)
- 시야 및 광원(횃불 / 야광버섯) 라이팅 시스템
- 이벤트 처리 시스템

### 🔽 이주현 - 몬스터 행동 / 보스 / 몬스터 랜덤배치

- 일반 / 보스 몬스터 AI
- 추적 / 공격 패턴 설계
- 시드 기반 맵 전체 배치 로직 구현

### 🔽 차승근 - 맵 제작 툴 / 벽돌 파괴 기능

- 타일맵 에디터 제작
- 벽 종류 구현 (일반, 튼튼, 파괴 가능)
- 맵 파싱 및 저장/불러오기

---

## 🛠 기술 스택

- **언어**: C++
- 개발 툴: Visual Studio 2022
- **API**: WinAPI
- **버전 관리**: Git
