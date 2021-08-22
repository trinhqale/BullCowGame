// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArray(Words, *WordListPath);  // load words from the txt file

    setupGame();
    
}

void UBullCowCartridge::setupGame()
{
    PrintLine(TEXT("Hello! Welcome to Bulls Cows Game"));
    TArray<FString> validWordList = getValidWords(Words);
    int32 random = FMath::RandRange(0, validWordList.Num() - 1);
    HiddenWord = validWordList[random];

    Lives = HiddenWord.Len() * HiddenWord.Len() / 2;
    GameOver = false;
    hintCount = 0;
    
    PrintLine(TEXT("Guess the %i letter word"), HiddenWord.Len());
    PrintLine(TEXT("Press [TAB] to access the game"));
    PrintLine(TEXT("Press [Enter] to submit your answer."));
    PrintLine(TEXT("You have %i attempt(s)"), Lives);
    PrintLine(TEXT("Type ""ltqa1202"" to trade 20 percent of total attempts for 1 random letter"));
    PrintLine(TEXT("Can only used once at the beginning."));
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if(GameOver)
    {
        ClearScreen();
        setupGame();
    }
    else
    {
        GameProcess(Input);
        ++hintCount;
    }
}


void UBullCowCartridge::endGame()
{
    GameOver = true;
    PrintLine(TEXT("Press Enter to play again."));
}


void UBullCowCartridge::GameProcess(const FString& Input)
{
    if(HiddenWord == Input) //guess correctly
    {
        PrintLine(TEXT("You have won!"));
        endGame();
        return;
    }

    if(Input == "ltqa1202")
    {
        if(hintCount == 0)
        {
            Lives *= 0.8;
            GiveHint();
            return;
        }
        PrintLine("No more hint!");
        return;
    }
    
    if(Input.Len() != HiddenWord.Len()) //check length
    {
        PrintLine(TEXT("Not the same length. Try again."));
        PrintLine(TEXT("Your answer has %i character(s)"), Input.Len());
        return;
    }

    if(!isIsogram(Input))  //check isogram
    {
        PrintLine(TEXT("Must be isogram. Guess again."));
        return;
    }
    
    //reduce attempts by 1
    PrintLine(TEXT("Wrong answer. You have %i attempt(s) left."), --Lives); 
    if(Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You Lost."));
        PrintLine(TEXT("The word is: %s"), *HiddenWord);
        endGame();
        return;
    }
    BullCowCount count = ShowHint(Input);
    PrintLine(TEXT("You have %i bulls and %i cows"), count.bull, count.cow);
}

bool UBullCowCartridge::isIsogram(const FString& str)
{
    for(int32 i=0; i < str.Len(); i++)
    {
        for (int32 j = i + 1; j <str.Len(); j++)
        {
            if(str[i] == str[j])
                return false;
        }
    }
    return true;
}

TArray<FString> UBullCowCartridge::getValidWords(const TArray<FString>& WordList)
{
    TArray<FString> validList = {};
    for(FString Word : WordList)
    {
        if(Word.Len() >= 4 && Word.Len() <= 9 && isIsogram(Word))
        {
            validList.Emplace(Word);
        }
    }
    return validList;    
}

BullCowCount UBullCowCartridge::ShowHint(const FString& Input) const
{
    BullCowCount count;
    for(int32 i = 0; i < Input.Len(); i++)
    {
        if(Input[i] == HiddenWord[i])
        {
            ++count.bull;
            continue;
        }
        for(int32 j = 1; j < HiddenWord.Len(); j++)
        {
            if(Input[i] == HiddenWord[j])
                ++count.cow;
                break;
        }
    }
    return count;
}

void UBullCowCartridge::GiveHint() const
{
    int32 randomIdx = FMath::RandRange(0, HiddenWord.Len() - 1);
    PrintLine(TEXT("Your random letter is ""%c"" and in index %i"), HiddenWord[randomIdx], randomIdx);
}