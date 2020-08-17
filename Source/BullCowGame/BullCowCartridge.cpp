// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    //PrintLine(TEXT("%i"), FMath::RandRange(0, 10));
    SetupGame();
    //GetValidWords(Words);
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;

    for (FString Word : WordList) // Range based for loop
    {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }
    return ValidWords;
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else // Checking PlayerGuess
    {
        ProcessGuess(PlayerInput);
    }
}

void UBullCowCartridge::SetupGame()
{
    // Welcoming the player
    PrintLine(TEXT("Welcome to bull cows!"));
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArray(Words, *WordListPath);
    Isograms = GetValidWords(Words);
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)]; //TEXT("Brioche");
    Lives = HiddenWord.Len();
    bGameOver = false;

    PrintLine(TEXT("You have %i lives, guess the %i letter Word!"), Lives, HiddenWord.Len());
    PrintLine(TEXT("Type in your guess. \nPress enter to continue..."));     // Prompt player for guess
    PrintLine(TEXT("The HiddenWord is: %s."), *HiddenWord); // Debug line
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (Guess == HiddenWord)
    {
        ClearScreen();
        PrintLine(TEXT("You guessed the word %s correctly\nYou have won the game"), *HiddenWord);
        EndGame();
        return;
    }

    if (Lives <= 1)
    {
        ClearScreen();
        PrintLine(TEXT("You have no lives left!"));
        PrintLine(TEXT("The hidden word was: %s!"), *HiddenWord);
        EndGame();
        return;
    }

    if (Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("Incorrect guess length!\nYou have lost a life and have \n%i Lives remaining! Guess again"),--Lives);
    }
    else
    {
        PrintLine(TEXT("Correct guess length, guess again"));
    }

    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeating letters, guess again"));
        return;
    }
}


void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Press enter to play again..."));
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    // Loop which checks each letter and increments
    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }
    return true;
}
