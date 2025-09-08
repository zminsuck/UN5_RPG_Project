#include "Characters/WarriorHeroCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "WarriorDebugHelper.h"


AWarriorHeroCharacter::AWarriorHeroCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f); // 충돌 캡슐 크기 설정

	// 컨트롤러 회전에 따라 캐릭터가 회전하지 않도록 설정 -> 대신 카메라만 회전
	bUseControllerRotationPitch = false; // Pitch(상하) 회전 비활성화
	bUseControllerRotationYaw = false;	 // Yaw(좌우) 회전 비활성화
	bUseControllerRotationRoll = false;	 // Roll(좌우 기울기) 회전 비활성화

	// ---------- 카메라 붐(스프링 암) 설정 ---------- //
	
	// 카메라를 캐릭터 뒤쪽에 배치는 스프링암 생성
	// 길이 200
	// 오프셋 위로 65 , 오른쪽으로 55 -> 카메라 약간 비스듬히 위치
	// 컨트롤러 회전 반영
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom ->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 200.0f;
	CameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f);
	CameraBoom->bUsePawnControlRotation = true;

	// ---------- 팔로우 카메라 설정 ---------- //
	
	// 실제 플레이가 보는 카메라
	// SpringArm 끝에 부착
	// 자체 회전은 비활성화 -> Spring Arm을 따라감
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// ---------- 캐릭터 이동 설정 ---------- //
	// 회전 속도: Yaw 500
	// 최대 이동 속도: 400
	// 감속도: 2000 -> 멈출 때 빠르게 멈춤
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
}

void AWarriorHeroCharacter::BeginPlay() // BeginPlay 게임 시작 시 호출되는 함수 
{
	Super::BeginPlay(); // 부모 클래스의 BeginPlay 함수 호출

	Debug::Print(TEXT("Working")); // 디버그 하여, "Working" 메시지를 화면에 출력
}
