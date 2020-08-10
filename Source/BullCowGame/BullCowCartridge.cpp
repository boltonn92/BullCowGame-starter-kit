// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    GetValidWords(Words);
    SetupGame();
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArray(Words, *WordListPath);
    PrintLine(TEXT("The number of possible words is %i"), Words.Num());
    PrintLine(TEXT("The number of valid words is: %i"), GetValidWords(Words).Num());
    //PrintLine(TEXT("The HiddenWord is: %s."), *HiddenWord); // Debug line
}

TArray<FString> UBullCowCartridge::GetValidWords(TArray<FString> WordList) const
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

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else // Checking PlayerGuess
    {
        ProcessGuess(Input);
    }
}

void UBullCowCartridge::SetupGame()
{
    // Welcoming the player
    PrintLine(TEXT("Welcome to bull cows!"));

    HiddenWord = TEXT("Brioche");
    Lives = HiddenWord.Len();
    bGameOver = false;

    PrintLine(TEXT("You have %i lives."), Lives);
    PrintLine(TEXT("Guess the %i letter word..."), HiddenWord.Len());
    PrintLine(TEXT("Type in your guess. \nPress enter to continue..."));     // Prompt player for guess
   }

void UBullCowCartridge::ProcessGuess(FString Guess)
{
    if (Guess == HiddenWord)
    {
        ClearScreen();
        PrintLine(TEXT("You guessed the word %s correctly\nYou have won the game"), *HiddenWord);
        EndGame();
        return;
    }

    // THIS BLOCK OF CODE FUCKS UP THE FLOW
    //if (Guess.Len() != HiddenWord.Len())
    //{
    //    LoseLife(Guess);
    //    return;
    //}

    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeating letters, guess again"));
        return;
    }
      else
      {
            if (Lives > 1)
            {
                LoseLife(Guess);
            }
            else
            {
                ClearScreen();
                PrintLine(TEXT("You have no lives left!"));
                PrintLine(TEXT("The hidden word was: %s!"), *HiddenWord);
                EndGame();
                return;
            }
      }
}

void UBullCowCartridge::LoseLife(FString Guess)
{
    if (Guess.Len() == HiddenWord.Len())
    {
        PrintLine(TEXT("You have lost a life! \nSorry, try guessing again! \nYou have %i lives remaining"), --Lives);
    }
    else
    {
        PrintLine(TEXT("The hidden word is %i letters long"), HiddenWord.Len());
        PrintLine(TEXT("You have lost a life! \nSorry, try guessing again! \nYou have %i lives remaining"), --Lives);
    }
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Press enter to play again..."));
}

bool UBullCowCartridge::IsIsogram(FString Word) const
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
