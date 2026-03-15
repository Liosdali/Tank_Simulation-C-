#include "TankController.h"
#include "EnhancedInputComponent.h"
#include "TankGameMode.h" 

void ATankController::SetupInputComponent() 
{
    Super::SetupInputComponent();

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
    {
        if (SwitchRoleAction)
        {
            EnhancedInputComponent->BindAction(SwitchRoleAction, ETriggerEvent::Started, this, &ATankController::RequestRoleSwitch);
        }
    }
}

void ATankController::RequestRoleSwitch(const FInputActionValue& Value)
{
    Server_RequestRoleSwitch();
}

void ATankController::Server_RequestRoleSwitch_Implementation()
{
    if (ATankGameMode* GM = Cast<ATankGameMode>(GetWorld()->GetAuthGameMode()))
    {
        GM->HandleRoleSwitchRequest(this);
    }
}

void ATankController::Server_SelectRole_Implementation(bool bIsDriver)
{
    if (ATankGameMode* GM = Cast<ATankGameMode>(GetWorld()->GetAuthGameMode()))
    {
        GM->AssignPlayerRole(this, bIsDriver);
    }
}