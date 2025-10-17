// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal.h"
#include "firstperson415Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APortal::APortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	boxComp = CreateDefaultSubobject<UBoxComponent>("Box Comp");
	sceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>("Capture");
	rootArrow = CreateDefaultSubobject<UArrowComponent>("RootArrow");

	RootComponent = boxComp;
	mesh->SetupAttachment(boxComp);
	sceneCapture->SetupAttachment(mesh);
	rootArrow->SetupAttachment(RootComponent);

	mesh->SetCollisionResponseToAllChannels(ECR_Ignore);

}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	Super::BeginPlay();
	

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnOverlapBegin);
	//makes it so it doesnt render itself i think
	mesh->SetHiddenInSceneCapture(true);


	//checks if material is valid before continuing
	if (mat)
	{
		mesh->SetMaterial(0, mat);
	}

}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdatePortals();
}

void APortal::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//casts to player character on overlap
	Afirstperson415Character* playerChar = Cast<Afirstperson415Character>(OtherActor);

	//checks that cast is valid
	if (playerChar)
	{
		//Checks that other portal is valid
		if (OtherPortal)
		{
			//makes sure character isnt teleporting already
			if (!playerChar->isTeleporting)
			{
				//teleports player
				playerChar->isTeleporting = true; 
				FVector loc = OtherPortal->rootArrow->GetComponentLocation();
				playerChar->SetActorLocation(loc);

				//creates a timer to regulate teleport cooldown
				FTimerHandle TimerHandle;
				FTimerDelegate TimerDelegate;
				TimerDelegate.BindUFunction(this, "SetBool", playerChar);
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 1, false);

			}
		}
	}


}

void APortal::SetBool(Afirstperson415Character* playerChar)
{
	
	//sets teleporting to false when the timer completes
	if (playerChar)
	{
		playerChar->isTeleporting = false;
	}
}

void APortal::UpdatePortals()
{
	
	//finds midpoint between this portal and the other portals locations in the world
	FVector Location = this->GetActorLocation() - OtherPortal->GetActorLocation();
	//gets player camera component world location
	FVector camLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetTransformComponent()->GetComponentLocation();
	//gets camera component world rotation
	FRotator camRotation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetTransformComponent()->GetComponentRotation();
	//combines the location of the player camera and the midpoint
	FVector CombinedLocation = camLocation + Location;

	sceneCapture->SetWorldLocationAndRotation(CombinedLocation, camRotation);
}
