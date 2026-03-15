#include "TankController.h"
#include "EnhancedInputComponent.h"
#include "TankGameMode.h" 
#include "Kismet/GameplayStatics.h"

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

void ATankController::HostGame(FString MapName)
{
    UE_LOG(LogTemp, Warning, TEXT("HOST BUTONUNA BASILDI! Harita: %s"), *MapName);
    UGameplayStatics::OpenLevel(GetWorld(), FName(*MapName), true, "listen");
}

void ATankController::JoinGame(FString IPAddress)
{
    // Belirtilen IP adresine (yerel test için 127.0.0.1) baðlanýr
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        PC->ClientTravel(IPAddress, ETravelType::TRAVEL_Absolute);
    }
}