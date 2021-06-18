# TankAndSpank with Popcorn Grenades
The C++ classes for my implementation of ToonTanks.  

  
  I haven't got the health progress bar widget updating correctly yet, but here's what I've ended up with so far.  
https://www.youtube.com/watch?v=w-RfGG-5yMg  
  

- Sound cues were used to play random sounds. Each impact/explosion/launcher sound effect has about 3-4 variations each, as well as modulated for slightly random pitch.  
![image](https://i.imgur.com/2RRmvXe.png)  
  
- Attenuation is used for binaural 3d audio spatialization as well as dampening through the air with distance, which is a big reason why it sounds cool I think. This takes a lot tweaking since you don't have visualizations for audio and frequencies like you do in something like Ableton. 

- With so many sound sources, I had to try and arrange priority through Attenuation as well. Bigger explosions, and things closer to you generally given higher priority. Sometimes sounds randomly don't play still and I haven't figured it out yet.  

- Since the grenades roll and bounce on the ground, constant contact while rolling would cause a mess of sound cue spam, so I tried to use game time to set cooldowns to avoid this, as well as choose sounds that included rolling sounds for tails to help fill it out a bit.
```cpp
/// Play sound effect at location, with a cooldown.
void AProjectileBase::PlaySoundNoSpam(USoundBase* SoundToPlay)
{
	// This is so the hit sound doesn't spam when rolling against the ground.
	WorldTime = UGameplayStatics::GetTimeSeconds(GetWorld());
	float Cooldown = WorldTime - TimeHitSoundPlayed;
	if (Cooldown >= 1) {
		TimeHitSoundPlayed = WorldTime;
		UGameplayStatics::PlaySoundAtLocation(this, SoundToPlay, GetActorLocation());
	}
}
```

- Early on I wanted a more over-the-shoulder view, so that required input changes, camera springarm hiearchy changes, and adding a counter-rotation to allow the view and turret to decouple from tank.
```cpp
// -------------------------------------------------------------------------------------------
/// Rotate turret with mouse (alternative to aiming at cursor).
void APawnTank::RotateView(float Input)
{
	FRotator Rotation = TurretMesh->GetRelativeRotation();
	Rotation.Yaw += Input * MouseSensitivity * GetWorld()->DeltaTimeSeconds;
	TurretMesh->SetRelativeRotation(Rotation);
}

// -------------------------------------------------------------------------------------------
/// Determine rotation speed and direction on y-axis (spinning/yaw). Update RotationDirection.
void APawnTank::RotateTank(float Input)
{
	float Pitch = 0;
	float Yaw = Input * TurnSpeed * GetWorld()->DeltaTimeSeconds;
	float Roll = 0;

	FRotator Rotation = FRotator(Pitch, Yaw, Roll);
	FRotator CounterRotation = FRotator(Pitch, -Yaw, Roll);

	RotationDirection = FQuat(Rotation);
	AddActorLocalRotation(RotationDirection, true);

	// Counter rotate the turret so the view stays the same.
	TurretMesh->AddLocalRotation(CounterRotation, false);
}
```
- I wanted to be able to hold down LMB to continuously fire, so I had to setup a little toggle function that's called by both Pressed and Released.
```cpp
// -------------------------------------------------------------------------------------------
void APawnTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForwardAndBack", this, &APawnTank::MoveTank);
	PlayerInputComponent->BindAxis("TurnRightAndLeft", this, &APawnTank::RotateTank);
	PlayerInputComponent->BindAxis("RotateTurret", this, &APawnTank::RotateView);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APawnTank::FireToggle);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &APawnTank::FireToggle);
}

// -------------------------------------------------------------------------------------------
/// If IsFiring is set to true then we'll keep shooting as fast as the FireRateTimer wants us to.
void APawnTank::CheckFireCondition()
{
	if (IsFiring && PlayerAlive)
		Fire();
}

// -------------------------------------------------------------------------------------------
/// Toggle IsFiring bool. Only ever called on press and on release from "Fire" input.
void APawnTank::FireToggle()
{
	if (!IsFiring) {
		IsFiring = true;
	}
	else if (IsFiring) {
		IsFiring = false;
	}
}
```

- I wanted the grenades to bounce around for awhile if they missed, so I used timers to delay explosions if they didn't hit any valid targets by the time their life span expires. This also meant I needed to be sure what the grenades were hitting were specifically a turret or the player before firing off all particles, or destruction, or sounds or anything else.

```cpp
/// When this actor hits another actor. Handles damage, type, etc.
void AProjectileBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* MyOwner = GetOwner();

	if (!MyOwner) {
		return;
	}

	// If we hit another actor:
	if (OtherActor) {
		// We can first check if the other actor is a PawnTurret or PawnTank, and save it as a bool.
		IsTurret = OtherActor->GetClass()->IsChildOf(APawnTurret::StaticClass());
		IsTank = OtherActor->GetClass()->IsChildOf(APawnTank::StaticClass());
	}

	// So if we hit a turret or the player, but not ourselves.
	if (IsTurret || IsTank && OtherActor != GetOwner()) {
		// Play hit particle.
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticle, GetActorLocation());
		// PLay metal impact sound when hit directly.
		PlaySoundNoSpam(DirectImpactSound);

		// Generate and apply the damage.
		UGameplayStatics::ApplyDamage(
			OtherActor,			    // Actor that will be damaged.
			Damage,				    // Damage amount.
			MyOwner->GetInstigatorController(), // Which player instigated it.
			this,				    // What actor caused the damage.
			DamageType			    // Type of damage done.
			);
		DestroyProjectile();
	}

	// Shake Camera if it's the player hit.
	if (IsTank) {
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitShake, HitShakeScale);

		DestroyProjectile();
	}

	// Play this sound whenever we bounce off anything.
	PlaySoundNoSpam(ImpactSound);

	// Explode the grenade after ExplosionTimer seconds if we never hit anything.
	GetWorld()->GetTimerManager().SetTimer(
		OUT ExplosionTimerHandle,
		this,
		&AProjectileBase::DestroyProjectile,
		ExplosionTimer,
		false
		);
}
```

- And I added a radial impulse to the grenades when they explode, to cause all the other nearby projectiles to act as a shrapnel and bring the chaos up:  
  
```cpp
/// Check for actors in radius of explosion and apply impulse.
void AProjectileBase::CreateExplosionImpulse(FVector Location)
{
	// Create basic collision shape!
	FCollisionShape Spherical = FCollisionShape::MakeSphere(ImpulseRadius);

	// Do a sweep check in a radius with SweepMultiChannel().
	bool SweepHit = GetWorld()->SweepMultiByChannel(
		OUT HitResults,		    // Our array of results.
		GetActorLocation(),	    // Start location.
		GetActorLocation() * 1.01f, // End location (has to be different than start).
		FQuat::Identity,            // Rotation (none needed, so blank FQuat).
		ECC_WorldStatic,	    // Collision channel.
		Spherical	   	    // Shape.
		);

	if (SweepHit) {
		for (auto& Hit: HitResults) {
			// First, see if the hit actor has a mesh component.
			UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(Hit.GetActor()->GetRootComponent());
			// If there is, we'll apply the radial impulse to it.
			if (Mesh) {
				// Grab the mass so we can use more reasonable force numbers.
				float Mass = Mesh->GetMass();

				Mesh->AddRadialImpulse(
					GetActorLocation(),  // Impulse Location.
					ImpulseRadius,	     // Radius.
					ImpulseForce * Mass, // Force.
					RIF_Constant,	     // RIF (Radial Impact Force) falloff type.
					false);
```

