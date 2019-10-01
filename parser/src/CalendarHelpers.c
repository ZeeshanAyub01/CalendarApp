#include "CalendarParser.h"
#include "LinkedListAPI.h"
#include "stringHelpers.h"
#include "CalendarHelpers.h"

ICalErrorCode parseOptionalProperties(char *propertyString, Property **propertyToReturn){
    char *propertyName = NULL;
    char *propertyDesc = NULL;
    ICalErrorCode proprtyCode = OK;
    int firstColonIndex = findFirstCharOccurence(propertyString, ':');
    int firstSemicolonIndex = findFirstCharOccurence(propertyString, ';');
    
    if( !containsChar(propertyString, ':') ){
        propertyName = copyCharacters(propertyString, 0, firstSemicolonIndex);
        propertyDesc = copyCharacters(propertyString, firstSemicolonIndex + 1, strlen(propertyString));
    }

    else if ( containsChar(propertyString, ':') ){
        //printf("This is the property: %s\n",propertyString);
        propertyName = copyCharacters(propertyString, 0, firstColonIndex);
        propertyDesc = copyCharacters(propertyString, firstColonIndex + 1, strlen(propertyString));
    }

    if( containsString(propertyName, ";")){
        free(propertyName);
        free(propertyDesc);
        propertyName = copyCharacters(propertyString, 0, findFirstCharOccurence(propertyString, ';'));
        propertyDesc = copyCharacters(propertyString, findFirstCharOccurence(propertyString, ';') + 1, strlen(propertyString));
    }

    if( (strlen(propertyName) == 0) || (strlen(propertyDesc) == 0) ){
        proprtyCode = OTHER_ERROR;
    }
    //printf("Property name = %s, Property description: %s\n", propertyName, propertyDesc);
    *propertyToReturn = createProperty(propertyName, propertyDesc);
    free(propertyName);
    free(propertyDesc);
    //return propertyToReturn;
    return proprtyCode;
}

ICalErrorCode parseAlarm(Array *alarmArray, Alarm **alarmToReturn){

    //char *newAlarmString = NULL;
    //Array *alarmProperties = NULL;
    List *optionalAlarmProperties = initializeList(printProperty, deleteProperty, compareProperties);;
    char **alarmPropStrings = NULL;
    char *action = NULL;
    char *trigger = NULL;
    Property *optionalProperty;
    //Alarm *alarmToReturn;
    ICalErrorCode propertyCode = OK;
    ICalErrorCode codeToReturn = OK;
    //char *alarmString = NULL;

    //printArray(alarmArray);
    alarmPropStrings = alarmArray->array;

    for(int i=0; i < alarmArray->size; i++){
        if( beginsWithIgnoreCase(alarmPropStrings[i], "ACTION") ){
            if( countCharOccurences(alarmPropStrings[i], ';') > 0){
                action = copyCharacters(alarmPropStrings[i], findFirstCharOccurence(alarmPropStrings[i],';') + 1, strlen(alarmPropStrings[i]));
            }
            else{
                action = copyCharacters(alarmPropStrings[i], findFirstCharOccurence(alarmPropStrings[i],':') + 1, strlen(alarmPropStrings[i]));
            }
        }

        else if( beginsWithIgnoreCase(alarmPropStrings[i], "TRIGGER") ){
            if( countCharOccurences(alarmPropStrings[i], ';') > 0){
                trigger = copyCharacters(alarmPropStrings[i], findFirstCharOccurence(alarmPropStrings[i],';') + 1, strlen(alarmPropStrings[i]));
            }
            else{
                trigger = copyCharacters(alarmPropStrings[i], findFirstCharOccurence(alarmPropStrings[i],':') + 1, strlen(alarmPropStrings[i]));
            }
        }

        else if( ( !beginsWithIgnoreCase(alarmPropStrings[i], "TRIGGER") ) && ( !beginsWithIgnoreCase(alarmPropStrings[i], "ACTION") ) && ( !beginsWithIgnoreCase(alarmPropStrings[i], "BEGIN") ) && ( !beginsWith(alarmPropStrings[i], "END") ) ){
            propertyCode = parseOptionalProperties(alarmPropStrings[i], &optionalProperty);
            insertBack(optionalAlarmProperties, optionalProperty);
            //printProperty(optionalProperty);
        }

    }

    if(propertyCode != OK) {
        
        free(action);
        free(trigger);
        freeList(optionalAlarmProperties);
        *alarmToReturn = NULL;
            return propertyCode;
        
    }

    *alarmToReturn = createAlarm(action, trigger, optionalAlarmProperties);
    codeToReturn = OK;

    free(action);
    free(trigger);
    
    return codeToReturn;
}

ICalErrorCode parseEvent(Array *eventArray, Event **eventToReturn){

Array *alarmArray = NULL;
char **alarmPropStrings = NULL;
Alarm *eventAlarm = NULL;
List *alarmsList = initializeList(printAlarm, deleteAlarm, compareAlarms);
List *eventOptionalPropList = initializeList(printProperty, deleteProperty, compareProperties);
int eventStringLength = addAllStringLengths(eventArray);
char **eventPropStrings = eventArray->array;
char *uid = NULL;
DateTime *startDateTime = NULL;
DateTime *creationDateTime = NULL;
Property *optionalProperty = NULL;
ICalErrorCode startDTCode = OK;
ICalErrorCode creationDTCode = OK;
ICalErrorCode propertyCode = OK;
ICalErrorCode alarmCode = OK;
ICalErrorCode codeToReturn = OK;
int j = 0;
//char *alarmString;

    for(int i=0; i < eventArray->size; i++){

        if( beginsWithIgnoreCase(eventPropStrings[i], "UID") ){
            if( countCharOccurences(eventPropStrings[i], ';') > 0){
                uid = copyCharacters(eventPropStrings[i], findFirstCharOccurence(eventPropStrings[i],';') + 1, strlen(eventPropStrings[i]));
            }
            else{
                uid = copyCharacters(eventPropStrings[i], findFirstCharOccurence(eventPropStrings[i],':') + 1, strlen(eventPropStrings[i]));
            }
        }

        else if( beginsWithIgnoreCase(eventPropStrings[i], "DTSTART") ){
            startDTCode = parseDateTime(eventPropStrings[i], &startDateTime);
            
        }
        else if( beginsWithIgnoreCase(eventPropStrings[i], "DTSTAMP") ){
            creationDTCode = parseDateTime(eventPropStrings[i], &creationDateTime);
            
        }

        else if( compareStringsIgnoreCase(eventPropStrings[i], "BEGIN:VALARM") ){

            alarmPropStrings = calloc(1, eventStringLength*sizeof(char)+1);
            j = 0;
            while( !compareStringsIgnoreCase(eventPropStrings[i], "END:VALARM")){
                alarmPropStrings[j] = copyString(eventPropStrings[i]);
                alarmPropStrings = realloc(alarmPropStrings, (j+2)*(strlen(alarmPropStrings[j])*sizeof(char)));
                j++;
                i++;
                if( compareStringsIgnoreCase(eventPropStrings[i], "END:VALARM")){
                    alarmPropStrings[j] = copyString(eventPropStrings[i]);
                    j++;
                    
                    alarmArray = createArray(alarmPropStrings, j, deleteStringArray, printStringArray);
                    alarmCode = parseAlarm(alarmArray, &eventAlarm);
                    
                    insertBack(alarmsList, eventAlarm);
                    
                    deleteArray(alarmArray);
                    continue;
                }
            }
        }

        else if( !beginsWithIgnoreCase(eventPropStrings[i], "DTSTAMP") && !beginsWithIgnoreCase(eventPropStrings[i], "DTSTART") && !beginsWithIgnoreCase(eventPropStrings[i], "UID") && !beginsWithIgnoreCase(eventPropStrings[i], "BEGIN") && !beginsWithIgnoreCase(eventPropStrings[i], "END") && (eventPropStrings[i][0] != ';') ){
            propertyCode = parseOptionalProperties( eventPropStrings[i], &optionalProperty );
            insertBack(eventOptionalPropList, optionalProperty);
            //printProperty(optionalProperty);
        }
   
    }

    if( ( startDTCode != OK) || (creationDTCode != OK) || (propertyCode != OK) || (alarmCode != OK) ){
        
        free(uid);
        free(creationDateTime);
        free(startDateTime);
        freeList(alarmsList);
        freeList(eventOptionalPropList);
        *eventToReturn = NULL;

        if( startDTCode!= OK){
            return startDTCode;
        }
        else if( creationDTCode != OK){
            return creationDTCode;
        }
        else if( propertyCode != OK){
            return propertyCode;
        }

    }

    *eventToReturn = createEvent(uid, *creationDateTime, *startDateTime, eventOptionalPropList, alarmsList);
    codeToReturn = OK;
    free(uid);
    
    free(creationDateTime);
    free(startDateTime);
    
    return codeToReturn;
}

ICalErrorCode parseDateTime(char *dateTimeString, DateTime **dateTimeToReturn){
    char *date = NULL;
    char *time = NULL;
    bool utc = false;
    char *tempString = NULL;
    //DateTime *toReturn = NULL;

    if(endsWith(dateTimeString, "Z")){
        tempString = copyCharacters(dateTimeString, findFirstCharOccurence(dateTimeString, ':')+1, strlen(dateTimeString) - 1);
        utc = true;
    }

    else{
        tempString = copyCharacters(dateTimeString, findFirstCharOccurence(dateTimeString, ':')+1, strlen(dateTimeString));
    }

    if(containsString(tempString, "T")){
        date = copyCharacters(tempString, 0, findFirstCharOccurence(tempString, 'T'));
        time = copyCharacters(tempString, findFirstCharOccurence(tempString, 'T')+1, strlen(tempString));
    }
    else{
        date = copyString(tempString);
        time = copyString("");
    }
    //printf("Date parsed: %s, time parsed: %s\n", date, time);
    *dateTimeToReturn = createDateTime(date, time, utc);
    free(tempString);
    free(date);
    free(time);
    //return toReturn;
    return OK;
}

int validateVersion(char *versionString){
    int colon = findFirstCharOccurence(versionString, ':');
    int decimal = findFirstCharOccurence(versionString, '.');
    int toReturn = 1;

    if( (colon <= 0) || (decimal <= 0) || (colon == strlen(versionString) - 1) || (decimal == strlen(versionString) - 1) ){
        return 0;
    }

    char *version = copyCharacters(versionString, colon + 1, decimal);
    char *decimals = copyCharacters(versionString, decimal +1, strlen(versionString));

    if( (!isNumeric(version)) || (!isNumeric(decimals)) || (strlen(version) == 0) || (strlen(decimals) == 0) ){
        toReturn = 0;
    }

    free(version);
    free(decimals);
    return toReturn;
}

int validateCreationDateTime( char *dateTimeToValidate ){

    if( containsString(dateTimeToValidate, "TZID") ){
        return 0;
    }

    if( !containsChar(dateTimeToValidate, ':') ){
        //printf("No value include in dateTime\n");
        return 0;
    }
    char *dateTime;
    //int containsTime = 0;
    dateTime = copyCharacters(dateTimeToValidate, findFirstCharOccurence(dateTimeToValidate, ':')+1, strlen(dateTimeToValidate));
    
    if( !containsChar(dateTime, 'T')){
        //containsTime = 1;
        free(dateTime);
        return 0;
    }

    if( endsWithIgnoreCase(dateTime, "T") ){
        //printf("Time value not included!\n");
        free(dateTime);
        return 0;
    }

    char *date = NULL;
    int toReturn = 1;
    char *time = NULL;

        date = copyCharacters(dateTime, 0, findFirstCharOccurence(dateTime, 'T') );
        //date = copyString(dateTime);
    
    //printf("DateTime is %s and date is: %s, and date string length = %d\n", dateTime, date, (int)strlen(date));

    if(  (strlen(date) != 8)  || ( !isNumeric(date) ) ){
        //printf("Date not correct string length: %s\n", date);
        free(dateTime);
        free(date);
        return 0;
    }

        if(endsWith(dateTime, "Z")){
            time = copyCharacters(dateTime, findFirstCharOccurence(dateTime, 'T')+1, findFirstCharOccurence(dateTime, 'Z'));
        }
        else{
            time = copyCharacters(dateTime, findFirstCharOccurence(dateTime, 'T')+1, strlen(dateTime));
        }
    
        if( (strlen(time) != 6) || ( !isNumeric(time) ) ){
            //printf("Time not correct string length!\n");
            toReturn = 0;
        }
    

    free(dateTime);
    free(date);
    free(time);
    
    return toReturn;
}

int validateStartDateTime( char *dateTimeToValidate ){

    if( containsString(dateTimeToValidate, "TZID") ){
        return 0;
    }

    if( !containsChar(dateTimeToValidate, ':') ){
        //printf("No value include in dateTime\n");
        return 0;
    }
    char *dateTime;
    int containsTime = 0;
    dateTime = copyCharacters(dateTimeToValidate, findFirstCharOccurence(dateTimeToValidate, ':')+1, strlen(dateTimeToValidate));
    
    if( containsChar(dateTime, 'T')){
        containsTime = 1;
        //free(dateTime);
        //return 0;
    }

    if( !containsTime && endsWith(dateTimeToValidate, "Z")){
        free(dateTime);
        return 0;
    }

    if( endsWithIgnoreCase(dateTime, "T") ){
        //printf("Time value not included!\n");
        free(dateTime);
        return 0;
    }

    char *date;
    int toReturn = 1;
    char *time;

    if(containsTime){
        date = copyCharacters(dateTime, 0, findFirstCharOccurence(dateTime, 'T') );
    }
    else{
        date = copyString(dateTime);
    }
    //printf("DateTime is %s and date is: %s, and date string length = %d\n", dateTime, date, (int)strlen(date));

    if(  (strlen(date) != 8)  || ( !isNumeric(date) )){
        //printf("Date not correct string length: %s\n", date);
        free(dateTime);
        free(date);
        return 0;
    }

    if(containsTime){
        if(endsWith(dateTime, "Z")){
            time = copyCharacters(dateTime, findFirstCharOccurence(dateTime, 'T')+1, findFirstCharOccurence(dateTime, 'Z'));
        }
        else{
            time = copyCharacters(dateTime, findFirstCharOccurence(dateTime, 'T')+1, strlen(dateTime));
        }
    }

    

    if(containsTime){
        if( (strlen(time) != 6) || ( !isNumeric(time) ) ){
            //printf("Time not correct string length!\n");
            toReturn = 0;
        }
    }

    free(dateTime);
    free(date);
    if(containsTime){
        free(time);
    }
    return toReturn;
}

ICalErrorCode validateFile(Array *calendarArray){

    char **contentLines = calendarArray->array;

    if( (!compareStringsIgnoreCase(contentLines[0], "BEGIN:VCALENDAR")) || (!compareStringsIgnoreCase(contentLines[calendarArray->size - 1], "END:VCALENDAR")) ){
           
            return INV_CAL;
        }

    //ICalErrorCode fileCode = OK;
    int checkingEvent = 0;
    int checkingAlarm = 0;
    int versions = 0;
    int prodids = 0;
    //int versionNumber = 0;
    int eventBeginnings = 0;
    int eventEndings = 0;
    int alarmBeginnings = 0;
    int alarmEndings = 0;
    int dtstarts = 0;
    int creationdts = 0;
    int uids = 0;
    int actions = 0;
    int triggers = 0;

    for(int i = 0; i < calendarArray->size; i++){
      

        if( (compareStringsIgnoreCase(contentLines[i], "BEGIN:VCALENDAR")) && ( i != 0 ) ){
            if(checkingAlarm == 1){
                //printf("Invalid alarm 1\n");
                return INV_ALARM;
            }
            if(checkingEvent == 1){
                errorPrinter("inv event 1",NULL);
                return INV_EVENT;
            }
            if( checkingEvent == 0 && checkingAlarm ==0){
                //printf("Invalid scope 1\n");
                return INV_CAL;
            }
        }

        if( (compareStringsIgnoreCase(contentLines[i], "END:VCALENDAR")) && ( i != calendarArray->size - 1 ) ){
            if(checkingAlarm == 1){
                //printf("Invalid alarm 2\n");
                return INV_ALARM;
            }
            if(checkingEvent == 1){
                errorPrinter("inv event 1",NULL);
                return INV_EVENT;
            }
            if( checkingEvent == 0 && checkingAlarm ==0){
                //printf("Invalid scope 2\n");
                return INV_CAL;
            }
        }

        if( (contentLines[i][0] != ';') /*&& (strlen(contentLines[i]) > 0)*/ ){//to ignore comments

        if( (containsChar(contentLines[i], '\r')) || (containsChar(contentLines[i], '\n')) ){
            //printf("Content line: %s\n",contentLines[i]);
            return INV_FILE;
        }


        if ( beginsWithIgnoreCase(contentLines[i], "VERSION") ){
            versions++;
            if( checkingEvent == 1){
                //printf("Version happening in an event!\n");
                return INV_CAL;
            }
            if( versions > 1 ){
                return DUP_VER;
            }
            if( !validateVersion(contentLines[i]) ){
                return INV_VER;
            }

        }
        if ( beginsWithIgnoreCase(contentLines[i], "PRODID") ){
            prodids++;
            if( prodids > 1 ){
                return DUP_PRODID;
            }
            if( (findFirstCharOccurence(contentLines[i], ':') == strlen(contentLines[i]) - 1) || (findFirstCharOccurence(contentLines[i], ':') <= 0) ){
                return INV_PRODID;
            }
        }

        if( compareStringsIgnoreCase(contentLines[i], "BEGIN:VEVENT") ){
                if( checkingEvent == 1 ){
                    //printf("1iii\n");
                    errorPrinter("inv event 3",NULL);
                    return INV_EVENT;
                }
                if(checkingAlarm == 1){
                    //printf("Invalid alarm 3\n");
                    return INV_ALARM;
                }
                checkingEvent = 1;
                eventBeginnings++;

            }
        if( compareStringsIgnoreCase(contentLines[i], "BEGIN:VALARM") ){
                if(checkingEvent == 0){
                    //printf("Invalid scope 3\n");
                    return INV_CAL;
                }
                if( checkingAlarm == 1 ){
                    //printf("Invalid alarm 4\n");
                    return INV_ALARM;
                }
                checkingAlarm = 1;
                alarmBeginnings++;
            }
        if( compareStringsIgnoreCase(contentLines[i], "END:VALARM") ){
                if( checkingEvent == 0 ){
                    //printf("Invalid scope 4\n");
                    return INV_CAL;
                }
                if( checkingAlarm == 0 ){
                    errorPrinter("inv event 4",NULL);
                    return INV_EVENT;
                }
                if( (triggers == 0) || (actions == 0) ){
                    //printf("Invalid alarm 5\n");
                    return INV_ALARM;
                }
                checkingAlarm = 0;
                alarmEndings++;
                triggers = 0;
                actions = 0;
            }
        if( compareStringsIgnoreCase(contentLines[i], "END:VEVENT") ){
            if( checkingEvent == 0 ){
                //printf("Invalid scope 5\n");
                    return INV_CAL;
                }
            if( checkingAlarm == 1 ){
                //printf("Invalid alarm 6\n");
                return INV_ALARM;
            }

            if( (dtstarts == 0) || (creationdts == 0) || (uids == 0) ){
                errorPrinter("inv event 55",NULL);
                return INV_EVENT;
            }
                checkingEvent = 0;
                dtstarts = 0;
                creationdts = 0;
                uids = 0;
                eventEndings++;
                
            }
            
        if( beginsWithIgnoreCase(contentLines[i], "DTSTAMP:")){
            if( checkingEvent == 0 ){
                //printf("4 iii\n");
                return INV_CAL;
            }
            if(checkingAlarm == 1){
                //printf("Invalid alarm 7\n");
                return INV_ALARM;
            }
            creationdts++;
            if(creationdts > 1){
                //printf("5 iii\n");
                errorPrinter("inv event 21",NULL);
                return INV_EVENT;
            }
            if( contentLines[i][strlen(contentLines[i]) - 1] == ':'){
                return INV_DT;
            }
            if( !validateCreationDateTime(contentLines[i])){
                //printf("Creation Date time wasn't validated!\n");
                return INV_DT;
            }

        }

        if( beginsWithIgnoreCase(contentLines[i], "DTSTART:")){
            if( checkingEvent == 0 ){
                //printf("6iii\n");
                return INV_CAL;
            }
            if(checkingAlarm == 1){
                //printf("Invalid alarm 8\n");
                return INV_ALARM;
            }
            dtstarts++;
            if(dtstarts > 1){
                //printf("7iii\n");
                errorPrinter("inv event 22",NULL);
                return INV_EVENT;
            }
            if( contentLines[i][strlen(contentLines[i]) - 1] == ':'){
                return INV_DT;
            }
            if( !validateStartDateTime(contentLines[i])){
                //printf("Start Date time wasn't validated!\n");
                return INV_DT;
            }
        }
        if( beginsWithIgnoreCase(contentLines[i], "UID")){
            if( checkingEvent == 0){
                //printf("13iii\n");
                return INV_CAL;
            }
            if(checkingAlarm == 1){
                //printf("Invalid alarm 9\n");
                return INV_ALARM;
            }
            uids++;
            if(uids > 1){
                //printf("8iii\n");
                errorPrinter("inv event 54",NULL);
                return INV_EVENT;
            }
            if( contentLines[i][strlen(contentLines[i]) - 1] == ':'){
                //printf("9iii\n");
                errorPrinter("inv event 111",NULL);
                return INV_EVENT;
            }

        }

        if( beginsWithIgnoreCase(contentLines[i], "ACTION")){
            if( checkingEvent == 0 ){
                //printf("Invalid scope 6i\n");
                return INV_CAL;
            }
            if( checkingAlarm == 0){
                //printf("10iii\n");
                errorPrinter("inv event 12",NULL);
                return INV_EVENT;
            }

            actions++;
            if(actions > 1){
                //printf("Invalid alarm 10\n");
                return INV_ALARM;
            }
            if( contentLines[i][strlen(contentLines[i]) - 1] == ':'){
                //printf("Invalid alarm 11\n");
                return INV_ALARM;
            }

        }

        if( beginsWithIgnoreCase(contentLines[i], "TRIGGER")){
            if( checkingEvent == 0 ){
                //printf("Invalid scope 7\n");
                return INV_CAL;
            }
            if( checkingAlarm == 0){
                //printf("11iii\n");
                errorPrinter("inv event 1434",NULL);
                return INV_EVENT;
            }

            triggers++;
            if(triggers > 1){
                //printf("Invalid alarm 12\n");
                return INV_ALARM;
            }
            if( contentLines[i][strlen(contentLines[i]) - 1] == ':'){
                //printf("Invalid alarm 13\n");
                return INV_ALARM;
            }

        }

        //if( !containsChar(contentLines[i], ':')){ // Change added 18/2/2019 - 1:37am
        if( !containsChar(contentLines[i], ':')  && (!containsChar(contentLines[i], ';')) ){
            if(checkingAlarm == 1){
                //printf("This is the content line: %s\n",contentLines[i]);
                //printf("Invalid alarm 14\n");
                return INV_ALARM;
            }
            if(checkingEvent == 1){
                errorPrinter("inv event 123232",NULL);
                return INV_EVENT;
            }
            if( checkingEvent == 0 && checkingAlarm ==0){
                //printf("Invalid scope 8\n");
                return INV_CAL;
            }
        }

        if( (contentLines[i][0] == ':') || (contentLines[i][strlen(contentLines[i]) - 1] == ':') ){
            if(checkingAlarm == 1){
                //printf("Invalid alarm 15\n");
                return INV_ALARM;
            }
            if(checkingEvent == 1){
                errorPrinter("inv event asds1",NULL);
                return INV_EVENT;
            }
            if( checkingEvent == 0 && checkingAlarm ==0){
                //printf("Invalid scope 9\n");
                return INV_CAL;
            }
        }

        }//end of the if statement that checks whether the line is a comment
        


        
    }//end of for loop

    if( (versions == 0) || (prodids == 0) ){
        //printf("No version or prodid!\n");
        return INV_CAL;
    }

    if( eventBeginnings == 0){
        //printf("No events!\n");
        return INV_CAL;
    }

    if( eventEndings < eventBeginnings ){
        //printf("12iii\n");
        errorPrinter("inv event 1232",NULL);
        return INV_EVENT;
    }

    if( eventEndings > eventBeginnings ){
        //printf("12iii\n");
        return INV_CAL;
    }

    if( alarmEndings < alarmBeginnings ){
        //printf("Invalid alarm 16\n");
        return INV_ALARM;
    }

    if( alarmEndings > alarmBeginnings ){
        //printf("Invalid scope 10\n");
        return INV_CAL;
    }

    return OK;
}//end of validateFile

ICalErrorCode parseCalendar(char *calendarString, Calendar **obj){
    
        if(endsWith(calendarString, "\r\n")){
            int originalStringLength = strlen(calendarString);
            calendarString[originalStringLength-2] = '\0';
            calendarString = realloc(calendarString, originalStringLength-1);
        }


  Array *calendarArray = splitString(calendarString, "\r\n");
  Array *eventArray = NULL;
  char **calProperties = calendarArray->array;
  char **eventPropArray = NULL;

  
  calendarArray = unfoldAllLines(calendarArray);
  ICalErrorCode fileValidation = validateFile(calendarArray);

  if( fileValidation != OK){
      deleteArray(calendarArray);
      free(calendarString);
      *obj = NULL;
      return fileValidation;
  }

  char *prodid = NULL;
  char *versionString = NULL;
  float version = 0;
  Property *optionalCalProperty = NULL;
  List *optionalCalProperties = initializeList(printProperty, deleteProperty, compareProperties);
  List *eventsList = initializeList(printEvent, deleteEvent, compareEvents);
  int j = 0;//integer to index event properties array
  Event *calendarEvent = NULL;
  ICalErrorCode eventCode = OK;
  ICalErrorCode propertyCode = OK;
    //char* eventJSON = NULL;

 for(int i=0; i < calendarArray->size; i++){
        if(beginsWithIgnoreCase(calProperties[i], "VERSION")){
            versionString = copyCharacters(calProperties[i], findFirstCharOccurence(calProperties[i], ':')+1, strlen(calProperties[i]));
            //version = (float)charToInt(versionString[0]);
            version = extractVersion(versionString);
        }
        else if( beginsWithIgnoreCase(calProperties[i], "PRODID") ){
            prodid = copyCharacters(calProperties[i], findFirstCharOccurence(calProperties[i], ':')+1, strlen(calProperties[i]));
        }
        else if( compareStringsIgnoreCase(calProperties[i], "BEGIN:VEVENT") ){

            eventPropArray = calloc(1, strlen(calendarString)*sizeof(char)+1);
            j = 0;
            while( !compareStringsIgnoreCase(calProperties[i], "END:VEVENT")){
                eventPropArray[j] = copyString(calProperties[i]);
                eventPropArray = realloc(eventPropArray, (j+2)*(strlen(eventPropArray[j]))*sizeof(char));
                j++;
                i++;
                if( compareStringsIgnoreCase(calProperties[i], "END:VEVENT")){
                    eventPropArray[j] = copyString(calProperties[i]);
                    j++;
                    //i++;
                    eventArray = createArray(eventPropArray, j, deleteStringArray, printStringArray);
                    eventCode = parseEvent(eventArray, &calendarEvent);
                    insertBack(eventsList, calendarEvent);
                    //eventJSON = eventToJSON(calendarEvent);
                    //printf("%s\n",eventJSON);
                    //free((eventJSON));
                    //printArray(eventArray);
                    deleteArray(eventArray);
                    continue;
                }
            }
        }
        else if( (!beginsWithIgnoreCase(calProperties[i], "PRODID")) && (!beginsWithIgnoreCase(calProperties[i], "VERSION")) && (!beginsWithIgnoreCase(calProperties[i], "BEGIN")) && (!beginsWithIgnoreCase(calProperties[i], "END")) && (calProperties[i][0] != ';') && (strlen(calProperties[i]) > 0) ){
            propertyCode = parseOptionalProperties(calProperties[i], &optionalCalProperty);
            insertBack(optionalCalProperties, optionalCalProperty);
            //printProperty(optionalCalProperty);
        }

    }

    if( ( eventCode != OK) || (propertyCode != OK) ){
        deleteArray(calendarArray);
        free(versionString);
        free(prodid);
        free(calendarString);
        freeList(eventsList);
        freeList(optionalCalProperties);

        if( eventCode!= OK){
            return eventCode;
        }
        else if( propertyCode != OK){
            return propertyCode;
        }
    }
  
  *obj = constructCalendar(version, prodid, eventsList, optionalCalProperties);
   
  deleteArray(calendarArray);
  free(versionString);
  free(prodid);
  //free(eventPropArray);
  free(calendarString);
  return OK;
}

Property *createProperty(char *propertyName, char *propertyDescription){

    Property *newProperty = NULL;
    newProperty = calloc(1, sizeof(Property) + strlen(propertyDescription) + 1);
    strcpy(newProperty->propName, propertyName);
    strcpy(newProperty->propDescr, propertyDescription);
    return newProperty;
}

Alarm *createAlarm(char *action, char *trigger, List *optionalProperties){
    Alarm *newAlarm = NULL;
    newAlarm = calloc(1, sizeof(Alarm));
    strcpy(newAlarm->action, action);
    newAlarm->trigger = copyString(trigger);
    newAlarm->properties = optionalProperties;
    return newAlarm;
}

DateTime *createDateTime(char *date, char *time, bool utc){
    DateTime *newDateTime = calloc(1, sizeof(DateTime));
    if (date != NULL){
        strcpy(newDateTime->date, date);
    }
    if (time != NULL){
        strcpy(newDateTime->time, time);
    }
    newDateTime->UTC = utc;
    return newDateTime;
}

Event *createEvent(char *uid, DateTime creationDateTime, DateTime startDateTime, List *optionalProperties, List *alarms){
    Event *newEvent = calloc(1, sizeof(Event));
    strcpy(newEvent->UID, uid);
    newEvent->creationDateTime = creationDateTime;
    newEvent->startDateTime = startDateTime;
    newEvent->properties = optionalProperties;
    newEvent->alarms = alarms;

    return newEvent;

}

Calendar *constructCalendar(float version, char *prodid, List *events, List *properties){
    Calendar *newCalendar = calloc(1, sizeof(Calendar));
    newCalendar->version = version;
    strcpy(newCalendar->prodID, prodid);
    newCalendar->events = events;
    newCalendar->properties = properties;
    return newCalendar;
}

char *findSummary(const Event* event){

    if(event == NULL){
        return NULL;
    }

    char *summary = NULL;
    List *properties = event->properties;

    if(event->properties->head == NULL){
        return NULL;
    }
    Node *curr = properties->head;
    //Node *prev = NULL;
    Property *currProperty = (Property *)curr->data;

    while(curr->next != NULL){
        currProperty = (Property *)curr->data;
        if( compareStringsIgnoreCase(currProperty->propName, "SUMMARY") ){
            summary = copyString(currProperty->propDescr);
            return summary;
        }
        curr = curr->next;
    }

    if(curr->next == NULL){
        currProperty = (Property *)curr->data;
        if( compareStringsIgnoreCase(currProperty->propName, "SUMMARY") ){
            summary = copyString(currProperty->propDescr);
            return summary;
        }
    }
    return summary;
}

ICalErrorCode validateEvent(Event *toValidate){

    if(toValidate == NULL){
        errorPrinter("Null event",NULL);
        return INV_EVENT;
    }

    if(strlen(toValidate->UID) < 1){
        errorPrinter("No UID",NULL);
        return INV_EVENT;
    }

    if(toValidate->properties == NULL){
        //errorPrinter("Null event properties");
        return INV_EVENT;
    }
    
    if(toValidate->alarms == NULL){
        errorPrinter("Null event alarms ",NULL);
        return INV_EVENT;
    }

    if( (validateDateTime(toValidate->creationDateTime) != OK) || (validateDateTime(toValidate->startDateTime) != OK) ){
        return INV_DT;
    }

    ICalErrorCode evtPropCode = validateEventProperties(toValidate->properties);
    
    if(evtPropCode != OK){
        errorPrinter("invalid event property",NULL);
        return INV_EVENT;
    }
    ICalErrorCode evtAlarmsCode = validateAllAlarms(toValidate->alarms);
    if(evtAlarmsCode != OK){
        return evtAlarmsCode;
    }
    return OK;
}

ICalErrorCode validateAlarm(Alarm *toValidate){
    
    if(toValidate == NULL){
        errorPrinter("null alarm",NULL);
        return INV_ALARM;
    }

    if(strlen(toValidate->action) < 1){
        errorPrinter("null alarm action",NULL);
        return INV_ALARM;
    }

    if(toValidate->trigger == NULL){
        errorPrinter("null alarm trigger",NULL);
        return INV_ALARM;
    }

    if( strlen(toValidate->trigger) < 1){
        errorPrinter("null alarm trgger 2",NULL);
        return INV_ALARM;
    }

    if(toValidate->properties == NULL){
        errorPrinter("null alarm properties",NULL);
        return INV_ALARM;
    }

    /*if( !compareStringsIgnoreCase(toValidate->action,"AUDIO") ){
        errorPrinter("action not audio",NULL);
        return INV_ALARM;
    }*/

    ICalErrorCode almPropCode = validateAlarmProperties(toValidate->properties);
    if( almPropCode != OK){
        return almPropCode;
    }

    return OK;
}

ICalErrorCode validateProperty(Property *toValidate){
    if(toValidate == NULL){
        return OTHER_ERROR;
    }
    if( strlen(toValidate->propName) < 1 || strlen(toValidate->propDescr) < 1 ){
        return INV_CAL;
    }

    if( compareStringsIgnoreCase(toValidate->propName, "BEGIN") || compareStringsIgnoreCase(toValidate->propName, "END") ){
        return INV_CAL;
    }

    return OK;
}

ICalErrorCode validateCalendarProperties(List *calProperties){//to validate the optional properties of a calendar

    if(calProperties == NULL){
        return INV_CAL;
    }

    if(calProperties->head == NULL){
        return OK;
    }
    
    int methodOccurs = 0;
    int calscaleOccurs = 0;

    ICalErrorCode propertyCode = OK;
    ICalErrorCode propVer = OK;
    Node *curr = calProperties->head;

    while( curr->next != NULL){
        Property *currProperty = (Property *)curr->data;
        propertyCode = validateProperty(currProperty);
        if(propertyCode != OK){
            return INV_CAL;
        }
        propVer = verifyCalendarProperty(currProperty);

        if(propVer != OK){
            return propVer;
        }

        if( compareStringsIgnoreCase(currProperty->propName, "METHOD") ){
            methodOccurs++;
            if(methodOccurs > 1){
                //errorPrinter("method > 1");
                return INV_CAL;
            }
        }

        if( compareStringsIgnoreCase(currProperty->propName, "CALSCALE") ){
            calscaleOccurs++;
            //errorPrinter("Calscale encountered",NULL);
            //printf("%s\n",printProperty(currProperty));
            if(calscaleOccurs > 1){
                //errorPrinter("calscale > 1");
                return INV_CAL;
            }
        }

        curr = curr->next;

    }

     if( curr->next == NULL){
        Property *currProperty = (Property *)curr->data;
        propertyCode = validateProperty(currProperty);
        if(propertyCode != OK){
            return INV_CAL;
        }
        propVer = verifyCalendarProperty(currProperty);

        if(propVer != OK){
            return propVer;
        }

        if( compareStringsIgnoreCase(currProperty->propName, "METHOD") ){
            methodOccurs++;
            if(methodOccurs > 1){
                return INV_CAL;
            }
        }

        if( compareStringsIgnoreCase(currProperty->propName, "CALSCALE") ){
            calscaleOccurs++;
            if(calscaleOccurs > 1){
                //errorPrinter("calscale > 1 - 2");
                return INV_CAL;
            }
        }


    }

    return OK;
}

ICalErrorCode validateEventProperties(List *evtProperties){
    if(evtProperties == NULL){
        errorPrinter("Null event properties",NULL);
        return INV_EVENT;
    }

    if(evtProperties->head == NULL){
        
        return OK;
    }

    ICalErrorCode propertyCode = OK;
    int propVer = 1;
    Node *curr = evtProperties->head;
    int classOcc = 0; int createdOcc = 0; int descOcc = 0; int geoOcc = 0;
    int lastmodOcc = 0; int locationOcc = 0; int organizer = 0; int priority = 0;
    int seq = 0; int statusOcc = 0; int summaryOcc = 0; int transp = 0; int urlOcc = 0;
    int recurid = 0; int rrule = 0;

    int dtend = 0;
    int durationOcc = 0;

    while( curr->next != NULL){
        Property *currProperty = (Property *)curr->data;
        propertyCode = validateProperty(currProperty);
        if(propertyCode != OK){
            errorPrinter("improper event property",NULL);
            return INV_EVENT;
        }
        propVer = verifyEventProperty(currProperty);

        if(propVer != 1){
            errorPrinter("wrong event property",NULL);
            return INV_EVENT;
        }

        if( compareStringsIgnoreCase(currProperty->propName, "CLASS") ){
            classOcc++;
            if(classOcc > 1){
                errorPrinter("Null event 1",NULL);
                return INV_EVENT;
            }
        }


        if( compareStringsIgnoreCase(currProperty->propName, "RRULE") ){
            rrule++;
            if(rrule > 1){
                errorPrinter("Null event 2",NULL);
                return INV_EVENT;
            }
        }


        if( compareStringsIgnoreCase(currProperty->propName, "CREATED") ){
            createdOcc++;
            if(createdOcc > 1){
                errorPrinter("Null event 3",NULL);
                return INV_EVENT;
            }
        }

        if( compareStringsIgnoreCase(currProperty->propName, "DESCRIPTION") ){
            descOcc++;
            if(descOcc > 1){
                errorPrinter("Null event 4",NULL);
                return INV_EVENT;
            }
        }

        if( compareStringsIgnoreCase(currProperty->propName, "GEO") ){
            geoOcc++;
            if(geoOcc > 1){
                errorPrinter("Null event 3",NULL);
                return INV_EVENT;
            }
        }


        if( compareStringsIgnoreCase(currProperty->propName, "DTEND") ){
            dtend++;
            if( durationOcc == 1 || (dtend > 1) ){
                errorPrinter("Null event 5",NULL);
                return INV_EVENT;
            }
        }

        if( compareStringsIgnoreCase(currProperty->propName, "DURATION") ){
            durationOcc++;
            if( dtend == 1 || (durationOcc > 1) ){
                errorPrinter("Null event 565656",NULL);
                return INV_EVENT;
            }
        }


        if( compareStringsIgnoreCase(currProperty->propName, "LAST-MODIFIED") ){
            lastmodOcc++;
            if(lastmodOcc > 1){
                errorPrinter("Null event 3",NULL);
                return INV_EVENT;
            }
        }

        if( compareStringsIgnoreCase(currProperty->propName, "LOCATION") ){
            locationOcc++;
            if(locationOcc > 1){
                errorPrinter("Null event 6",NULL);
                return INV_EVENT;
            }
        }

        if( compareStringsIgnoreCase(currProperty->propName, "ORGANIZER") ){
            organizer++;
            if(organizer > 1){
                errorPrinter("Null event 7",NULL);
                return INV_EVENT;
            }
        }

        if( compareStringsIgnoreCase(currProperty->propName, "PRIORITY") ){
            priority++;
            if(priority > 1){
                errorPrinter("Null event 9",NULL);
                return INV_EVENT;
            }
        }

        if( compareStringsIgnoreCase(currProperty->propName, "SEQUENCE") ){
            seq++;
            if(seq > 1){
                errorPrinter("Null event 8",NULL);
                return INV_EVENT;
            }
        }

        if( compareStringsIgnoreCase(currProperty->propName, "STATUS") ){
            statusOcc++;
            if(statusOcc > 1){
                errorPrinter("Null event 10",NULL);
                return INV_EVENT;
            }
        }

        if( compareStringsIgnoreCase(currProperty->propName, "SUMMARY") ){
            summaryOcc++;
            if(summaryOcc > 1){
                errorPrinter("fhasjfhjk", NULL);
                return INV_EVENT;
            }
        }

        if( compareStringsIgnoreCase(currProperty->propName, "TRANSP") ){
            transp++;
            if(transp > 1){
                errorPrinter("Null event 11",NULL);
                return INV_EVENT;
            }
        }

        if( compareStringsIgnoreCase(currProperty->propName, "URL") ){
            urlOcc++;
            if(urlOcc > 1){
                errorPrinter("Null event 5rrr",NULL);
                return INV_EVENT;
            }
        }

        if( compareStringsIgnoreCase(currProperty->propName, "RECURID") ){
            recurid++;
            if(recurid > 1){
                errorPrinter("Null event 12345656",NULL);
                return INV_EVENT;
            }
        }

    curr = curr->next;
    }

     if( curr->next == NULL){//if this the last or the only event in the calendar
        Property *currProperty = (Property *)curr->data;
        propertyCode = validateProperty(currProperty);
        if(propertyCode != OK){
            errorPrinter("Null event ",NULL);
            return INV_EVENT;
        }
        propVer = verifyEventProperty(currProperty);

        if(propVer != 1){
            return INV_EVENT;
        }


        if( compareStringsIgnoreCase(currProperty->propName, "CLASS") ){
            classOcc++;
            if(classOcc > 1){
                return INV_EVENT;
            }
        }

        if( compareStringsIgnoreCase(currProperty->propName, "RRULE") ){
            rrule++;
            if(rrule > 1){
                return INV_EVENT;
            }
        }

        if( compareStringsIgnoreCase(currProperty->propName, "CREATED") ){
            createdOcc++;
            if(createdOcc > 1){
                return INV_EVENT;
            }
        }

        if( compareStringsIgnoreCase(currProperty->propName, "DESCRIPTION") ){
            descOcc++;
            if(descOcc > 1){
                return INV_EVENT;
            }
        }

        if( compareStringsIgnoreCase(currProperty->propName, "GEO") ){
            geoOcc++;
            if(geoOcc > 1){
                return INV_EVENT;
            }
        }

        if( compareStringsIgnoreCase(currProperty->propName, "LAST-MODIFIED") ){
            lastmodOcc++;
            if(lastmodOcc > 1){
                return INV_EVENT;
            }
        }

        if( compareStringsIgnoreCase(currProperty->propName, "LOCATION") ){
            locationOcc++;
            if(locationOcc > 1){
                return INV_EVENT;
            }
        }

        if( compareStringsIgnoreCase(currProperty->propName, "DTEND") ){
            dtend++;
            if( durationOcc == 1 || dtend > 1){
                return INV_EVENT;
            }
        }

        if( compareStringsIgnoreCase(currProperty->propName, "DURATION") ){
            durationOcc++;
            if( dtend == 1 || durationOcc > 1){
                return INV_EVENT;
            }
        }

        if( compareStringsIgnoreCase(currProperty->propName, "ORGANIZER") ){
            organizer++;
            if(organizer > 1){
                return INV_EVENT;
            }
        }

        if( compareStringsIgnoreCase(currProperty->propName, "PRIORITY") ){
            priority++;
            if(priority > 1){
                return INV_EVENT;
            }
        }

        if( compareStringsIgnoreCase(currProperty->propName, "SEQUENCE") ){
            seq++;
            if(seq > 1){
                return INV_EVENT;
            }
        }

        if( compareStringsIgnoreCase(currProperty->propName, "STATUS") ){
            statusOcc++;
            if(statusOcc > 1){
                return INV_EVENT;
            }
        }

        if( compareStringsIgnoreCase(currProperty->propName, "SUMMARY") ){
            summaryOcc++;
            if(summaryOcc > 1){
                return INV_EVENT;
            }
        }

        if( compareStringsIgnoreCase(currProperty->propName, "TRANSP") ){
            transp++;
            if(transp > 1){
                return INV_EVENT;
            }
        }

        if( compareStringsIgnoreCase(currProperty->propName, "URL") ){
            urlOcc++;
            if(urlOcc > 1){
                return INV_EVENT;
            }
        }

        if( compareStringsIgnoreCase(currProperty->propName, "RECURID") ){
            recurid++;
            if(recurid > 1){
                return INV_EVENT;
            }
        }

    }

    return OK;
}

ICalErrorCode validateAlarmProperties(List *almProperties){

    if(almProperties == NULL){
        errorPrinter("invalid alarm 56",NULL);
        return INV_ALARM;
    }

    if(almProperties->head == NULL){
        return OK;
    }

    ICalErrorCode propertyCode = OK;
    int propVer = 1;
    Node *curr = almProperties->head;
    int repeatOcc = 0;
    int durationOcc = 0;
    int attachOcc = 0;    

    while( curr->next != NULL){
        Property *currProperty = (Property *)curr->data;
        propertyCode = validateProperty(currProperty);
        if(propertyCode != OK){
            errorPrinter("invalid aarm property",NULL);
            return INV_ALARM;
        }
        propVer = verifyAlarmProperty(currProperty);

        if(propVer != 1){
            errorPrinter("invalid alarm property 2",NULL);
            return INV_ALARM;
        }

        if( compareStringsIgnoreCase(currProperty->propName, "DURATION") ){
            durationOcc++;
            if(durationOcc > 1){
                errorPrinter("invalid alarm property d",NULL);
                return INV_ALARM;
            }
        }

        if( compareStringsIgnoreCase(currProperty->propName, "REPEAT") ){
            repeatOcc++;
            if(repeatOcc > 1){
                errorPrinter("invalid alarm property 2",NULL);
                return INV_ALARM;
            }
        }

        if( compareStringsIgnoreCase(currProperty->propName, "ATTACH") ){
            attachOcc++;
            if(attachOcc > 1){
                errorPrinter("fsfhjsafd",NULL);
                return INV_ALARM;
            }
        }

        curr = curr->next;
    }

     if( curr->next == NULL){
        Property *currProperty = (Property *)curr->data;
        propertyCode = validateProperty(currProperty);
        if(propertyCode != OK){
            errorPrinter("invalid alarm property 3",NULL);
            return INV_ALARM;
        }
        propVer = verifyAlarmProperty(currProperty);

        if(propVer != 1){
            errorPrinter("invalid alarm property 4",NULL);
            return INV_ALARM;
        }

        if( compareStringsIgnoreCase(currProperty->propName, "DURATION") ){
            durationOcc++;
            if(durationOcc > 1){
                errorPrinter("invalid alarm property 2",NULL);
                return INV_ALARM;
            }
        }

        if( compareStringsIgnoreCase(currProperty->propName, "REPEAT") ){
            repeatOcc++;
            if(repeatOcc > 1){
                errorPrinter("invalid alarm property dr",NULL);
                return INV_ALARM;
            }
        }

        if( compareStringsIgnoreCase(currProperty->propName, "ATTACH") ){
            attachOcc++;
            if(attachOcc > 1){
                return INV_ALARM;
            }
        }

}

    if(durationOcc != repeatOcc){
        errorPrinter("invalid alarm property dur and rep not equal 1",NULL);
        return INV_ALARM;
    }

    return OK;
}

ICalErrorCode validateAllEvents(List *allEvents){

    if( allEvents == NULL ){
        return INV_CAL;
    }

    if( allEvents->head == NULL ){
        return INV_CAL;
    }

    ICalErrorCode eventCode = OK;

    Node *curr = allEvents->head;

    while( curr->next != NULL){
        Event *currEvent = (Event *)curr->data;
        eventCode = validateEvent(currEvent);
        if(eventCode != OK){
            return eventCode;
        }
        curr = curr->next;
    }

     if( curr->next == NULL){
        Event *currEvent = (Event *)curr->data;
        eventCode = validateEvent(currEvent);
        if(eventCode != OK){
            return eventCode;
        }
    }

    return OK;
}

ICalErrorCode validateAllAlarms(List *allAlarms){

    if(allAlarms == NULL){
        return INV_EVENT;
    }

    if(allAlarms->head == NULL){
        return OK;
    }

    ICalErrorCode alarmCode = OK;

    Node *curr = allAlarms->head;

    while( curr->next != NULL){
        Alarm *currAlarm = (Alarm *)curr->data;
        alarmCode = validateAlarm(currAlarm);
        if(alarmCode != OK){
            return alarmCode;
        }
        curr = curr->next;
    }

     if( curr->next == NULL){
        Alarm *currAlarm = (Alarm *)curr->data;
        alarmCode = validateAlarm(currAlarm);
        if(alarmCode != OK){
            return alarmCode;
        }
    }

    return OK;
}

int verifyAlarmProperty(Property *toVerify){

    if( compareStringsIgnoreCase(toVerify->propName, "UID") || compareStringsIgnoreCase(toVerify->propName, "VERSION") || compareStringsIgnoreCase(toVerify->propName, "PRODID") || compareStringsIgnoreCase(toVerify->propName, "CALSCALE") || compareStringsIgnoreCase(toVerify->propName, "DTSTART") || compareStringsIgnoreCase(toVerify->propName, "DTSTAMP") || compareStringsIgnoreCase(toVerify->propName, "DTEND") || compareStringsIgnoreCase(toVerify->propName, "METHOD") || compareStringsIgnoreCase(toVerify->propName, "ACTION") || compareStringsIgnoreCase(toVerify->propName, "TRIGGER")){
        errorPrinter("invalid alarm property 1",toVerify->propName);
        return 0;
    }

    if( !compareStringsIgnoreCase(toVerify->propName, "DURATION") && !compareStringsIgnoreCase(toVerify->propName, "REPEAT") && !compareStringsIgnoreCase(toVerify->propName, "ATTACH") ){
        errorPrinter("invalid alarm property 2",toVerify->propName);
        return 0;
    }

    return 1;
}

int verifyEventProperty(Property *toVerify){

    if( compareStringsIgnoreCase(toVerify->propName, "ACTION") || compareStringsIgnoreCase(toVerify->propName, "VERSION") || compareStringsIgnoreCase(toVerify->propName, "PRODID") || compareStringsIgnoreCase(toVerify->propName, "CALSCALE") || compareStringsIgnoreCase(toVerify->propName, "DTSTART") || compareStringsIgnoreCase(toVerify->propName, "DTSTAMP") || compareStringsIgnoreCase(toVerify->propName, "TRIGGER") || compareStringsIgnoreCase(toVerify->propName, "METHOD") ||  compareStringsIgnoreCase(toVerify->propName, "UID")){
        errorPrinter("wp1",toVerify->propName);
        return 0;
    }

    if( !compareStringsIgnoreCase(toVerify->propName,"CLASS") && !compareStringsIgnoreCase(toVerify->propName,"CREATED") && !compareStringsIgnoreCase(toVerify->propName,"DESCRIPTION") && !compareStringsIgnoreCase(toVerify->propName,"GEO") && !compareStringsIgnoreCase(toVerify->propName,"LAST-MODIFIED") && !compareStringsIgnoreCase(toVerify->propName,"LOCATION")  &&  !compareStringsIgnoreCase(toVerify->propName,"ORGANIZER") && !compareStringsIgnoreCase(toVerify->propName,"PRIORITY") && !compareStringsIgnoreCase(toVerify->propName,"SEQUENCE") && !compareStringsIgnoreCase(toVerify->propName,"STATUS") && !compareStringsIgnoreCase(toVerify->propName,"SUMMARY") && !compareStringsIgnoreCase(toVerify->propName,"TRANSP") && !compareStringsIgnoreCase(toVerify->propName,"URL") && !compareStringsIgnoreCase(toVerify->propName,"RECURID")  && !compareStringsIgnoreCase(toVerify->propName,"DTEND") && !compareStringsIgnoreCase(toVerify->propName,"DURATION")  && !compareStringsIgnoreCase(toVerify->propName,"ATTACH") && !compareStringsIgnoreCase(toVerify->propName,"ATTENDEE") && !compareStringsIgnoreCase(toVerify->propName,"CATEGORIES") && !compareStringsIgnoreCase(toVerify->propName,"COMMENT") && !compareStringsIgnoreCase(toVerify->propName,"CONTACT") && !compareStringsIgnoreCase(toVerify->propName,"EXDATE") && !compareStringsIgnoreCase(toVerify->propName,"RSTATUS") && !compareStringsIgnoreCase(toVerify->propName,"RELATED")  && !compareStringsIgnoreCase(toVerify->propName,"RESOURCES") && !compareStringsIgnoreCase(toVerify->propName,"RDATE") && !compareStringsIgnoreCase(toVerify->propName,"RRULE") ){
        errorPrinter("wp2",toVerify->propName);
        return 0;
    }

    return 1;
}

ICalErrorCode verifyCalendarProperty(Property *toVerify){
//checks against a list of disallowed properties

    if( compareStringsIgnoreCase(toVerify->propName, "VERSION")){
        return DUP_VER;
    }

    if( compareStringsIgnoreCase(toVerify->propName, "PRODID")){
        return DUP_PRODID;
    }

    if( compareStringsIgnoreCase(toVerify->propName, "UID") || compareStringsIgnoreCase(toVerify->propName, "ACTION") || compareStringsIgnoreCase(toVerify->propName, "PRODID") || compareStringsIgnoreCase(toVerify->propName, "TRIGGER") || compareStringsIgnoreCase(toVerify->propName, "DTSTART") || compareStringsIgnoreCase(toVerify->propName, "DTSTAMP") || compareStringsIgnoreCase(toVerify->propName, "DTEND") ){
        return INV_CAL;
    }

    if( !compareStringsIgnoreCase(toVerify->propName, "METHOD") && !compareStringsIgnoreCase(toVerify->propName, "CALSCALE") ){
        return INV_CAL;
    }

    return OK;

}

ICalErrorCode validateDateTime(DateTime toValidate){
    if( (strlen(toValidate.date) < 8) || (strlen(toValidate.time) < 6) ){
        
        return INV_DT;
    }

    if( !isNumeric(toValidate.date) || !isNumeric(toValidate.time) ){
        
        return INV_DT;
    }

    return OK;
}

char* modifiedToString(List * list){//This is function is the same as the one given by the instructor, with a slight modification
	ListIterator iter = createIterator(list);
	char* str;
		
	str = (char*)malloc(sizeof(char));
	strcpy(str, "");
	
	void* elem;
	while((elem = nextElement(&iter)) != NULL){
		char* currDescr = list->printData(elem);
		int newLen = strlen(str)+50+strlen(currDescr);
		str = (char*)realloc(str, newLen);
		strcat(str, "\r\n");
		strcat(str, currDescr);
		
		free(currDescr);
	}
	
	return str;
}

float extractVersion(char *versionString){
    int ver1 = charToInt(versionString[0]);
    int ver2 = charToInt( versionString[findFirstCharOccurence(versionString,'.')+1] );
    float verL = (float)ver1;
    float verR = (float)ver2;
    float version = verL + (0.1)*verR;
    return version;
}

void errorPrinter(char *errorMessage, char *errorLoc){
    //Prints out errors and possibly their locations
    int debug = 0;
    
    if(debug == 1){
        printf("%s\n",errorMessage);

        if(errorLoc != NULL){
        printf("at %s\n",errorLoc);
        }
    }

    

}

char *calFileToJSON(char *fileName){
    ICalErrorCode calCode;
    Calendar *obj;
    char temp[100];
    strcpy(temp,"./uploads/");
    strcat(temp,fileName);
    char *stringToReturn = NULL;
    ICalErrorCode valCode = OK;
    //printf("%s something random\n",fileName);
    calCode = createCalendar(temp,&obj);

    //stringToReturn = calendarToJSON(obj);

    if(calCode == OK){
        valCode = validateCalendar(obj);
        if(valCode == OK){
            stringToReturn = calendarToJSON(obj);
        }
        else{
            stringToReturn = calendarToJSON(NULL);
        }
    }
    else{
        stringToReturn = calendarToJSON(NULL);
    }

    deleteCalendar(obj);

    return stringToReturn;

}


char *eventsToJSON(char *fileName){
    ICalErrorCode calCode;
    Calendar *obj;
    char temp[100];
    strcpy(temp,"./uploads/");
    strcat(temp,fileName);
    char *stringToReturn = NULL;

    calCode = createCalendar(temp,&obj);
    
    if(calCode == OK){
        stringToReturn = eventListToJSON(obj->events);
    }
    else{
        stringToReturn = eventListToJSON(NULL);
    }

    deleteCalendar(obj);

    return stringToReturn;
}

char *alarmToJSON(Alarm *alarm){

    if(alarm == NULL){
        char *alarmJSON = copyString("{}");
        return alarmJSON;
    }

    char *alarmJSON = calloc(1,100*sizeof(char));

    sprintf(alarmJSON,"{\"action\":\"%s\",\"trigger\":\"%s\",\"numProps\":%d}",alarm->action,alarm->trigger,alarm->properties->length+2);
    return alarmJSON;
}


char *alarmListToJSON(List *alarmsList){

    if(alarmsList == NULL){
        char *alarmListJSON = copyString("[]");
        return alarmListJSON;
    }

    if(alarmsList->head == NULL){
        char *alarmListJSON = copyString("[]");
        return alarmListJSON;
    }

    if(alarmsList->length <= 0 ){
        char *alarmListJSON = copyString("[]");
        return alarmListJSON;
    }
    
    int noOfAlarms = alarmsList->length;
    char *alarmListJSON = calloc(1, (noOfAlarms*160)*sizeof(char));
    Node *curr = alarmsList->head;
    Alarm *currAlarm = (Alarm *)curr->data;
    char *alarmJSON = NULL;
    strcpy(alarmListJSON, "[");
    free(alarmJSON);

    while( curr->next != NULL ){
        currAlarm = (Alarm *)curr->data;
        alarmJSON = alarmToJSON(currAlarm);
        strcat(alarmListJSON, alarmJSON);
        strcat(alarmListJSON, ",");
        free(alarmJSON);
        curr = curr->next;
    }

    if( curr->next == NULL ){
        currAlarm = (Alarm *)curr->data;
        alarmJSON = alarmToJSON(currAlarm);
        strcat(alarmListJSON, alarmJSON);
        free(alarmJSON);
        
    }

    strcat(alarmListJSON, "]");

    return alarmListJSON;
}



Event *fetchEventAtIndex(List *eventList, int index){

    if(index > eventList->length){
        return NULL;
    }

    int i = 1;
    Node *curr = eventList->head;
    Event *toReturn = NULL;

    while( (curr->next != NULL) && ( i < index) ){
        curr = curr->next;
        i++;
    }

    toReturn = (Event *)curr->data;

    return toReturn;
}

char *fetchEvtProps(Event *event){

    if(event->properties == NULL){
        char *propString = copyString("");
        return propString;
    }

    if(event->properties->head == NULL){
        char *propString = copyString("");
        return propString;
    }

    if(event->properties->length == 0){
        char *propString = copyString("");
        return propString;
    }

    char *optProps = modifiedToString(event->properties);
    return optProps;

}

char *alarmsToJSON(char *fileName, int eventIndex){

    ICalErrorCode calCode;
    Calendar *obj;
    char temp[100];
    strcpy(temp,"./uploads/");
    strcat(temp,fileName);
    char *stringToReturn = NULL;
    Event *eventFetched = NULL;
    calCode = createCalendar(temp,&obj);
    
    if(calCode == OK){
        eventFetched = fetchEventAtIndex(obj->events,eventIndex);
        stringToReturn = alarmListToJSON(eventFetched->alarms);
    }
    else{
        stringToReturn = alarmListToJSON(NULL);
    }

    deleteCalendar(obj);

    return stringToReturn;

}

char *fetchOptionalProperties(char *fileName,int eventIndex){

    ICalErrorCode calCode;
    Calendar *obj;
    char temp[100];
    strcpy(temp,"./uploads/");
    strcat(temp,fileName);
    char *stringToReturn = NULL;
    Event *eventFetched = NULL;
    calCode = createCalendar(temp,&obj);
    
    if(calCode == OK){
        eventFetched = fetchEventAtIndex(obj->events,eventIndex);
        if(eventFetched->properties->length > 0){
            stringToReturn = propertyListToJSON(eventFetched->properties);
        }
        else{
            stringToReturn = copyString("[]");
        }
    }
    else{
        stringToReturn = modifiedToString(NULL);
    }

    deleteCalendar(obj);

    return stringToReturn;

}

char *propertyToJSON(Property *property){

    if(property == NULL){
        char *propertyJSON = copyString("{}");
        return propertyJSON;
    }
    
    char *propertyJSON = calloc(1,2000*sizeof(char));
    sprintf(propertyJSON,"{\"propName\":\"%s\",\"propDescr\":\"%s\"}",property->propName,property->propDescr);
    return propertyJSON;

}

char *propertyListToJSON(List *propertyList){

    if(propertyList == NULL){
        char *propertyListJSON = copyString("[]");
        return propertyListJSON;
    }

    if(propertyList->head == NULL){
        char *propertyListJSON = copyString("[]");
        return propertyListJSON;
    }
    
    int noOfProps = propertyList->length;
    char *propertyListJSON = calloc(1, (noOfProps*2000)*sizeof(char));
    Node *curr = propertyList->head;
    Property *currProp = (Property *)curr->data;
    char *propertyJSON = NULL;
    strcpy(propertyListJSON, "[");
    //free(propertyJSON);

    while( curr->next != NULL ){
        currProp = (Property *)curr->data;
        propertyJSON = propertyToJSON(currProp);
        strcat(propertyListJSON, propertyJSON);
        strcat(propertyListJSON, ",");
        free(propertyJSON);
        curr = curr->next;
    }

    if( curr->next == NULL ){
        currProp = (Property *)curr->data;
        propertyJSON = propertyToJSON(currProp);
        strcat(propertyListJSON, propertyJSON);
        free(propertyJSON);
        
    }

    strcat(propertyListJSON, "]");

    return propertyListJSON;

}

Event *createDynamicEvent(char *UID, char *startDT, char *createDT, char *summ){

DateTime *stDT;
DateTime *ctDT;
Property *summary = NULL;
ICalErrorCode stDTCode = parseDateTime(startDT,&stDT);
ICalErrorCode ctDTCode = parseDateTime(createDT,&ctDT);

if(strlen(summ) > 0){
    summary = createProperty("SUMMARY",summ);
}

List *alarms = initializeList(printAlarm,deleteAlarm,compareAlarms);
List *properties = initializeList(printProperty,deleteProperty,compareProperties);
if(summary != NULL){
    insertBack(properties,summary);
}
Event *event = createEvent(UID,*stDT,*ctDT,properties,alarms);

if(stDTCode == OK && ctDTCode == OK){
    return event;
}
else{
    return NULL;
}

}

char *addEventDynamic(char *fileName, char *UID, char *startDT, char *createDT, char *summ){

    ICalErrorCode calCode;
    Calendar *obj;
    char temp[100];
    strcpy(temp,"./uploads/");
    strcat(temp,fileName);
    Event *eventToAdd = NULL;
    calCode = createCalendar(temp,&obj);
    char *addCode = NULL;
    ICalErrorCode writeCode = WRITE_ERROR;

    if(calCode == OK){
        eventToAdd = createDynamicEvent(UID,createDT,startDT,summ);
        if(eventToAdd != NULL){
            addEvent(obj,eventToAdd);
            writeCode = writeCalendar(temp,obj);
            addCode = printError(writeCode);
        }
            
    }

    deleteCalendar(obj);

    return addCode;

}

char *addCalendarDynamic(char *fileName, char *prodid, char *UID, char *startDT, char *createDT, char *summ){
    
    Calendar *obj;
    char temp[100];
    strcpy(temp,"./uploads/");
    strcat(temp,fileName);
    char *calCodeString = NULL;
    ICalErrorCode writeCode = WRITE_ERROR;
    ICalErrorCode calCode = OK;

    List *eventList = initializeList(printEvent,deleteEvent,compareEvents);
    List *propertyList = initializeList(printProperty,deleteProperty,compareProperties);
    Event *firstEvent = createDynamicEvent(UID,startDT,createDT,summ);
    insertBack(eventList,firstEvent);
    
    obj = constructCalendar(2.0,prodid,eventList,propertyList);
    calCode = validateCalendar(obj);
    if(calCode == OK){
        writeCode = writeCalendar(temp,obj);
    }
    deleteCalendar(obj);
    calCodeString = printError(writeCode);

    return calCodeString;
}

char *eventToJSONAlt(Event *event){

    char *eventJSON = calloc(1,6000*sizeof(char));

    if(event == NULL){
        eventJSON = copyString("{}");
        return eventJSON;
    }

    char *startDTJSON = dtToJSON(event->startDateTime);
    char *alarms = alarmListToJSON(event->alarms);
    char *properties = propertyListToJSON(event->properties);

    /*char *eventSummary = findSummary(event);
    if(eventSummary == NULL){
        eventSummary = copyString("");
    }*/
    sprintf(eventJSON,"{\"startDT\":%s,\"properties\":%s,\"alarms\":%s}",startDTJSON,properties,alarms);
    
    free(startDTJSON);
    free(alarms);
    free(properties);

    return eventJSON;
}

char *eventListToJSONAlt(List *eventList){

    if(eventList == NULL){
        char *eventListJSON = copyString("[]");
        return eventListJSON;
    }
    if(eventList->head == NULL){
        char *eventListJSON = copyString("[]");
        return eventListJSON;
    }
    if(eventList->length == 0){
        char *eventListJSON = copyString("[]");
        return eventListJSON;
    }

    int noOfEvents = eventList->length;
    
    char *eventListJSON = calloc(1, (noOfEvents*10000)*sizeof(char));
    Node *curr = eventList->head;
    Event *currEvent = (Event *)curr->data;
    char *eventJSON = NULL;
    strcpy(eventListJSON, "[");

    while( curr->next != NULL ){
        currEvent = (Event *)curr->data;
        eventJSON = eventToJSONAlt(currEvent);
        strcat(eventListJSON, eventJSON);
        strcat(eventListJSON, ",");
        
        curr = curr->next;
    }

    if( curr->next == NULL ){
        currEvent = (Event *)curr->data;
        eventJSON = eventToJSONAlt(currEvent);
        strcat(eventListJSON, eventJSON);
        
    }

    strcat(eventListJSON, "]");

    return eventListJSON;
}

char *eventsToJSONAlt(char *fileName){
    ICalErrorCode calCode;
    Calendar *obj;
    char temp[100];
    strcpy(temp,"./uploads/");
    strcat(temp,fileName);
    char *stringToReturn = NULL;

    calCode = createCalendar(temp,&obj);
    
    if(calCode == OK){
        stringToReturn = eventListToJSONAlt(obj->events);
    }
    else{
        stringToReturn = eventListToJSONAlt(NULL);
    }

    deleteCalendar(obj);

    return stringToReturn;
}
