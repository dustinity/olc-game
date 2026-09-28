#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OLCCrashSequenceWidget.generated.h"

class SImage;
class UTexture2D;

UCLASS()
class OURLASTCHANCE_API UOLCCrashSequenceWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UOLCCrashSequenceWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "OLC|Cinematics")
	void SetCrashPhase(int32 InPhase);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	void LoadPlateTextures();
	UTexture2D* LoadTextureFromFile(const FString& Path);
	FText GetPhaseCaption() const;
	FLinearColor GetPlateTint() const;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UTexture2D>> CrashPlateTextures;

	TSharedPtr<SImage> PlateImage;

	int32 CurrentPhase = 0;
};
