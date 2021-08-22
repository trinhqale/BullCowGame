// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

struct BullCowCount
{
	int32 bull = 0;
	int32 cow = 0;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

	public:
	virtual void BeginPlay() override;
	virtual void OnInput(const FString& Input) override;
	void setupGame();
	void endGame();
	void GameProcess(const FString& Input);
	TArray<FString> getValidWords(const TArray<FString>& WordList);
	bool isIsogram(const FString&);
	BullCowCount ShowHint(const FString& Input) const;

	void GiveHint() const;

	private:
	FString HiddenWord;
	int32 Lives;
	bool GameOver;
	TArray<FString> Words;
	int32 hintCount;
	
};

