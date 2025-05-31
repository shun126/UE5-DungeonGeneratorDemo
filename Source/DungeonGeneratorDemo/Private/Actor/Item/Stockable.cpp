/**
在庫可能なアイテムのクラス ソースファイル

https://historia.co.jp/archives/3990/
*/

#include "Stockable.h"
#include "Helper/Log.h"
#include "Helper/Math.h"
#include "Widget/HUD/BalloonWidgetBase.h"
#include <Components/StaticMeshComponent.h>
#include <Components/WidgetComponent.h>
#include <Internationalization/StringTable.h>
#include <cmath>

#include "Actor/PlayerBase.h"

AStockable::AStockable(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// StaticMeshコンポーネントを生成
	Mesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	check(IsValid(Mesh));
	Mesh->SetMobility(EComponentMobility::Movable);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//Mesh->SetCollisionObjectType(ECC_Item);
	//Mesh->SetCollisionProfileName(CPN_Item);
	Mesh->SetSimulatePhysics(true);
	Mesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	SetRootComponent(Mesh);

	// Sphereコンポーネントを生成
	Sphere = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("Sphere"));
	check(IsValid(Sphere));
	Sphere->SetMobility(EComponentMobility::Movable);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionProfileName(TEXT("OverlapAll"));
	Sphere->SetSimulatePhysics(false);
	Sphere->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	Sphere->InitSphereRadius(50.f);
	Sphere->SetupAttachment(Mesh);

	// UWidgetComponentコンポーネントを生成
	Widget = ObjectInitializer.CreateDefaultSubobject<UWidgetComponent>(this, TEXT("Widget"));
	check(IsValid(Widget));
	Widget->SetWidgetSpace(EWidgetSpace::Screen);
	Widget->SetWidgetClass(UBalloonWidgetBase::StaticClass());
	Widget->SetDrawAtDesiredSize(true);
	Widget->SetupAttachment(Mesh);
}

void AStockable::Equip_Implementation(const bool playerAttack)
{
	if (IsValid(Mesh))
	{
		Mesh->SetSimulatePhysics(false);
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (IsValid(Widget))
	{
		Widget->Deactivate();
	}
	SetActorTickEnabled(false);
	PopOnBegin = false;
	OnEquip(playerAttack);
}

void AStockable::UnEquip_Implementation()
{
	if (IsValid(Mesh))
	{
		Mesh->SetSimulatePhysics(true);
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	if (IsValid(Widget))
	{
		Widget->Activate();
	}
	SetActorTickEnabled(true);
	OnUnEquip();
}

FVector AStockable::GetImpulseAtAppear() const
{
	const float theta = mRandomStream.RandRange(-3.14159265359f, 3.14159265359f);
	const float length = mRandomStream.GetFraction() * 50.f;
	const float x = std::sin(theta) * length;
	const float y = std::cos(theta) * length;
	const float z = mRandomStream.RandRange(400.f, 600.f);
	return FVector(x, y, z);
}

FVector AStockable::GetImpulseAtDrop() const
{
	const float theta = mRandomStream.RandRange(-3.14159265359f, 3.14159265359f);
	const float length = mRandomStream.GetFraction() * 50.f;
	return FVector(std::sin(theta) * length, std::cos(theta) * length, 0.f);
}

FText AStockable::GetLabel_Implementation() const
{
	//DUNGEON_GENERATOR_DEMO_ERROR(TEXT("Cannot obtain StringTable name and key for actor (%s)"), *GetName());
	return FText();
}

UStaticMeshComponent* AStockable::GetMesh()
{
	return Mesh;
}

void AStockable::BeginPlay()
{
	// 親アクターを呼び出す
	Super::BeginPlay();

	// ラベル名を設定します
	if (auto* balloonWidget = GetValid(Cast<UBalloonWidgetBase>(Widget->GetUserWidgetObject())))
	{
		balloonWidget->SetLabel(GetLabel());
	}
	else
	{
#if WITH_EDITOR
		const FString name = GetName();
		DUNGEON_GENERATOR_DEMO_ERROR(TEXT("アクター(%s)のユーザーウィジットはUBalloonWidgetBaseの派生クラスではありません"), *name);
#endif
	}

	// 弾け出す
	if (PopOnBegin && IsValid(Mesh))
	{
#if WITH_EDITOR
		if (!Mesh->IsSimulatingPhysics())
		{
			const FString& name = GetName();
			DUNGEON_GENERATOR_DEMO_ERROR(TEXT("アクター(%s)は物理シミュレーションが無効でした"), *name);
		}
#endif

		const FVector& impulse = GetImpulseAtAppear();
		Mesh->AddImpulse(impulse, NAME_None, true);
	}
}

void AStockable::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AStockable::Tick(float DeltaSeconds)
{
	// 親アクターを呼び出す
	Super::Tick(DeltaSeconds);

	// 噴き出しを更新
	if (auto* balloonWidget = GetValid(Cast<UBalloonWidgetBase>(Widget->GetUserWidgetObject())))
	{
		balloonWidget->Update(this);
	}

	TimeUntilExpiration -= DeltaSeconds;
	if (TimeUntilExpiration <= 0.f)
	{
		Destroy();
	}
}
