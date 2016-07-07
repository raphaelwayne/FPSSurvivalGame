// Copyright 2016, raphaelwayne

#include "MyProject.h"
#include "FPSProjectile.h"

#define print(DebugString) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Yellow, DebugString)

// Sets default values
AFPSProjectile::AFPSProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

	// Die after 3 seconds default
	InitialLifeSpan = 3.f;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	// Attributes needed for collision
	CollisionComp->BodyInstance.SetCollisionProfileName("FPSProjectile");
	CollisionComp->InitSphereRadius(15.f);
	CollisionComp->SetNotifyRigidBodyCollision(true);

	// Players can't walk on the projectile
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	CollisionComp->bGenerateOverlapEvents = false;
	// Setup a notification for when this sphere collides with sth
	CollisionComp->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit);
	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	// Set the component to update to our SphereComponent
	ProjectileMovement->UpdatedComponent = CollisionComp;
	// Set attributes of the ProjectileMovement
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 10000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.1f;
}

// Called when the game starts or when spawned
void AFPSProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFPSProjectile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AFPSProjectile::InitVelocity(const FVector& ShootDirection)
{
	if (ProjectileMovement != NULL)
	{
		// Set the projectile velocity to the desired direction
		ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->MaxSpeed;
	}
}

void AFPSProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		print(HitComp->GetName() + " hit " + OtherComp->GetName());
		OtherComp->AddImpulseAtLocation(GetVelocity() * 50.f, GetActorLocation());
		Destroy();
	}
}
