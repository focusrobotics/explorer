// This class provides a simple API between the Robot and various client processes
class RobotAPI {
    // pass in a url which points to the base API
    // passing it in instead of assuming makes it easier to reuse this code with different robots and from different pages
    constructor(url) {
    }

    // cmds is just an object which will be serialized and sent as JSON to the robot
    // path is optional and specifies a sub-path with respect to the base url passed in the constructor
    sendCmd(cmds, path) {
	var xmlhttp = new XMLHttpRequest();   // new HttpRequest instance 
	xmlhttp.open("POST", "/api");
	xmlhttp.setRequestHeader("Content-Type", "application/json");
	xmlhttp.send(JSON.stringify(cmds));
    }

    // path is optional and specifies a sub-path with respect to the base url pased to the constructor
    // the call returns an object with the status data
    getStatus(path) {
    }

    // This is to support status sent asynchronously over websockets, but I'm not sure how it will look yet
    setStatusCallback() {
    }

    // There will likely be other calls to support video streaming, although that could also be done completely separately
    
}

// The joystick is probably for motion control, but it might be nice to make it more generic
// It could control motor power, velocity, arms or grippers
class JoystickControls {

}

// The joystick is clumsy for motion. Sliders could more accurately control velocity and more easily
// keep right and left motors locked.
class SliderControls {

}

// For power off, battery status, debug controls, error messages
// could also provide entry boxes for velocity and pid constants or that could be another component
class SystemControls {
}

// Probably for velocity and pose display, maybe motor power and encoder ticks
// Should I name it something about motion or navigation if that's what it is displaying
class StatusDisplay {
    constructor(divName, api) {
	this.div = document.getElementById(divName);
	this.api = api;
    }
    drawStatusDisplay() {
    }
    // status is updated periodically either from data coming over the websocket or from a GET call made occasionally on the API
    // This routine takes the obj and just updates the html fields
    updateStatus() {
    }
}

// maybe PoseDiagram? I want a picture of the robot showing it's heading, but not sure what to call it
// I also want a real occupancy grid to be displayed. That could be OccupancyMap
class PoseMap {}

// I definitley don't have video streams figured out yet. Here are some ideas.
class pollingVideoStream {
}
class asyncVideoStream {
}
class RobotVideoStream {
}
