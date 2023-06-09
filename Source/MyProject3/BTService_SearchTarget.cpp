// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_SearchTarget.h"
#include "MyAIController.h"
#include "MyCharacter.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_SearchTarget::UBTService_SearchTarget()
{
	NodeName = TEXT("SearchTarget");
	Interval = 1.f;
}

void UBTService_SearchTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn != nullptr)
	{
		UWorld* World = CurrentPawn->GetWorld();
		FVector Center = CurrentPawn->GetActorLocation();
		float SearchDistance = 500.f;

		if (World != nullptr)
		{
			TArray<FOverlapResult> OverlapResults;
			FCollisionQueryParams QueryParams(NAME_None, false, CurrentPawn);

			bool Result = World->OverlapMultiByChannel(
				OverlapResults,
				Center,
				FQuat::Identity,
				ECollisionChannel::ECC_EngineTraceChannel2,
				FCollisionShape::MakeSphere(SearchDistance),
				QueryParams);

			if (Result)
			{
				for (auto& OverlapResult : OverlapResults)
				{
					AMyCharacter* MyCharacter = Cast<AMyCharacter>(OverlapResult.GetActor());
					if (MyCharacter)
					{
						if (MyCharacter->GetController()->IsPlayerController())
						{
							OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName("Target"), MyCharacter);
							DrawDebugSphere(World, Center, SearchDistance, 10, FColor::Green, false, 0.5f);
							return;
						}
						
					}
					
				}
			}
			
			DrawDebugSphere(World, Center, SearchDistance, 10, FColor::Red, false, 0.5f);
		}
		else
		{
			DrawDebugSphere(World, Center, SearchDistance, 10, FColor::Red, false, 0.5f);
		}
	}
}
