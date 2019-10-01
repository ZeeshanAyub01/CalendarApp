#ifndef CALENDAR_HELPERS_H
#define CALENDAR_HELPERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "CalendarParser.h"
#include "LinkedListAPI.h"
#include "stringHelpers.h"

Array *extractEvents( char *calendarString );

ICalErrorCode parseEvent(Array *eventArray, Event **eventToReturn);

char *removeCalComponents(char *calendarString, Array *Events);

Array *returnCalProperties(char *calString);

ICalErrorCode parseCalendar(char *calendarFile, Calendar **obj);

char *removeEventComponents(char *eventString, Array *Alarms);

ICalErrorCode parseOptionalProperties(char *propertyString, Property **propertyToReturn);

ICalErrorCode parseAlarm(Array *alarmArray, Alarm **alarmToReturn);

Property *createProperty(char *propertyName, char *propertyDescription);

Alarm *createAlarm(char *action, char *trigger, List *optionalProperties);

ICalErrorCode parseDateTime(char *dateTimeString, DateTime **dateTimeToReturn);

DateTime *createDateTime(char *date, char *time, bool utc);

Event *createEvent(char *uid, DateTime creationDateTime, DateTime startDateTime, List *optionalProperties, List *alarms);

Calendar *constructCalendar(float version, char *prodid, List *events, List *properties);

ICalErrorCode validateFile(Array *calendarArray);

int validateStartDateTime( char *dateTimeToValidate );

int validateCreationDateTime( char *dateTimeToValidate );

int validateVersion(char *versionString);

char *findSummary(const Event* event);

ICalErrorCode validateEvent(Event *toValidate);

ICalErrorCode validateAlarm(Alarm *toValidate);

ICalErrorCode validateProperty(Property *toValidate);

ICalErrorCode validateCalendarProperties(List *calProperties);

ICalErrorCode validateEventProperties(List *evtProperties);

ICalErrorCode validateAlarmProperties(List *almProperties);

ICalErrorCode validateAllEvents(List *allEvents);

ICalErrorCode validateAllAlarms(List *allAlarms);

int verifyAlarmProperty(Property *toVerify);

int verifyEventProperty(Property *toVerify);

ICalErrorCode verifyCalendarProperty(Property *toVerify);

char* modifiedToString(List * list);

ICalErrorCode validateDateTime(DateTime toValidate);

float extractVersion(char *versionString);

void errorPrinter(char *errorMessage, char *errorLoc);

char *calFileToJSON(char *fileName);

char *eventsToJSON(char *fileName);

char *fetchEvtProps(Event *event);

Event *fetchEventAtIndex(List *eventList, int index);

char *alarmToJSON(Alarm *alarm);

char *alarmsToJSON(char *fileName, int eventIndex);

char *fetchOptionalProperties(char *fileName,int eventIndex);

char *propertyToJSON(Property *property);

char *propertyListToJSON(List *propertyList);

Event *createDynamicEvent(char *UID, char *startDT, char *createDT, char *summ);

char *addEventDynamic(char *fileName, char *UID, char *startDT, char *createDT, char *summ);

char *addCalendarDynamic(char *fileName, char *prodid, char *UID, char *startDT, char *createDT, char *summ);

char *eventToJSONAlt(Event *event);

char *eventsToJSONAlt(char *fileName);

#endif