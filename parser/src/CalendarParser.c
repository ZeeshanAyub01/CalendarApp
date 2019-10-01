#include <stdlib.h>
#include "CalendarParser.h"
#include "LinkedListAPI.h"
#include "stringHelpers.h"
#include "CalendarHelpers.h"
#include "CalendarParser.h"

void deleteEvent(void* toBeDeleted){
    Event *eventToDelete = (Event *)toBeDeleted;
    deleteDate(&eventToDelete->creationDateTime);
    deleteDate(&eventToDelete->startDateTime);
    freeList(eventToDelete->properties);
    freeList(eventToDelete->alarms);
    free(eventToDelete);
}
int compareEvents(const void* first, const void* second){
    return 0;
}
char* printEvent(void* toBePrinted){
    Event *eventToPrint = (Event *)toBePrinted;
    char *creationDT = printDate(&eventToPrint->creationDateTime);
    char *startDT = printDate(&eventToPrint->startDateTime);
    char *eventProperties = modifiedToString(eventToPrint->properties);
    char *eventAlarms = modifiedToString(eventToPrint->alarms);
    char *eventString = malloc(sizeof(char)*(strlen(creationDT) + strlen(startDT) + strlen(eventToPrint->UID) + strlen(eventProperties) + strlen(eventAlarms) + 70) );
    strcpy(eventString, "BEGIN:VEVENT\r\n");
    strcat(eventString, "UID:" );
    strcat(eventString, eventToPrint->UID);
    strcat(eventString, "\r\nDTSTAMP:");
    strcat(eventString, creationDT);
    strcat(eventString, "\r\nDTSTART:");
    strcat(eventString, startDT);
    //strcat(eventString, "\n");
    strcat(eventString, eventProperties);
    strcat(eventString, eventAlarms);
    strcat(eventString, "\r\n");
    strcat(eventString, "END:VEVENT");
    free(creationDT);
    free(startDT);
    free(eventProperties);
    free(eventAlarms);
    return eventString;
}

void deleteAlarm(void* toBeDeleted){
    Alarm *alarmToDelete = (Alarm *)toBeDeleted;
    free(alarmToDelete->trigger);
    freeList(alarmToDelete->properties);
    free(alarmToDelete);
}
int compareAlarms(const void* first, const void* second){
    return 0;
}
char* printAlarm(void* toBePrinted){
    Alarm *alarmToPrint = (Alarm *)toBePrinted;
    //printf("Action: %s Trigger: %s\n", alarmToPrint->action, alarmToPrint->trigger);
    char *alarmProperties = modifiedToString(alarmToPrint->properties);
    char *alarmString = malloc(sizeof(char)*(strlen(alarmToPrint->action)+strlen(alarmToPrint->trigger) + 50 + strlen(alarmProperties)));
    strcpy(alarmString, "BEGIN:VALARM");
    strcat(alarmString, "\r\nACTION:");
    strcat(alarmString, alarmToPrint->action);
    strcat(alarmString, "\r\nTRIGGER;");
    strcat(alarmString, alarmToPrint->trigger);
    //strcat(alarmString, "\n");
    strcat(alarmString, alarmProperties);
    strcat(alarmString, "\r\nEND:VALARM");
    free(alarmProperties);

    return alarmString;
}


void deleteProperty(void* toBeDeleted){
    Property *propertyToDelete = (Property *)toBeDeleted;
    free(propertyToDelete);
}
int compareProperties(const void* first, const void* second){
    return 0;
}
char* printProperty(void* toBePrinted){
    Property *propertyToPrint = (Property *)toBePrinted;
    //printf("Property Name: %s, Property Description: %s\n", propertyToPrint->propName, propertyToPrint->propDescr);
    char *propertyString = malloc( (strlen(propertyToPrint->propName) + strlen(propertyToPrint->propDescr) + 50)*sizeof(char));
    //strcpy(propertyString, "Property Name: ");
    strcpy(propertyString, propertyToPrint->propName);
    if( compareStringsIgnoreCase(propertyToPrint->propName,"ATTACH") ){
        strcat(propertyString, ";");
    }
    else{
        strcat(propertyString, ":");
    }
    strcat(propertyString, propertyToPrint->propDescr);
    //strcat(propertyString,"\r\n");
    return propertyString;
}

void deleteDate(void* toBeDeleted){
   // DateTime *toDelete = (DateTime *)toBeDeleted;
   // free(toDelete);
}
int compareDates(const void* first, const void* second){
    return 0;
}
char* printDate(void* toBePrinted){
    DateTime *dateTimeToPrint = (DateTime *)toBePrinted;
    //printf("Date: %s Time: %s UTC: ", dateTimeToPrint->date, dateTimeToPrint->time);
    char *dateTime = malloc(sizeof(char)*(strlen(dateTimeToPrint->date) + strlen(dateTimeToPrint->time) + 50) );
    //strcpy(dateTime, "Date: ");
    strcpy(dateTime, dateTimeToPrint->date);

    if( strlen(dateTimeToPrint->time) != 0){
        strcat(dateTime, "T");
        strcat(dateTime, dateTimeToPrint->time);
    }
    if(dateTimeToPrint->UTC == true){
        strcat(dateTime, "Z");
    }
    return dateTime;
}

void deleteCalendar(Calendar *obj){
    if(obj == NULL){
        return;
    }
    freeList(obj->events);
    freeList(obj->properties);
    free(obj);
    obj = NULL;
}

ICalErrorCode createCalendar(char *fileName, Calendar **obj){

    if( fileName == NULL ){
        *obj = NULL;
        return INV_FILE;
    }

    if( !endsWith(fileName, ".ics") ){
        *obj = NULL;
        return INV_FILE;
    }
    char *calendarString = fileRead(fileName);

    if( calendarString == NULL){
        *obj = NULL;
        return INV_FILE;
    }

    ICalErrorCode calendarStatus = parseCalendar(calendarString, obj);

    return calendarStatus;
}

char *printCalendar(const Calendar *obj){

    if(obj == NULL){
        return NULL;
    }
    
    char *calendarEvents = modifiedToString(obj->events);
    char *calendarProperties = modifiedToString(obj->properties);
    char *verDecString = NULL;
    //char version = intToChar((int )obj->version);
    int ver = (int)obj->version;
    //printf("%f\n",obj->version);
    float versionDecimal = obj->version - ver;
    //printf("vd : %f\n",versionDecimal);
    versionDecimal = versionDecimal*10 + 0.1;
    //printf("vd*10 : %f\n",versionDecimal);
    verDecString = intToString((int)versionDecimal);

    char *versionString = intToString((int)obj->version);
    versionString = realloc(versionString, 4*sizeof(char));
    //itoa((int)obj->version, versionString, 10);
    strcat(versionString, ".");
    strcat(versionString, verDecString);
    char *calendarString = malloc(sizeof(char)*(strlen(calendarEvents)+strlen(calendarProperties)+200) );
    strcpy(calendarString, "BEGIN:VCALENDAR\r\n");
    strcat(calendarString, "VERSION:");
    //printf("verrrr: %s\n",versionString);
    strcat(calendarString, versionString);
    strcat(calendarString, "\r\n");
    strcat(calendarString, "PRODID:");
    strcat(calendarString, obj->prodID);
    //strcat(calendarString, ".0");
    //strcat(calendarString, "\r\n");
    strcat(calendarString, calendarProperties);
    strcat(calendarString, calendarEvents);
    strcat(calendarString, "\r\nEND:VCALENDAR");
    free(versionString);
    free(verDecString);
    free(calendarEvents);
    free(calendarProperties);
    return calendarString;
}

char *printError(ICalErrorCode err){

    char *errorString = NULL;

    switch (err)
    {
        case OK:
            errorString = copyString("Calendar OK!");
            break;

        case INV_FILE:
            errorString = copyString("Invalid file!");
            break;

        case INV_CAL:
            errorString = copyString("Invalid Calendar!");
            break;

        case INV_VER:
            errorString = copyString("Invalid version!");
            break;

        case DUP_VER:
            errorString = copyString("Duplicate version!");
            break;

        case DUP_PRODID:
            errorString = copyString("Duplicate Product ID!");
            break;

        case INV_PRODID:
            errorString = copyString("Invalid PRODID!");
            break;

        case INV_EVENT:
            errorString = copyString("Invalid event!");
            break;

        case INV_DT:
            errorString = copyString("Invalid Date time!");
            break;

        case INV_ALARM:
            errorString = copyString("Invalid alarm!");
            break;

        case WRITE_ERROR:
            errorString = copyString("Write error!");
            break;
    
        default:
            errorString = copyString("Other error!");
            break;
    }

    return errorString;
}

ICalErrorCode writeCalendar(char* fileName, const Calendar* obj){

    if(fileName == NULL){
        return WRITE_ERROR;
    }

    if(obj == NULL){
        return INV_CAL;
    }
    
    if( !endsWith(fileName, ".ics") ){
        return INV_FILE;
    }

    FILE *fp = NULL;
    char *calendarString = printCalendar(obj);
    fp = fopen(fileName,"w+");

    if(fp == NULL){
        free(calendarString);
        return WRITE_ERROR;
    }

    fprintf(fp,"%s\r\n",calendarString);
    free(calendarString);
    fclose(fp);
    return OK;
}

//

char *dtToJSON(DateTime Prop){

    char *dtJSON = calloc(50,sizeof(char));
    char *isUTC = NULL;

    if(Prop.UTC){
        isUTC = copyString("true");
    }
    else{
        isUTC = copyString("false");
    }
    sprintf(dtJSON,"{\"date\":\"%s\",\"time\":\"%s\",\"isUTC\":%s}",Prop.date,Prop.time,isUTC);
    
    free(isUTC);
    return dtJSON;
}

char *eventToJSON(const Event *event){

    char *eventJSON = calloc(1,5000*sizeof(char));

    if(event == NULL){
        eventJSON = copyString("{}");
        return eventJSON;
    }

    
    char *startDTJSON = dtToJSON(event->startDateTime);
    //printf("%s\n",startDTJSON);
    char *eventSummary = findSummary(event);
    if(eventSummary == NULL){
        eventSummary = copyString("");
    }
    sprintf(eventJSON,"{\"startDT\":%s,\"numProps\":%d,\"numAlarms\":%d,\"summary\":\"%s\"}",startDTJSON,event->properties->length+3,event->alarms->length,eventSummary);
    
    if(eventSummary != NULL){
        free(eventSummary);
    }
    free(startDTJSON);
    return eventJSON;
}

char *eventListToJSON(const List *eventList){
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

    //printf("....\n");
    int noOfEvents = eventList->length;
    //int i = 1;
    char *eventListJSON = calloc(1, (noOfEvents*10000)*sizeof(char));
    Node *curr = eventList->head;
    Event *currEvent = (Event *)curr->data;
    char *eventJSON = NULL;
    strcpy(eventListJSON, "[");
    //strcat(eventListJSON, eventJSON);
    //strcat(eventListJSON, ",");
    //free(eventJSON);

    while( curr->next != NULL ){
        currEvent = (Event *)curr->data;
        eventJSON = eventToJSON(currEvent);
        strcat(eventListJSON, eventJSON);
        strcat(eventListJSON, ",");
        //free(eventJSON);
        curr = curr->next;
    }

    if( curr->next == NULL ){
        currEvent = (Event *)curr->data;
        eventJSON = eventToJSON(currEvent);
        strcat(eventListJSON, eventJSON);
        //strcat(eventListJSON, ",");
        //free(eventJSON);
        //curr = curr->next;
    }

    strcat(eventListJSON, "]");

    return eventListJSON;
}

char *calendarToJSON(const Calendar *cal){

    char *calendarJSON = NULL;

    if(cal == NULL){
        calendarJSON = calloc(1,10*sizeof(char));
        strcpy(calendarJSON,"{}");
        return calendarJSON;
    }

    calendarJSON = calloc(1,100*sizeof(char));
    sprintf(calendarJSON,"{\"version\":%d,\"prodID\":\"%s\",\"numProps\":%d,\"numEvents\":%d}",(int)cal->version,cal->prodID,cal->properties->length+2,cal->events->length);

    return calendarJSON;
}

Calendar *JSONtoCalendar(const char* str){

    if(str == NULL){
        return NULL;
    }

    char *vap = copyString((char*)str);
    Array *verAndprodid = splitStringIntoTwo(vap, ",");
    char **strings = verAndprodid->array;
    char *prodID = copyCharacters(strings[1], 10, strlen(strings[1])-2);
    float ver = (float)charToInt(strings[0][strlen(strings[0])-1]);
    List *optionalCalProperties = initializeList(printProperty, deleteProperty, compareProperties);
    List *eventsList = initializeList(printEvent, deleteEvent, compareEvents);
    Calendar *calendar = constructCalendar(ver,prodID,eventsList,optionalCalProperties);
    
    free(prodID);
    free(vap);
    deleteArray(verAndprodid);
    return calendar;
}

Event *JSONtoEvent(const char* str){

    if(str == NULL){
        return NULL;
    }

    char *uidJson = copyString((char*)str);
    Array *splitJson = splitStringIntoTwo(uidJson, ":");
    char **strings = splitJson->array;
    char *uid = copyCharacters(strings[1], 1, strlen(strings[1])-2);
    
    List *alarmsList = initializeList(printAlarm, deleteAlarm, compareAlarms);
    List *eventOptionalPropList = initializeList(printProperty, deleteProperty, compareProperties);
    DateTime startDateTime;
    DateTime creationDateTime;
    Event *eventToReturn = createEvent(uid, creationDateTime, startDateTime, eventOptionalPropList, alarmsList);
    free(uid);
    free(uidJson);
    deleteArray(splitJson);
    return eventToReturn;
}

void addEvent(Calendar *cal, Event *toBeAdded){
    
    if( cal == NULL || toBeAdded == NULL){
        return;
    }
    insertBack(cal->events, toBeAdded);
}


ICalErrorCode validateCalendar(const Calendar* obj){

    if(obj == NULL){
        return INV_CAL;
    }

    if(obj->events == NULL){
       return INV_CAL; 
    }

    if(obj->properties == NULL){
        return INV_CAL;
    }

    if( obj->events->length == 0){
        return INV_CAL;
    }
        
    if(obj->version <= 0){
        return INV_CAL;
    }

    if( strlen(obj->prodID) < 1 ){
        return INV_CAL;
    }

    ICalErrorCode calCode = OK;
    calCode = validateCalendarProperties(obj->properties);
    if(calCode != OK){
        return calCode;
    }
    calCode = validateAllEvents(obj->events);

    if(calCode != OK){
        return calCode;
    }

    return OK;
}