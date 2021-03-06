var express = require("express");
var app = express();
var bodyParser = require('body-parser');
var errorHandler = require('errorhandler');
var methodOverride = require('method-override');
var hostname = process.env.HOSTNAME || 'localhost';
var port = 8080;


var MS = require('mongoskin');
var db = MS.db('mongodb://ame:394@127.0.0.1:27017/ame394');
db.collection('data').findOne({b:2}, function(err, result) {
  console.log(result);
});
db.collection('data').find().toArray(function(err, result){
  console.log(result);
});

var light;
var temp;
var humidity;

app.get("/update", function (req, res) {
    console.log("GET req arrived")
    var info = req.query;
    info.time = new Date().getTime();

    db.collection('data').insert(info, function(err, result) {
      console.log(result);
    });


    light = req.query.light || light;
    temp = req.query.temp || temp;
    humidity = req.query.humidity || humidity;
        res.send("1")
});


app.get("/get", function (req, res) {
  var outS = "";
    outS +=  "Light: " + light + "<br>";
    outS +=  "Temperature: " + temp + "<br>";
    outS +=  "Humidity: " + humidity;
    res.send(outS);
});


app.get("/getData", function (req, res) {
    db.collection('data').find().sort({time:1}).toArray(function(err, result){
        var rObj = result[0];
        console.log(rObj);
        res.send(JSON.stringify(rObj));
    });
});

app.use(methodOverride());
app.use(bodyParser());
app.use(express.static(__dirname + '/public'));
app.use(errorHandler());

console.log("Simple static server listening at http://" + hostname + ":" + port);
app.listen(port);


