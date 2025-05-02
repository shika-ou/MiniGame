// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/Camera/Modifier/Combat_CameraAnimationModifier.h"

#include "CameraAnimationSequencePlayer.h"
#include "Camera/CameraAnimationHelper.h"
#include "Combat/Camera/Combat_PlayerCameraMangaer.h"
#include "GameFramework/Character.h"

void UCombat_CameraAnimationModifier::CombatTickActiveAnimation(float DeltaTime, FMinimalViewInfo& InOutPOV)
{    
    CombatCameraManager = Cast<ACombat_PlayerCameraMangaer>(CameraOwner);
    ensureMsgf(CombatCameraManager, TEXT("Please use Combat Camera Modifiers only with a player camera manager inheriting from CombatCameraManager."));
    if (!CombatCameraManager)
    {
        return;
    }

    ensureMsgf(ActiveAnimations.Num() <= 1, TEXT("UCombat_CameraAnimationModifier: Trying to play multiple camera animations at the same time. Please use PlaySingleCameraAnimation!"));
    if (ActiveAnimations.Num() >= 1)
    {
        FActiveCameraAnimationInfo& ActiveAnimation = ActiveAnimations[0];
        if (ActiveAnimation.IsValid())
        {
#if ENABLE_DRAW_DEBUG
            //UGCDebugAnimation(ActiveAnimation, DeltaTime);
#endif
            // float const Dilation = UGameplayStatics::GetGlobalTimeDilation(GetWorld());
            // float const UndilatedDeltaTime = FMath::IsNearlyZero(Dilation) ? 0.f : DeltaTime / Dilation;
            CombatTickAnimation(ActiveAnimation, DeltaTime, InOutPOV);

            if (ActiveAnimation.Player->GetPlaybackStatus() == EMovieScenePlayerStatus::Stopped)
            {
                // Here animation has just finished (ease out has completed as well)
                CombatDeactivateCameraAnimation(ActiveAnimation);
                OnAnimationEnded.ExecuteIfBound(ActiveAnimation.Sequence, false);
            }
        }
    }
}

void UCombat_CameraAnimationModifier::CombatTickAnimation(FActiveCameraAnimationInfo& CameraAnimation, float DeltaTime, FMinimalViewInfo& InOutPOV)
{
    check(CameraAnimation.Player);
    check(CameraAnimation.CameraStandIn);

    const FCameraAnimationParams Params = CameraAnimation.Params;
    UCameraAnimationSequencePlayer* Player = CameraAnimation.Player;
    UCameraAnimationSequenceCameraStandIn* CameraStandIn = CameraAnimation.CameraStandIn;

    const FFrameRate InputRate = Player->GetInputRate();
    const FFrameTime CurrentPosition = Player->GetCurrentPosition();
    const float CurrentTime = InputRate.AsSeconds(CurrentPosition);
    const float DurationTime = InputRate.AsSeconds(Player->GetDuration()) * Params.PlayRate;

    const float ScaledDeltaTime = DeltaTime * Params.PlayRate;

    const float NewTime = CurrentTime + ScaledDeltaTime;
    const FFrameTime NewPosition = CurrentPosition + DeltaTime * Params.PlayRate * InputRate;

    // Advance any easing times.
    if (CameraAnimation.bIsEasingIn)
    {
        CameraAnimation.EaseInCurrentTime += DeltaTime;
    }
    if (CameraAnimation.bIsEasingOut)
    {
        CameraAnimation.EaseOutCurrentTime += DeltaTime;
    }

    // Start easing out if we're nearing the end.
    // CameraAnimation may already be easing out if StopCameraAnimation has been called.
    // TODO Interruption and Easing events aren't broadcast when StopCameraAnimation is called.
    if (!Player->GetIsLooping() && !CameraAnimation.bIsEasingOut)
    {
        const float BlendOutStartTime = DurationTime - Params.EaseOutDuration;
        if (NewTime > BlendOutStartTime)
        {
            CameraAnimation.bIsEasingOut = true;
            CameraAnimation.EaseOutCurrentTime = NewTime - BlendOutStartTime;

            if (!bWasEasingOut)
            {
                // Here animation has just started easing out but hasn't finished yet
                OnAnimationEaseOutStarted.ExecuteIfBound(CameraAnimation.Sequence);
            }
        }
    }

    // Check if we're done easing in or out.
    bool bIsDoneEasingOut = false;
    if (CameraAnimation.bIsEasingIn)
    {
        if (CameraAnimation.EaseInCurrentTime > Params.EaseInDuration || Params.EaseInDuration == 0.f)
        {
            CameraAnimation.bIsEasingIn = false;
        }
    }
    if (CameraAnimation.bIsEasingOut)
    {
        if (CameraAnimation.EaseOutCurrentTime > Params.EaseOutDuration)
        {
            bIsDoneEasingOut = true;
        }
    }

    // Figure out the final easing weight.
    const float EasingInT = FMath::Clamp((CameraAnimation.EaseInCurrentTime / Params.EaseInDuration), 0.f, 1.f);
    const float EasingInWeight = CameraAnimation.bIsEasingIn ?
        EvaluateEasing(Params.EaseInType, EasingInT) : 1.f;

    const float EasingOutT = FMath::Clamp((1.f - CameraAnimation.EaseOutCurrentTime / Params.EaseOutDuration), 0.f, 1.f);
    const float EasingOutWeight = CameraAnimation.bIsEasingOut ?
        EvaluateEasing(Params.EaseOutType, EasingOutT) : 1.f;

    const float TotalEasingWeight = FMath::Min(EasingInWeight, EasingOutWeight);

    // We might be done playing. Normally the player will stop on its own, but there are other situation in which
    // the responsibility falls to this code:
    // - If the animation is looping and waiting for an explicit Stop() call on us.
    // - If there was a Stop() call with bImmediate=false to let an animation blend out.
    if (bIsDoneEasingOut || TotalEasingWeight <= 0.f)
    {
        Player->Stop();
        return;
    }

    UMovieSceneEntitySystemLinker* Linker = Player->GetEvaluationTemplate().GetEntitySystemLinker();
    CameraStandIn->Reset(InOutPOV, Linker);

    // Get the "unanimated" properties that need to be treated additively.
    const float OriginalFieldOfView = CameraStandIn->FieldOfView;

    // Update the sequence.
    Player->Update(NewPosition);

    // Recalculate properties that might be invalidated by other properties having been animated.
    CameraStandIn->RecalcDerivedData();

    // Grab the final animated (animated) values, figure out the delta, apply scale, and feed that into the result.
    // Transform is always treated as a local, additive value. The data better be good.
    const float Scale = Params.Scale * TotalEasingWeight;
    const FTransform AnimatedTransform = CameraStandIn->GetTransform();
    FVector AnimatedLocation = AnimatedTransform.GetLocation() * Scale;
    FRotator AnimatedRotation = AnimatedTransform.GetRotation().Rotator() * Scale;
    const FCameraAnimationHelperOffset CameraOffset{ AnimatedLocation, AnimatedRotation };

    FVector OwnerLocation = GetViewTarget()->GetActorLocation();

    // If using a character, camera should start from the pivot location of the mesh.
    {
        ACharacter* OwnerCharacter = Cast<ACharacter>(GetViewTarget());
        if (OwnerCharacter && OwnerCharacter->GetMesh())
        {
            OwnerLocation = OwnerCharacter->GetMesh()->GetComponentLocation();
        }
    }

    FRotator const OwnerRot = FRotator(0.f, GetViewTarget()->GetActorRotation().Yaw, 0.f);
    const FMatrix OwnerRotationMatrix = FRotationMatrix(OwnerRot);

    FMinimalViewInfo InPOV = InOutPOV;

    // Blend from current camera location to actor location
    InPOV.Location = FMath::Lerp(InOutPOV.Location, OwnerLocation, Scale);
    InPOV.Rotation = FMath::Lerp(InOutPOV.Rotation, OwnerRot, Scale);

    FCameraAnimationHelper::ApplyOffset(OwnerRotationMatrix, InPOV, CameraOffset, AnimatedLocation, AnimatedRotation);

    InOutPOV.Location = AnimatedLocation;
    InOutPOV.Rotation = AnimatedRotation;

    // Blend back depending on reset type
    if (CurrentResetType != ECameraAnimationResetType::BackToStart && CameraOwner && CameraOwner->GetOwningPlayerController() && CameraAnimation.bIsEasingOut && !bWasEasingOut)
    {
        FRotator TargetRot = OwnerRot;
        bool const bIsStrafing = CombatCameraManager->IsOwnerStrafing();
        if (!bIsStrafing && CurrentResetType == ECameraAnimationResetType::ContinueFromEnd)
        {
            TargetRot = FRotator(InOutPOV.Rotation.Pitch, InOutPOV.Rotation.Yaw, 0.f);
        }
        CameraOwner->GetOwningPlayerController()->SetControlRotation(TargetRot);
    }

    // FieldOfView follows the current camera's value every frame, so we can compute how much the animation is
    // changing it.
    const float AnimatedFieldOfView = CameraStandIn->FieldOfView;
    const float DeltaFieldOfView = AnimatedFieldOfView - OriginalFieldOfView;
    InOutPOV.FOV = OriginalFieldOfView + DeltaFieldOfView * Scale;

    // Add the post-process settings.
    if (CameraOwner != nullptr && CameraStandIn->PostProcessBlendWeight > 0.f)
    {
        CameraOwner->AddCachedPPBlend(CameraStandIn->PostProcessSettings, CameraStandIn->PostProcessBlendWeight);
    }

    bWasEasingOut = CameraAnimation.bIsEasingOut;
}

void UCombat_CameraAnimationModifier::CombatDeactivateCameraAnimation(FActiveCameraAnimationInfo& ActiveAnimation)
{
    for (auto& ActiveAnimation : ActiveAnimations)
    {
        if (ActiveAnimation.Handle == ActiveAnimation.Handle)
        {
            if (ActiveAnimation.Player && !ensure(ActiveAnimation.Player->GetPlaybackStatus() == EMovieScenePlayerStatus::Stopped))
            {
                ActiveAnimation.Player->Stop();
            }

            ActiveAnimation = FActiveCameraAnimationInfo();
        }
    }
}

bool UCombat_CameraAnimationModifier::ModifyCamera(float DeltaTime, FMinimalViewInfo& InOutPOV)
{
    UCameraModifier::ModifyCamera(DeltaTime, InOutPOV);
    CombatTickActiveAnimation(DeltaTime, InOutPOV);
    return false;
}
