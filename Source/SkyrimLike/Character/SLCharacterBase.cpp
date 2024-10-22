// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SLCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SLCharacterControlData.h"
#include "Animation/AnimMontage.h"
#include "SLComboActionData.h"
#include "Physics/SLCollision.h"
#include "Engine/DamageEvents.h"
#include "CharacterStat/SLCharacterStatComponent.h"
#include "Equipment/SLEquipmentComponent.h"
#include "Inventory/SLInventoryComponent.h"
#include "UI/SLWidgetComponent.h"
#include "UI/SLHpBarWidget.h"

// Sets default values
ASLCharacterBase::ASLCharacterBase()
{
	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_SLCAPSULE);

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 300.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName("NoCollision");

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Barbarous.SK_CharM_Barbarous'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/SkyrimLike/Animation/ABP_SLCharacter1.ABP_SLCharacter1_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FObjectFinder<USLCharacterControlData> ThirdPersonViewDataRef(TEXT("/Script/SkyrimLike.SLCharacterControlData'/Game/SkyrimLike/CharacterControl/SLC_ThirdPersonView.SLC_ThirdPersonView'"));
	if (ThirdPersonViewDataRef.Object)
	{
		ViewModeManager.Add(EViewMode::ThirdPersonView, ThirdPersonViewDataRef.Object);
	}
	static ConstructorHelpers::FObjectFinder<USLCharacterControlData> FirstPersonViewDataRef(TEXT("/Script/SkyrimLike.SLCharacterControlData'/Game/SkyrimLike/CharacterControl/SLC_FirstPersonView.SLC_FirstPersonView'"));
	if (FirstPersonViewDataRef.Object)
	{
		ViewModeManager.Add(EViewMode::FirstPersonView, FirstPersonViewDataRef.Object);
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/SkyrimLike/Animation/AM_ComboAttack1.AM_ComboAttack1'"));
	if (ComboActionMontageRef.Object)
	{
		ComboActionMontage = ComboActionMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<USLComboActionData> ComboActionDataRef(TEXT("/Script/SkyrimLike.SLComboActionData'/Game/SkyrimLike/CharacterAction/SLA_ComboAttack.SLA_ComboAttack'"));
	if (ComboActionDataRef.Object)
	{
		ComboActionData = ComboActionDataRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/SkyrimLike/Animation/AM_Dead1.AM_Dead1'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}

	// *** Components ***
	InventoryComponent = CreateDefaultSubobject<USLInventoryComponent>(TEXT("InventoryComponent"));
	EquipmentComponent = CreateDefaultSubobject<USLEquipmentComponent>(TEXT("EquipmentComponent"));
	Stat = CreateDefaultSubobject<USLCharacterStatComponent>(TEXT("Stat"));
	
	// Widget
	HpBar = CreateDefaultSubobject<USLWidgetComponent>(TEXT("Widget")); // Widget Component 초기화
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.f, 0.f, 180.f));
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/SkyrimLike/UI/WBP_HpBar.WBP_HpBar_C'"));
	if (HpBarWidgetRef.Class)
	{
		HpBar->SetWidgetClass(HpBarWidgetRef.Class); // 위젯 클래스 설정
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(150.f, 15.f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
}

void ASLCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//Stat->OnHpZero.AddUObject(this, &ASLCharacterBase::SetDead);
}

void ASLCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (!HUDWidgetClass)
	{
		HUDWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/SkyrimLike/UI/WBP_PlayerHUD.WBP_PlayerHUD_C"));
	}
	if (!DisplayWidgetClass)
	{
		DisplayWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/SkyrimLike/UI/WBP_PlayerDisplay.WBP_PlayerDisplay_C"));
	}
}

void ASLCharacterBase::SetCharacterControlData(const USLCharacterControlData* CharacterControlData)
{
	// Pawn
	bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

	// CharacterMovement
	GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;
}

void ASLCharacterBase::ProcessComboCommand()
{
	if (CurrentCombo == 0)	// 콤보 시작 전이라면
	{
		ComboActionBegin();
		return;
	}
	// 타이머가 비활성화 상태라면 : 타이머를 놓쳤거나, 진행할 필요가 없는 경우
	if (!ComboTimerHandle.IsValid())
	{
		HasNextComboCommand = false;
	}
	else // 콤보 입력 감지
	{
		HasNextComboCommand = true;
	}
}

void ASLCharacterBase::ComboActionBegin()
{
	// Combo Status
	CurrentCombo = 1;

	// Movement Setting
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);	// 콤보 중 이동 불가

	// Animation Setting
	const float AttackSpeedRate = 1.0f;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);

	// 콤보 중 이동 불가
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ASLCharacterBase::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

	// 콤보 타이머 발동
	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

void ASLCharacterBase::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	ensure(CurrentCombo != 0);
	CurrentCombo = 0;
	
	// 이동 불가 해제
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void ASLCharacterBase::SetComboCheckTimer()
{
	int32 ComboIndex = CurrentCombo  -1;

	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	const float AttackSpeedRate = 1.0f;
	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;

	if (ComboEffectiveTime > 0.0f)
	{
		// TimerHandle에게 ComboEffectiveTime 내에 ComboCheck함수 호출(반복 없이 한 번만)
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &ASLCharacterBase::ComboCheck, ComboEffectiveTime, false);
	}
}

void ASLCharacterBase::ComboCheck()
{
	// Timer 해제
	ComboTimerHandle.Invalidate();
	
	// 시간 내에 다음 콤보 커맨드가 들어왔다면
	if (HasNextComboCommand)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		// Combo Status를 1에서 MaxCombo 값 사이로 Clamp
		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);

		// 몽타주 이름(ComboAttack) 에 현재 Combo Status 값(1~4)을 붙여 다음 재생할 섹션 이름 조합
		FName NextSection = *FString::Printf(TEXT("%s%d"),
			                *ComboActionData->MontageSectionNamePrefix, CurrentCombo);
		AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);
		
		// 타이머 다시 켜기
		SetComboCheckTimer();
		
		// 입력 여부값 초기화
		HasNextComboCommand = false;
	}
}

void ASLCharacterBase::AttackHitCheck()
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackRange = 40.0f;
	const float AttackRadius = 50.0f;
	const float AttackDamage = 30.0f;
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_SLACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
	if (HitDetected)
	{
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}

/*#if ENABLE_DRAW_DEBUG

	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);

#endif*/
}

float ASLCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	//SetDead();
	Stat->ApplyDamage(DamageAmount);

	return DamageAmount;
}

void ASLCharacterBase::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnimation();
	SetActorEnableCollision(false);
	HpBar->SetHiddenInGame(true);
}

void ASLCharacterBase::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(DeadMontage, 1.0f);
}

void ASLCharacterBase::SetupCharacterWidget(USLUserWidget* InUserWidget)
{
	USLHpBarWidget* HpBarWidget = Cast<USLHpBarWidget>(InUserWidget);
	if (HpBarWidget)
	{
		HpBarWidget->SetMaxHp(Stat->GetMaxHp());
		HpBarWidget->UpdateHpBar(Stat->GetCurrentHp());
		Stat->OnHpChanged.AddUObject(HpBarWidget, &USLHpBarWidget::UpdateHpBar);
		Stat->OnHpZero.AddUObject(this, &ASLCharacterBase::SetDead);
	}
}


