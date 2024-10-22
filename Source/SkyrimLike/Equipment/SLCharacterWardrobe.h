// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SLCharacterWardrobe.generated.h"

UCLASS()
class SKYRIMLIKE_API ASLCharacterWardrobe : public AActor
{
	GENERATED_BODY()
	
public:	
	ASLCharacterWardrobe();

protected:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USceneComponent> SceneRoot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> WardrobeArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USceneCaptureComponent2D> WardrobeCapture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	uint8 bShouldRotate = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AActor* TargetActor;
	
public:
	virtual void BeginPlay() override;

	void SetTargetActor(AActor* inTargetActor);

	//UFUNCTION(BlueprintCallable)
	//FORCEINLINE void AllowRotation() { if(bShouldRotate) bShouldRotate = false; else bShouldRotate = true; };

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetShouldRotate(bool bRotate) { bShouldRotate = bRotate; };
	
	UFUNCTION(BlueprintCallable)
	void ResetCameraArm();

	UFUNCTION(BlueprintCallable)
	void ZoomIn();

	UFUNCTION(BlueprintCallable)
	void ZoomOut();

	UFUNCTION(BlueprintCallable)
	void HorizontalMove(float AxisValue);
	
protected:
	// 타이머로 반복 호출될 함수
	void UpdateActorLocation();

};
