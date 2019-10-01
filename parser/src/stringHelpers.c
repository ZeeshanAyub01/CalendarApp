#include "CalendarParser.h"
#include "LinkedListAPI.h"
#include "stringHelpers.h"


void printStringArray(void *toPrint){
    Array *toBePrinted = (Array *)toPrint;
    //printf("The print array function gets called!\n");
    if(toBePrinted->size == 0){
        return;
    }
    char **strings = toBePrinted->array;
    //printf("The print array function assigns the array successfully!\n");
    for(int i=0; i < toBePrinted->size; i++){
        //printf("iterating through the strings array for the %d time\n", i+1);
        printf("%s\n", strings[i]);
        //printf("iterated through the strings array for the %d time\n", i+1);
    }
    //printf("The print array function prints the array successfully!\n");
}

void printString(void *toPrint){

    char *string = (char *)toPrint;

    printf("%s\n", string);
}

void printIntegerArray(void *toPrint){
    Array *toBePrinted = (Array *)toPrint;
    if(toBePrinted->size == 0){
        return;
    }

    int *integers = toBePrinted->array;

    for(int i=0; i<toBePrinted->size; i++){
        printf("%d\n", integers[i]);
    }
}

void deleteStringArray(void *toDelete){
    
    Array *arrayToDelete = (Array *)toDelete;
    if(arrayToDelete->size == 0){
        return;
    }
    char **strings = arrayToDelete->array;

    for(int i = 0; i < arrayToDelete->size; i++){
    free(strings[i]);
    }

    free(strings);
}

void deleteIntegerArray(void *toDelete){
    
    Array *arrayToDelete = (Array *)toDelete;
    /*if(arrayToDelete->size == 0){
        return;
    }*/
    int *integers = arrayToDelete->array;

    /*for(int i = 0; i < arrayToDelete->size; i++){
    free(integers[i]);
    }*/

    free(integers);
}

Array *createArray(void *array, int size, void (*deleteFunction)(void *toBeDeleted), void (*printFunction)(void *toBePrinted) ){

    Array *newArray;
    newArray = calloc(1, sizeof(Array));
    newArray->array = array;
    newArray->size = size;
    newArray->deleteData = deleteFunction;
    newArray->printData = printFunction;
    return newArray;
}

void deleteArray(Array *toDelete){
    if(toDelete == NULL){
        //printf("There's no array to delete\n");
        return;
    }

    Array *arrayToDelete = (Array *)toDelete;
    arrayToDelete->deleteData(arrayToDelete);
    free(arrayToDelete);
}

void printArray(Array *toPrint){
    if(toPrint == NULL){
        //printf("There's no array to print\n");
        return;
    }
    Array *toBePrinted = (Array *)toPrint;
    toBePrinted->printData(toBePrinted);

}

int charToInt(char c){
    
    if(c == '1'){
        return 1;
    }
    if(c == '2'){
        return 2;
    }
    if(c == '3'){
        return 3;
    }
    if(c == '4'){
        return 4;
    }
    if(c == '5'){
        return 5;
    }
    if(c == '6'){
        return 6;
    }
    if(c == '7'){
        return 7;
    }
    if(c == '8'){
        return 8;
    }
    if(c == '9'){
        return 9;
    }
    else{
        return 0;
    }

}

char *singleIntToString(int c){
    
    char *numString = calloc(1, sizeof(char)*2);

    if(c == 1){
        strcpy(numString, "1");
    }
    else if(c == 2){
        strcpy(numString, "2");
    }
    else if(c == 3){
        strcpy(numString, "3");
    }
    else if(c == 4){
        strcpy(numString, "4");
    }
    else if(c == 5){
        strcpy(numString, "5");
    }
    else if(c == 6){
        strcpy(numString, "6");
    }
    else if(c == 7){
        strcpy(numString, "7");
    }
    else if(c == 8){
        strcpy(numString, "8");
    }
    else if(c == 9){
        strcpy(numString, "9");
    }
    else{
        strcpy(numString, "0");
    }
    return numString;

}


char *intToString( int toConvert){
    int digits = 1;
    int c = toConvert;
    char *numString = NULL;
    //int i = 0;

    while(c / 10 > 0){
        c = c/10;
        digits++;
    }
    
    if(digits == 1){
        return singleIntToString(toConvert);
    }
    
    numString = calloc(3, sizeof(char));
    char *one = singleIntToString(toConvert/10);
    char *two = singleIntToString(toConvert%10);
    numString[0] = one[0];
    numString[1] = two[0];
    numString[2] = '\0';
    free(one);
    free(two);
    return numString;
}


int powerOf(int number, int power){

    if(power == 0){
        return 1;
    }

    int toReturn = number;
    while( power > 1){
        toReturn = number*toReturn;
        power--;
    }
    return toReturn;
}

int stringToInt(char *numberString){

    int intToReturn = 0;

    if( !isdigit(numberString[0]) && numberString[0] != '-'){
        return 0;
    }

    for(int i=1; i < strlen(numberString); i++){
        if( !isdigit(numberString[i])){
            return 0;
        }
    }

    for(int i=0; i < strlen(numberString); i++){
        intToReturn = intToReturn + charToInt(numberString[i])*powerOf(10, strlen(numberString) - i - 1);
    }

    if(numberString[0] == '-'){
        intToReturn = intToReturn*(-1);
    }

    return intToReturn;
}


char *copyCharacters(char *toCopy, int startIndex, int endIndex){
    char *toReturn;
    int i = startIndex;
    int index = 0;
    toReturn = calloc(endIndex - startIndex + 1, sizeof(char));
    //strcpy(toReturn, toCopy[startIndex]);
    
    for(i=startIndex; i<endIndex; i++){
        //strcat(toReturn, &toCopy[i]);
        toReturn[index] = toCopy[i];
        index++;
    }
    //strcat(toReturn, "");
    toReturn[index] = '\0';
    return toReturn;
}

char *copyString(char *toCopy){
    char *toReturn = copyCharacters(toCopy, 0, strlen(toCopy));
    return toReturn;
}

int countStringOccurences(char *toCountIn, char *toCount){

    /*if( countStringOccurences(toCountIn, toCount) == 0){
        return NULL;
    }*/

    //Array *indicesOfOccurences;
    char *stringToCount = copyString(toCount);                          //-and stores the indices in an array
    char *tempString;
    //int i = 0;
    int occurences = 0;
    //int *indices;
    Array *indiciesOfFirstChar = findCharOccurences(toCountIn, toCount[0]);
    int *potentialIndices = indiciesOfFirstChar->array;
    //indices = calloc(1, sizeof(int));

    for(int i=0; i<indiciesOfFirstChar->size;i++){
        tempString = copyCharacters(toCountIn,potentialIndices[i], potentialIndices[i]+strlen(toCount));
        //printf("tempString now: %s\n", tempString);
            if(strcmp(tempString, toCount) == 0){
                //indices[occurences] = i; 
                occurences++;
                //indices = realloc(indices, (occurences+1)*sizeof(int));
            }
            //i++;//to advance the index where the string is being checked
       
           free(tempString);
    }

    deleteArray(indiciesOfFirstChar);
    free(stringToCount);
    return occurences;
}

int findFirstOccurence(char *toFindIn, char *toFind){
    char *stringToFind = copyString(toFind);                          //-and stores the indices in an array
    char *tempString;
    int firstOccurence = 0;
    Array *indicesOfFirstChar = findCharOccurences(toFindIn, toFind[0]);
    int *potentialIndices = indicesOfFirstChar->array;
    //printf("toCount is: %s\n",toCount);

    for(int i=0; i<indicesOfFirstChar->size; i++){
        tempString = copyCharacters(toFindIn, potentialIndices[i], potentialIndices[i]+strlen(toFind));
        //printf("tempString now: %s\n", tempString);
            if(strcmp(tempString, toFind) == 0){
                firstOccurence = potentialIndices[i];
                deleteArray(indicesOfFirstChar);
                free(tempString);
                free(stringToFind);
                return firstOccurence;
    }
    free(tempString);
    }
    deleteArray(indicesOfFirstChar);
    free(stringToFind);
    return -1;
}

Array *findStringOccurencesEfficient(char *toCountIn, char *toCount){

    if( countStringOccurences(toCountIn, toCount) == 0){
        return NULL;
    }

    Array *indicesOfOccurences;
    char *stringToCount = copyString(toCount);                          //-and stores the indices in an array
    char *tempString;
    //int i = 0;
    int occurences = 0;
    int *indices;
    Array *indiciesOfFirstChar = findCharOccurences(toCountIn, toCount[0]);
    int *potentialIndices = indiciesOfFirstChar->array;
    indices = calloc(1, sizeof(int));

    for(int i=0; i<indiciesOfFirstChar->size;i++){
        tempString = copyCharacters(toCountIn,potentialIndices[i], potentialIndices[i]+strlen(toCount));
        //printf("tempString now: %s\n", tempString);
            if(strcmp(tempString, toCount) == 0){
                indices[occurences] = i; 
                occurences++;
                indices = realloc(indices, (occurences+1)*sizeof(int));
            }
            i++;//to advance the index where the string is being checked
       
           free(tempString);
    }

    indicesOfOccurences = createArray(indices, occurences, deleteIntegerArray, printIntegerArray);
    free(stringToCount);
    return indicesOfOccurences;
}


Array *findStringOccurences(char *toCountIn, char *toCount){//counts how many times a string or character occurs in a string-
    
    if( countStringOccurences(toCountIn, toCount) == 0){
        return NULL;
    }

    Array *indicesOfOccurences;
    char *stringToCount = copyString(toCount);                          //-and stores the indices in an array
    char *tempString;
    int i = 0;
    int occurences = 0;
    int *indices;

    indices = calloc(1, sizeof(int));

    while(i+strlen(toCount) <= strlen(toCountIn)){
        tempString = copyCharacters(toCountIn, i, i+strlen(toCount));
        //printf("tempString now: %s\n", tempString);
            if(strcmp(tempString, toCount) == 0){
                indices[occurences] = i; 
                occurences++;
                indices = realloc(indices, (occurences+1)*sizeof(int));
            }
            i++;//to advance the index where the string is being checked
       
           free(tempString);
    }

    indicesOfOccurences = createArray(indices, occurences, deleteIntegerArray, printIntegerArray);
    free(stringToCount);
    return indicesOfOccurences;
}

int containsChar(char *string, char toCheck){

    for(int i = 0; i<strlen(string); i++){
        if(string[i] == toCheck){
            return 1;
        }
    }

    return 0;
}

int containsString(char *string, char *toCheck){
    if(findFirstOccurence(string, toCheck) >= 0){
        return 1;
    }
    else{
        return 0;
    }
}

int countCharOccurences(char *toCountIn, char toCount){//counts how many times a character occurs in a string-
    
    int occurences = 0;

    for(int i=0; i<strlen(toCountIn);i++){
        if(toCountIn[i] == toCount){
            occurences++;
        }
    }

    return occurences;
}

int findFirstCharOccurence(char *string, char toFind){
    for(int i=0; i < strlen(string); i++){
        if(string[i] == toFind){
            return i;
        }
    }

    return -1;
}

Array *findCharOccurences(char *toCountIn, char toCount){//counts how many times a character occurs in a string-
    
    /*if( countStringOccurences(toCountIn, toCount) == 0){
        return NULL;
    }*/

    Array *indicesOfOccurences;
   // char *stringToCount = copyString(toCount);                          //-and stores the indices in an array
    //char *tempString;
    
    int occurences = 0;
    int *indices = NULL;

    indices = calloc(1, sizeof(int));

    for(int i=0; i < strlen(toCountIn); i++){
        if( toCountIn[i] == toCount){
                indices[occurences] = i; 
                occurences++;
                indices = realloc(indices, (occurences+1)*sizeof(int));
            }
    }

    indicesOfOccurences = createArray(indices, occurences, deleteIntegerArray, printIntegerArray);
    
    return indicesOfOccurences;
}

int countQuotedStrings(char *string){

    return ( (countCharOccurences(string, '"'))/2 );
}

char *extractFirstQuote(char *string){
    if( countQuotedStrings(string) == 0){
        return NULL;
    }

    Array *indexesOfQuotes = findCharOccurences(string, '"');
    int *intArray = indexesOfQuotes->array;

    char *toReturn = copyCharacters(string, intArray[0]+1, intArray[1]);
    deleteArray(indexesOfQuotes);
    return toReturn;
}

char *stringToUpper(char *toChange){

    char *toChangeCopy = copyString(toChange);

    for(int i=0;i<strlen(toChangeCopy);i++){
        toChangeCopy[i] = toupper(toChangeCopy[i]);
    }

    return toChangeCopy;
}

char *stringToLower(char *toChange){

    char *toChangeCopy = copyString(toChange);

    for(int i=0;i<strlen(toChangeCopy);i++){
        toChangeCopy[i] = tolower(toChangeCopy[i]);
    }

    return toChangeCopy;
}

Array *splitStringIntoTwo(char *string, char *delimiter){
    Array *stringArray;
    char **tokenArray;
    int index = 0;

    tokenArray = calloc( (int)strlen(string), (int)strlen(string)*sizeof(char));
    index = findFirstOccurence(string, delimiter);

    tokenArray[0] = copyCharacters(string, 0, index);
    tokenArray[1] = copyCharacters(string, index + strlen(delimiter), strlen(string) );

    stringArray = createArray(tokenArray, 2, deleteStringArray, printStringArray);
    return stringArray;
}

Array *splitStringCustom(char *string, char *delimiter, int numberOfSplits){
    int occurences = 0;
    int timesSplit = 0;
    char **tokenArrayTemp;
    char **tokenArray;
    int tokenArrayIndex = 0;
    Array *stringArray;
    Array *stringArrayTemp;
    occurences = countStringOccurences(string, delimiter);


    if(occurences == 0){//code for when occurences are 0
        tokenArray = calloc( (int)strlen(string), (int)strlen(string)*sizeof(char));
        tokenArray[0] = copyString(string);
        stringArray = createArray(tokenArray, 1, deleteStringArray, printStringArray);
        //printf("This is the new string:\n%s\n", tokenArray[0]);
        return stringArray;
    }

    if(numberOfSplits > occurences){//to make sure that the maximum number of splits does not exceed the number-
        numberOfSplits = occurences;// -of times the delimiter occurs in the string
    }

    
    if(numberOfSplits == 1){
        stringArray = splitStringIntoTwo(string, delimiter);
        return stringArray;
    }

    else if(numberOfSplits > 1){

        tokenArray = calloc( (int)strlen(string), (int)strlen(string)*sizeof(char));
        tokenArray[tokenArrayIndex] = copyString(string);

        while(timesSplit < numberOfSplits){

        stringArrayTemp = splitStringIntoTwo(tokenArray[tokenArrayIndex], delimiter);
        tokenArrayTemp = stringArrayTemp->array;

        if(timesSplit != numberOfSplits){
            free(tokenArray[tokenArrayIndex]);//frees the string that needs to be broken down further
        }
        tokenArrayIndex++;
        tokenArray[tokenArrayIndex - 1] = copyString(tokenArrayTemp[0]);
        tokenArray[tokenArrayIndex] = copyString(tokenArrayTemp[1]);
        /*free(tokenArrayTemp[0]);
        free(tokenArrayTemp[1]);
        free(tokenArrayTemp);*/
        deleteArray(stringArrayTemp);
        timesSplit++;

        }
    }

    stringArray = createArray(tokenArray, numberOfSplits+1, deleteStringArray, printStringArray);

    return stringArray;
}

Array *splitString(char *string, char *delimiter){
    int occurences = countStringOccurences(string, delimiter);
    Array *tokenArray = splitStringCustom(string, delimiter, occurences);

    return tokenArray;
}


char *insertCharacter(char*string, char charToInsert, int index){//does NOT return a new string (modifies the old string)

    if(index < 0){
        return string;
    }

    if(index > strlen(string) ){
        index = strlen(string);
    }

    //char *newString;
    int i=index;
    int length = strlen(string) - 1;
    string = realloc(string, strlen(string) + 2);

    for( i=length;i>=index;i--){
        string[i+1] = string[i];
        //printf("inserted %c at index: %d\n", string[i+1], i+1);
        //printf("The first string is now temporarily: %s\n", string);
    }
    string[index] = charToInsert;
    string[length+2] = '\0';
    return string;
}

char *insertString(char *toInsertIn, char *toInsert, int index){//returns a new string

    if(index > strlen(toInsertIn)){
    index = strlen(toInsertIn);
    }

    int i = 0;
    int j = 0;
    //int newIndex = index;
    //int originalLength = strlen(toInsertIn);
    //int finalLength = originalLength + strlen(toInsert);
    char *newString;
    newString = copyString(toInsertIn);

    for(i=index; i < index + strlen(toInsert); i++){
    newString = insertCharacter(newString, toInsert[j], i);
    j++;
    }

    return newString;
}

char *concatenateString(char *string1, char *string2){//returns a new string
    //char *newString = copyString(string1);
    char *newString = insertString(string1, string2, strlen(string1));
    return newString;
}

int isNumeric(char *string){
    for(int i = 0; i<strlen(string); i++){
        if(!isdigit(string[i])){
            return 0;
        }
    }

    return 1;
}

int isAlpha(char *string){
    for(int i = 0; i<strlen(string); i++){
        if(!isalpha(string[i])){
            return 0;
        }
    }

    return 1;
}

int isLowerCase(char c){
    int ascii = (int)c;
    if( (ascii <= 122) && (ascii >= 91) ){
        return 1;
    }
    return 0;
}

int isUpperCase(char c){
    int ascii = (int)c;
    if( (ascii <= 90) && (ascii >= 65) ){
        return 1;
    }
    return 0;
}

int isAllLowerCase(char *string){

    for(int i=0; i<strlen(string);i++){
        if(isUpperCase(string[i])){
            return 0;
        }
    }

    return 1;
}

int isAllUpperCase(char *string){

    for(int i=0; i<strlen(string);i++){
        if(isLowerCase(string[i])){
            return 0;
        }
    }

    return 1;
}

char changeCase(char c){
    if(isUpperCase(c)){
        return c+32;
    }
    else if(isLowerCase(c)){
        return c-32;
    }
    else{
        return c;
    }
}


int compareStrings(char *string1, char *string2){
    if(strlen(string1) != strlen(string2)){
        return 0;
    }

    for(int i=0; i<strlen(string1);i++){
        if(string1[i] != string2[i]){
            return 0;
        }
    }

    return 1;
}

int compareStringsIgnoreCase(char *string1, char *string2){
    int comparison;
    char *tempString1 = stringToLower(string1);
    char *tempString2 = stringToLower(string2);

    comparison = compareStrings(tempString1, tempString2);
    free(tempString1);
    free(tempString2);
    return comparison;
}

int beginsWith(char *string, char *toCheck){

    if(strlen(toCheck) > strlen(string)){
        return 0;
    }

    char *tempString = copyCharacters(string, 0, strlen(toCheck));
    if( compareStrings(tempString, toCheck) ){
        free(tempString);
        return 1;
    }
    else{
        free(tempString);
        return 0;
    }
}

int beginsWithIgnoreCase(char *string, char *toCheck){

    if(strlen(toCheck) > strlen(string)){
        return 0;
    }

    char *tempString = copyCharacters(string, 0, strlen(toCheck));
    if( compareStringsIgnoreCase(tempString, toCheck) ){
        free(tempString);
        return 1;
    }
    else{
        free(tempString);
        return 0;
    }
}

int endsWith(char *string, char *toCheck){

    if(strlen(toCheck) > strlen(string)){
        return 0;
    }

    char *tempString = copyCharacters(string, strlen(string) - strlen(toCheck),strlen(string));
    if( compareStrings(tempString, toCheck) ){
        free(tempString);
        return 1;
    }
    else{
        free(tempString);
        return 0;
    }
}

int endsWithIgnoreCase(char *string, char *toCheck){

    if(strlen(toCheck) > strlen(string)){
        return 0;
    }

    char *tempString = copyCharacters(string, strlen(string) - strlen(toCheck),strlen(string));
    if( compareStringsIgnoreCase(tempString, toCheck) ){
        free(tempString);
        return 1;
    }
    else{
        free(tempString);
        return 0;
    }
}

char *removeInitialSpaces(char *string){
    int i = 0;
    char *toReturn;

    while( (string[i] == ' ') || (string[i] == '\t') ) {
        i++;
    }

    if(i==0){
        toReturn = string;
    }
    else{
    //toReturn = copyCharacters(string, i, strlen(string));
    toReturn = copyCharacters(string, 1, strlen(string));
    }

    return toReturn;
}

char *removeStringOnce(char *string, char *toRemove){
    int occurences = countStringOccurences(string, toRemove);
    char *toReturn = NULL;
    Array *stringArray = NULL;
    char **strings = NULL;

    if(occurences == 0){
        toReturn = copyString(string);
        return toReturn;
    }

    stringArray = splitStringIntoTwo(string, toRemove);
    strings = stringArray->array;
    toReturn = concatenateString(strings[0], strings[1]);
    deleteArray(stringArray);
    return toReturn;

}

char *fileRead(char *fileName){

  FILE *fptr = NULL;
  fptr = fopen(fileName, "r");
  char *buffer;
  char c;
  int i = 0;
  c = ' ';
  int j = 1;

  if( fptr == NULL){
    //printf("Unable to open %s!!\n", fileName);
    return NULL;
  }
  else{
    buffer = calloc(1, sizeof(char));
    //printf("File opened successfully!\nI'm going to read it now...\n");
        while( !feof(fptr) ){
      fscanf(fptr, "%c", &c);
      buffer[i] = c;
      //printf("%c", c);
      buffer = realloc( buffer, (j+1 )*(sizeof(char)) );
      j++;
      i++;
        }
    }

    buffer[i-1] = '\0';
    //printf("\nTotal characters read in: %d\n", i+1);
    fclose(fptr);

  return buffer;
}

int addAllStringLengths(Array *stringsArray){
    int sum = 0;
    char **strings = stringsArray->array;

    for(int i = 0; i < stringsArray->size; i++){
        sum = sum + (int)strlen(strings[i]);
    }

    return sum;
}

Array *moveStringsUpByOne(Array *toModify, int index){//deletes content at the specified index, and moves everything else up by one
    
    char **arrayToModify = toModify->array;

    if( index < toModify->size - 1 ){
    
    for(int i = index; i < toModify->size - 1 ; i++){
        free(arrayToModify[i]);
        arrayToModify[i] = copyString(arrayToModify[i+1]);
    }
    }

    free( arrayToModify[toModify->size - 1] );
    toModify->size--;
    toModify = realloc(toModify, addAllStringLengths(toModify)*sizeof(char) );
    return toModify;

}

Array *unfoldAllLines(Array *stringsArray){
    char **strings = stringsArray->array;
    char *spacesRemoved = NULL;
    char *unfoldedString = NULL;

    for(int i = 0; i < stringsArray->size; i++){

        if( (beginsWith(strings[i], " ") || (beginsWith(strings[i], "\t")) )){
           spacesRemoved = removeInitialSpaces(strings[i]);
           //printf("%s length: %ld \n", spacesRemoved, strlen(spacesRemoved) );
           if(spacesRemoved[0] != ';'){//checks to see if the unfolded line is a comment
           unfoldedString = concatenateString(strings[i-1], spacesRemoved);
           free(strings[i-1]);
           strings[i-1] = copyString(unfoldedString);
           stringsArray = moveStringsUpByOne(stringsArray, i);
           i--;
           free(unfoldedString);
           }
           free(spacesRemoved);
           
    
        }
    }

    return stringsArray;
}



