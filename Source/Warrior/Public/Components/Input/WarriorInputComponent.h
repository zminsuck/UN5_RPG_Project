#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "WarriorInputComponent.generated.h"

UCLASS()
class WARRIOR_API UWarriorInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	// 주어진 입력 구성 데이터 자산과 입력 태그를 사용하여 네이티브 입력 액션을 바인딩하는 템플릿 함수
	template<class UserObject, typename CallbackFunc>
	// BindNativeInputAction 함수 선언
	void BindNativeInputAction(const UDataAsset_InputConfig* InInputConfig,
		const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent,
		UserObject* ContextObject, CallbackFunc Func);
};

template<class UserObject, typename CallbackFunc>
inline void UWarriorInputComponent::BindNativeInputAction(const UDataAsset_InputConfig*
	InInputConfig, const FGameplayTag& InInputTag, ETriggerEvent TriggerEvent,
	UserObject* ContextObject, CallbackFunc Func)
{	
	// 입력 구성 데이터 자산이 유효한지 확인
	checkf(InInputConfig, TEXT("Input config data asset is null,can not proceed with binding"));
	
	// 입력 태그가 유효한지 확인
	if (UInputAction* FoundAction = InInputConfig->FindNativeInputActionByTag(InInputTag))
	{
		// 유효한 입력 액션이 발견되면, 해당 액션을 지정된 이벤트와 콜백 함수에 바인딩
		BindAction(FoundAction, TriggerEvent, ContextObject, Func);
	}
}