// 
// 
// 
const http = require('http');
const fs = require('fs');

const useSerial = !process.env.NO_SERIAL; // set NO_SERIAL in the environment to run without serial port deps
console.log("useSerial == " + useSerial);
const port = process.env.PORT || 3000; // get the port to run on from the environment or default to 3000

const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline')
var sp;
if(useSerial) {
    sp = new SerialPort('/dev/ttyACM0', {
	baudRate: 115200
    })
}

const mimeTypes = {
    "html": "text/html",
    "jpeg": "image/jpeg",
    "jpg": "image/jpeg",
    "png": "image/png",
    "svg": "image/svg+xml",
    "json": "application/json",
    "js": "text/javascript",
    "css": "text/css"
};

const html_dir = __dirname + "/html";

// Don't create the response at all until req.url is considered
// if req.url starts with /api then pass it to the robot api object which will respond with json
// if req.url starts with /video (maybe /camera or /cam is better) then pass it to the streaming class
// if req.url is just a / then return index.html
// otherwise attempt to return the file in req.url
const server = http.createServer((req, res) => {
    if(req.url == '/') {
	fs.readFile(html_dir + "/index.html", (err, data) => {
	    res.setHeader("Content-Type", "text/html");
	    res.writeHead(200);
	    res.end(data);
        });

    } else if(req.url == '/cam/stream.jpg') {
	fs.readFile(__dirname + "/snapshot.jpg", (err, data) => {
	    res.setHeader('Content-Type', 'image/jpeg');
	    res.setHeader('Cache-Control', 'no-store, max-age=0');
	    res.writeHead(200);
	    res.end(data);
        });

    } else if(req.url == '/cam/api') {
	res.statusCode = 200;
	res.setHeader('Content-Type', 'text/html');
	res.write('<h1>Welcome to the camera api!</h1>');
	res.write(req.url);
	res.end();
	
    } else if(req.url == '/api') {
	// The /api endpoint accepts POST requests with type application/json
	//  these posts will give control data to the robot such as joystick and button state
	// It will also accept GET requests and return type application/json
	//  these gets will return sensor data from the robot such as encoder counts and sonar distances
	//  and motor current and IMU measurements.
	//  The returned data will be selectable via the api with POSTs since everything all the time
	//  might be too much after the initial prototype. Also I'll only care about some data at any
	//  given time.
	// This API should be handled by a separate object. I'm just in-lining the code here to get started
	const contentType = req.headers['content-type'];
	if(req.method=='POST' && contentType=='application/json') {
	    let rawData = ''
	    req.on('data', chunk => {
		rawData += chunk
	    })
	    req.on('end', () => {
		let parsedData = JSON.parse(rawData)
		if(useSerial) {
		    sp.write(rawData, function(err) {
			if (err) {
			    return console.log('Error on write: ', err.message)
			}
			//console.log('message written')
		    })
		} else {
		    console.log("Serial port not connected, would write: " + rawData)
		}
	    })
	    res.statusCode = 200;
	    res.end();
	} else if(req.method=='GET') {
	    res.statusCode = 200;
	    res.setHeader('Content-Type', 'application/json');
	    let mydata = {enc0:5978, enc1:2345, vel:0.3487}
	    res.write(JSON.stringify(mydata));
	    //res.write(req.url);
	    res.end();
	} else {
	    res.statusCode = 404;
	    res.end();
	}
    } else {
	const filename = html_dir + req.url;
	fs.exists(filename, function(exists) {
	    if(!exists) {
		res.writeHead(404, { "Content-Type": "text/plain" });
		res.write("404 Not Found\n");
		res.end();
		return;
	    }

	    if (fs.statSync(filename).isDirectory()) 
		filename += '/index.html';

	    fs.readFile(filename, "binary", function(err, file) {
		if(err) {        
		    res.writeHead(500, {"Content-Type": "text/plain"});
		    res.write(err + "\n");
		    res.end();
		    return;
		}

		var mimeType = mimeTypes[filename.split('.').pop()];

		if (!mimeType) {
		    mimeType = 'text/plain';
		}

		res.writeHead(200, { "Content-Type": mimeType });
		res.write(file, "binary");
		res.end();
		console.log("Successfully served the file: " + filename);
	    });
	});
    }
});

server.listen(port, () => {
    console.log(`Server running at port ${port}`);
})

//sp.on('readable', function () {
//    console.log('Data:', sp.read())
//})

//sp.on('data', function (data) {
//  console.log('Data:', data)
//})

if(useSerial) {
    const lines = sp.pipe(new Readline())
    lines.on('data', console.log)
}

