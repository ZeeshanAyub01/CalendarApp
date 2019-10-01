'use strict'

// C library API
const ffi = require('ffi');

// Express App (Routes)
const express = require("express");
const app     = express();
const path    = require("path");
const fileUpload = require('express-fileupload');

// Global variable for mysql
const mysql = require('mysql');
let connection;
//additional global variables to use
var numFl = 0;
var numEv = 0;
var numAl = 0;

app.use(fileUpload());

// Minimization
const fs = require('fs');
const JavaScriptObfuscator = require('javascript-obfuscator');

// Important, pass in port as in `npm run dev 1234`, do not change
const portNum = process.argv[2];

// Send HTML at root, do not change
app.get('/',function(req,res){
  res.sendFile(path.join(__dirname+'/public/index.html'));
});

// Send Style, do not change
app.get('/style.css',function(req,res){
  //Feel free to change the contents of style.css to prettify your Web app
  res.sendFile(path.join(__dirname+'/public/style.css'));
});

// Send obfuscated JS, do not change
app.get('/index.js',function(req,res){
  fs.readFile(path.join(__dirname+'/public/index.js'), 'utf8', function(err, contents) {
    const minimizedContents = JavaScriptObfuscator.obfuscate(contents, {compact: true, controlFlowFlattening: true});
    res.contentType('application/javascript');
    res.send(minimizedContents._obfuscatedCode);
  });
});

//Respond to POST requests that upload files to uploads/ directory
app.post('/upload', function(req, res) {
  if(!req.files) {
    return res.status(400).send('No files were uploaded.');
  }
 
  var fileOk = 1;
  let uploadFile = req.files.uploadFile;
  

  

  // Use the mv() method to place the file somewhere on your server

  if(fileOk==1){//to check so only valid files are uploaded

  uploadFile.mv('uploads/' + uploadFile.name, function(err) {
    if(err) {
      return res.status(500).send(err);
    }

    res.redirect('/');
  });
}//closing the if-statement

});

//Respond to GET requests for files in the uploads/ directory
app.get('/uploads/:name', function(req , res){
  fs.stat('uploads/' + req.params.name, function(err, stat) {
    console.log(err);
    if(err == null) {
      res.sendFile(path.join(__dirname+'/uploads/' + req.params.name));
    } else {
      res.send('');
    }
  });
});

//******************** Your code goes here ******************** 

//Sample endpoint

let sharedLib = ffi.Library('./libcal',{
  'calFileToJSON' : ['string',['string']],
  'eventsToJSON' : ['string',['string']],
  'alarmsToJSON' : ['string',['string','int']],
  'fetchOptionalProperties' : ['string',['string','int']],
  'addEventDynamic' : ['string',['string','string','string','string','string']],
  'addCalendarDynamic' : ['string',['string','string','string','string','string','string']],
  'eventsToJSONAlt' : ['string',['string']]
});


app.get('/someendpoint', function(req , res){
  res.send({
    foo: "bar"
  });
});

app.get('/viewCalJSON', function(req , res){
  let files = fs.readdirSync('./uploads');
  let array = [];
  
  for(var i=0 ; i < files.length; i++){
    array[i] = sharedLib.calFileToJSON(files[i]);
    //console.log(files[i]);
  }

  res.send({

    arr : array,
    fileNames : files});

});

app.get('/viewEventList', function(req , res){
  let file = req.query.filename;
  let events = sharedLib.eventsToJSON(file);
  
  //console.log(events);

  res.send({
    ev : events});

});

app.get('/viewAlarmList', function(req , res){
  let file = req.query.filename;
  let index = req.query.index;
  let alarms = sharedLib.alarmsToJSON(file,index);
  
  //console.log(alarms);
  //console.log(file);
  //console.log(index);

  res.send({
    alm : alarms});

});

app.get('/viewEvtOptProp', function(req , res){
  let file = req.query.filename;
  let index = req.query.index;
  let properties = sharedLib.fetchOptionalProperties(file,index);
  
  //console.log(properties);
  //console.log(file);
  //console.log(index);

  res.send({
    prop : properties});

});


app.get('/addEvent', function(req , res){
  let file = req.query.filename;
  
  //console.log(file);

  res.send({
    });
});


app.get('/addEventToFile', function(req , res){
  let file = req.query.filename;
  let evtUID = req.query.UID;
  let startdt = req.query.stDT;
  let createdt = req.query.ctDT;
  let summary = req.query.summ;
  
  let writeCode = sharedLib.addEventDynamic(file,evtUID,startdt,createdt,summary);
  //console.log(writeCode);

  res.send({
    fN : file,
    wC : writeCode
    });
});


app.get('/addNewCal', function(req , res){
  let file = req.query.filename;
  let prodID = req.query.prodid;
  let evtUID = req.query.UID;
  let startdt = req.query.stDT;
  let createdt = req.query.ctDT;
  let summary = req.query.summ;
  let exisFiles = fs.readdirSync('./uploads');
  var flag = 0;
  var wC = '';
  let array = [];
  var newCalJSON = '';
  
  for(var i=0; i < exisFiles.length; i++){
    if(exisFiles[i] == file){
      flag = 1;
    }
  }

    if(flag == 1){
      //console.log('That file already exists');
    }
    else{
      //console.log('That file doesn\'t exist');
      wC = sharedLib.addCalendarDynamic(file,prodID,evtUID,startdt,createdt,summary);
      newCalJSON = sharedLib.calFileToJSON(file);
      
    }
  //let writeCode = sharedLib.addEventDynamic(file,evtUID,startdt,createdt,summary);
  //console.log(writeCode);

  res.send({
    ncj: newCalJSON,
    arr : array,
    ef : exisFiles,
    fl : flag,
    writeCode : wC,
    fN : file
    });
});

app.get('/addDB', function(req , res){
  let uName = req.query.uN;
  let uPassword = req.query.uP;
  let uDatabase = req.query.uD;
  var connectFlag = false;

  console.log('User: '+uName+' Database: '+uDatabase);
  connection = mysql.createConnection({
    host  : 'dursley.socs.uoguelph.ca',
    user  : uName,
    password : uPassword,
    database  : uDatabase,  
  });

  connection.connect(function(error){
    if(error){
      connectFlag = false;
      console.log('Connection to '+uDatabase+' failed! Error: '+error+'\n');
      res.send(connectFlag);
    }
    else{
      connectFlag = true;
      console.log('Connection to '+uDatabase+' successful!\n');
      res.send(connectFlag);
      createAllTables();
    }
  });

  
  //connection.end();

});

function createAllTables(){

  var ftc = 0;
  var etc = 0;
  var atc = 0;

  var fileTableCreation = "CREATE TABLE IF NOT EXISTS FILE ( \
    cal_id INT AUTO_INCREMENT PRIMARY KEY,\
    file_Name VARCHAR(60) NOT NULL,\
    version INT NOT NULL,\
    prod_id VARCHAR(256) NOT NULL);";

    var eventTableCreation = "CREATE TABLE IF NOT EXISTS EVENT ( \
      event_id INT AUTO_INCREMENT PRIMARY KEY,\
      summary VARCHAR(1024),\
      start_time DATETIME NOT NULL,\
      location VARCHAR(60),\
      organizer VARCHAR(256),\
      cal_file INT NOT NULL,\
      FOREIGN KEY(cal_file) REFERENCES FILE(cal_id) ON DELETE CASCADE);";

      var alarmTableCreation = "CREATE TABLE IF NOT EXISTS ALARM ( \
        alarm_id INT AUTO_INCREMENT PRIMARY KEY,\
        action VARCHAR(256) NOT NULL,\
        `trigger` VARCHAR(256) NOT NULL,\
        event INT NOT NULL,\
        FOREIGN KEY(event) REFERENCES EVENT(event_id) ON DELETE CASCADE);";

        connection.query(fileTableCreation,function(err,rows,fields){
          if(err){
            console.log('Error encountered while creating the file table: '+err);
          }
          else{
            console.log('File table created, or it already exists');
            ftc = 1;
          }
        });

        connection.query(eventTableCreation,function(err,rows,fields){
          if(err){
            console.log('Error encountered while creating the event table: '+err);
          }
          else{
            console.log('Event table created, or it already exists');
            etc = 1;
          }
        });

        connection.query(alarmTableCreation,function(err,rows,fields){
          if(err){
            console.log('Error encountered while creating the alarm table: '+err);
          }
          else{
            console.log('Alarm table created, or it already exists');
            atc = 1;
          }
        });
        
        
        //console.log(fileTableCreation);
        //console.log(eventTableCreation);
        //console.log(alarmTableCreation);
  
}


app.get('/getFiles', function(req , res){
  let files = fs.readdirSync('./uploads');
  let array = [];
  
  for(var i=0 ; i < files.length; i++){
    array[i] = sharedLib.calFileToJSON(files[i]);
  }

  res.send({

    arr : array,
    fileNames : files});

});


app.get('/storeFiles', function(req , res){
  let files = fs.readdirSync('./uploads');
  let array = [];
  var fileQuery = '';
  var calID = 0;

  clearEverything();
  connection.query("ALTER TABLE FILE AUTO_INCREMENT = 1",function(err,rows,fields){});

  for(var i=0 ; i < files.length; i++){
    array[i] = sharedLib.calFileToJSON(files[i]);
    if(array[i] != '{}'){
      numFl = Number(numFl) + 1;
      let obj = JSON.parse(array[i]);
      fileQuery = "INSERT INTO FILE (file_Name, version, prod_id) VALUES ('"+files[i]+"','"+obj.version+"','"+obj.prodID+"')";
      //console.log(fileQuery);
      connection.query(fileQuery,function(err,rows,fields){
        if(err){
          console.log('Error encountered while adding to the file table: '+err);
        }
        else{
          //console.log('Data added successfully to the FILE table');
          
        }
      });
      addEvents(files[i]);
     

      
      }
  }


  res.send({

    arr : array,
    fileNames : files});

});


app.get('/clearTables', function(req , res){
  
  clearEverything();

  res.send({
  });

});


app.get('/sortedEvents', function(req , res){
  
  connection.query("SELECT * FROM EVENT ORDER BY start_time",function(err,rows,fields){

    let array = [];
    var i = 0;
    if(err){
      console.log('Error fetching ordered events: '+err);
    }else{
      
      for(let row of rows){
        
        let r = '{"event_id" : "'+row.event_id+'","start_time" : "'+row.start_time+'","summary" : "'+row.summary+'","location" : "'+row.location+'","organizer" : "'+row.organizer+'"}';
        array[i] = r;
        //console.log(r);
        //array[i] = 'EVENT ID: '+row.event_id+', START TIME: '+row.start_time+', SUMMARY: '+row.summary+', LOCATION: '+row.location+', ORGANIZER: '+row.organizer;
        //console.log('Event ID: '+row.event_id+' Summary: '+row.summary+' Start time: '+row.start_time+' Location: '+row.location+' Organizer: '+row.organizer);
        i = Number(i) + 1;
      }
      
    }
    res.send({arr : array});
  });


});


app.get('/fileEvents', function(req , res){
  
  let file = req.query.fileName;
  var qq = "SELECT * FROM FILE INNER JOIN EVENT ON FILE.cal_id = EVENT.cal_file WHERE file_Name = '"+file+"';";
  //console.log(qq);
  connection.query(qq,function(err,rows,fields){
    let array = [];
    var i = 0;

    if(err){
      console.log('Error getting events from a file on database: '+err);
    }else{
      
      for(let row of rows){
        
        //array[i] = 'Event ID: '+row.event_id+' Summary: '+row.summary+' Start time: '+row.start_time+' Location: '+row.location+' Organizer: '+row.organizer;
        let r = '{"event_id" : "'+row.event_id+'","start_time" : "'+row.start_time+'","summary" : "'+row.summary+'","location" : "'+row.location+'","organizer" : "'+row.organizer+'"}';
        array[i] = r;
        //console.log(r);
        //console.log('Event ID: '+row.event_id+' Summary: '+row.summary+' Start time: '+row.start_time+' Location: '+row.location+' Organizer: '+row.organizer);
        i = Number(i) + 1;
      }
      
    
    }
    res.send({arr : array});
  });

});


function addEvents(file){
  var calIdQ = "SELECT cal_id FROM FILE WHERE file_Name = '"+file+"';";
  var calAlmQ = "SELECT event_id,cal_id,file_Name FROM FILE INNER JOIN EVENT ON FILE.cal_id = EVENT.cal_file WHERE file_Name = '"+file+"';";
  var calID = 0;
  

  
  connection.query(calIdQ,function(err,rows,fields){
    if(err){
      console.log('something went wrong while fetching the cal_id for '+file+': '+err);
    }else{

      
      connection.query("ALTER TABLE EVENT AUTO_INCREMENT = 1",function(err,rows,fields){});
      connection.query("ALTER TABLE ALARM AUTO_INCREMENT = 1",function(err,rows,fields){});

      for(let row of rows){//Only one row
        calID = row.cal_id;
        
        let eventList2 = sharedLib.eventsToJSONAlt(file);
        //console.log(eventList2);
        let evts2 = JSON.parse(eventList2);
        let eventList = sharedLib.eventsToJSON(file);
        let events = JSON.parse(eventList);
        var oP;
        var summary = null;
        var organizer = null;
        var location = null;

        for(var i=0; i < events.length; i++){
          oP = '';
          oP = sharedLib.fetchOptionalProperties(file,i+1);//fetches optional properties for a specific event from the file
          //oP = evts2.properties;
          let stdt = events[i].startDT.date;
          let sttm = events[i].startDT.time;
          var evtstdt = stdt[0]+stdt[1]+stdt[2]+stdt[3]+'-'+stdt[4]+stdt[5]+'-'+stdt[6]+stdt[7];
          var evtsttm = sttm[0]+sttm[1]+':'+sttm[2]+sttm[3]+':'+sttm[4]+sttm[5];
          var evtStartDT = evtstdt+' '+evtsttm;
          var evtAddQuery= '';

          summary = '';
          organizer = '';
          location = '';

          if(oP != '[]'){
            //console.log(oP);
            var optProp = JSON.parse(oP);
            for(var j = 0; j < optProp.length; j++){
              if(optProp[j].propName == 'SUMMARY'){
                summary = optProp[j].propDescr;
                summary = fixSingleQuotes(summary);
              }
              if(optProp[j].propName == 'LOCATION'){
                location = optProp[j].propDescr;
              }
              if(optProp[j].propName == 'ORGANIZER'){
                organizer = optProp[j].propDescr;
              }
            }
            
            
          }//end of if for oP
          //console.log(events[i].startDT);
          //console.log('Cal_id: '+calID+' Evt No: '+ Number(i+1) +' start DT: '+evtStartDT+' Summary: '+summary+' Location: '+location+' Org: '+organizer);
          evtAddQuery = "INSERT INTO EVENT (summary, start_time,location,organizer,cal_file) VALUES ('"+summary+"','"+evtStartDT+"','"+location+"','"+organizer+"',"+calID+")";
          numEv = Number(numEv) + 1;
          connection.query(evtAddQuery,function(err,rows,fields){
            if(err){
              console.log('Something went wrong while adding events: '+err);
           }else{
                //console.log('entered something to EVENT');
            }
            
          });
        }//end of for all events
        
        //add alarms could potentially be added here
        connection.query(calAlmQ,function(err,rows,fields){
          if(err){
            console.log('Error adding alarms: '+err);
          }else{
            var i = 0;
            var cal = 0;
            var evtFile;
            var fl = '';
            var evts = [];
            var alm;
            for(let row of rows){
              alm = '';
              if(row.cal_id != cal){
                i = 0;
                evtFile = sharedLib.eventsToJSONAlt(row.file_Name);
                evts = JSON.parse(evtFile);
              }
              
              if(evts[i] != undefined){
              cal = row.cal_id;
              i = Number(i) + 1;
              alm = evts[Number(i)-1].alarms;
              
              //let alarm = JSON.parse(alm);
              //console.log('i: '+i+' Evt id: '+row.event_id+' cal iD: '+row.cal_id+' file: '+row.file_Name+' Alarms: '+alm+' No. alms: '+alm.length);
              if(alm.length > 0){
                for(var j=0; j < alm.length; j++){
                  var almAddQ = "INSERT INTO ALARM (action, `trigger`,event) VALUES ('"+alm[j].action+"','"+alm[j].trigger+"',"+row.event_id+")";
                  numAl = Number(numAl) + 1;
                  //console.log('Evt id: '+row.event_id+' Alarm: '+alm[j].trigger);
                  //console.log(almAddQ);
                  connection.query(almAddQ,function(err,rows,fields){
                    if(err){
                      console.log('Error adding to the ALARM table: '+err);
                    }else{
                      //console.log('Added successfully to the ALARM table ');
                    }
                  });

                }
              }
              }
            }
          }
        });

        
      }//end of for the rows (there will always be only one row because one file)
    }
    
  });
  //console.log('from the first: '+calID);
  
}


function clearEverything(){

  var fileTableClear = "DELETE FROM FILE";
  var eventTableClear = "DELETE FROM EVENT";
  var alarmTableClear = "DELETE FROM ALARM";


  connection.query(fileTableClear,function(err,rows,fields){
    if(err){
      console.log('Error encountered while clearing the FILE table: '+err);
    }
    else{
      console.log('Data removed from FILE table');
    }
  });

  connection.query(eventTableClear,function(err,rows,fields){
    if(err){
      console.log('Error encountered while clearing the EVENT table: '+err);
    }
    else{
      console.log('Data removed from EVENT table');
    }
  });

  connection.query(alarmTableClear,function(err,rows,fields){
    if(err){
      console.log('Error encountered while clearing the ALARM table: '+err);
    }
    else{
      console.log('Data removed from ALARM table');
    }
  });


}

function fixSingleQuotes(string){
  var newString = string;

  for(var k=0; k < string.length; k++){
    if(string[k] == '\''){
      //string.splice(k,0,"\'");
      newString = string.slice(0,k) + '\'' + string.slice(k,string.length);
    }
  }
  return newString;
}

app.get('/dbStatus', function(req , res){

  connection.query("SELECT COUNT(*) AS CF FROM FILE",function(err,rows,fields){
    if(err){
      console.log('Error counting FILES: '+err);
    }else{
      var numFiles;
      for(let row of rows){
        numFiles = row.CF;
      }
      
      res.send({
        nF : numFiles
      });
    }
  });

});

app.get('/numEvts', function(req , res){
  
  connection.query("SELECT COUNT(*) AS NE FROM EVENT",function(err,rows,fields){
    if(err){
      console.log('Error counting Events: '+err);
    }else{
      var numEvents;
        for(let row of rows){
          numEvents = row.NE;
        }
      console.log(numEvents);

      res.send({
        nE : numEvents
      });
    }
  });

});

app.get('/numAlm', function(req , res){
  
  connection.query("SELECT COUNT(*) AS NA FROM ALARM",function(err,rows,fields){
    if(err){
      console.log('Error counting ALARMS: '+err);
    }else{
      var numAlarms = 0;
      for(let row of rows){
        numAlarms = row.NA;
      }
      console.log(numAlarms);
      res.send({
        nA : numAlarms
      });
    }
  });

});

app.get('/calAndEvents', function(req , res){
  
  var q = "SELECT DISTINCT cal_id,file_Name,count(event_id) AS EI FROM (SELECT cal_id,event_id,file_Name FROM FILE INNER JOIN EVENT ON FILE.cal_id = EVENT.cal_file) AS EF group by file_Name ORDER BY cal_id;";

  connection.query(q,function(err,rows,fields){
    let array = [];
  var i = 0;

    if(err){
      console.log('Error while counting events for each file: '+err);
    }else{
      
      for(let row of rows){
        
        let r = '{"cal_id" : "'+row.cal_id+'","file_Name" : "'+row.file_Name+'","numEvents" : "'+row.EI+'"}';
        
        array[i] = r;
        i = Number(i) + 1;
      }
      res.send({
        arr : array
      });

    }
  })
  

});


app.get('/confEvents', function(req , res){
  
  var q = "SELECT DISTINCT e1.event_id AS id1,e1.start_time AS st1,e1.summary as sm1,e1.organizer as org1,e2.event_id as id2,e2.start_time as st2,e2.summary as sm2,e2.organizer as org2 FROM EVENT e1 JOIN EVENT\
  e2 WHERE e1.start_time = e2.start_time AND e1.event_id != e2.event_id;";

  connection.query(q,function(err,rows,fields){
    let array = [];
    var i = 0;

    if(err){
      console.log('Error while counting events for each file: '+err);
    }else{
      
      for(let row of rows){
        
        let r = '{"id1" : "'+row.id1+'","st1" : "'+row.st1+'","sm1" : "'+row.sm1+'","org1" : "'+row.org1+'","id2" : "'+row.id2+'","st2" : "'+row.st2+'","sm2" : "'+row.sm2+'","org2" : "'+row.org2+'"}';
        
        array[i] = r;
        i = Number(i) + 1;
      }
      res.send({
        arr : array
      });

    }
  })
  

});


app.get('/fileAlms', function(req , res){
  
  let file = req.query.fileName;
  var qq = "SELECT alarm_id,action,`trigger` FROM ALARM INNER JOIN EVENT ON ALARM.event = EVENT.event_id WHERE EVENT.cal_file = (SELECT cal_id FROM FILE\
    WHERE file_Name = '"+file+"');";

  connection.query(qq,function(err,rows,fields){
    let array = [];
    var i = 0;

    if(err){
      console.log('Error getting events from a file on database: '+err);
    }else{
      
      for(let row of rows){
        
        let r = '{"file" : "'+file+'","alarm_id" : '+row.alarm_id+',"action" : "'+row.action+'","trigger" : "'+row.trigger+'"}';
        array[i] = r;
        
        i = Number(i) + 1;
      }
      
    
    }
    res.send({arr : array});
  });

});


app.get('/sortedAlarms', function(req , res){
  
  connection.query("SELECT alarm_id,action,`trigger`,event_id,start_time,summary FROM ALARM INNER JOIN EVENT ON ALARM.event = EVENT.event_id ORDER BY  start_time;",function(err,rows,fields){

    let array = [];
    var i = 0;
    if(err){
      console.log('Error fetching ordered events: '+err);
    }else{
      
      for(let row of rows){
        
        let r = '{"alarm_id" : "'+row.alarm_id+'","action" : "'+row.action+'","trigger" : "'+row.trigger+'","event_id" : "'+row.event_id+'","start_time" : "'+row.start_time+'","summary" : "'+row.summary+'"}';
        array[i] = r;
        i = Number(i) + 1;
      }
      
    }
    res.send({arr : array});
  });


});


app.listen(portNum);
console.log('Running app at localhost: ' + portNum);