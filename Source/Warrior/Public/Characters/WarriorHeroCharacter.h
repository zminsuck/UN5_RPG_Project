#pragma once

#include "CoreMinimal.h"
#include "Characters/WarriorBaseCharacter.h"
#include "WarriorHeroCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;

UCLASS()
class WARRIOR_API AWarriorHeroCharacter : public AWarriorBaseCharacter
{
	GENERATED_BODY()

public:
	AWarriorHeroCharacter();
	
protected: // BeginPlay() 재정의
	virtual void BeginPlay() override; // BeginPlay 게임 시작 시 호출되는 함수

private:

#pragma region Components

	// 카메라를 캐릭터 뒤에 위치시키고, 캐릭터의 회전에 따라 카메라가 회전하도록 하는 스프링 암 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom; // 스프링 암 컴포넌트

	// 실제 게임 플레이 시 사용되는 카메라 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera; // 팔로우 카메라 컴포넌트
#pragma endregion
};
