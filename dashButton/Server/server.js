var express = require("express");
var app = express();
var bodyParser = require('body-parser');
var errorHandler = require('errorhandler');
var methodOverride = require('method-override');
var hostname = process.env.HOSTNAME || 'localhost';
var port = 8080;

app.get("/", function (req, res) {
    console.log("GET req arrived")
        res.send("hello world")
});

app.use(methodOverride());
app.use(bodyParser());
//app.use(express.static(__dirname + '/public'));
app.use(errorHandler());

console.log("Simple static server listening at http://" + hostname + ":" + port);
app.listen(port);

var nodemailer = require("nodemailer");
nodemailer.createTestAccount((err,account) => {
    let transporter = nodemailer.createTransport({
        host: 'smtp.ethereal.email',
        port: 587,
        secure: false,
        auth: {
            user: account.user,
            pass: account.pass
        }
    });

    let mailOptions = {
        from: '"Fred Foo" <email@email.com>',
        to : 'email@email.com',
        subject: 'AME 394 Test Email',
        text: 'Yoooo',
        html: '<b>Hello world</b>'
    };

    transporter.sendMail(mailOptions, (error, info) => {
        if (error){
            return console.log(error);
        }
        console.log('Message sent: %s', info.messageId);
        console.log('Preview URL: %s', nodemailer.getTestMessangerURL(info));
    });
});


//https://nodemailer.com/about/