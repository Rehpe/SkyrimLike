// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/SLCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SLCharacterControlData.h"
#include "Blueprint/UserWidget.h"
#include "CharacterStat/SLCharacterStatComponent.h"
#include "UI/SLPlayerInfoWidget.h"
#include "Equipment/SLEquipmentComponent.h"
#include "Inventory/SLInventoryComponent.h"
#include "Item/SLItemDataStructs.h"
#include "Kismet/GameplayStatics.h"

ASLCharacterPlayer::ASLCharacterPlayer()
{
	// Mesh
	Mesh_Hair = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh_Hair"));
	Mesh_Hair->SetupAttachment(GetMesh());

	Mesh_FacialHair = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh_FacialHair"));
	Mesh_FacialHair->SetupAttachment(GetMesh());

	Mesh_Accessory = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh_Accessory"));
	Mesh_Accessory->SetupAttachment(GetMesh());

	Mesh_Torso = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh_Torso"));
	Mesh_Torso->SetupAttachment(GetMesh());

	Mesh_Hands = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh_Hands"));
	Mesh_Hands->SetupAttachment(GetMesh());

	Mesh_Legs = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh_Legs"));
	Mesh_Legs->SetupAttachment(GetMesh());

	Mesh_MeleeWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh_MeleeWeapon"));
	Mesh_MeleeWeapon->SetupAttachment(GetMesh(), FName(TEXT("L_Hand_Weapon")));
	
	Mesh_RangedWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh_RangedWeapon"));
	Mesh_RangedWeapon->SetupAttachment(GetMesh());

	MeshComps = {Mesh_Hair, Mesh_FacialHair,  Mesh_Accessory, Mesh_Torso, Mesh_Hands, Mesh_Legs,Mesh_MeleeWeapon, Mesh_RangedWeapon};
	SetLeaderPose();
	
	// Camera
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->bUsePawnControlRotation = true;

	FollowCam = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCam"));
	FollowCam->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	FollowCam->bUsePawnControlRotation = false;

	// Input
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/SkyrimLike/Input/Actions/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputChangeActionControlRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/SkyrimLike/Input/Actions/IA_ChangeControl.IA_ChangeControl'"));
	if (nullptr != InputChangeActionControlRef.Object)
	{
		ChangeControlAction = InputChangeActionControlRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/SkyrimLike/Input/Actions/IA_Move.IA_Move'"));
	if (nullptr != InputActionMoveRef.Object)
	{
		MoveAction = InputActionMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAttackRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/SkyrimLike/Input/Actions/IA_Attack.IA_Attack'"));
	if (nullptr != InputActionAttackRef.Object)
	{
		AttackAction = InputActionAttackRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionInteractRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/SkyrimLike/Input/Actions/IA_Interact.IA_Interact'"));
	if (nullptr != InputActionInteractRef.Object)
	{
		InteractAction = InputActionInteractRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionInventoryRef(
		TEXT("/Script/EnhancedInput.InputAction'/Game/SkyrimLike/Input/Actions/IA_Inventory.IA_Inventory'"));
	if (nullptr != InputActionInventoryRef.Object)
	{
		InventoryAction = InputActionInventoryRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionThirdPersonLookRef(
		TEXT(
			"/Script/EnhancedInput.InputAction'/Game/SkyrimLike/Input/Actions/IA_ThirdPersonLook.IA_ThirdPersonLook'"));
	if (nullptr != InputActionThirdPersonLookRef.Object)
	{
		ThirdPersonLookAction = InputActionThirdPersonLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionFirstPersonLookRef(
		TEXT(
			"/Script/EnhancedInput.InputAction'/Game/SkyrimLike/Input/Actions/IA_FirstPersonLook.IA_FirstPersonLook'"));
	if (nullptr != InputActionFirstPersonLookRef.Object)
	{
		FirstPersonLookAction = InputActionFirstPersonLookRef.Object;
	}
	
	CurrentViewMode = EViewMode::ThirdPersonView;
}

void ASLCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	SetViewMode(CurrentViewMode);
	
	// Component 재할당
	EquipmentComponent = FindComponentByClass<USLEquipmentComponent>();
	InventoryComponent = FindComponentByClass<USLInventoryComponent>();
	Stat = FindComponentByClass<USLCharacterStatComponent>();
	
	// 무기 소켓 지정
	Mesh_MeleeWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(TEXT("L_Hand_Weapon")));

	if(InventoryComponent)
	{
		InventoryComponent->BeginDetect();
	}
	
	if(EquipmentComponent)
	{
		EquipmentComponent->SetEquipmentMeshSlot(MeshComps);
		// 장비 Stat 변경 시 Stat 컴포넌트에게 이를 전달
		EquipmentComponent->OnEquipmentStatChanged.AddDynamic(this,&ASLCharacterPlayer::HandleOnStatChanged);
		InventoryComponent->OnEquipmentUsed.AddDynamic(this, &ASLCharacterPlayer::HandleOnEquip);
	}
}

void ASLCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// CastChecked 함수를 통해 EnhancedInputComponent(향상된 입력 컴포넌트)를 사용하지 않는다면 에러를 발생시킴
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Triggered, this,
	                                   &ASLCharacterPlayer::ChangeViewMode);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASLCharacterPlayer::Move);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ASLCharacterPlayer::Attack);
	EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this,
	                                   &ASLCharacterPlayer::OpenPlayerInfo);
	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ASLCharacterPlayer::Interact);
	EnhancedInputComponent->BindAction(ThirdPersonLookAction, ETriggerEvent::Triggered, this,
	                                   &ASLCharacterPlayer::ThirdPersonLook);
	EnhancedInputComponent->BindAction(FirstPersonLookAction, ETriggerEvent::Triggered, this,
	                                   &ASLCharacterPlayer::FirstPersonLook);
}

void ASLCharacterPlayer::SetLeaderPose()
{
	if(GetMesh())
	{
		if(Mesh_Hair)
			Mesh_Hair->SetLeaderPoseComponent(GetMesh());
		if(Mesh_FacialHair)
			Mesh_FacialHair->SetLeaderPoseComponent(GetMesh());
		if(Mesh_Accessory)
			Mesh_Accessory->SetLeaderPoseComponent(GetMesh());
		if(Mesh_Torso)
			Mesh_Torso->SetLeaderPoseComponent(GetMesh());
		if(Mesh_Hands)
			Mesh_Hands->SetLeaderPoseComponent(GetMesh());
		if(Mesh_Legs)
			Mesh_Legs->SetLeaderPoseComponent(GetMesh());
		if(Mesh_MeleeWeapon)
			Mesh_MeleeWeapon->SetLeaderPoseComponent(GetMesh());
		if(Mesh_RangedWeapon)
			Mesh_RangedWeapon->SetLeaderPoseComponent(GetMesh());
	}
}

void ASLCharacterPlayer::ChangeViewMode()
{
	if (CurrentViewMode == EViewMode::FirstPersonView)
	{
		SetViewMode(EViewMode::ThirdPersonView);
	}
	else if (CurrentViewMode == EViewMode::ThirdPersonView)
	{
		SetViewMode(EViewMode::FirstPersonView);
	}
}

void ASLCharacterPlayer::SetViewMode(EViewMode NewViewMode)
{
	USLCharacterControlData* NewViewModeData = ViewModeManager[NewViewMode];
	check(NewViewModeData);

	SetCharacterControlData(NewViewModeData);

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings(); // IMC 초기화
		UInputMappingContext* NewMappingContext = NewViewModeData->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}

	CurrentViewMode = NewViewMode;
}

void ASLCharacterPlayer::SetCharacterControlData(const USLCharacterControlData* CharacterControlData)
{
	Super::SetCharacterControlData(CharacterControlData);

	SpringArm->TargetArmLength = CharacterControlData->TargetArmLength;
	SpringArm->SetRelativeRotation(CharacterControlData->RelativeRotation);
	SpringArm->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	SpringArm->bInheritPitch = CharacterControlData->bInheritPitch;
	SpringArm->bInheritYaw = CharacterControlData->bInheritYaw;
	SpringArm->bInheritRoll = CharacterControlData->bInheritRoll;
	SpringArm->bDoCollisionTest = CharacterControlData->bDoCollisionTest;

	FollowCam->SetRelativeLocation(CharacterControlData->RelativeLocation);
	GetMesh()->SetOwnerNoSee(CharacterControlData->bHideMesh);
}

void ASLCharacterPlayer::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void ASLCharacterPlayer::ThirdPersonLook(const FInputActionValue& Value)
{
	if (PlayerInfoWidget && PlayerInfoWidget->bIsWidgetOn) return;
	
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void ASLCharacterPlayer::FirstPersonLook(const FInputActionValue& Value)
{
	if (PlayerInfoWidget && PlayerInfoWidget->bIsWidgetOn) return;
	
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void ASLCharacterPlayer::Attack()
{
	if (PlayerInfoWidget && PlayerInfoWidget->bIsWidgetOn) return;

	ProcessComboCommand();
}

void ASLCharacterPlayer::Interact()
{
	if(InventoryComponent)
	{
		InventoryComponent->TryInteract();
	}
}

void ASLCharacterPlayer::OpenPlayerInfo(const FInputActionValue& ActionValue)
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	if (!PlayerInfoWidget)
	{
		PlayerInfoWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/SkyrimLike/UI/WB_PlayerInfoWidget.WB_PlayerInfoWidget_C"));
		
		// 위젯 초기화
		PlayerInfoWidget = CreateWidget<USLPlayerInfoWidget>(GetWorld(), PlayerInfoWidgetClass, TEXT("PlayerInfo"));
		PlayerInfoWidget->SetInventoryComponent(InventoryComponent);
		PlayerInfoWidget->SetEquipmentComponent(EquipmentComponent);
		PlayerInfoWidget->SetStatComponent(Stat);
		PlayerInfoWidget->CreateWidgets();
	}

	// 위젯이 열려있는 경우 위젯을 닫음
	if (PlayerInfoWidget->bIsWidgetOn)
	{
		PlayerInfoWidget->CloseWidgets();
		PlayerInfoWidget->bIsWidgetOn = false;
		PlayerInfoWidget->RemoveFromParent();
		
		if (PlayerController)
		{
			// 마우스 커서를 숨기고, 입력 모드를 게임 전용으로 설정
			PlayerController->bShowMouseCursor = false;
			FInputModeGameOnly InputMode;
			PlayerController->SetInputMode(InputMode);
		}
	}
	// 위젯이 닫혀있으면 위젯을 열음
	else
	{
		PlayerInfoWidget->OpenWidgets();
		PlayerInfoWidget->AddToViewport();
		PlayerInfoWidget->bIsWidgetOn = true;
		if (PlayerController)
		{
			// 마우스 커서를 보이게하고, 입력 모드를 UI & 게임 전용으로 설정
			PlayerController->bShowMouseCursor = true;
			FInputModeGameAndUI InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			//InputMode.SetHideCursorDuringCapture(false);  // 커서 숨김 해제

			// 이 위젯을 포커스 대상으로 설정
			InputMode.SetWidgetToFocus(PlayerInfoWidget->TakeWidget());
			PlayerController->SetInputMode(InputMode);
		}
	}
}

void ASLCharacterPlayer::HandleOnStatChanged()
{
	// Equipment에서 Modified Stat을 받아옴
	if(EquipmentComponent)
	{
		FItemStat ModifiedStat = EquipmentComponent->GetModifiedStat();
		if(Stat)
		{
			// Stat컴포넌트에게 ModifiedStat 전달
			Stat->SetEquipmentModifiedStat(ModifiedStat);
			Stat->CaculateFinalStat();
		}
	}
}

void ASLCharacterPlayer::HandleOnEquip(EEquipmentCategory inEquipmentCategory, FName ItemID)
{
	if(EquipmentComponent)
	{
		EquipmentComponent->EquipItem(inEquipmentCategory, ItemID);
	}
}
