// Copyright Epic Games, Inc. All Rights Reserved.

#include "Project3TutorialCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Kismet/KismetMathLibrary.h"
//////////////////////////////////////////////////////////////////////////
// AProject3TutorialCharacter

AProject3TutorialCharacter::AProject3TutorialCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 700.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	// The camera follows at this distance behind the character
	CameraBoom->TargetArmLength = 500.0F;
	//The Camera looks down at the player
	CameraBoom->SetRelativeRotation(FRotator(-70.0F, -40.0F, 0.0F));
	//Do not rotate the arm based on the controller!
	CameraBoom->bUsePawnControlRotation = true;
	//Ignore pawn's pitch, yaw and roll!
	CameraBoom->bInheritPitch = true;
	CameraBoom->bInheritRoll = true;
	CameraBoom->bInheritYaw = true;
	
	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AProject3TutorialCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input
void AProject3TutorialCharacter::Move(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();
	if (Controller != nullptr && (InputValue.X != 0.0F || InputValue.Y != 0.0F)) 
	{
		const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);
		if (InputValue.X != 0.0F)
		{
			// get right vector
			const FVector RightDirection = UKismetMathLibrary::GetRightVector(YawRotation);
			// add movement in that direction
			AddMovementInput(RightDirection, InputValue.X);
		}
		if (InputValue.Y != 0.0F)
		{
			const FVector ForwardDirection = YawRotation.Vector();
			AddMovementInput(ForwardDirection, InputValue.Y);
		}
	}
}
void AProject3TutorialCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	UEnhancedInputComponent* EnhancedPlayerInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedPlayerInputComponent != nullptr)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if (PlayerController != nullptr)
		{
			UEnhancedInputLocalPlayerSubsystem* EnhancedSubsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
			if(EnhancedSubsystem != nullptr)
			{
				EnhancedSubsystem->AddMappingContext(IC_Character, 1);
				
			}
			EnhancedPlayerInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AProject3TutorialCharacter::Move);
		}
	}


}






