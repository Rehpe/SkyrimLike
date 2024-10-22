// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/SLCharacterWardrobe.h"
#include "Blueprint/UserWidget.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASLCharacterWardrobe::ASLCharacterWardrobe()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	RootComponent = SceneRoot;
	
	WardrobeArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("WardrobeArm"));
	WardrobeArm->SetupAttachment(SceneRoot);
	WardrobeArm->TargetArmLength = 120.0f;
	
	WardrobeCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("WardrobeCapture"));
	WardrobeCapture->SetupAttachment(WardrobeArm);
}

void ASLCharacterWardrobe::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ASLCharacterWardrobe::UpdateActorLocation, 0.2f, true);
}

void ASLCharacterWardrobe::SetTargetActor(AActor* inTargetActor)
{
	TargetActor = inTargetActor;
	WardrobeCapture->ShowOnlyActorComponents(TargetActor,false);
}

void ASLCharacterWardrobe::ResetCameraArm()
{
	WardrobeArm->SetRelativeLocation(FVector(0, 0, 20.f));
	WardrobeArm->SetRelativeRotation(FRotator(5.f, 190.0f, 0));
	WardrobeArm->TargetArmLength = 120.0f;
}

void ASLCharacterWardrobe::ZoomIn()
{
	float ArmLength = WardrobeArm->TargetArmLength;
	float NewArmLength = FMath::Clamp((ArmLength - 20.f), 100.0f, 180.0f);
	WardrobeArm->TargetArmLength = NewArmLength;
}

void ASLCharacterWardrobe::ZoomOut()
{
	float ArmLength = WardrobeArm->TargetArmLength;
	float NewArmLength = FMath::Clamp((ArmLength + 20.f), 100.0f, 180.0f);
	WardrobeArm->TargetArmLength = NewArmLength;
}

void ASLCharacterWardrobe::HorizontalMove(float AxisValue)
{
	if(AxisValue != 0.0f)
	{
		// 현재 카메라 암의 월드 회전값 가져오기
		FRotator CurrentRotation = WardrobeArm->GetComponentRotation();

		// 새로운 Yaw 값 계산 (마우스 입력에 따라 회전 값 조정)
		float NewYaw = CurrentRotation.Yaw + (AxisValue * -4.0f); // -4.0f는 회전 속도

		// 새로운 회전값 설정
		FRotator NewRotation = FRotator(CurrentRotation.Pitch, NewYaw, CurrentRotation.Roll);
		WardrobeArm->SetWorldRotation(NewRotation);
	}
}

void ASLCharacterWardrobe::UpdateActorLocation()
{
	// 플레이어를 가져옴
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController && PlayerController->GetPawn())
	{
		// 현재 액터와 플레이어의 위치 비교
		FTransform ActorTransform = GetActorTransform();
		FTransform PlayerTransform = PlayerController->GetPawn()->GetActorTransform();

		// 위치가 다르면 현재 액터의 위치를 플레이어의 위치로 설정
		if (!ActorTransform.Equals(PlayerTransform))
		{
			SetActorTransform(PlayerTransform);
		}
	}
}


