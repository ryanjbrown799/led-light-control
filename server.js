const express = require('express')
var request = require('request');

const app = express()
const port = 3000

app.use(express.static('public'));
app.use(express.urlencoded({ extended: true }));
app.use(express.json()); 


app.post('/test', (req, res) => {
	console.log(req.body)
	res.send("recieved")
	
	//send post back using ip recieved 
	request.post({
		headers: {'content-type' : 'application/x-www-form-urlencoded'},
		url:     "http://"+req.body.ip+"/LED",
		body:    "heydude"
	}, function(error, response, body){
		console.log(body);
	});
		
})

app.get('/', (req, res) => {
	res.sendFile("index.html")
})

app.listen(port, () => {
	console.log("listening on port " +port)
})


