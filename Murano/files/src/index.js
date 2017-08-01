const sayHello = () => {
    /*eslint-disable no-console */
    console.log("START");
    console.log("=====================");
};

function end() {
	location.reload();
	console.log("=====================");
	console.log("END");
}

function setRGB() {
	if(checkRed() && checkGreen() && checkBlue())
		setValues();
	else
		console.log("terminating setRGB(), not running setValues()")
}


function test(){
	console.log("test() executed");

	fetch("/test ", {
		method: "GET",
}).then( res => {
		console.log(res);
	})


}

function setValues(){
	console.log("setValues() executed");
	var form =  {"r":document.getElementById('red').value.toString(),
				"g":document.getElementById('green').value.toString(),
				"b":document.getElementById('blue').value.toString(),
				"l":document.getElementById('brightness').value.toString(),
				"t":document.getElementById('switch').value.toString()};
	console.log(form);
	var sendJSON = JSON.stringify(form);
	console.log(sendJSON);
	fetch("/device/setValues", {
	    method: "POST",
	    headers: { "Content-Type": "application/json" },
	    body: sendJSON
	}).then( res => {
		console.log(res);
	})

}


function alarmOn(){
	console.log("alarmOn() executed");
	fetch("/device/setValues", {
	    method: "POST",
	    headers: { "Content-Type": "application/json" },
	    body: '{"a":"1"}'
	}).then( res => {
		console.log(res);
	})


}

function alarmOff(){
	console.log("alarmOff() executed");
	fetch("/device/setValues", {
	    method: "POST",
	    headers: { "Content-Type": "application/json" },
	    body: '{"a":"0"}'
	}).then( res => {
		console.log(res);
	})


}


function checkRed(){
	console.log("checkRed() executed");

	var form =  document.getElementById('red').value;
	console.log("Red Data:");
	console.log(form);
		if(form=='')
		{
			console.log("No data logged...terminating checkRed()");
			return false;
		}
		if(form>255)
		{
			console.log("Data exceeds max value... terminating checkRed()");
			return false;
		}
		if(form<0)
		{
			console.log("Data preceds min value... terminating checkRed()");
			return false;
		}

	console.log("checkRed() edning...");

	return true;
}

function checkGreen(){

	console.log("checkGreen() executed");

	var form =  document.getElementById('green').value;
	console.log("Green Data:");
	console.log(form);
		if(form=='')
		{
			console.log("No data logged...terminating checkGreen()");
			return false;
		}
		if(form>255)
		{
			console.log("Data exceeds max value... terminating checkGreen()");
			return false;
		}
		if(form<0)
		{
			console.log("Data preceds min value... terminating checkGreen()");
			return false;
		}

	console.log("checkGreen() edning...");

	return true;
	
}

function checkBlue(){
	console.log("checkBlue() executed");

	var form =  document.getElementById('blue').value;
	console.log("Blue Data:");
	console.log(form);
		if(form=='')
		{
			console.log("No data logged...terminating checkBlue()");
			return false;
		}
		if(form>255)
		{
			console.log("Data exceeds max value... terminating checkBlue()");
			return false;
		}
		if(form<0)
		{
			console.log("Data preceds min value... terminating checkBlue()");
			return false;
		}
	
	console.log("checkBlue() edning...");

	return true;

}



HTMLElement.prototype.serialize = function(){
    var obj = {};
    var elements = this.querySelectorAll( "input, select, textarea" );
    for( var i = 0; i < elements.length; ++i ) {
        var element = elements[i];
        var name = element.name;
        var value = element.value;

        if( name ) {

            obj[ name ] = value;
            
        }
    }
    return JSON.stringify( obj );
}

sayHello();


