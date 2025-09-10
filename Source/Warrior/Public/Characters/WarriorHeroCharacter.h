#pragma once

#include "CoreMinimal.h"
#include "Characters/WarriorBaseCharacter.h"
#include "WarriorHeroCharacter.generated.h"


class USpringArmComponent;		// 스프링 암 컴포넌트
class UCameraComponent;			// 카메라 컴포넌트
class UDataAsset_InputConfig;	// 입력 구성 데이터 에셋
struct FInputActionValue;		// 입력 액션 값 구조체

UCLASS()
class WARRIOR_API AWarriorHeroCharacter : public AWarriorBaseCharacter
{
	GENERATED_BODY()

public:
	AWarriorHeroCharacter(); // 생성자
	
protected: // BeginPlay() 재정의

	// 플레이어 입력 컴포넌트 설정 함수 재정의
	virtual void SetupPlayerInputComponent(class UInputComponent*
	PlayerInputComponent) override;

	virtual void BeginPlay() override; // BeginPlay 게임 시작 시 호출되는 함수

private:

#pragma region Components

	// 카메라를 캐릭터 뒤에 위치시키고, 캐릭터의 회전에 따라 카메라가 회전하도록 하는 스프링 암 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category =
	"Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom; // 스프링 암 컴포넌트

	// 실제 게임 플레이 시 사용되는 카메라 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category =
	"Camera", meta = (AllowPrivateAccess = "true"))

	UCameraComponent* FollowCamera; // 팔로우 카메라 컴포넌트
#pragma endregion


#pragma region Inputs

	// 입력 구성 데이터 에셋
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category =
	"CharacterData", meta = (AllowPrivateAccess = "true"))
	
	UDataAsset_InputConfig* InputConfigDataAsset; // 입력 구성 데이터 에셋

	// 이동 입력 처리 함수
	void Input_Move(const FInputActionValue& InputActionValue);
	
	// 시점 변경 입력 처리 함수
	void Input_Look(const FInputActionValue& InputActionValue);

#pragma endregion

};
