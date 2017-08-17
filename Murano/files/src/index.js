const sayHello = () => {
    /*eslint-disable no-console */
    console.log("START");
    console.log("=====================");
};

function end() {
	console.log("=====================");
	console.log("END");
}

function setRGB() {
	if(checkRed() && checkGreen() && checkBlue())
		setValues();
	else
		console.log("terminating setRGB(), not running setValues()")
}

/*
function getLockStatus(){
	fetch("/getLockStatus", {
	    method: "GET",
	    headers: { "Content-Type": "application/json" }
	}).then( res => {
		if (res.status >= 200) {
			res.json().then(json => {
				console.log(json)
				let lockedState = json.lockstate;
				selectImage(status);
				console.log(lockedState);
				return lockedState;
			})
		}
		else {
			return "sadface"
		}

	})
}

function selectImage(state)
{
	let imageBlock = document.querySelector("div.lockState")[0]
	if (state === "unlocked") {
		imageBlock.querySlector("img.image_unlocked")[0].hidden = true;
		imageBlock.querySlector("img.image_locked")[0].hidden = false;
		// do false for "locked'"
	} else {
		imageBlock.querySlector("img.image_locked")[0].hidden = true;
		imageBlock.querySlector("img.image_unlocked")[0].hidden = false;
		// inverse
	}

}
*/
function showMeWhatYouGot(){
	console.log("NOT ACTIVE 1.......alarm()");
    console.log("NOT ACTVE 911.....cherries(10-wait)");
    console.log("1337....rainbowCycle(10-wait)");
    console.log("1338....rainbow(30-cycles, 0-wait)");
    console.log("1970....disco(40-cycles, currentColor-color, 90-wait)");
    console.log("NOT ACTIVE 10010...twinkle(10-numberOfLED's, 40-cycles, 100-wait);");
    console.log("12321...pulse(currentColor-color, brightness-brightness, 15-wait");
    console.log("12340...wipeFade(currentColor-color, brightness-brightness, 20-wait");
    console.log("12821...randomPulse(10-cycles, brightness-brightness, 15-wait)");
    console.log("31325...CMY(brightness-brightness)");
    console.log("45654...guideBounce(currentColor-color, 20-wait)");
    console.log("45678...guide(currentColor-color, 20-wait");
    console.log("45918...guideStack(currentColor-color, 20-wait)");
}

function setValues(){
	console.log("setValues() executed");
	var form =  {"r":document.getElementById('red').value.toString(),
				"g":document.getElementById('green').value.toString(),
				"b":document.getElementById('blue').value.toString(),
				"l":document.getElementById('brightness').value.toString(),
				"t":document.getElementById('switch').value.toString(),
				"e":"0"};
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

function setStatus(){
	console.log("setStatus() executed");
	var form = {"t":document.getElementById('switch').value.toString(),
				"e":"0"};
	console.log(form);
	var sendJSON = JSON.stringify(form);
	fetch("/device/setValues", {
		method: "POST",
		headers: { "Content-Type": "application/json" },
		body: sendJSON
	}).then(res => {
		console.log(res);
	})
}

function setBrightness(){
	console.log("setBrightness() executed");
	var form = {"l":document.getElementById('brightness').value.toString()};
	console.log(form);
	var sendJSON = JSON.stringify(form);
	fetch("/device/setValues", {
		method: "POST",
		headers: { "Content-Type": "application/json" },
		body: sendJSON
	}).then(res => {
		console.log(res);
	})
	
}


function extra(number){
	console.log("extra() executed");
	var form = {"e":number.toString()};
	console.log(form);
	var sendJSON = JSON.stringify(form);
	fetch("/device/setValues", {
		method: "POST",
		headers: { "Content-Type": "application/json" },
		body: sendJSON
	}).then( res => {
		console.log(res);

		if(number==1337 &&checkResponse(res)==200)
			allOfTheLights();

	})


}

function extraOff(){
	console.log("alarmOff() executed");
	fetch("/device/setValues", {
	    method: "POST",
	    headers: { "Content-Type": "application/json" },
	    body: '{"e":"0"}'
	}).then( res => {
		console.log(res);
	})


}

function checkResponse(response){
try {
 // this is how you parse a string into JSON 
 var result = (response.status);
 return result;
} catch (ex) {
  console.log(ex);
}
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

function allOfTheLights(){
	var audio = new Audio('extras/airHorn.mp3');
	audio.play();
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


