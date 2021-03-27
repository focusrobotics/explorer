// 
// 
// 
const http = require('http');
const WebSocket = require('ws');
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

// Perhaps better would be to have the image come directly to the stdout of raspistill and read from that
// When I'm not running on a raspi or just not running video I want the image to be a camera with a line through it
// 
const liveImages = !process.env.NO_LIVE_IMAGES;
console.log("liveImages==" + liveImages);
var liveSnap = __dirname + "/snapshot.jpg";
if(liveImages) {
    liveSnap = "/mnt/ramdisk/snapshot.jpg";
}

const { spawn } = require('child_process');
var poweroffCommand = 'poweroff';
if(process.env.HOSTNAME != "explorer") {
    poweroffCommand = 'echo';
}

var liveStreamEnabled = 0;

var raspividStream = require('raspivid-stream');
var videoStream;
if(liveImages) {
    videoStream = raspividStream();
}

const wss = new WebSocket.Server({ noServer: true });
const wssVid = new WebSocket.Server({ noServer: true });
wss.on('connection', function connection(ws) {
    //
    ws.on('message', function incoming(message) {
	console.log('Status websocket received %s', message);
    });
    ws.send('websocket message from server');
});
wssVid.on('connection', function connection(ws) {
    //
    ws.on('message', function incoming(message) {
	console.log('Video websocket received %s', message);
    });
    //ws.send('websocket message from server');
    console.log("video client connected");
    ws.send(JSON.stringify({
      action: 'init',
      width: '960',
      height: '540'
    }));
    videoStream.on('data', (data) => {
        ws.send(data, { binary: true }, (error) => { if (error) console.error(error); });
    });
    ws.on('close', () => {
        console.log('video Client left');
        videoStream.removeAllListeners('data');
    });
});

// Don't create the response at all until req.url is considered
// if req.url starts with /api then pass it to the robot api object which will respond with json
// if req.url starts with /video (maybe /camera or /cam is better) then pass it to the streaming class
// if req.url is just a / then return index.html
// otherwise attempt to return the file in req.url
const server = http.createServer((req, res) => {
    // This first if case just covers the default route when you do a get on the server with no path. It returns index.html
    // which is the default robot control page. Access to other paths on the server is called out below.
    if(req.url == '/') {
	fs.readFile(html_dir + "/index.html", (err, data) => {
	    res.setHeader("Content-Type", "text/html");
	    res.writeHead(200);
	    res.end(data);
        });
    }

    // The /cam/stream.jpg endoint returns the latest jpg image captured from the camera which is basically a live stream if the camera
    // is capturing at a high rate. This produces somewhat higher latency that I'd like in the browser so it would probably be better
    // to send the livestream via websockets in the future. This works for now though.
    else if(req.url == '/cam/stream.jpg') {
	//fs.readFile(__dirname + "/snapshot.jpg", (err, data) => {
	fs.readFile(liveSnap, (err, data) => {
	    res.setHeader('Content-Type', 'image/jpeg');
	    res.setHeader('Cache-Control', 'no-store, max-age=0');
	    res.writeHead(200);
	    res.end(data);
        });
    }


    // The /cam/api endpoint can be used to turn the camera stream on and off and set params with POST or get information about the camera with GET
    // 
    else if(req.url == '/cam/api') {
	const contentType = req.headers['content-type'];
	if(req.method=='POST' && contentType=='application/json') {
	    let rawData = ''
	    req.on('data', chunk => {
		rawData += chunk
	    })
	    req.on('end', () => {
		let parsedData = JSON.parse(rawData)
		console.log("Got this data from the camera API: " + rawData);
		// startStream option so that the raspistill program is only started when requested
		// The client needs to send {startStream:1} to start the raspistill subprocess and {startStream:0} to stop it
		if(parsedData.startStream && !liveStreamEnabled) {
		    console.log("Starting camera stream");
		    // Only spawn a raspistill process if there isn't one running!
		    // The status of raspistill is tracked here with the liveStreamEnabled variable
		    liveStreamEnabled = 1;
		    const cs = spawn('raspistill', ['-vf', '-hf', '-w', '640', '-h', '480', '-o', '/mnt/ramdisk/snapshot.jpg', '-tl', '100', '-t', '600000']);
		    cs.on('error', (err) => {
			liveStreamEnabled = 0;
			console.log("Error when trying to spawn raspistill: " + err);
		    });
		    cs.on('exit', (code, signal) => {
			liveStreamEnabled = 0;
			console.log("raspistill exited with code: " + code);
		    });
		} else {
		    console.log("Stopping camera stream");
		}
	    })
	    res.statusCode = 200;
	    res.end();
	} else if(req.method=='GET') {
	    res.statusCode = 200;
	    res.setHeader('Content-Type', 'application/json');
	    // return whether the stream is started on the server
	    // client start and stop of the stream is different than the server start and stop stream; both are currently independent
	    let mydata = {configLivestream:liveImages, configSerial:useSerial, raspistill:liveStreamEnabled}
	    res.write(JSON.stringify(mydata));
	    res.end();
	} else {
	    res.statusCode = 404;
	    res.end();
	}
    }


    // The /api endpoint accepts POST requests with type application/json
    //  these posts will give control data to the robot such as joystick and button state
    // It will also accept GET requests and return type application/json
    //  these gets will return sensor data from the robot such as encoder counts and sonar distances
    //  and motor current and IMU measurements.
    //  The returned data will be selectable via the api with POSTs since everything all the time
    //  might be too much after the initial prototype. Also I'll only care about some data at any
    //  given time.
    // This API should be handled by a separate object. I'm just in-lining the code here to get started
    // This informaiton transfer should probably be handled with websockets to lower the latency and overhead.
    // It is logically a persistant connection and it probably doesn't make sense to keep openeing up a new connection for every transfer.
    else if(req.url == '/api') {
	const contentType = req.headers['content-type'];
	if(req.method=='POST' && contentType=='application/json') {
	    let rawData = ''
	    req.on('data', chunk => {
		rawData += chunk
	    })
	    req.on('end', () => {
		let parsedData = JSON.parse(rawData)
		// If the parsedData includes shutdown:true then call poweroff for the raspi!
		console.log(parsedData);
		if(parsedData.shutdown == 'true') {
		    console.log("Shut down now!");
		    const po = spawn('sudo', ['poweroff']);
		    po.on('error', (err) => {
                        console.log("Error when trying to spawn poweroff: " + err);
                    });
		    po.stdout.on('data', (data) => {
                        console.log("stdout when trying to spawn poweroff: " + data);
                    });
		    po.stderr.on('data', (data) => {
                        console.log("stdout when trying to spawn poweroff: " + data);
                    });

		}
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
	    let mydata = {enc1:5978, enc2:2345, vel1:0.3487, vel2:0.345}
	    res.write(JSON.stringify(mydata));
	    //res.write(req.url);
	    res.end();
	} else {
	    res.statusCode = 404;
	    res.end();
	}
    }

    // Finally, this route covers everything else. If a request gets here and the path provided exists in the html dir and
    // is of a known mime type then the file will be returned just like in a normal web server. This allows for external css
    // and js files, documentation, and anything else I can think up.
    else {
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

server.on('upgrade', function upgrade(request, socket, head) {
    //const pathname = url.parse(request.url).pathname;
    const pathname = request.url;

    if (pathname === '/api/status') {
	wss.handleUpgrade(request, socket, head, function done(ws) {
	    wss.emit('connection', ws, request);
	});
    } else if (pathname === '/api/vidstream') {
	wssVid.handleUpgrade(request, socket, head, function done(ws) {
	    wssVid.emit('connection', ws, request);
	});
    } else {
	socket.destroy();
    }
});

server.listen(port, () => {
    console.log(`Server running at port ${port}`);
})

// Parse serial data from the uc as JSON and keep track of the values to return via the API when requested
// Probably send it to the client via websockets when I get that set up
if(useSerial) {
    const lines = sp.pipe(new Readline())
    lines.on('data', function(dat) {
	console.log(dat);
	// Probably should qualify the JSON from the micro, but for now just send it straight on to the web browser for display
	//let parsedData = JSON.parse(dat);
	//if(parsedData.foo != null) {
	//    console.log("got heading");
	//}
	wss.clients.forEach(function each(client) {
	    if(client.readyState === WebSocket.OPEN) {
		client.send(dat);
	    }
	});
    });
}

//if(liveImages) {
//    videoStream.on('data', (data) => {
	//console.log("New videoStream data");
//	wssVid.clients.forEach(function each(client) {
	    //console.log("A client for video data");
//	    if(client.readyState === WebSocket.OPEN) {
//		client.send(data, { binary: true }, (error) => { if (error) console.error(error); });
		//console.log("videoStream data send to a client");
//	    }
//	});
//    });
//}
