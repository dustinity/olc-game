#include "UI/OLCCrashSequenceWidget.h"
#include "OurLastChance.h"

#include "Brushes/SlateImageBrush.h"
#include "Engine/Texture2D.h"
#include "HAL/PlatformFileManager.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Misc/FileHelper.h"
#include "Modules/ModuleManager.h"
#include "Rendering/Texture2DResource.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "OLCCrashSequenceWidget"

namespace
{
	const TCHAR* CrashPlateFiles[] = {
		TEXT("Crash-01-Night-Approach-Wing-Shear.png"),
		TEXT("Crash-02-Night-Camera-Rock-Strike.png"),
		TEXT("Crash-03-Predawn-Hard-Ground-Crash.png")
	};
}

UOLCCrashSequenceWidget::UOLCCrashSequenceWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UOLCCrashSequenceWidget::SetCrashPhase(int32 InPhase)
{
	CurrentPhase = FMath::Max(0, InPhase);
	InvalidateLayoutAndVolatility();
}

TSharedRef<SWidget> UOLCCrashSequenceWidget::RebuildWidget()
{
	LoadPlateTextures();

	return SNew(SOverlay)
		+ SOverlay::Slot()
		[
			SNew(SBorder)
			.BorderBackgroundColor(FLinearColor::Black)
			[
				SNew(SImage)
				.Image_Lambda([this]() -> const FSlateBrush*
				{
					const int32 PlateIndex = FMath::Clamp(CurrentPhase, 0, 2);
					if (CrashPlateTextures.IsValidIndex(PlateIndex) && CrashPlateTextures[PlateIndex])
					{
						static FSlateImageBrush Brush(NAME_None, FVector2D(1920.0f, 1080.0f));
						Brush.SetResourceObject(CrashPlateTextures[PlateIndex]);
						Brush.TintColor = GetPlateTint();
						return &Brush;
					}
					return nullptr;
				})
			]
		]
		+ SOverlay::Slot()
		.VAlign(VAlign_Bottom)
		.HAlign(HAlign_Fill)
		.Padding(42.0f)
		[
			SNew(SBorder)
			.Padding(18.0f)
			.BorderBackgroundColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.58f))
			[
				SNew(STextBlock)
				.Text_Lambda([this]() { return GetPhaseCaption(); })
				.ColorAndOpacity(FLinearColor(0.95f, 0.86f, 0.62f))
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 22))
			]
		];
}

void UOLCCrashSequenceWidget::LoadPlateTextures()
{
	if (!CrashPlateTextures.IsEmpty())
	{
		return;
	}

	const FString BasePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir() / TEXT("../../../Assets/Cinematics/DropshipCrash"));
	for (const TCHAR* PlateFile : CrashPlateFiles)
	{
		CrashPlateTextures.Add(LoadTextureFromFile(BasePath / PlateFile));
	}
}

UTexture2D* UOLCCrashSequenceWidget::LoadTextureFromFile(const FString& Path)
{
	TArray<uint8> FileData;
	if (!FFileHelper::LoadFileToArray(FileData, *Path))
	{
		UE_LOG(LogOLC, Warning, TEXT("[OLC] Crash plate not found: %s"), *Path);
		return nullptr;
	}

	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(TEXT("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);
	if (!ImageWrapper.IsValid() || !ImageWrapper->SetCompressed(FileData.GetData(), FileData.Num()))
	{
		return nullptr;
	}

	TArray<uint8> RawData;
	if (!ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, RawData))
	{
		return nullptr;
	}

	UTexture2D* Texture = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);
	if (!Texture)
	{
		return nullptr;
	}

	void* TextureData = Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(TextureData, RawData.GetData(), RawData.Num());
	Texture->GetPlatformData()->Mips[0].BulkData.Unlock();
	Texture->UpdateResource();
	return Texture;
}

FText UOLCCrashSequenceWidget::GetPhaseCaption() const
{
	switch (CurrentPhase)
	{
		case 0: return LOCTEXT("PhaseApproach", "NIGHT APPROACH - WING SHEAR");
		case 1: return LOCTEXT("PhaseRockStrike", "CAMERA-SIDE ROCK STRIKE");
		case 2: return LOCTEXT("PhaseImpact", "HARD GROUND CRASH");
		case 3: return LOCTEXT("PhaseDaybreak", "DAYBREAK CROSSFADE");
		default: return LOCTEXT("PhaseHandoff", "RTS CAMERA HANDOFF");
	}
}

FLinearColor UOLCCrashSequenceWidget::GetPlateTint() const
{
	if (CurrentPhase == 3)
	{
		return FLinearColor(1.0f, 0.82f, 0.62f, 0.72f);
	}
	if (CurrentPhase >= 4)
	{
		return FLinearColor(1.0f, 0.95f, 0.82f, 0.45f);
	}
	return FLinearColor::White;
}

#undef LOCTEXT_NAMESPACE
