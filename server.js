const express = require('express')
var request = require('request');
var fs = require('fs');

const app = express()
const port = 3000

app.use(express.static('public'));
app.use(express.urlencoded({ extended: true }));
app.use(express.json()); 

lights = [];

//class for lights
class Light {
	constructor(name, domain,red=255,green=255,blue=255){
		this.name = name;
		this.domain = domain;
		this.red=red;
		this.green=green;
		this.blue=blue;
	}
}


/*
Load light info from data.json 
*/
fs.readFile("./data.json", 'utf8', function(err, data) {
    if(err) {
        console.error("Could not open file: %s", err);
    }
	else{
		console.log(data)
		
		if(data.length > 0){
			//load info for each light and add to array
			for (var i=0;i<((JSON.parse(data)).length);i++){
				lights.push(new Light(JSON.parse(data)[i].name,
				JSON.parse(data)[i].domain,
				JSON.parse(data)[i].red,
				JSON.parse(data)[i].green,
				JSON.parse(data)[i].blue))
				
			}	
		}	
	}
});

/*
write lights[] to file data.json
*/
function SaveLights(){
	fs.writeFile('./data.json', JSON.stringify(lights), function(err) {
		if(err) {
			console.error("Could not write file: %s", err);
		}
	});
}

/*
finds light domain name and sends new rgb to led strip
changes rgb value for light object and updates data.json so colour persists
*/
function ChangeLightColour(name,red,green,blue){
	var light=null;
	
	for(var i=0;i<lights.length;i++){
		if (lights[i].name == name){
			light=lights[i];
			break;
		}
	}
	//light found
	if(light != null){
		//send new rgb values
		request.post({
		headers: {'content-type' : 'application/x-www-form-urlencoded'},
		url: "http://"+light.domain+".local/colour",
		body:    "red="+red+"&blue="+blue+"&green="+green
		}, function(error, response, body){
			if (error){
				console.log('failed to change colour');
			}
			else{
				//console.log(response.statusCode);
				if(response.statusCode==200)
				{
					light.red=red;
					light.green=green;
					light.blue=blue;
					console.log(lights);
					SaveLights();	
				}
			}
		});
	}		
}


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
if not new don't add but set colour to previously used colour
*/
app.post('/init', (req, res) => {
	console.log(req.body.name)
	
	var exists = false;
	
	for (var i=0;i<lights.length;i++){
		if(lights[i].domain == req.body.name){
			console.log('light exists')
			exists = true;
			
			ChangeLightColour(lights[i].name,lights[i].red,lights[i].green,lights[i].blue);
			
			break;
		}
	}
	
	//new light added 
	if(!exists){
		lights.push(new Light("temp",req.body.name));
		console.log('light added');
		
	}
	
	//console.log(lights);
	
	res.send("connected");
})

app.post('/change', (req, res) => {
	console.log(req.body);
	ChangeLightColour('temp',req.body.red,req.body.blue,req.body.green);
	
})

app.get('/', (req, res) => {
	res.sendFile("index.html")
})

app.listen(port, () => {
	console.log("listening on port " +port)
})


