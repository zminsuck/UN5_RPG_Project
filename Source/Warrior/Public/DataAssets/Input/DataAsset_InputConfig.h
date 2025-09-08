// Vince Petrelli All Rights Reseved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h" // For FGameplayTag를 위해 추가
#include "DataAsset_InputConfig.generated.h"


class UInputAction;
class UInputMappingContext;

USTRUCT(BlueprintType)
struct FWarriorInputActionConfig
{
	GENERATED_BODY()

public:
	// InputTag는 "InputTag" 카테고리에 속하는 태그만 허용
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	FGameplayTag InputTag; // 입력 태그

	// 이 태그에 연결된 입력 액션
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputAction; // 입력 액션
};

UCLASS()
class WARRIOR_API UDataAsset_InputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	// 기본 매핑 컨텍스트
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	// 기본 매핑 컨텍스트
	UInputMappingContext* DefaultMappingContext; 

	// 네이티브 입력 액션 구성 배열
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	// 네이티브 입력 액션 구성 배열
	TArray<FWarriorInputActionConfig> NativeInputActions;

	// 주어진 입력 태그에 해당하는 네이티브 입력 액션을 찾는 함수
	UInputAction* FindNativeInputActionByTag(const FGameplayTag& InInputTag) const;
};
