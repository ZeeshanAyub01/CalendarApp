**************************************************
Name: Muhammad Zeeshan Ayub,
CalendarApp
**************************************************

ASSUMPTIONS:
	1. All files stored in the uploads folder must have UNIQUE NAMES, other items do not have to be unique,
	2. The database may contain files previously stored on them, even if they have been removed from the 
	   uploads folder. In order to update the database the user must click on the 'Store Files button'. This
	   will delete the previous records on the database, and add new ones for the files currently on the server.


ADDITIONS SINCE last version:
	1. The database login: users can login using their Uoguelph central login id's as the user name and database name.
	   Upon success or failure to connect a message area will notify the user about the result.
	2. Store files: By clicking the button 'Store files' users can update the database, because the button will
	   delete the previous records and new ones for the files present in the uploads folder.
	3. Clear Files: This will simply delete all the records on the database. This will also disable all te query
	   buttons except for the display database status. 
	4. Display Database Status: This button will show the total number of files, events and alarms on the database at
	   that specific time.
	5. Additional queries:
		i. View Events Sorted: This will display all events in order of ascending start time,
		ii. View Events from a file: The user must enter a valid iCalendar file name in the text area above the query buttons,
		    and the program will show them details of all the events in that file,
		iii. View conflicting events: this will show all the events with the same start date and time,
		iv. List database files: this query will display all the files currently on the database and their number of events,
		v. Alarms from a file: The user must enter a valid iCalendar file name in the text area above the query buttons,
		   and the program will show them details of all the alarms in that file,
		vi. Alarms Sorted: This query will show all alarms on the database in the order of their events' start times.

Introduction:
	This ICalendar Viewer allows users to view,download and upload ICalendar files, to add events to existing 
	ICalendar files, and to access files stored in the database. Users can log in using their central login accounts 
	to login into the database. The functionality is the same as that of Assignment-3, with the addition of the 
	database functionality (read the section above).

How the ICalendar Viewer works:

	Upon refresh the page will parse all the files included in the uploads folder,
and display their information in the file Log Panel. If there are no files, or if all the 
files failed to parse, the table will simply display a message saying, 'No calendars to show'.

	To download a file, the user must click on the name of the file, and to upload the user must
click on the 'CHOOSE FILE', and then upload. Note: the program will only detect the .ics files.

	In order to view the events in a calendar the user must select a calendar file fro the dropdown,
and click on view events. If the view events button was clicked when no calendar was selected, a 
message will be displayed both on the status panel and the display box asking the user to select
a calendar before viewing events. Upon success however, all the events will be numbered and displayed.

	To view, the alarms and optional properties for an event, it's the same procedure. The event 
number must be selected from a seperate dropdown for them, and the appropriate button must be selected 
from the button group labelled 'View Event Actions'. If those buttons are clicked when no event has been 
selected, a message will show on the box asking the user to select a calendar and then an event.

	To add an event to an existing calendar, click the button saying 'Add a New Event'. Again, the user
must ensure that a calendar file was selected from the dropdown prior to this. If the button was clicked when
a calendar was selected, a panel will populate with all the fields to fill out to add the event. ALL THE FIELDS
EXCEPT FOR THE SUMMARY ARE MANDATORY. If any of those fields is not filled, a seperate message panel will dispay
the appropriate error under the panel, and the window will automatiacally scroll down so the user can see that 
error.The same error will also show on the status panel. Upon success the new event will be added to the list of 
events, and the user will be able to see it by clicking on the 'View Calendar Events' button.

	To add a new calendar the user must click on the button saying 'Add a New Calendar'. This will populate 
the panel underneathh it with all the fields that need to filled out. The panel will ask the user to fill out the 
details for one event. If the user wishes to add more events, they can do so later using 'Add a New Event'. Again, 
all the fields except for the summary are mandatory, and any are not filled, an appropriate error will be displayed 
on the panel underneath it, and the status panel. Upon success the user will be able to see calendar in the file 
Log panel, and add events to it.

	To know how to use the database functionality refer to the heading above, Additions Since Assignment-3.
