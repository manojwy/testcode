var EXPOSED_TO_NATIVE = {
	js_ObjC_bridge : {
		ready : true
	}
};

var CMIOSJSBRIDGE = {

};

CMIOSJSBRIDGE.queue = {
	commands : [],
	timer : null
};

CMIOSJSBRIDGE.run_command = function() {

	if(!EXPOSED_TO_NATIVE.js_ObjC_bridge.ready) {
		return;
	}

	EXPOSED_TO_NATIVE.js_ObjC_bridge.ready = false;

	var queue = CMIOSJSBRIDGE.queue;

	var arg = queue.commands.shift();
	if(queue.commands.length === 0) {
		window.clearInterval(queue.timer);
		//queue.timer = null;
	}

	var url = "native://cm" + "/?" + encodeURIComponent(arg);

	document.location = url;

};

CMIOSJSBRIDGE.exec = function(a) {

	var queue = CMIOSJSBRIDGE.queue;

	queue.commands.push(a);

	window.clearInterval(queue.timer);
	//if (queue.timer === null) {
	queue.timer = window.setInterval(CMIOSJSBRIDGE.run_command, 10);
	//}
};
var external = {};
external.notify = CMIOSJSBRIDGE.exec;
