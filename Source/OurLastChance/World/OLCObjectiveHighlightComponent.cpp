#include "World/OLCObjectiveHighlightComponent.h"

#include "Components/BillboardComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Core/OLCTutorialSubsystem.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

// ---------------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------------

UOLCObjectiveHighlightComponent::UOLCObjectiveHighlightComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;

	// Billboard sprite — always faces the camera (including a top-down RTS
	// view), no depth test by default, so it stays visible at any distance.
	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	Billboard->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Billboard->SetMobility(EComponentMobility::Movable);

	// Pulsing ground ring. NOTE: the engine content ships no torus/ring mesh
	// (checked /Engine/BasicShapes + editor content), so a flat-scaled Cylinder
	// is used as the ring — the standard RTS selection-pad look, visible from
	// directly above. See Knowledge/runs/WP-129/OPEN.md (step 3).
	Ring = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ring"));
	Ring->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Ring->SetMobility(EComponentMobility::Movable);
	Ring->SetGenerateOverlapEvents(false);
	Ring->SetCastShadow(false);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	if (CylinderMesh.Succeeded())
	{
		Ring->SetStaticMesh(CylinderMesh.Object);
	}
}

// ---------------------------------------------------------------------------
// Lifecycle
// ---------------------------------------------------------------------------

void UOLCObjectiveHighlightComponent::BeginPlay()
{
	Super::BeginPlay();
	ApplyVisualParameters();
}

void UOLCObjectiveHighlightComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	PulseTime += DeltaTime;
	const float Wave = FMath::Sin(2.0f * PI * PulseSpeedHz * PulseTime);
	const float Factor = 1.0f + PulseAmplitude * Wave;

	if (Ring)
	{
		Ring->SetRelativeScale3D(FVector(BaseRingScale.X * Factor, BaseRingScale.Y * Factor, 0.12f));
	}

	// UE 5.8 billboards have no SpriteScale — size is world scale (base sprite
	// = texture pixel width in world units at scale 1).
	if (Billboard)
	{
		Billboard->SetWorldScale3D(BaseBillboardScale * FMath::Max(0.25f, 1.0f + 0.5f * PulseAmplitude * Wave));
	}
}

// ---------------------------------------------------------------------------
// Visual parameters from UOLCTutorialData
// ---------------------------------------------------------------------------

void UOLCObjectiveHighlightComponent::ApplyVisualParameters()
{
	const FOLCTutorialHighlightParams Params = [&]() -> FOLCTutorialHighlightParams
	{
		if (const UWorld* World = GetWorld())
		{
			if (const UGameInstance* GI = World->GetGameInstance())
			{
				if (const UOLCTutorialSubsystem* Tutorial = GI->GetSubsystem<UOLCTutorialSubsystem>())
				{
					if (const UOLCTutorialData* Data = Tutorial->GetTutorialData())
					{
						return Data->HighlightParams;
					}
				}
			}
		}
		return UOLCTutorialData::GetDefaultHighlightParams();
	}();

	PulseSpeedHz = FMath::Max(0.05f, Params.PulseSpeed);
	PulseAmplitude = FMath::Clamp(Params.PulseAmplitude, 0.0f, 1.0f);

	// Ring size: data-driven radius wins over the component fallback.
	const float EffectiveRingRadius = Params.RingRadius > 0.0f ? Params.RingRadius : RingRadius;
	BaseRingScale = FVector2D(EffectiveRingRadius, EffectiveRingRadius);
	if (Ring)
	{
		Ring->SetRelativeLocation(FVector::ZeroVector);
		Ring->SetRelativeScale3D(FVector(BaseRingScale.X, BaseRingScale.Y, 0.12f));

		// Apply the tutorial highlight color when the base material exposes a
		// Color parameter (no-op otherwise — the ring still pulses via scale).
		if (UMaterialInterface* BaseMaterial = Ring->GetMaterial(0))
		{
			RingMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, nullptr);
			if (RingMaterial)
			{
				RingMaterial->SetVectorParameterValue(TEXT("Color"), Params.HighlightColor);
				Ring->SetMaterial(0, RingMaterial);
			}
		}
	}

	// Billboard texture: data-driven when set, otherwise the engine white square.
	UTexture2D* Texture = Params.BillboardTexture.LoadSynchronous();
	if (!Texture)
	{
		Texture = LoadObject<UTexture2D>(nullptr, TEXT("/Engine/EngineResources/WhiteSquareTexture"));
	}
	BaseSpriteScale = BillboardSpriteScale;
	if (Billboard && Texture)
	{
		Billboard->SetSprite(Texture);
		Billboard->SetRelativeLocation(FVector(0.0f, 0.0f, BillboardHeight));

		// World-scale so the sprite spans ~BillboardSpriteScale world units.
		// FSpriteSceneProxy: ViewedSize = MaxAxisScale * 0.25 * TextureWidth,
		// hence the factor of 4 (a non-screen-scaled billboard renders its
		// full texture at Scale*PixelSize world units).
		const float TexSize = FMath::Max(1.0f, static_cast<float>(Texture->GetSurfaceWidth()));
		BaseBillboardScale = FVector::OneVector * (4.0f * BaseSpriteScale / TexSize);
		Billboard->SetWorldScale3D(BaseBillboardScale);
	}
}
