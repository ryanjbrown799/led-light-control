const express = require('express')
var request = require('request');

const app = express()
const port = 3000

app.use(express.static('public'));
app.use(express.urlencoded({ extended: true }));
app.use(express.json()); 

//class for lights
class Light {
	constructor(name, domain){
		this.name = name;
		this.domain = domain;
	}
}


lights = [];

/*
test recieving and sending using .local addresses
*/
app.post('/test', (req, res) => {
	console.log(req.body)
	console.log(req.body.name)
	res.send("recieved")
	
	//send post back using ip recieved 
	/*
	request.post({
		headers: {'content-type' : 'application/x-www-form-urlencoded'},
		url: "http://"+req.body.name+".local/LED",
		body:    "heydude"
	}, function(error, response, body){
		console.log(body);
	});*/
	
	//change led
	request.post({
		headers: {'content-type' : 'application/x-www-form-urlencoded'},
		url: "http://"+lights[0].domain+".local/colour",
		body:    "red=126&blue=20&green=160"
	}, function(error, response, body){
		console.log(body);
	});
		
		
})

/*
recives http post with domain.local name from led that has turned on
if led has never connected before create a new light object add to light list
if not new don't add
*/
app.post('/init', (req, res) => {
	console.log(req.body.name)
	
	var exists = false;
	for (var i=0;i<lights.length;i++){
		if(lights[i].domain == req.body.name){
			console.log('light exists')
			exists = true;
			break;
		}
	}
	if(!exists){
		lights.push(new Light("temp",req.body.name))
		console.log('light added')
	}
	console.log(lights)
	
	res.send("connected")	
})

app.get('/', (req, res) => {
	res.sendFile("index.html")
})

app.listen(port, () => {
	console.log("listening on port " +port)
})


