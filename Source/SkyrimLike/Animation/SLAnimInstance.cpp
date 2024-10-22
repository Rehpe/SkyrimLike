// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/SLAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

USLAnimInstance::USLAnimInstance()
{
	MovingThreshold = 3.0f;
	JumpingThreshold = 100.0f;
}

void USLAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void USLAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D(); // 지면속력 : Velocity의 x, y 크기(z 제외, Size2D())
		bIsIdle = GroundSpeed < MovingThreshold;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsJumping & (Velocity.Z > JumpingThreshold);
	}
}
