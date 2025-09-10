#include "Characters/WarriorHeroCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "Components/Input/WarriorInputComponent.h"
#include "WarriorGameplayTags.h"

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

// 플레이어 입력 컴포넌트 설정 함수 재정의
void AWarriorHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// InputConfigDataAsset가 유효한지 확인
	checkf(InputConfigDataAsset, TEXT("Forgot to assign a valid data asset as input config"));

	// 로컬 플레이어 가져오기
	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();

	// UEnhancedInputLocalPlayerSubsystem 가져오기
	UEnhancedInputLocalPlayerSubsystem* Subsystem =
	ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	check(Subsystem); // Subsystem이 유효한지 확인

	// 기본 매핑 컨텍스트 추가
	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);

	// UWarriorInputComponent로 캐스팅
	UWarriorInputComponent* WarriorInputComponent =
	CastChecked<UWarriorInputComponent>(PlayerInputComponent);

	// 이동 및 시점 변경 입력 바인딩
	WarriorInputComponent->BindNativeInputAction(InputConfigDataAsset,
	WarriorGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);

	// 시점 변경 입력 바인딩
	WarriorInputComponent->BindNativeInputAction(InputConfigDataAsset,
	WarriorGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);
}

void AWarriorHeroCharacter::BeginPlay() // BeginPlay 게임 시작 시 호출되는 함수 
{
	Super::BeginPlay(); // 부모 클래스의 BeginPlay 함수 호출

	Debug::Print(TEXT("Working")); // 디버그 하여, "Working" 메시지를 화면에 출력
}

// 이동 입력 처리 함수
void AWarriorHeroCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	// FVector2D 타입으로 변환
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();

	// 입력이 없으면 함수 종료
	const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

	// Y축 입력이 있으면 전진/후진
	if (MovementVector.Y != 0.f)
	{
		// 컨트롤러의 Yaw 회전을 기준으로 전진 방향 벡터 계산
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);

		// 전진 방향으로 이동 입력 추가
		AddMovementInput(ForwardDirection, MovementVector.Y);
	}

	// X축 입력이 있으면 좌우 이동
	if (MovementVector.X != 0.f)
	{
		// 컨트롤러의 Yaw 회전을 기준으로 오른쪽 방향 벡터 계산
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);

		// 오른쪽 방향으로 이동 입력 추가
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

// 시점 변경 입력 처리 함수
void AWarriorHeroCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	// FVector2D 타입으로 변환
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	// X축 입력이 있으면 Yaw 회전, Y축 입력이 있으면 Pitch 회전 (좌우 회전)
	if (LookAxisVector.X != 0.f)
	{
		AddControllerYawInput(LookAxisVector.X); // Yaw 회전 입력 추가
	}
	
	// Y축 입력이 있으면 Pitch 회전, X축 입력이 있으면 Yaw 회전 (상하 회전)
	if (LookAxisVector.Y != 0.f)
	{
		AddControllerPitchInput(LookAxisVector.Y); // Pitch 회전 입력 추가
	}
}
