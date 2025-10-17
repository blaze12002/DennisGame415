// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "firstperson415Projectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;

UCLASS(config=Game)
class Afirstperson415Projectile : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	//Creates a reference to the projectiles static mesh
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* ballMesh;

	//Creates a reference for the projectiles basic material
	UPROPERTY(EditAnywhere)
		UMaterial* baseMat;

	//Creates a reference to a random color
	UPROPERTY()
		FLinearColor randColor;

	//Creates a material interface reference
	UPROPERTY(EditAnywhere)
		UMaterialInterface* projMat;

	//creates a dynamic material instance reference
	UPROPERTY()
		UMaterialInstanceDynamic* dmiMat;

	UPROPERTY(EditAnywhere)
		UNiagaraSystem* colorP;

public:
	Afirstperson415Projectile();

protected:
	//triggers when projectile spawns into world
	virtual void BeginPlay();


public:
	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};

