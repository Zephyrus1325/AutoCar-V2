#include <Arduino.h>

// HTML web page
const char index_html[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html>
<head>
<title>AutoCar V2 Inteface</title>
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.7.1/jquery.min.js"></script>
<style>
body {
    background-color: #121212;
    color: #f0ece2;
}

p {
    color: lightgray;
    font-family: Arial, Helvetica, sans-serif;
    font-size: 15px;
}

.indicators {
	display: flex;
}

.indicator {
    border-radius: 12px;
	border: solid lightgray 2px;
	height: 520px;
	width: 180px;	
	margin: 5px;
}


</style>
</head>
<body>
<div class="indicators">
	<div class="indicator">
		<div style="text-align: center;">
			<p style="font-size: 26px; font-weight: bold; margin:auto; margin-top:20px; width: 90%;">Left Motor</p>
		</div>
        <div style="text-align:center;">
            <p style="margin:0px; font-size: 20px;">Actual Speed:</p>
            <p style="margin:0px; font-size: 20px;" id="indicator_motorleft_actspeed">10.0 cm/s</p>
        </div>
		<div style="display: flex; text-align:center; height: 10%;">
			<div style="width:50%; margin: auto; border: solid gray 3px;">
				<p style="margin:0px;">Setpoint:</p>
				<p style="margin:0px;" id="indicator_motorleft_setpoint">10.0 cm/s</p>
			</div>
			<div style="width:50%; margin: auto; border: solid gray 3px;">
				<p style="margin:0px;">Throttle:</p>
				<p style="margin:0px;" id="indicator_motorleft_throttle">100%</p>
			</div>
		</div>
		<div>
			<div style="margin:auto; width: 80%;">
				<label>Setpoint</label>
				<input type="text" id="input_motorleft_setpoint" style="width: 40px; text-align: center;"> cm/s
				<label>Throttle</label>
				<input type="text" id="input_motorleft_throttle"style="width: 40px; text-align: center; margin-left: 1.5px;"> %  
			</div>
			<div style="margin:auto; width: 50%;">
				<label>Kp</label>
				<input type="text" id="input_motorleft_kp" style="width: 40px; text-align: center; margin-left: 6.5px;">
				<label>Ki	</label>
				<input type="text" id="input_motorleft_ki" style="width: 40px; text-align: center; margin-left: 10px;">
				<label>Kd</label>
				<input type="text" id="input_motorleft_kd" style="width: 40px; text-align: center; margin-left: 6.5px;"> 
				<button style="width: 100%;" id="input_motorleft_sendbutton">send</button>
			</div>
			<div style="text-align:center">
				<p style="margin:0px" id="indicator_motorleft_kp">Kp: 0.00</p>
				<p style="margin:0px" id="indicator_motorleft_ki">Ki: 0.00</p>
				<p style="margin:0px" id="indicator_motorleft_kd">Kd: 0.00</p>
			</div>
		</div>
		<div  style="display: flex;">
			<button style="width: 45%; height: 50px; margin: auto;" id="input_motorleft_closedloop_button">CLOSED LOOP</button>
            <button style="width: 45%; height: 50px; margin: auto;" id="input_motorleft_openloop_button">OPEN LOOP</button>
		</div>
		
	</div>
	<div class="indicator">
		<div style="text-align: center;">
			<p style="font-size: 26px; font-weight: bold; margin:auto; margin-top:20px; width: 95%;">Right Motor</p>
		</div>
        <div style="text-align:center;">
            <p style="margin:0px; font-size: 20px;">Actual Speed:</p>
            <p style="margin:0px; font-size: 20px;" id="indicator_motorright_actspeed">10.0 cm/s</p>
        </div>
		<div style="display: flex; text-align:center; height: 10%;">
			<div style="width:50%; margin: auto; border: solid gray 3px;">
				<p style="margin:0px;">Setpoint:</p>
				<p style="margin:0px;" id="indicator_motorright_setpoint">10.0 cm/s</p>
			</div>
			<div style="width:50%; margin: auto; border: solid gray 3px;">
				<p style="margin:0px;">Throttle:</p>
				<p style="margin:0px;" id="indicator_motorright_throttle">100%</p>
			</div>
		</div>
		<div>
			<div style="margin:auto; width: 80%;">
				<label>Setpoint</label>
				<input type="text" id="input_motorright_setpoint" style="width: 40px; text-align: center;"> cm/s
				<label>Throttle</label>
				<input type="text" id="input_motorright_throttle" style="width: 40px; text-align: center; margin-left: 1.5px;"> %  
			</div>
			<div style="margin:auto; width: 50%;">
				<label>Kp</label>
				<input type="text" id="input_motorright_kp" style="width: 40px; text-align: center; margin-left: 6.5px;">
				<label>Ki</label>
				<input type="text" id="input_motorright_ki" style="width: 40px; text-align: center; margin-left: 10px;">
				<label>Kd</label>
				<input type="text" id="input_motorright_kd" style="width: 40px; text-align: center; margin-left: 6.5px;"> 
				<button id="input_motorright_sendbutton" style="width: 100%;">send</button>
			</div>
			<div style="text-align:center">
				<p id="indicator_motorright_kp"style="margin:0px">Kp: 0.00</p>
				<p id="indicator_motorright_ki"style="margin:0px">Ki: 0.00</p>
				<p id="indicator_motorright_kd"style="margin:0px">Kd: 0.00</p>
			</div>
		</div>
		<div  style="display: flex;">
			<button  id="input_motorright_closedloop_button" style="width: 45%; height: 50px; margin: auto;">CLOSED LOOP</button>
			<button  id="input_motorright_openloop_button" style="width: 45%; height: 50px; margin: auto;">OPEN LOOP</button>
		</div>
		
	</div>
	<div class="indicator">
		<div style="text-align: center;">
			<p style="font-size: 26px; font-weight: bold; margin:auto; margin-top:20px; width: 90%;">Ultrassounds</p>
		</div>
		<div>
			<p id="indicator_ultrassound_front" style="margin:0px; margin-left: 5px;">Front: 000 cm</p>
			<p id="indicator_ultrassound_front_left" style="margin:0px; margin-left: 5px;">Front Left: 000 cm</p>
			<p id="indicator_ultrassound_front_right" style="margin:0px; margin-left: 5px;">Front Right: 000 cm</p>
			<p id="indicator_ultrassound_left" style="margin:0px; margin-left: 5px;">Left: 000 cm</p>
			<p id="indicator_ultrassound_right" style="margin:0px; margin-left: 5px;">Right: 000 cm</p>
			<p id="indicator_ultrassound_back" style="margin:0px; margin-left: 5px;">Back: 000 cm</p>
			<p id="indicator_ultrassound_back_left" style="margin:0px; margin-left: 5px;">Back Left: 000 cm</p>
			<p id="indicator_ultrassound_back_right" style="margin:0px; margin-left: 5px;">Back Right: 000 cm</p>
		</div>
		<div style="text-align: center;">
			<p style="font-size: 26px; font-weight: bold; margin:auto; width: 90%;">Car Control</p>
		</div>
		<div style="display: flex;">
			<button id="input_carmode_auto" style="width: 45%; height: 50px; margin: auto;">AUTO</button>
			<button id="input_carmode_manual" style="width: 45%; height: 50px; margin: auto;">MANUAL</button>
		</div>
        <div style="text-align: center;">
			<p style="font-size: 26px; font-weight: bold; margin:auto; width: 90%;">Inertial Unit</p>
		</div>
        <div>
			<p id="indicator_imu_accelx" style="margin:0px; margin-left: 5px;">AccelX: 0.00 m/s2</p>
			<p id="indicator_imu_accely" style="margin:0px; margin-left: 5px;">AccelY: 0.00 m/s2</p>
			<p id="indicator_imu_accelz" style="margin:0px; margin-left: 5px;">AccelZ: 0.00 m/s2</p>
			<p id="indicator_imu_gyrox" style="margin:0px; margin-left: 5px;">GyroX: 0.00 rad/s</p>
			<p id="indicator_imu_gyroy" style="margin:0px; margin-left: 5px;">GyroY: 0.00 rad/s</p>
			<p id="indicator_imu_gyroz" style="margin:0px; margin-left: 5px;">GyroZ: 0.00 rad/s</p>
            <p id="indicator_imu_temperature" style="margin:0px; margin-left: 5px;">Temperature: 25C</p>
            <p id="indicator_imu_pressure" style="margin:0px; margin-left: 5px;">Pressure: 101360 Pa</p>
		</div>
	</div>
    <div class="indicator">
		<div style="text-align: center;">
			<p style="font-size: 26px; font-weight: bold; margin:auto; margin-top:20px; width: 90%;">Navigation</p>
		</div>
		<div>
            <p id="indicator_navigation_position_x" style="margin:0px; margin-left: 5px;">PosX: 0 cm</p>
			<p id="indicator_navigation_position_y" style="margin:0px; margin-left: 5px;">PosY: 0 cm</p>
			<p id="indicator_navigation_position_z" style="margin:0px; margin-left: 5px;">PosZ: 0 cm</p>
			<p id="indicator_imu_pitch" style="margin:0px; margin-left: 5px;">Pitch: 0.00 deg</p>
			<p id="indicator_imu_roll" style="margin:0px; margin-left: 5px;">Roll: 0.00 deg</p>
            <p id="indicator_imu_heading" style="margin:0px; margin-left: 5px;">Heading: 360 deg</p>
        </div>
        <div style="text-align: center;">
            <p style="font-size: 26px; font-weight: bold; margin:auto; margin-top:20px; width: 90%;">Random Info</p>
        </div>
        <div style="text-align:center;">
            <p id="indicator_battery_voltage" style="margin: 5px;">Battery: 11.1 V</p>
            <p id="indicator_latency"style="margin: 5px;">Latency: 0 ms</p>
            <p style="margin: 5px; margin-bottom:0px">Update Frequency:</p>
            <p id="indicator_update_frequency" style="margin: 5px; margin-top:0px">10.0 Hz</p>
            <p id="indicator_toolate" style="margin: 5px;"></p>
            <p id="indicator_lowbattery" style="margin: 5px; color: red"></p>
			<button  id="input_walk_button" style="width: 45%; height: 50px; margin: auto;">WALK 3M</button>
        </div>
    </div>
</div>
</body>
<script>
	
	// Configuração de WebSocket

	
	var gateway = `ws://${window.location.hostname}/ws`;
	var websocket;	
    var updateFrequency = 0;
    var latency = 0;
    var now = new Date();
    var millis = now.getMilliseconds();
    var lastReading = millis;
    var latency_low = true;
	// Init web socket when the page loads
	window.addEventListener('load', onload);

    setInterval(function(){
        if(!latency_low){
            document.getElementById("indicator_toolate").innerText = "LOST CONNECTION WITH ESP!";
            websocket.close();
        } else {
            document.getElementById("indicator_toolate").innerText = "";
        }
        latency_low = false;
    }, 1000);

	function onload(event) {
	    initWebSocket();
	}

	function initWebSocket() {
	    console.log('Trying to open a WebSocket connection...');
	    websocket = new WebSocket(gateway);
	    websocket.onopen = onOpen;
	    websocket.onclose = onClose;
	    websocket.onmessage = onMessage;
        websocket.onerror = onError;
	}

	// When websocket is established, call the getReadings() function
	function onOpen(event) {
	    console.log('Connection opened');
	}

	function onClose(event) {
	    console.log('Connection closed');
	    setTimeout(initWebSocket, 2000);
	}

    function onError(event){
        console.log("An Socket error ocurred ", event," closing socket");
        websocket.close();
    }

	// Function that receives the message from the ESP32 with the readings
	function onMessage(event) {
		data = JSON.parse(event.data);
        now = new Date();
        millis = now.getMilliseconds();
        if(millis - lastReading > 0){
            latency = millis - lastReading;
        }
        if(latency < 500){
            latency_low = true;
        }  
		if(data.battery.voltage < 9.3){
            document.getElementById("indicator_lowbattery").innerText = "BATTERY LOW!!!";
        } else {
            document.getElementById("indicator_lowbattery").innerText = "";
        }
        lastReading = millis;
		updateIndicators(data);
	}

	document.getElementById("input_motorleft_sendbutton").addEventListener("click", e =>{
		websocket.send(JSON.stringify({"toChange" : "leftSetpoint", "changeTo" : document.getElementById("input_motorleft_setpoint").value}));
		websocket.send(JSON.stringify({"toChange" : "leftKp", "changeTo" : document.getElementById("input_motorleft_kp").value}));
		websocket.send(JSON.stringify({"toChange" : "leftKi", "changeTo" : document.getElementById("input_motorleft_ki").value}));
		websocket.send(JSON.stringify({"toChange" : "leftKd", "changeTo" : document.getElementById("input_motorleft_kd").value}));
		websocket.send(JSON.stringify({"toChange" : "leftThrottle", "changeTo" : document.getElementById("input_motorleft_throttle").value}));
	});

	document.getElementById("input_motorright_sendbutton").addEventListener("click", e =>{
		websocket.send(JSON.stringify({"toChange" : "rightSetpoint", "changeTo" : document.getElementById("input_motorright_setpoint").value}));
		websocket.send(JSON.stringify({"toChange" : "rightKp", "changeTo" : document.getElementById("input_motorright_kp").value}));
		websocket.send(JSON.stringify({"toChange" : "rightKi", "changeTo" : document.getElementById("input_motorright_ki").value}));
		websocket.send(JSON.stringify({"toChange" : "rightKd", "changeTo" : document.getElementById("input_motorright_kd").value}));
		websocket.send(JSON.stringify({"toChange" : "rightThrottle", "changeTo" : document.getElementById("input_motorright_throttle").value}));
	});
	
    document.getElementById("input_motorleft_closedloop_button").addEventListener("click", e =>{
        websocket.send(JSON.stringify({"toChange" : "leftMode", "changeTo" : "0"}));
    });

    document.getElementById("input_motorleft_openloop_button").addEventListener("click", e =>{
        websocket.send(JSON.stringify({"toChange" : "leftMode", "changeTo" : "1"}));
    });
	

    document.getElementById("input_motorright_closedloop_button").addEventListener("click", e =>{
        websocket.send(JSON.stringify({"toChange" : "rightMode", "changeTo":"0"}));
    });

    document.getElementById("input_motorright_openloop_button").addEventListener("click", e =>{
        websocket.send(JSON.stringify({"toChange" : "rightMode", "changeTo" : "1"}));
    });

    document.getElementById("input_carmode_auto").addEventListener("click", e =>{
        websocket.send(JSON.stringify({"toChange" : "navigationMode", "changeTo" : "0"}));
    });

    document.getElementById("input_carmode_manual").addEventListener("click", e =>{
        websocket.send(JSON.stringify({"toChange" : "navigationMode", "changeTo" : "1"}));
    });

	document.getElementById("input_walk_button").addEventListener("click", e =>{
        websocket.send(JSON.stringify({"toChange" : "walk", "changeTo" : "0"}));
    });
	
	
	// ---------------------------------------------------------------------
	// Função de atualização de todos os indicadores após receber uma mensagem
	// ---------------------------------------------------------------------
	
	function updateIndicators(data){
        document.getElementById("indicator_motorleft_actspeed").innerText = data.motor.left.speed.toFixed(1) + " cm/s";
        document.getElementById("indicator_motorleft_setpoint").innerText = data.motor.left.setpoint.toFixed(1) + " cm/s";
        document.getElementById("indicator_motorleft_throttle").innerText = data.motor.left.throttle;
        document.getElementById("indicator_motorleft_kp").innerText = "Kp: " + data.motor.left.kp.toFixed(2);
        document.getElementById("indicator_motorleft_ki").innerText = "Ki: " + data.motor.left.ki.toFixed(2);
        document.getElementById("indicator_motorleft_kd").innerText = "Kd: " + data.motor.left.kd.toFixed(2);
        document.getElementById("input_motorleft_closedloop_button").style.backgroundColor = data.motor.left.mode == 0 ? "#00FF00" : "#101010";
        document.getElementById("input_motorleft_openloop_button").style.backgroundColor = data.motor.left.mode == 0 ? "#101010" : "#00FF00";
        document.getElementById("indicator_motorright_actspeed").innerText = data.motor.right.speed.toFixed(1) + " cm/s";
        document.getElementById("indicator_motorright_setpoint").innerText = data.motor.right.setpoint.toFixed(1) + " cm/s";
        document.getElementById("indicator_motorright_throttle").innerText = data.motor.right.throttle;
        document.getElementById("indicator_motorright_kp").innerText = "Kp: " + data.motor.right.kp.toFixed(2);
        document.getElementById("indicator_motorright_ki").innerText = "Ki: " + data.motor.right.ki.toFixed(2);
        document.getElementById("indicator_motorright_kd").innerText = "Kd: " + data.motor.right.kd.toFixed(2);
        document.getElementById("input_motorright_closedloop_button").style.backgroundColor = data.motor.right.mode == 0 ? "#00FF00" : "#101010";
        document.getElementById("input_motorright_openloop_button").style.backgroundColor = data.motor.right.mode == 0 ? "#101010" : "#00FF00";
        document.getElementById("indicator_ultrassound_front").innerText = "Front: " + data.ultrassound.front + " cm";
        document.getElementById("indicator_ultrassound_front_left").innerText = "Front Left: " +  data.ultrassound.front_left + " cm";
        document.getElementById("indicator_ultrassound_front_right").innerText = "Front Right: " + data.ultrassound.front_right + " cm";
        document.getElementById("indicator_ultrassound_left").innerText = "Left: " + data.ultrassound.left + " cm";
        document.getElementById("indicator_ultrassound_right").innerText = "Right: " + data.ultrassound.right + " cm";
        document.getElementById("indicator_ultrassound_back").innerText = "Back: " + data.ultrassound.back + " cm";
        document.getElementById("indicator_ultrassound_back_left").innerText = "Back Left: " + data.ultrassound.back_left + " cm";
        document.getElementById("indicator_ultrassound_back_right").innerText = "Back Right: " + data.ultrassound.back_right + " cm";
        document.getElementById("input_carmode_auto").style.backgroundColor = data.navigation.mode == 0 ? "#00FF00" : "#101010";
        document.getElementById("input_carmode_manual").style.backgroundColor = data.navigation.mode == 0 ? "#101010" : "#00FF00";
        document.getElementById("indicator_imu_accelx").innerText = "AccelX: " + data.imu.acc.treated.x.toFixed(1) + " m/s2";
        document.getElementById("indicator_imu_accely").innerText = "AccelY: " + data.imu.acc.treated.y.toFixed(1) + " m/s2";
        document.getElementById("indicator_imu_accelz").innerText = "AccelZ: " + data.imu.acc.treated.z.toFixed(1) + " m/s2";
        document.getElementById("indicator_imu_gyrox").innerText = "GyroX: " + data.imu.gyro.treated.x.toFixed(1) + " rad/s";
        document.getElementById("indicator_imu_gyroy").innerText = "GyroY: " + data.imu.gyro.treated.y.toFixed(1) + " rad/s";
        document.getElementById("indicator_imu_gyroz").innerText = "GyroZ: " + data.imu.gyro.treated.z.toFixed(1) + " rad/s";
        document.getElementById("indicator_imu_temperature").innerText = "Temperature: " + data.imu.baro.treated.temperature.toFixed(1) + " C";
        document.getElementById("indicator_imu_pressure").innerText = "Pressure: " + data.imu.baro.treated.pressure.toFixed(0) + " Pa";
        document.getElementById("indicator_imu_pitch").innerText = "Pitch: " + data.navigation.position.pitch.toFixed(1) + " deg";
        document.getElementById("indicator_imu_roll").innerText = "Roll: " +  data.navigation.position.roll.toFixed(1) + " deg";
        document.getElementById("indicator_imu_heading").innerText = "Heading: " + data.navigation.position.heading.toFixed(0) + " deg";
		document.getElementById("indicator_navigation_position_x").innerText = "PosX: " + data.navigation.position.x.toFixed(0) + " cm";
        document.getElementById("indicator_navigation_position_y").innerText = "PosY: " + data.navigation.position.y.toFixed(0) + " cm";
        document.getElementById("indicator_navigation_position_z").innerText = "PosZ: " + data.navigation.position.z.toFixed(0) + " cm";
        document.getElementById("indicator_battery_voltage").innerText = "Voltage:" + data.battery.voltage.toFixed(1) + " V";
        document.getElementById("indicator_latency").innerText ="Latency: " + latency + " ms";
        document.getElementById("indicator_update_frequency").innerText = (1000/latency).toFixed(1) + " Hz";
	}
	
	
</script>
</html>




)rawliteral";