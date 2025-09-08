#pragma once

namespace Debug
{
	// 화면에 디버그 메시지를 출력하는 함수
	static void Print(const FString& Msg, const FColor& Color = FColor::MakeRandomColor(), int32 InKey = -1)
	{
		// GEngine 포인터가 유효한지 확인
		if (GEngine)
		{
			// 화면에 디버그 메시지를 출력
			GEngine->AddOnScreenDebugMessage(InKey, 7.f, Color, Msg);
			
			// 콘솔 로그에도 동일한 메시지를 출력
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
		}
	}
}
