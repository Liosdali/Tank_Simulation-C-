#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "TankController.generated.h" // <-- Senin dosya adýna uygun

UCLASS()
class SIMULATION_API ATankController : public APlayerController // <-- ATankPlayerController yerine ATankController
{
    GENERATED_BODY()

protected:
    virtual void SetupInputComponent() override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputAction* SwitchRoleAction;

    void RequestRoleSwitch(const FInputActionValue& Value);

    UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Multiplayer")
    void Server_SelectRole(bool bIsDriver);

    UFUNCTION(Server, Reliable)
    void Server_RequestRoleSwitch();

    // Sunucu kurma (Host)
    UFUNCTION(BlueprintCallable, Category = "Multiplayer")
    void HostGame(FString MapName);

    // Sunucuya katýlma (Join)
    UFUNCTION(BlueprintCallable, Category = "Multiplayer")
    void JoinGame(FString IPAddress);
};