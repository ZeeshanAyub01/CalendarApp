// Put all onload AJAX calls here, and event listeners
$(document).ready(function() {

    $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/viewCalJSON',   //The server endpoint we are connecting to
        success: function (data) {
            var noOfValidFiles = 0;
            $('#fileLogPanel').empty();

            if(data.arr.length == 0){
                $('#fileLogPanel').append('<p>No Files to show</p>');
            }
            else{
            $('#fileLogPanel').append("<table class=\"table table-striped header-fixed;\" id=\"test\"style=\"background-color:lightblue\";>\
                    <tr>\
                        <th > File Name </th>\
                        <th> Version </th>\
                        <th> Product Id </th>\
                        <th>  No of Events  </th>\
                        <th>  No of Properties  </th>\
                    </tr>\
            </table>"    );
            for(var i = 0; i < data.arr.length; i++){
                let obj = JSON.parse(data.arr[i]);
                let fName = data.fileNames[i];
                if(data.arr[i] != "{}"){
                    //console.log(fName);
                    let row = "<tr><td><a href=\"/uploads/"+fName+"\">"+fName+"</a></td><td>"+obj.version+"</td><td>"+obj.prodID+"</td><td>"+obj.numEvents+"</td><td>"+obj.numProps+"</td></tr>";
                    //let link = "<a href="+fName+">"+fName+"</a>";
                    $("#test").append(row);
                    $('#sel').append("<option value="+fName+">"+fName+"</option>");
                    noOfValidFiles++;
                }
                else{
                    let row = "<tr><td></td><td></td><td>Invalid File</td><td></td><td></td></tr>";
                    $("#test").append(row);
                    $('#Status').append('Could not parse ');
                    $('#Status').append(fName+'\n');
                    var element = document.getElementById("Status");//https://stackoverflow.com/questions/18614301/keep-overflow-div-scrolled-to-bottom-unless-user-scrolls-up
                    element.scrollTop = element.scrollHeight;
                    
                }
            }
            if(noOfValidFiles == 0){
                $('#fileLogPanel').empty();
                $('#fileLogPanel').append('<p>No Calendars to show</p>');
            }
           
            console.log(data.fileNames); 
        }
        },
        fail: function(error) {
            // Non-200 return, do something with error
            console.log(error); 
        }
    });
    


    $('#clear').click(function(e){
        //let blank = $('#Status').val();
        $('#Status').empty();
        //console.log(blank);
        //Pass data to the Ajax call, so it gets passed to the 
        //$.ajax({});
    });

    $('#clearQuery').click(function(e){
        
        $('#Query').empty();
        
    });

    $('#btn').click(function(e){
        let blank = $('#sel').val();
        $('#calView').empty();
        $('#misc').empty();
        $('#misc3').empty();
        document.getElementById('evtAddBtn').disabled = true;
        document.getElementById('calAddBtn').disabled = true;

        if(blank != 'Select a Calendar to View'){
            //console.log(blank);
            $('#misc').append('<p>Showing all events for the chosen calendar:</p>');
            $.ajax({
                type: 'get',           
                dataType: 'json',
                data: {filename : blank},	       
                url: '/viewEventList',   
                success: function (data) {
                    let eventList = JSON.parse(data.ev);

                    $('#calView').text('');
                    $('#eventSel').empty();

                    $('#calView').append("<table class=\"table table-striped header-fixed;\" id=\"compTable\">\
                    <tr>\
                        <th > Event No  </th>\
                        <th> Start Date </th>\
                        <th> Start Time </th>\
                        <th>  No of Props  </th>\
                        <th>  No of Alarms  </th>\
                        <th>  Summary </th>\
                    </tr>\
            </table>"    );


                    for(var i=0; i < eventList.length; i++){
                        let event = eventList[i];
                        let eventNum = i+1;
                        let sDT = event.startDT;
                        //let startDT = JSON.parse(sDT);
                        //console.log(sDT);
                        let sDate = sDT.date;
                        let sTime = sDT.time;
                        let sDFormatted = sDate[0]+sDate[1]+sDate[2]+sDate[3]+'/'+sDate[4]+sDate[5]+'/'+sDate[6]+sDate[7];
                        let sTFormatted = sTime[0]+sTime[1]+':'+sTime[2]+sTime[3]+':'+sTime[4]+sTime[5];
                        if(sDT.isUTC == true){
                            sTFormatted = sTFormatted+' (UTC)';
                        }
                        //console.log(sDate);
                        //console.log(sTime);
                        let row = "<tr><td>"+eventNum+"</td><td>"+sDFormatted+"</td><td>"+sTFormatted+"</td><td>"+event.numProps+"</td><td>"+event.numAlarms+"</td><td>"+event.summary+"</td></tr>";
                    //console.log(event.numProps);
                        $('#compTable').append(row);
                        $('#eventSel').append("<option value="+eventNum+">"+eventNum+"</option>");
                        
                    }
                    
                },
                fail: function(error) {
                    console.log(error); 
                }
                
            });



        }
        else{
            $('#Status').append('No Calendar selected to view events\n');
            $('#calView').append('<p>You must first select a Calendar to view events</p>');
            var element = document.getElementById("Status");//https://stackoverflow.com/questions/18614301/keep-overflow-div-scrolled-to-bottom-unless-user-scrolls-up
            element.scrollTop = element.scrollHeight;
            //$('#Status').append(<div class="alert alert-primary" role="alert">No Calendar selected!</div>);
            
        }
    });



    $('#almView').click(function(e){
        let blank = $('#sel').val();
        let evtNum = $('#eventSel').val();
        $('#eventAct').empty();
        document.getElementById('evtAddBtn').disabled = true;
        document.getElementById('calAddBtn').disabled = true;

        if(evtNum != 'Select an Event to view alarms or optional Properties'){

            $.ajax({
                type: 'get',           
                dataType: 'json',
                data: {filename : blank, index : evtNum},	       
                url: '/viewAlarmList',   
                success: function (data) {

                    $('#eventAct').empty();
                    if(data.alm != '[]'){
                    $('#eventAct').append("<table class=\"table table-striped header-fixed;\" id=\"almTable\">\
                    <tr>\
                        <th > Alarm No  </th>\
                        <th>  Action  </th>\
                        <th>  Trigger  </th>\
                        <th>  No of Props </th>\
                    </tr>\
            </table>"    );

                    let alarmList = JSON.parse(data.alm);
                    for(var i=0; i < alarmList.length; i++){
                        let alarm = alarmList[i];
                        let alarmNum = i+1;
                        let row = "<tr><td>"+alarmNum+"</td><td>"+alarm.action+"</td><td>"+alarm.trigger+"</td><td>"+alarm.numProps+"</td></tr>";
                        $('#almTable').append(row);
                        //console.log(alarm);
                    }
                }
                else{
                    $('#eventAct').text('The selected event contains no alarms'); 
                }
                    
                },
                fail: function(error) {
                    console.log(error); 
                }
                
            });

        }
        else{
            $('#Status').append('You must select a calendar, and then an event to view alarms\n');
            var element = document.getElementById("Status");//https://stackoverflow.com/questions/18614301/keep-overflow-div-scrolled-to-bottom-unless-user-scrolls-up
            element.scrollTop = element.scrollHeight;
            $('#eventAct').text('You must select a calendar, and then an event to view alarms an optional properties'); 
        }
    });



    $('#optPropView').click(function(e){
        let blank = $('#sel').val();
        let evtNum = $('#eventSel').val();
        $('#eventAct').empty();
        document.getElementById('evtAddBtn').disabled = true;
        document.getElementById('calAddBtn').disabled = true;

        if(evtNum != 'Select an Event to view alarms or optional Properties'){
            $.ajax({
                type: 'get',           
                dataType: 'json',
                data: {filename : blank, index : evtNum},	       
                url: '/viewEvtOptProp',   
                success: function (data) {

                    $('#eventAct').empty();
                    if(data.prop != '[]'){
                    $('#eventAct').append("<table class=\"table table-striped header-fixed;\" id=\"propTable\">\
                    <tr>\
                        <th > Property No  </th>\
                        <th>  Property Name  </th>\
                        <th>  Property Description </th>\
                    </tr>\
            </table>"    );

                    let propList = JSON.parse(data.prop);
                    for(var i=0; i < propList.length; i++){
                        let property = propList[i];
                        let propNum = i+1;
                        let row = "<tr><td>"+propNum+"</td><td>"+property.propName+"</td><td>"+property.propDescr+"</td></tr>";
                        $('#propTable').append(row);
                        //console.log(alarm);
                    }
                }
                else{
                    $('#eventAct').text('The selected event contains no optional properties'); 
                }
                    
                },
                fail: function(error) {
                    console.log(error); 
                }
                
            });

        }
        else{
            $('#Status').append('You must select a calendar, and then an event to view alarms\n');
            $('#eventAct').append('<p>You must first select a Calendar and then an event to view alarms and optional properties!</p>');
            var element = document.getElementById("Status");//https://stackoverflow.com/questions/18614301/keep-overflow-div-scrolled-to-bottom-unless-user-scrolls-up
            element.scrollTop = element.scrollHeight;
        }
    });



    $('#addEvt').click(function(e){
        let blank = $('#sel').val();
        document.getElementById('calAddBtn').disabled = true;
        //$('#misc2').empty();
        $('#calView').empty();
        $('#misc').empty();
        $('#misc3').empty();

        if(blank != 'Select a Calendar to View'){
            
            $('#misc').append('<p>Enter the details for the event to be added:</p>');
            $.ajax({
                type: 'get',           
                dataType: 'json',
                data: {filename : blank},	       
                url: '/addEvent',   
                success: function (data) {
                    let eventForm = '<form><div style=\"overflow:hidden;margin:0px\" class=\"form-group\">\
                    <label for=\"eventUID\">UID:</label>\
                    <input type=\"text\" class=\"form-control\" id=\"eventUID\" placeholder="Enter UID">\
                    </div>\
                    <div style=\"overflow:hidden;\" class=\"form-row\" id=\"startDTInp\">\
                    <label for=\"startDTInp\" style=\"margin:5px\">Start Date:</label>\
                    <div style=\"overflow:hidden;\" class=\"col\" >\
                    <input type=\"text\" class=\"form-control\" id=\"startDYear\" placeholder="YYYY">\
                    </div>\
                    <div style=\"overflow:hidden;\" class=\"col\">\
                    <input type=\"text\" class=\"form-control\" id=\"startDMonth\" placeholder="MM">\
                    </div>\
                    <div style=\"overflow:hidden;\" class=\"col\">\
                    <input type=\"text\" class=\"form-control\" id=\"startDDay\" placeholder="DD">\
                    </div>\
                    </div>\
                    <div style=\"overflow:hidden;\" class=\"form-row\" id=\"startTimeInp\">\
                    <label for=\"startDTInp\" style=\"margin:5px\">Start Time (24hr format):</label>\
                    <div style=\"overflow:hidden;\" class=\"col\" >\
                    <input type=\"text\" class=\"form-control\" id=\"startTimeHour\" placeholder="hh">\
                    </div>\
                    <div style=\"overflow:hidden;\" class=\"col\">\
                    <input type=\"text\" class=\"form-control\" id=\"startTimeMinute\" placeholder="mm">\
                    </div>\
                    <div class="form-group form-check" style=\"overflow:hidden\">\
                    <input type="checkbox" class="form-check-input" id="utcCheck">\
                    <label class="form-check-label" for="exampleCheck1">UTC time</label>\
                    </div>\
                    </div>\
                    <div style=\"overflow:hidden;margin:0px\" class=\"form-group\">\
                    <label for=\"Summary\">Summary(Optional):</label>\
                    <input type=\"text\" class=\"form-control\" id=\"evtSumm\" placeholder="Summary...">\
                    </div>\
                    </form>';
                    //$('#evtDeets').text('Event details will be taken here');
                    $('#calView').append(eventForm);
                    document.getElementById('evtAddBtn').disabled = false;
                    //$('#misc2').html('<button class=\"btn btn-primary\" id=\"evtAddBtn\">Submit</button>');
                    //console.log('<button class=\"btn btn-primary\" id=\"evtAddBtn\">Submit</button>');
                },
                fail: function(error) {
                    console.log(error); 
                }
                
            });

        }
        else{
            $('#Status').append('No Calendar selected to add events\n');
            $('#calView').append('<p>You must first select a Calendar to add events</p>');
            var element = document.getElementById("Status");//https://stackoverflow.com/questions/18614301/keep-overflow-div-scrolled-to-bottom-unless-user-scrolls-up
            element.scrollTop = element.scrollHeight;
        }


    });


    $('#evtAddBtn').click(function(e){
        let blank = $('#sel').val();
        $('#misc3').empty();
        var dateCorrect = 1;
        var timeCorrect = 1;
        var uidOK = 1;
        var uid = $('#eventUID').val();
        var year = $('#startDYear').val();
        var today = new Date();
        var currYear = today.getFullYear();
        var month = $('#startDMonth').val();
        var currMonth = today.getMonth() + 1;
        var currDay = today.getDate();
        var day = $('#startDDay').val();
        var hour = $('#startTimeHour').val();
        var minute = $('#startTimeMinute').val();
        //var utc = $('#utcCheck').val();
        var utc = document.getElementById('utcCheck').checked;
        var currHour = today.getHours();
        var currMin = today.getMinutes();
        var summary = $('#evtSumm').val();

        if(uid == ''){
            $('#misc3').append('<p>Error: UID for the event can\'t be empty!</p>');
            window.scrollBy(0,50); 
            uidOK = 0; 
        }

        if(year == ''){
            $('#misc3').append('<p>Error: No value entered for the start year!</p>'); 
            window.scrollBy(0,50);
            //$('#misc3').append('<div class=\"alert alert-danger\" role=\"alert\">Oh crap!</div>');
            dateCorrect = 0;
        }
        else if( isNaN(year) ){
            $('#misc3').append('<p>Error: Invalid input for year!</p>');
            window.scrollBy(0,50);
            $('#startDYear').val('');
            dateCorrect = 0;
        }
        else if( year < currYear ){
            $('#misc3').append('<p>Error: The start year cannot be smaller than the current year!</p>');
            window.scrollBy(0,50);
            $('#startDYear').val('');
            dateCorrect = 0;
        }
        else if( year > 3000  ){
            $('#misc3').append('<p>Error: The start year entered is too large!</p>');
            window.scrollBy(0,50);
            $('#startDYear').val('');
            dateCorrect = 0;
        }

        if(month == ''){
            $('#misc3').append('<p>Error: No value entered for the start month!</p>');
            window.scrollBy(0,50);
            dateCorrect = 0;
        }
        else if( isNaN(month) ){
            $('#misc3').append('<p>Error: Invalid input for month!</p>');
            window.scrollBy(0,50);
            $('#startDMonth').val('');
            dateCorrect = 0;
        }
        else if( month > 12 || month < 1 ){
            $('#misc3').append('<p>Error: Invalid input for month!</p>');
            window.scrollBy(0,50);
            $('#startDMonth').val('');
            dateCorrect = 0;
        }
        else if( year == currYear && month < currMonth){
            $('#misc3').append('<p>Error: The start date cannot be smaller than the current date. Enter month again!</p>');
            window.scrollBy(0,100);
            $('#startDMonth').val('');
            dateCorrect = 0;
        }

        if(day == ''){
            $('#misc3').append('<p>Error: No value entered for the start day!</p>'); 
            window.scrollBy(0,100);
            dateCorrect = 0;
        }
        else if( isNaN(day) ){
            $('#misc3').append('<p>Error: Invalid input for day!</p>');
            window.scrollBy(0,100);
            $('#startDDay').val('');
            dateCorrect = 0;
        }
        else if( day > 31 ){
            $('#misc3').append('<p>Error: Day can never be greater than 31!</p>');
            window.scrollBy(0,100);
            $('#startDDay').val('');
            dateCorrect = 0;
        }
        else if( (month == 2 || month == 4 || month == 6 || month == 9 || month == 11) && (day > 30) ){
            $('#misc3').append('<p>Error: Invalid combination of month and day!</p>');
            window.scrollBy(0,100);
            $('#startDDay').val('');
            dateCorrect = 0;
        }
        else if( (month == 2 ) && (day > 29) ){
            $('#misc3').append('<p>Error: Invalid combination of month and day!</p>');
            window.scrollBy(0,100);
            $('#startDDay').val('');
            dateCorrect = 0;
        }
        else if( (month == 2 ) && (year%4 !=0) && (day > 28) ){
            $('#misc3').append('<p>Error: The year entered is not a leap year!</p>');
            window.scrollBy(0,100);
            $('#startDDay').val('');
            dateCorrect = 0;
        }
        else if( year == currYear && month == currMonth && day < currDay){
            $('#misc3').append('<p>Error: The start date cannot be smaller than the current date. Enter month again!</p>');
            window.scrollBy(0,100);
            $('#startDDay').val('');
            dateCorrect = 0;
        }

        if(hour == ''){
            $('#misc3').append('<p>Error: No value entered for the start time hour!</p>');
            window.scrollBy(0,100); 
            timeCorrect = 0;
        }
        else if( isNaN(hour) ){
            $('#misc3').append('<p>Error: Invalid input for start time hour!</p>');
            window.scrollBy(0,100);
            $('#startTimeHour').val('');
            timeCorrect = 0;
        }
        else if( hour < 0 || hour > 24 ){
            $('#misc3').append('<p>Error: Invalid value for the start time hour!</p>');
            window.scrollBy(0,100);
            $('#startTimeHour').val('');
            timeCorrect = 0;
        }

        if(minute == ''){
            $('#misc3').append('<p>Error: No value entered for the start time minute!</p>'); 
            window.scrollBy(0,100);
            timeCorrect = 0;
        }
        else if( isNaN(minute) ){
            $('#misc3').append('<p>Error: Invalid input for start time minute!</p>');
            window.scrollBy(0,100);
            $('#startTimeMinute').val('');
            timeCorrect = 0;
        }
        else if( minute < 0 || minute > 59 ){
            $('#misc3').append('<p>Error: Invalid value for the start time minute (it is between 0 and 59)!</p>');
            window.scrollBy(0,100);
            $('#startTimeHour').val('');
            timeCorrect = 0;
        }

        if(dateCorrect == 1 && timeCorrect == 1 && uidOK == 1){
            var m = Number(month);
            var d = Number(day);
            var cm = Number(currMonth);
            var cd = Number(currDay);
            var h = Number(hour);
            var min = Number(minute);
            var ch = Number(currHour);
            var cmin = Number(currMin);

            if( m < 10){
                m = '0' + m;
            }
            if(d < 10){
                d = '0' + d;
            }
            if(cd < 10){
                cd = '0' + cd;
            }
            if(cm < 10){
                cm = '0' + cm;
            }
            if(h < 10){
                h = '0' + h;
            }
            if(min < 10){
                min = '0' + min;
            }
            if(ch < 10){
                ch = '0' + ch;
            }
            if(cmin < 10){
                cmin = '0' + cmin;
            }

            let startD = year+m+d;
            let startT = h+''+min+'00';
            let currD = currYear+''+cm+''+cd;
            
            if(utc == true){
                startT = startT + 'Z';
            }
            let startDT = startD+'T'+startT;
            let createDT = currD+'T'+ch+cmin+'00';

            $.ajax({
                type: 'get',           
                dataType: 'json',
                data: {filename : blank, UID: uid, stDT : startDT, ctDT : createDT, summ : summary},	       
                url: '/addEventToFile',   
                success: function (data) {
                    let writeC = data.wC;
                    let fileName = data.fN;

                    if(writeC == 'Calendar OK!'){
                        $('#Status').text('Event added successfully to the file: ');
                        $('#Status').append(fileName+'\n');
                        var element = document.getElementById("Status");//https://stackoverflow.com/questions/18614301/keep-overflow-div-scrolled-to-bottom-unless-user-scrolls-up
                        element.scrollTop = element.scrollHeight;
                        $('#eventUID').val('');
                        $('#startDYear').val('');
                        $('#startDMonth').val('');
                        $('#startDDay').val('');
                        $('#startTimeHour').val('');
                        $('#startTimeMinute').val('');
                        $('#evtSumm').val('');
                        $('#misc3').append('<p>Event added successfully!</p>');
                    }
                        
                   
                    
                },
                fail: function(error) {
                    console.log(error); 
                }
                
            });



        }

    });



    $('#addCal').click(function(e){
        let blank = $('#sel').val();
        $('#misc3').empty();
        document.getElementById('evtAddBtn').disabled = true;
        document.getElementById('calAddBtn').disabled = false;
        //$('#misc2').empty();
        $('#calView').empty();
        $('#misc').empty();
            //console.log(blank+' and this is the event Number: '+evtNum);
            $('#misc').append('<p>Enter the details for the Calendar and Event to be added:</p>');
            $.ajax({
                type: 'get',           
                dataType: 'json',
                data: {filename : blank},	       
                url: '/addEvent',   
                success: function (data) {
                    let eventForm = '<form>\
                    <div style=\"overflow:hidden;margin:0px\" class=\"form-group\">\
                    <label for=\"fileName\">New File Name:</label>\
                    <input type=\"text\" class=\"form-control\" id=\"newFileName\" placeholder="Enter New File Name">\
                    </div>\
                    <div style=\"overflow:hidden;margin:0px\" class=\"form-group\">\
                    <label for=\"prodID\">Product ID:</label>\
                    <input type=\"text\" class=\"form-control\" id=\"prodid\" placeholder="Enter Product ID">\
                    </div>\
                    <div style=\"overflow:hidden;margin:0px\" class=\"form-group\">\
                    <label for=\"UID\">UID:</label>\
                    <input type=\"text\" class=\"form-control\" id=\"eventUID\" placeholder="Enter UID">\
                    </div>\
                    <div style=\"overflow:hidden;\" class=\"form-row\" id=\"startDTInp\">\
                    <label for=\"startDTInp\" style=\"margin:5px\">Start Date:</label>\
                    <div style=\"overflow:hidden;\" class=\"col\" >\
                    <input type=\"text\" class=\"form-control\" id=\"startDYear\" placeholder="YYYY">\
                    </div>\
                    <p id=\"testInput\"></p>\
                    <div style=\"overflow:hidden;\" class=\"col\">\
                    <input type=\"text\" class=\"form-control\" id=\"startDMonth\" placeholder="MM">\
                    </div>\
                    <div style=\"overflow:hidden;\" class=\"col\">\
                    <input type=\"text\" class=\"form-control\" id=\"startDDay\" placeholder="DD">\
                    </div>\
                    </div>\
                    <div style=\"overflow:hidden;\" class=\"form-row\" id=\"startTimeInp\">\
                    <label for=\"startTimeInp\" style=\"margin:5px\">Start Time (24hr format):</label>\
                    <div style=\"overflow:hidden;\" class=\"col\" >\
                    <input type=\"text\" class=\"form-control\" id=\"startTimeHour\" placeholder="hh">\
                    </div>\
                    <div style=\"overflow:hidden;\" class=\"col\">\
                    <input type=\"text\" class=\"form-control\" id=\"startTimeMinute\" placeholder="mm">\
                    </div>\
                    <div class="form-group form-check" style=\"overflow:hidden\">\
                    <input type="checkbox" class="form-check-input" id="utcCheck">\
                    <label class="form-check-label" for="exampleCheck1">UTC time</label>\
                    </div>\
                    </div>\
                    <div style=\"overflow:hidden;margin:0px\" class=\"form-group\">\
                    <label for=\"Summary\">Summary(Optional):</label>\
                    <input type=\"text\" class=\"form-control\" id=\"evtSumm\" placeholder="Summary...">\
                    </div>\
                    </form>';
                    $('#calView').append(eventForm);
                    
                },
                fail: function(error) {
                    console.log(error); 
                }
                
            });
            var element = document.getElementById("Status");//https://stackoverflow.com/questions/18614301/keep-overflow-div-scrolled-to-bottom-unless-user-scrolls-up
            element.scrollTop = element.scrollHeight;

    });



    $('#calAddBtn').click(function(e){
        $('#misc3').empty();
        var newFileName = $('#newFileName').val();
        var newProdid = $('#prodid').val();
        var dateCorrect = 1;
        var timeCorrect = 1;
        var uidOK = 1;
        var uid = $('#eventUID').val();
        var year = $('#startDYear').val();
        var today = new Date();
        var currYear = today.getFullYear();
        var month = $('#startDMonth').val();
        var currMonth = today.getMonth() + 1;
        var currDay = today.getDate();
        var day = $('#startDDay').val();
        var hour = $('#startTimeHour').val();
        var minute = $('#startTimeMinute').val();
        //var utc = $('#utcCheck').val();
        var utc = document.getElementById('utcCheck').checked;
        var currHour = today.getHours();
        var currMin = today.getMinutes();
        var summary = $('#evtSumm').val();
        var length = newFileName.length;
       

        if(newFileName == ''){
            $('#misc3').append('<p>Error: New name for the file can\'t be empty!</p>');
            $('#Status').append('Nothing entered for the name of the file \n');
            window.scrollBy(0,60);
            uidOK = 0; 
        }
        
        if( (newFileName[length-4] != '.') || (newFileName[length-3] != 'i') || (newFileName[length-2] != 'c') || (newFileName[length-1] != 's') ){
            $('#misc3').append('<p>Error: Invalid extension for the file name! It must be \'.ics\'</p>');
            $('#Status').append('Invalid extension for the file! \n');
            window.scrollBy(0,50);
            $('#newFileName').val(''); 
            uidOK = 0; 
        }

        if(newProdid == ''){
            $('#misc3').append('<p>Error: New Product ID for the Calendar can\'t be empty!</p>');
            $('#Status').append('Nothing entered for the Prodict ID of the calendar! \n');
            window.scrollBy(0,50);
            uidOK = 0; 
        }

        if(uid == ''){
            $('#misc3').append('<p>Error: UID for the event can\'t be empty!</p>');
            $('#Status').append('Nothing entered for the UID of the event! \n');
            window.scrollBy(0,50); 
            uidOK = 0; 
        }

        if(year == ''){
            $('#misc3').append('<p>Error: Start year is required!</p>');
            $('#Status').append('No value entered for the start year! \n');
            window.scrollBy(0,50); 
            var element = document.getElementById("Status");//https://stackoverflow.com/questions/18614301/keep-overflow-div-scrolled-to-bottom-unless-user-scrolls-up
            $('#Status').append('Nothing entered for the start year \n');
            element.scrollTop = element.scrollHeight;
            dateCorrect = 0;
        }
        else if( isNaN(year) ){
            $('#misc3').append('<p>Error: Invalid input for year!</p>');
            $('#Status').append('Invalid input for year! \n');
            window.scrollBy(0,50);
            $('#startDYear').val('');
            dateCorrect = 0;
        }
        else if( year < currYear ){
            $('#misc3').append('<p>Error: The start year cannot be smaller than the current year!</p>');
            $('#Status').append('Invalid value for year! \n');
            window.scrollBy(0,50); 
            $('#startDYear').val('');
            dateCorrect = 0;
        }
        else if( year > 9999  ){
            $('#misc3').append('<p>Error: The start year entered is too large!</p>');
            $('#Status').append('Invalid value for year! \n');
            $('#startDYear').val('');
            window.scrollBy(0,50); 
            dateCorrect = 0;
        }

        if(month == ''){
            $('#misc3').append('<p>Error: Start Month is required!</p>');
            $('#Status').append('Nothing entered for start month! \n');
            window.scrollBy(0,50); 
            dateCorrect = 0;
        }
        else if( isNaN(month) ){
            $('#misc3').append('<p>Error: Invalid input for month!</p>');
            $('#Status').append('Invalid input for month! \n');
            window.scrollBy(0,50);
            $('#startDMonth').val('');
            dateCorrect = 0;
        }
        else if( month > 12 || month < 1 ){
            $('#misc3').append('<p>Error: Invalid input for month!</p>');
            $('#Status').append('Invalid value for month! \n');
            window.scrollBy(0,50);
            $('#startDMonth').val('');
            dateCorrect = 0;
        }
        else if( year == currYear && month < currMonth){
            $('#misc3').append('<p>Error: The start date cannot be smaller than the current date. Enter month again!</p>');
            $('#Status').append('Invalid date entered! \n');
            window.scrollBy(0,50);
            $('#startDMonth').val('');
            dateCorrect = 0;
        }

        if(day == ''){
            $('#misc3').append('<p>Error: No value entered for the start day!</p>');
            $('#Status').append('Nothing entered for day! \n');
            window.scrollBy(0,50); 
            dateCorrect = 0;
        }
        else if( isNaN(day) ){
            $('#misc3').append('<p>Error: Invalid input for day!</p>');
            $('#Status').append('Invalid input for day! \n');
            window.scrollBy(0,50);
            $('#startDDay').val('');
            dateCorrect = 0;
        }
        else if( day > 31 ){
            $('#misc3').append('<p>Error: Day can never be greater than 31!</p>');
            $('#Status').append('Invalid value for day! \n');
            window.scrollBy(0,50);
            $('#startDDay').val('');
            dateCorrect = 0;
        }
        else if( (month == 2 || month == 4 || month == 6 || month == 9 || month == 11) && (day > 30) ){
            $('#misc3').append('<p>Error: Invalid combination of month and day!</p>');
            $('#Status').append('Invalid value for day! \n');
            window.scrollBy(0,50); 
            $('#startDDay').val('');
            dateCorrect = 0;
        }
        else if( (month == 2 ) && (day > 29) ){
            $('#misc3').append('<p>Error: Invalid combination of month and day!</p>');
            $('#Status').append('Invalid value for day! \n');
            window.scrollBy(0,50);
            $('#startDDay').val('');
            dateCorrect = 0;
        }
        else if( (month == 2 ) && (year%4 !=0) && (day > 28) ){
            $('#misc3').append('<p>Error: Invalid combination of month and day, Make sure that the year entered is a leap year!</p>');
            $('#Status').append('Invalid value for day! \n');
            window.scrollBy(0,50);
            $('#startDDay').val('');
            dateCorrect = 0;
        }
        else if( year == currYear && month == currMonth && day < currDay){
            $('#misc3').append('<p>Error: The start date cannot be smaller than the current date. Enter month again!</p>');
            $('#Status').append('Invalid date entered! \n');
            window.scrollBy(0,50);
            $('#startDDay').val('');
            dateCorrect = 0;
        }

        if(hour == ''){
            $('#misc3').append('<p>Error: No value entered for the start time hour!</p>');
            $('#Status').append('Nothing entered for start hour! \n');
            window.scrollBy(0,50); 
            timeCorrect = 0;
        }
        else if( isNaN(hour) ){
            $('#misc3').append('<p>Error: Invalid input for start time hour!</p>');
            $('#Status').append('Invalid input for the starting hour! \n');
            window.scrollBy(0,50);
            $('#startTimeHour').val('');
            timeCorrect = 0;
        }
        else if( hour < 0 || hour > 24 ){
            $('#misc3').append('<p>Error: Invalid value for the start time hour!</p>');
            $('#Status').append('Invalid value for hour! \n');
            window.scrollBy(0,50);
            $('#startTimeHour').val('');
            timeCorrect = 0;
        }

        if(minute == ''){
            $('#misc3').append('<p>Error: No value entered for the start time minute!</p>');
            $('#Status').append('Nothing entered for the starting minute! \n');
            window.scrollBy(0,50); 
            timeCorrect = 0;
        }
        else if( isNaN(minute) ){
            $('#misc3').append('<p>Error: Invalid input for start time minute!</p>');
            $('#Status').append('Invalid input for the starting minute! \n');
            window.scrollBy(0,50);
            $('#startTimeMinute').val('');
            timeCorrect = 0;
        }
        else if( minute < 0 || minute > 59 ){
            $('#misc3').append('<p>Invalid value for the start time minute (it is between 0 and 59)!</p>');
            $('#Status').append('Invalid value for the minute! \n');
            window.scrollBy(0,50);
            $('#startTimeHour').val('');
            timeCorrect = 0;
        }

        if(dateCorrect == 1 && timeCorrect == 1 && uidOK == 1){
            var m = Number(month);
            var d = Number(day);
            var cm = Number(currMonth);
            var cd = Number(currDay);
            var h = Number(hour);
            var min = Number(minute);
            var ch = Number(currHour);
            var cmin = Number(currMin);

            if( m < 10){
                m = '0' + m;
            }
            if(d < 10){
                d = '0' + d;
            }
            if(cd < 10){
                cd = '0' + cd;
            }
            if(cm < 10){
                cm = '0' + cm;
            }
            if(h < 10){
                h = '0' + h;
            }
            if(min < 10){
                min = '0' + min;
            }
            if(ch < 10){
                ch = '0' + ch;
            }
            if(cmin < 10){
                cmin = '0' + cmin;
            }

            let startD = year+m+d;
            let startT = h+''+min+'00';
            let currD = currYear+''+cm+''+cd;
            
            if(utc == true){
                startT = startT + 'Z';
            }
            let startDT = startD+'T'+startT;
            let createDT = currD+'T'+ch+cmin+'00';
        
           $.ajax({
                type: 'get',           
                dataType: 'json',
                data: {filename : newFileName, prodid: newProdid, UID: uid, stDT : startDT, ctDT : createDT, summ : summary},	       
                url: '/addNewCal',   
                success: function (data) {
                    let flag = data.fl;
                    let writeC = data.writeCode;
                    let filename = data.fN;

                    if(flag == 1){
                        console.log('That file already exists');
                        $('#Status').append('That file already exists, enter a different file name \n');
                        $('#misc').empty();
                        $('#misc3').append('<p>A file with that name already exists, enter a different file name!</p>');
                    }
                    else{
                        //console.log('That file doesn\'t exist');
                        if(writeC == 'Calendar OK!'){
                            $('#Status').text('Calendar created successfully to the file: ');
                            $('#misc3').empty();
                            $('#misc3').append('<p>Calendar created successfully!</p>');
                            $('#Status').append(filename + '\n');
                            $('#newFileName').val('');
                            $('#prodid').val('');
                            $('#eventUID').val('');
                            $('#startDYear').val('');
                            $('#startDMonth').val('');
                            $('#startDDay').val('');
                            $('#startTimeHour').val('');
                            $('#startTimeMinute').val('');
                            $('#evtSumm').val('');
                            $('#misc3').append('<p>Calendar created successfully!</p>');
                            let obj = JSON.parse(data.ncj);
                            let fName = data.fN;
                            let row = "<tr><td><a href=\"/uploads/"+fName+"\">"+fName+"</a></td><td>"+obj.version+"</td><td>"+obj.prodID+"</td><td>"+obj.numEvents+"</td><td>"+obj.numProps+"</td></tr>";
                            $("#test").append(row);
                            $('#sel').append("<option value="+fName+">"+fName+"</option>");

                        }
                        else{
                            $('#Status').text('Couldn\'t create the file, reason: '+writeC);
                        }
                    }
                        
                   
                    
                },
                fail: function(error) {
                    console.log(error); 
                }
                
            });



        }
        var element = document.getElementById("Status");//https://stackoverflow.com/questions/18614301/keep-overflow-div-scrolled-to-bottom-unless-user-scrolls-up
        element.scrollTop = element.scrollHeight;

    });


    $('#login').click(function(e){
        e.preventDefault();
        let uName = $('#userName1').val();
        let passw = $('#userPassword1').val();
        let uDB = $('#dbName1').val();
        console.log('The db info has been entered '+uName+', *passwd*, '+uDB);
        //Pass data to the Ajax call, so it gets passed to the 
        $.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/addDB',
            data: {uN : uName,uP:passw,uD:uDB},
            success: function(data){
                
                if(data){
                    console.log('Connection successful');
                    $('#Status').append('Connection to database successful\n');
                    $('#qStat').val('Login successful! User logged in');
                    $('#userName1').val('');
                    $('#userPassword1').val('');
                    $('#dbName1').val('');
                    databaseFunc();
                }
                else{
                    console.log('Connection failed');
                    $('#Status').append('Connection to database failed\n');
                    $('#qStat').val('Login Failed, try again');
                }
                
            },
            fail: function(error){
                console.log(error);
            }
        });
    });


    function databaseFunc(){
        $.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/getFiles',
            data: {},
            success: function(data){
                var validFiles = 0;

                for(var i =0; i < data.arr.length; i++){
                    if(data.arr[i] != '{}'){
                        validFiles = validFiles + 1;
                    }
                }

                if(validFiles > 0){
                    document.getElementById('store_files').disabled = false;
                    document.getElementById('clear_tables').disabled = false;
                    document.getElementById('disp_dbstatus').disabled = false;
                    document.getElementById('sortedEvts').disabled = false;
                    document.getElementById('fileEvts').disabled = false;
                    document.getElementById('conflEvts').disabled = false;
                    document.getElementById('cq1').disabled = false;
                    document.getElementById('cq2').disabled = false;
                    document.getElementById('cq3').disabled = false;
                    $('#Status').append(validFiles + ' files available to load onto the database\n');
                    var element = document.getElementById("Status");//https://stackoverflow.com/questions/18614301/keep-overflow-div-scrolled-to-bottom-unless-user-scrolls-up
                    element.scrollTop = element.scrollHeight;
                }else{
                    $('#Status').append('No valid files available to load onto the database\n');
                    var element = document.getElementById("Status");//https://stackoverflow.com/questions/18614301/keep-overflow-div-scrolled-to-bottom-unless-user-scrolls-up
                    element.scrollTop = element.scrollHeight;
                }

            },
            fail: function(error){
                console.log(error);
            }
        });
    }


    $('#store_files').click(function(e){
        e.preventDefault();
        console.log('Storing all files');
        $.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/storeFiles',
            data: {},
            success: function(data){
                console.log('Stored all files');
                document.getElementById('store_files').disabled = true;
                document.getElementById('sortedEvts').disabled = false;
                document.getElementById('fileEvts').disabled = false;
                document.getElementById('conflEvts').disabled = false;
                document.getElementById('cq1').disabled = false;
                document.getElementById('cq2').disabled = false;
                document.getElementById('cq3').disabled = false;
                //document.getElementById('disp_dbstatus').disabled = false;
                $('#qStat').val('All files stored, you can now perform queries');
            },
            fail: function(error){
                console.log(error);
            }
        });
    });

    $('#clear_tables').click(function(e){
        e.preventDefault();
        document.getElementById('store_files').disabled = false;
        document.getElementById('sortedEvts').disabled = true;
        document.getElementById('fileEvts').disabled = true;
        document.getElementById('conflEvts').disabled = true;
        document.getElementById('cq1').disabled = true;
        document.getElementById('cq2').disabled = true;
        document.getElementById('cq3').disabled = true;
        $('#Query').empty();

        $.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/clearTables',
            data: {},
            success: function(data){
                $('#qStat').val('All data cleared from the database');
            },
            fail: function(error){
                console.log(error);
            }
        });
    });


    $('#sortedEvts').click(function(e){
        e.preventDefault();
        
        $.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/sortedEvents',
            data: {},
            success: function(data){
                
                if(data){
                    if(data.arr.length > 0){
                    $('#qStat').val('Showing all events sorted by the start date in the panel below');
                    $('#Query').empty();
                    $('#Query').append("<table class=\"table table-striped header-fixed;\" id=\"sortEvtTable\">\
                    <tr>\
                        <th > Event_id  </th>\
                        <th>  Start Time  </th>\
                        <th>  Summary  </th>\
                        <th>  Location </th>\
                        <th>  Organizer </th>\
                    </tr>\
            </table>"    );

                    for(var i=0; i < data.arr.length; i++){
                        let abc = data.arr[i];
                        var a = JSON.parse(abc);
                        let row = "<tr><td>"+a.event_id+"</td><td>"+a.start_time+"</td><td>"+a.summary+"</td><td>"+a.location+"</td><td>"+a.organizer+"</td></tr>";
                        //console.log(data.arr[i]);
                        //$('#Query').append('............................................\n'+data.arr[i]+'\n............................................\n');
                        $('#sortEvtTable').append(row);
                    }
                }else{
                    $('#qStat').val('Showing all events sorted by start date in the panel below');
                    $('#Query').empty();
                    $('#Query').append('<p>No events to show on the database, store some files on the database</p>');
                }
                }//for if(data)
                else{
                    $('#qStat').val('Showing all events sorted by start date in the panel below');
                    $('#Query').empty();
                    $('#Query').append('<p>No events to show on the database, store some files on the database</p>');
                }
            },
            fail: function(error){
                console.log(error);
            }
        });
    });


    $('#fileEvts').click(function(e){
        e.preventDefault();
        let file = $('#formForQ').val();
        if(file == ''){
            $('#Status').append('No Calendar name entered to view events\n');
            $('#Query').empty();
            $('#Query').append('<p>Please type in a valid file name to view events\n</p>');
            $('#qStat').val('First type in a valid Calendar file name!');
        }else{
            $('#qStat').val('Showing events from the selected Calendar in the panel below');
        $.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/fileEvents',
            data: {fileName : file},
            success: function(data){
                
                if(data){
                    if(data.arr.length > 0){
                    $('#qStat').val('Showing all events from the calendar chosen in the panel below');
                    $('#Query').empty();
                    $('#Query').append("<table class=\"table table-striped header-fixed;\" id=\"sortEvtTable\">\
                    <tr>\
                        <th > Event_id  </th>\
                        <th>  Start Time  </th>\
                        <th>  Summary  </th>\
                        <th>  Location </th>\
                        <th>  Organizer </th>\
                    </tr>\
            </table>"    );

                    for(var i=0; i < data.arr.length; i++){
                        let abc = data.arr[i];
                        var a = JSON.parse(abc);
                        let row = "<tr><td>"+a.event_id+"</td><td>"+a.start_time+"</td><td>"+a.summary+"</td><td>"+a.location+"</td><td>"+a.organizer+"</td></tr>";
                        //console.log(data.arr[i]);
                        //$('#Query').append('............................................\n'+data.arr[i]+'\n............................................\n');
                        $('#sortEvtTable').append(row);
                    }
                }else{
                    $('#qStat').val('Showing all events from the chosen Calendar in the panel below');
                    $('#Query').empty();
                    $('#Query').append('<p>That file does not exist on the database</p>');
                }
                }else{
                    $('#qStat').val('Showing all events from the chosen Calendar in the panel below');
                    $('#Query').empty();
                    $('#Query').append('<p>That file does not exist on the database</p>');
                }

            },
            fail: function(error){
                console.log(error);
            }
        });
    }
    });


    $('#disp_dbstatus').click(function(e){
        $('#qStat').val('Showing Database status in the panel below');
        $.ajax({//count files
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/dbStatus',
            data: {},
            success: function(data){
                if(data){
                    $('#Query').empty();
                    $('#Query').append('<p>Database contains '+data.nF+' files\n</p>');
                    
                }
            },
            fail: function(error){
                console.log(error);
            }
        });



        $.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/numEvts',
            data: {},
            success: function(data){
                if(data){

                    $('#Query').append('<p>Database contains '+data.nE+' events\n</p>');
                    
                }
            },
            fail: function(error){
                console.log(error);
            }
        });



        $.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/numAlm',
            data: {},
            success: function(data){
                if(data){
                    $('#Query').append('<p>Database contains '+data.nA+' alarms\n</p>');
                    
                }
            },
            fail: function(error){
                console.log(error);
            }
        });


    });

    
    $('#cq1').click(function(e){
            
            $('#qStat').val('Showing all calendars and number of events in the panel below');

        $.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/calAndEvents',
            data: {},
            success: function(data){
                
                if(data){
                    if(data.arr.length > 0){
                    $('#qStat').val('Showing all events from the calendar chosen in the panel below');
                    $('#Query').empty();
                    $('#Query').append("<table class=\"table table-striped header-fixed;\" id=\"sortEvtTable\">\
                    <tr>\
                        <th > Cal_id  </th>\
                        <th>  File Name  </th>\
                        <th>  Number of Events  </th>\
                    </tr>\
            </table>"    );

                    for(var i=0; i < data.arr.length; i++){
                        let abc = data.arr[i];
                        var a = JSON.parse(abc);
                        let row = "<tr><td>"+a.cal_id+"</td><td>"+a.file_Name+"</td><td>"+a.numEvents+"</td></tr>";
                        //console.log(row);
                        $('#sortEvtTable').append(row);
                    }
                }else{
                    $('#qStat').val('Listing files on the database in the panel below');
                    $('#Query').empty();
                    $('#Query').append('<p>There is no file on the database</p>');
                }
                }else{
                    $('#qStat').val('Listing files on the database in the panel below');
                    $('#Query').empty();
                    $('#Query').append('<p>There is no file on the database</p>');
                }

            },
            fail: function(error){
                console.log(error);
            }
        });
    
    });//end of cq1


    $('#conflEvts').click(function(e){
        e.preventDefault();
        
        $.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/confEvents',
            data: {},
            success: function(data){
                
                if(data){
                    if(data.arr.length > 0){
                    $('#qStat').val('Showing all events with conflicting start times in the panel below');
                    $('#Query').empty();
                    $('#Query').append("<table class=\"table table-striped header-fixed;\" id=\"conflEvtTable\">\
                    <tr>\
                        <th > Event_id 1 </th>\
                        <th>  Start Time </th>\
                        <th>  Summary 1 </th>\
                        <th>  Organizer 1 </th>\
                        <th > Event_id 2 </th>\
                        <th>  Summary 2 </th>\
                        <th>  Organizer 2 </th>\
                    </tr>\
            </table>"    );

                    for(var i=0; i < data.arr.length; i++){
                        let abc = data.arr[i];
                        var a = JSON.parse(abc);
                        let row = "<tr><td>"+a.id1+"</td><td>"+a.st1+"</td><td>"+a.sm1+"</td><td>"+a.org1+"</td><td>"+a.id2+"</td><td>"+a.sm2+"</td><td>"+a.org2+"</td></tr>";
                        //console.log(data.arr[i]);
                        //$('#Query').append('............................................\n'+data.arr[i]+'\n............................................\n');
                        $('#conflEvtTable').append(row);
                    }
                }else{
                    $('#qStat').val('Showing all events with conflicting start times the panel below');
                    $('#Query').empty();
                    $('#Query').append('<p>No conflicting events found</p>');
                }
                }else{
                    $('#qStat').val('Showing all events with conflicting start times the panel below');
                    $('#Query').empty();
                    $('#Query').append('<p>No conflicting events found</p>');
                }

            },
            fail: function(error){
                console.log(error);
            }
        });
    
    });//end of conflEvts


    $('#cq2').click(function(e){
        e.preventDefault();
        let file = $('#formForQ').val();
        if(file == ''){
            $('#Status').append('No Calendar name entered to view alarms\n');
            $('#Query').empty();
            $('#Query').append('<p>Please type in a valid file name to view alarms\n</p>');
            $('#qStat').val('First input a valid calendar name in for the query!');
        }else{
            $('#qStat').val('Showing alarms from the selected Calendar in the panel below');
        $.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/fileAlms',
            data: {fileName : file},
            success: function(data){
                
                if(data){
                    if(data.arr.length > 0){
                    $('#qStat').val('Showing all events from the calendar chosen in the panel below');
                    $('#Query').empty();
                    $('#Query').append("<table class=\"table table-striped header-fixed;\" id=\"fileAlmTable\">\
                    <tr>\
                        <th > File  </th>\
                        <th>  Alarm_id  </th>\
                        <th>  Action </th>\
                        <th>  Trigger </th>\
                    </tr>\
            </table>"    );

                    for(var i=0; i < data.arr.length; i++){
                        let abc = data.arr[i];
                        var a = JSON.parse(abc);
                        let row = "<tr><td>"+a.file+"</td><td>"+a.alarm_id+"</td><td>"+a.action+"</td><td>"+a.trigger+"</td></tr>";
                        
                        //$('#Query').append('............................................\n'+data.arr[i]+'\n............................................\n');
                        $('#fileAlmTable').append(row);
                    }
                }else{
                    $('#qStat').val('Showing all alarms from the chosen Calendar in the panel below');
                    $('#Query').empty();
                    $('#Query').append('<p>That file does not exist on the database</p>');
                }
                }else{
                    $('#qStat').val('Showing all alarms from the chosen Calendar in the panel below');
                    $('#Query').empty();
                    $('#Query').append('<p>That file does not exist on the database</p>');
                }

            },
            fail: function(error){
                console.log(error);
            }
        });
    }
    });

    $('#cq3').click(function(e){
        e.preventDefault();
        
        $.ajax({
            type: 'get',            //Request type
            dataType: 'json',       //Data type - we will use JSON for almost everything 
            url: '/sortedAlarms',
            data: {},
            success: function(data){
                
                if(data){
                    if(data.arr.length > 0){
                    $('#qStat').val('Showing all alarms sorted by their events\' start date in the panel below');
                    $('#Query').empty();
                    $('#Query').append("<table class=\"table table-striped header-fixed;\" id=\"sortAlmTable\">\
                    <tr>\
                        <th > Alarm ID  </th>\
                        <th>  Action  </th>\
                        <th>  Trigger  </th>\
                        <th>  Event ID </th>\
                        <th>  Start Time </th>\
                        <th>  Summary </th>\
                    </tr>\
            </table>"    );

                    for(var i=0; i < data.arr.length; i++){
                        let abc = data.arr[i];
                        var a = JSON.parse(abc);
                        let row = "<tr><td>"+a.alarm_id+"</td><td>"+a.action+"</td><td>"+a.trigger+"</td><td>"+a.event_id+"</td><td>"+a.start_time+"</td><td>"+a.summary+"</td></tr>";
                        //console.log(data.arr[i]);
                        //$('#Query').append('............................................\n'+data.arr[i]+'\n............................................\n');
                        $('#sortAlmTable').append(row);
                    }
                }else{
                    $('#qStat').val('Showing all alarms sorted by their events\' start date in the panel below');
                    $('#Query').empty();
                    $('#Query').append('<p>No alarms to show on the database, store some files on the database</p>');
                }
                }//for if(data)
                else{
                    $('#qStat').val('Showing all alarms sorted by their events\' start date in the panel below');
                    $('#Query').empty();
                    $('#Query').append('<p>No alarms to show on the database, store some files on the database</p>');
                }
            },
            fail: function(error){
                console.log(error);
            }
        });
    });








    // On page-load AJAX Example
    $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/someendpoint',   //The server endpoint we are connecting to
        success: function (data) {
            /*  Do something with returned object
                Note that what we get is an object, not a string, 
                so we do not need to parse it on the server.
                JavaScript really does handle JSONs seamlessly
            */
            $('#blah').html("On page load, Received string '"+JSON.stringify(data)+"' from server");
            //We write the object to the console to show that the request was successful
            console.log(data); 

        },
        fail: function(error) {
            // Non-200 return, do something with error
            console.log(error); 
        }
    });

    // Event listener form replacement example, building a Single-Page-App, no redirects if possible
    $('#someform').submit(function(e){
        $('#blah').html("Callback from the form");
        e.preventDefault();
        //Pass data to the Ajax call, so it gets passed to the 
        $.ajax({});
    });

    
   
});
        