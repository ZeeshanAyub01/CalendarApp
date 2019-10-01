/*
The functions in this API help with string manipulation with file parsing with C
*/
#ifndef STRING_HELPERS_H
#define STRING_HELPERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "CalendarParser.h"
#include "LinkedListAPI.h"
#include "stringHelpers.h"

typedef struct ARRAY{
    void *array;
    int size;
    void (*deleteData)(void *toBeDeleted);
    void (*printData)(void *toBePrinted);
    
}Array;

void printStringArray(void *toPrint);

void printString(void *toPrint);

void printIntegerArray(void *toPrint);

void deleteStringArray(void *toDelete);

void deleteIntegerArray(void *toDelete);

Array *createArray(void *array, int size, void (*deleteFunction)(void *toBeDeleted), void (*printFunction)(void *toBePrinted) );

void deleteArray(Array *toDelete);

void printArray(Array *toPrint);

int charToInt(char c);

char *singleIntToString(int c);

char *intToString(int toConvert);

int stringToInt(char *numberString);

int powerOf(int number, int power);

char *copyCharacters(char *toCopy, int startIndex, int endIndex);

char *copyString(char *toCopy);

int containsString(char *string, char *toCheck);

int containsChar(char *string, char toCheck);

int countStringOccurences(char *toCountIn, char *toCount);

int findFirstOccurence(char *toFindIn, char *toFind);

Array *findStringOccurences(char *toCountIn, char *toCount);

int countCharOccurences(char *toCountIn, char toCount);

int findFirstCharOccurence(char *string, char toFind);

Array *findCharOccurences(char *toCountIn, char toCount);

int countQuotedStrings(char *string);

char *extractFirstQuote(char *string);

char *stringToUpper(char *toChange);

char *stringToLower(char *toChange);

Array *splitStringIntoTwo(char *string, char *delimiter);

Array *splitStringCustom(char *string, char *delimiter, int numberOfSplits);

Array *splitString(char *string, char *delimiter);

char *insertCharacter(char*string, char charToInsert, int index);

char *insertString(char *toInsertIn, char *toInsert, int index);

char *concatenateString(char *string1, char *string2);

int isNumeric(char *string);

int isAlpha(char *string);

int isLowerCase(char c);

int isUpperCase(char c);

int isAllLowerCase(char *string);

int isAllUpperCase(char *string);

char changeCase(char c);

int compareStrings(char *string1, char *string2);

int compareStringsIgnoreCase(char *string1, char *string2);

int beginsWith(char *string, char *toCheck);

int beginsWithIgnoreCase(char *string, char *toCheck);

int endsWith(char *string, char *toCheck);

int endsWithIgnoreCase(char *string, char *toCheck);

char *removeInitialSpaces(char *string);

char *removeStringOnce(char *string, char *toRemove);

char *fileRead(char *fileName);

int addAllStringLengths(Array *stringsArray);

Array *moveStringsUpByOne(Array *toModify, int index);

Array *unfoldAllLines(Array *stringsArray);

Array *findStringOccurencesEfficient(char *toCountIn, char *toCount);

#endif