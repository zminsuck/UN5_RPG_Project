#include "DataAssets/Input/DataAsset_InputConfig.h"

// 입력 시스템 관련 헤더
UInputAction* UDataAsset_InputConfig::FindNativeInputActionByTag(const FGameplayTag& InInputTag) const
{
	// NativeInputActions 배열을 순회하며 입력 태그에 해당하는 입력 액션을 찾음
	for (const FWarriorInputActionConfig& InputActionConfig : NativeInputActions)
	{
		// 입력 태그가 일치하고, 해당 입력 액션이 유효한 경우 반환
		if (InputActionConfig.InputTag == InInputTag && InputActionConfig.InputAction)
		{
			// 일치하는 입력 액션 반환
			return InputActionConfig.InputAction;
		}
	}

	// 일치하는 입력 액션이 없으면 nullptr 반환
	return nullptr;
}