#include <Arduino.h>

// HTML web page
const char index_html[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html>
<head>
<title>AutoCar V2 Inteface</title>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0"> 
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

div {
    border: solid gray 1px; 
}

.motor_variable {
    margin: 3px;
    text-align: center;
    width: 20%;
}

.navigation_variable {
    margin: 3px;
    text-align: center;
    width: 20%;
}

.ultrassound_variable {
    margin: 3px;
    text-align: center;
    width: 20%;
}

.navigation_details{
    margin: 3px;
    text-align: left;
}

</style>
</head>
<body>
<div style="display: flex;">
    <!-- Parte Esquerda (Mapa e Feedback)-->
    <div style="width: 70%; height: auto;">
        <!-- Header com Avisos e informações genericas -->
        <div style="display: flex;  width: auto; height: 45px; margin: 5px;">
            <p id="indicator_status" style="width: 80%; text-align: left; margin-left: 10px;">Healthy</p>
            <p id="indicator_latency" style="width: 10%; text-align: right;">100 ms</p>
            <p id="indicator_battery_percentage" style="width: 10%; text-align: right; margin-right: 10px;">100%</p>
        </div>
        <canvas id="indicator_map" width="400px"; height="400px" style="width: auto; height: auto; background-color: #757575; margin: 10px;"></canvas>
    </div>
    <div>

    </div>
    <!-- Parte Direita (Comandos)-->
     
    <div>
        <p style="width: auto; padding: 13px; margin: 5px; border: solid gray 1px; text-align: center">Parameters</p>
        <div style="width: 100%; height: 500px; overflow-y: scroll">
            <!-- Parametros dos Motores -->
            <p style="width: auto; padding: 3px; margin: 5px; border: solid gray 1px; text-align: center">Motors</p>
            <div>
            <div style="display: flex">
                <p style="width: 20%; text-align: center;">-</p>
                <p style="width: 40%; text-align: center;">Left Motor</p>
                <p style="width: 40%; text-align: center;">Right Motor</p>
            </div>
            <div style="display: flex">
                <p class="motor_variable">Act Spd</p>
                <p id="indicator_motor_left_actspeed" class="motor_variable" style="width: 40%">20 cm/s</p>
                <p id="indicator_motor_right_actspeed" class="motor_variable" style="width: 40%">20 cm/s</p>
            </div>
            <div style="display: flex">
                <p class="motor_variable">Setpoint</p>
                <p id="indicator_motor_left_setpoint" class="motor_variable">20 cm/s</p>
                <input id="input_motor_left_setpoint" type="number" class="motor_variable">
                <p id="indicator_motor_right_setpoint" class="motor_variable">20 cm/s</p>
                <input id="input_motor_right_setpoint" type="number" class="motor_variable">
            </div>
            <div style="display: flex;">
                <p class="motor_variable">Throttle</p>
                <p id="indicator_motor_left_throttle" class="motor_variable">255</p>
                <input id="input_motor_left_throttle" type="number" class="motor_variable">
                <p id="indicator_motor_right_throttle" class="motor_variable">255</p>
                <input id="input_motor_right_throttle" type="number" class="motor_variable">
            </div>
            <div style="display: flex">
                <p class="motor_variable"></p>
                <button id="input_motor_left_sendpower" style="width: 40%">Send</button>
                <button id="input_motor_right_sendpower" style="width: 40%">Send</button>
            </div>
            <div style="display: flex;">
                <p class="motor_variable">Kp</p>
                <p id="indicator_motor_left_kp" class="motor_variable">0.00</p>
                <input id="input_motor_left_kp" type="number" class="motor_variable">
                <p id="indicator_motor_right_kp" class="motor_variable">0.00</p>
                <input id="input_motor_right_kp" type="number" class="motor_variable">
            </div>
            <div style="display: flex;">
                <p class="motor_variable">Ki</p>
                <p id="indicator_motor_left_ki" class="motor_variable">0.00</p>
                <input id="input_motor_left_ki" type="number" class="motor_variable">
                <p id="indicator_motor_right_ki" class="motor_variable">0.00</p>
                <input id="input_motor_right_ki" type="number" class="motor_variable">
            </div>
            <div style="display: flex;">
                <p class="motor_variable">Kd</p>
                <p id="indicator_motor_left_kd" class="motor_variable">0.00</p>
                <input id="input_motor_left_kd" type="number" class="motor_variable">
                <p id="indicator_motor_right_kd" class="motor_variable">0.00</p>
                <input id="input_motor_right_kd" type="number" class="motor_variable">
            </div>
            <div style="display: flex">
                <p class="motor_variable"></p>
                <button id="input_motor_left_sendconstants" style="width: 40%">Send</button>
                <button id="input_motor_right_sendconstants" style="width: 40%">Send</button>
            </div>
            <div style="display: flex;">
                <p style="width: 20%; text-align: center;">PID</p>
                <button id="input_motor_left_enablePID" style="width: 20%">On</button>
                <button id="input_motor_left_disablePID" style="width: 20%">Off</button>
                <button id="input_motor_right_enablePID" style="width: 20%">On</button>
                <button id="input_motor_right_disablePID" style="width: 20%">Off</button>
            </div>
            </div>
            <!-- Parametros de Navegação-->
            <p style="width: auto; padding: 3px; margin: 5px; border: solid gray 1px; text-align: center">Navigation</p>
            <div>
                <div style="display: flex">
                    <p class="navigation_variable">Nav Mode</p>
                    <button id="input_navigation_setmode_auto" class="navigation_variable" style="width: 40%">Auto</button>
                    <button id="input_navigation_setmode_manual" class="navigation_variable" style="width: 40%">Manual</button>
                </div>
                <div style="display: flex">
                    <p class="navigation_variable">Pos</p>
                    <p id="indicator_navigation_position_x" class="navigation_variable" style="width: 40%">X = 100</p>
                    <p id="indicator_navigation_position_y" class="navigation_variable" style="width: 40%">Y = -200</p>
                </div>
                <div style="display: flex">
                    <p class="navigation_variable">Angles</p>
                    <p id="indicator_imu_heading" class="navigation_variable" style="width: 26.6%">Head = 100</p>
                    <p id="indicator_imu_pitch" class="navigation_variable" style="width: 26.6%">Pitch = -200</p>
                    <p id="indicator_imu_roll" class="navigation_variable" style="width: 26.6%">Roll = -200</p>
                </div>
                <div style="display: flex">
                    <p class="navigation_variable">Dest</p>
                    <p class="navigation_variable">X = 100</p>
                    <input type="number" class="navigation_variable">
                    <p class="navigation_variable">Y = -200</p>
                    <input type="number" class="navigation_variable">
                </div>
                <div style="display: flex">
                    <p class="navigation_variable">-</p>
                    <button id="input_walk_button" class="navigation_variable" style="width: 80%">Walk</button>
                </div>
                <!-- Permite ver todos os waypoints ativos-->
                <details>
                    <summary>
                        Waypoints Buffer (Actual: 4) 
                    </summary>
                    <div style="display: flex">
                        <p class="navigation_details" style="width: 5% ">0:</p>
                        <p class="navigation_details" style="width: 23%">X: 10</p>
                        <p class="navigation_details" style="width: 23%">Y: 50</p>
                        <p class="navigation_details" style="width: 49%">Distance: 100 cm</p>
                    </div>
                    <div style="display: flex">
                        <p class="navigation_details" style="width: 5% ">1:</p>
                        <p class="navigation_details" style="width: 23%">X: 10</p>
                        <p class="navigation_details" style="width: 23%">Y: -50</p>
                        <p class="navigation_details" style="width: 49%">Distance: 300 cm</p>
                    </div>
                    <div style="display: flex">
                        <p class="navigation_details" style="width: 5% ">2:</p>
                        <p class="navigation_details" style="width: 23%">X: -100</p>
                        <p class="navigation_details" style="width: 23%">Y: 500</p>
                        <p class="navigation_details" style="width: 49%">Distance: 1000 cm</p>
                    </div>
                    <div style="display: flex">
                        <p class="navigation_details" style="width: 5% ">3:</p>
                        <p class="navigation_details" style="width: 23%">X: -10000</p>
                        <p class="navigation_details" style="width: 23%">Y: 50000</p>
                        <p class="navigation_details" style="width: 49%">Distance: 10000 cm</p>
                    </div>
                </details>
            </div>
            <!-- Leituras de Ultrassom-->
            <p style="width: auto; padding: 3px; margin: 5px; border: solid gray 1px; text-align: center">Ultrassound</p>
            <div>
                <div style="display: flex">
                    <p class="ultrassound_variable"  style="width: 20%">Front</p>
                    <p id="indicator_ultrassound_front_left" class="ultrassound_variable">99</p>
                    <p id="indicator_ultrassound_front" class="ultrassound_variable">99</p>
                    <p id="indicator_ultrassound_front_right" class="ultrassound_variable">99</p>
                </div>
                <div style="display: flex">
                    <p class="ultrassound_variable"  style="width: 20%">Sides</p>
                    <p id="indicator_ultrassound_left" class="ultrassound_variable">99</p>
                    <p class="ultrassound_variable"></p>
                    <p id="indicator_ultrassound_right" class="ultrassound_variable">99</p>
                </div>
                <div style="display: flex">
                    <p class="ultrassound_variable"  style="width: 20%">Back</p>
                    <p id="indicator_ultrassound_back_left" class="ultrassound_variable">99</p>
                    <p id="indicator_ultrassound_back" class="ultrassound_variable">99</p>
                    <p id="indicator_ultrassound_back_right" class="ultrassound_variable">99</p>
                </div>
            </div>
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
    var problem_flag = false;
    var mapData = new Array(100).fill(0).map(() => new Array(100).fill(0));
    var posX = 0;
    var posY = 0;
	// Init web socket when the page loads
	window.addEventListener('load', onload);

    setInterval(function(){
        if(!latency_low){
            document.getElementById("indicator_status").innerText = "Connection Lost";
            problem_flag = true;
            websocket.close();
        }
        if(!problem_flag){
            document.getElementById("indicator_status").innerText = "Healthy";
        }
        problem_flag = false;
        latency_low = false;
    }, 1000);

    setInterval(function(){
        updateCanvas();
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
        if(data.type == "carInfo"){
            if(latency < 500){
                latency_low = true;
            }  
		    if(data.battery.voltage < 9.3){
                document.getElementById("indicator_status").innerText = "Battery Low";
                problem_flag = true;
            }
            lastReading = millis;
            posX = data.navigation.position.x/10;
            posY = data.navigation.position.y/10;
            updateIndicators(data);
        } else if(data.type == "chunkData"){
            fillMapData(data);
        }		
	}

    document.getElementById("input_walk_button").addEventListener("click", e =>{
		websocket.send(JSON.stringify({"toChange" : "walk", "changeTo" : 1}));
	});

    document.getElementById("input_motor_left_sendpower").addEventListener("click", e =>{
		websocket.send(JSON.stringify({"toChange" : "leftSetpoint", "changeTo" : document.getElementById("input_motor_left_setpoint").value}));
		websocket.send(JSON.stringify({"toChange" : "leftThrottle", "changeTo" : document.getElementById("input_motor_left_throttle").value}));
	});

    document.getElementById("input_motor_right_sendpower").addEventListener("click", e =>{
		websocket.send(JSON.stringify({"toChange" : "rightSetpoint", "changeTo" : document.getElementById("input_motor_right_setpoint").value}));
		websocket.send(JSON.stringify({"toChange" : "rightThrottle", "changeTo" : document.getElementById("input_motor_right_throttle").value}));
	});

	document.getElementById("input_motor_left_sendconstants").addEventListener("click", e =>{
		websocket.send(JSON.stringify({"toChange" : "leftKp", "changeTo" : document.getElementById("input_motor_left_kp").value}));
		websocket.send(JSON.stringify({"toChange" : "leftKi", "changeTo" : document.getElementById("input_motor_left_ki").value}));
		websocket.send(JSON.stringify({"toChange" : "leftKd", "changeTo" : document.getElementById("input_motor_left_kd").value}));
	});

	document.getElementById("input_motor_right_sendconstants").addEventListener("click", e =>{
		websocket.send(JSON.stringify({"toChange" : "rightKp", "changeTo" : document.getElementById("input_motor_right_kp").value}));
		websocket.send(JSON.stringify({"toChange" : "rightKi", "changeTo" : document.getElementById("input_motor_right_ki").value}));
		websocket.send(JSON.stringify({"toChange" : "rightKd", "changeTo" : document.getElementById("input_motor_right_kd").value}));
	});
	
    document.getElementById("input_motor_left_enablePID").addEventListener("click", e =>{
        websocket.send(JSON.stringify({"toChange" : "leftMode", "changeTo" : "0"}));
    });

    document.getElementById("input_motor_left_disablePID").addEventListener("click", e =>{
        websocket.send(JSON.stringify({"toChange" : "leftMode", "changeTo" : "1"}));
    });
	

    document.getElementById("input_motor_right_enablePID").addEventListener("click", e =>{
        websocket.send(JSON.stringify({"toChange" : "rightMode", "changeTo":"0"}));
    });

    document.getElementById("input_motor_right_disablePID").addEventListener("click", e =>{
        websocket.send(JSON.stringify({"toChange" : "rightMode", "changeTo" : "1"}));
    });

    document.getElementById("input_navigation_setmode_auto").addEventListener("click", e =>{
        websocket.send(JSON.stringify({"toChange" : "navigationMode", "changeTo" : "0"}));
    });

    document.getElementById("input_navigation_setmode_manual").addEventListener("click", e =>{
        websocket.send(JSON.stringify({"toChange" : "navigationMode", "changeTo" : "1"}));
    });
	
	
	// ---------------------------------------------------------------------
	// Função de atualização de todos os indicadores após receber uma mensagem
	// ---------------------------------------------------------------------
	
	function updateIndicators(data){
        document.getElementById("indicator_motor_left_actspeed").innerText = data.motor.left.speed.toFixed(1) + " cm/s";
        document.getElementById("indicator_motor_left_setpoint").innerText = data.motor.left.setpoint.toFixed(1) + " cm/s";
        document.getElementById("indicator_motor_left_throttle").innerText = data.motor.left.throttle;
        document.getElementById("indicator_motor_left_kp").innerText = "Kp: " + data.motor.left.kp.toFixed(2);
        document.getElementById("indicator_motor_left_ki").innerText = "Ki: " + data.motor.left.ki.toFixed(2);
        document.getElementById("indicator_motor_left_kd").innerText = "Kd: " + data.motor.left.kd.toFixed(2);
        document.getElementById("input_motor_left_enablePID").style.backgroundColor = data.motor.left.mode == 0 ? "#00FF00" : "#101010";
        document.getElementById("input_motor_left_disablePID").style.backgroundColor = data.motor.left.mode == 0 ? "#101010" : "#00FF00";
        document.getElementById("indicator_motor_right_actspeed").innerText = data.motor.right.speed.toFixed(1) + " cm/s";
        document.getElementById("indicator_motor_right_setpoint").innerText = data.motor.right.setpoint.toFixed(1) + " cm/s";
        document.getElementById("indicator_motor_right_throttle").innerText = data.motor.right.throttle;
        document.getElementById("indicator_motor_right_kp").innerText = "Kp: " + data.motor.right.kp.toFixed(2);
        document.getElementById("indicator_motor_right_ki").innerText = "Ki: " + data.motor.right.ki.toFixed(2);
        document.getElementById("indicator_motor_right_kd").innerText = "Kd: " + data.motor.right.kd.toFixed(2);
        document.getElementById("input_motor_right_enablePID").style.backgroundColor = data.motor.right.mode == 0 ? "#00FF00" : "#101010";
        document.getElementById("input_motor_right_disablePID").style.backgroundColor = data.motor.right.mode == 0 ? "#101010" : "#00FF00";
        document.getElementById("indicator_ultrassound_front").innerText = data.ultrassound.front;
        document.getElementById("indicator_ultrassound_front_left").innerText = data.ultrassound.front_left;
        document.getElementById("indicator_ultrassound_front_right").innerText = data.ultrassound.front_right;
        document.getElementById("indicator_ultrassound_left").innerText = data.ultrassound.left;
        document.getElementById("indicator_ultrassound_right").innerText = data.ultrassound.right;
        document.getElementById("indicator_ultrassound_back").innerText = data.ultrassound.back;
        document.getElementById("indicator_ultrassound_back_left").innerText = data.ultrassound.back_left;
        document.getElementById("indicator_ultrassound_back_right").innerText = data.ultrassound.back_right;
        document.getElementById("input_navigation_setmode_auto").style.backgroundColor = data.navigation.mode == 0 ? "#00FF00" : "#101010";
        document.getElementById("input_navigation_setmode_manual").style.backgroundColor = data.navigation.mode == 0 ? "#101010" : "#00FF00";
        //document.getElementById("indicator_imu_accelx").innerText = "AccelX: " + data.imu.acc.treated.x.toFixed(1) + " m/s2";
        //document.getElementById("indicator_imu_accely").innerText = "AccelY: " + data.imu.acc.treated.y.toFixed(1) + " m/s2";
        //document.getElementById("indicator_imu_accelz").innerText = "AccelZ: " + data.imu.acc.treated.z.toFixed(1) + " m/s2";
        //document.getElementById("indicator_imu_gyrox").innerText = "GyroX: " + data.imu.gyro.treated.x.toFixed(1) + " deg/s";
        //document.getElementById("indicator_imu_gyroy").innerText = "GyroY: " + data.imu.gyro.treated.y.toFixed(1) + " deg/s";
        //document.getElementById("indicator_imu_gyroz").innerText = "GyroZ: " + data.imu.gyro.treated.z.toFixed(1) + " deg/s";
        //document.getElementById("indicator_imu_temperature").innerText = "Temperature: " + data.imu.baro.treated.temperature.toFixed(1) + " C";
        //document.getElementById("indicator_imu_pressure").innerText = "Pressure: " + data.imu.baro.treated.pressure.toFixed(0) + " Pa";
        document.getElementById("indicator_imu_pitch").innerText = data.navigation.position.pitch.toFixed(1);
        document.getElementById("indicator_imu_roll").innerText = data.navigation.position.roll.toFixed(1);
        document.getElementById("indicator_imu_heading").innerText = "Head: " + data.navigation.position.heading.toFixed(0) + " deg";
		document.getElementById("indicator_navigation_position_x").innerText = "X = " + data.navigation.position.x.toFixed(0);
        document.getElementById("indicator_navigation_position_y").innerText = "Y = " + data.navigation.position.y.toFixed(0);
        //document.getElementById("indicator_navigation_position_z").innerText = "PosZ: " + data.navigation.position.z.toFixed(0) + " cm";
		//document.getElementById("indicator_navigation_waypoint_x").innerText = "DestX: " + data.navigation.destination.x.toFixed(0) + " cm";
        //document.getElementById("indicator_navigation_waypoint_y").innerText = "DestY: " + data.navigation.destination.y.toFixed(0) + " cm";
        //document.getElementById("indicator_navigation_waypoint_z").innerText = "DestZ: " + data.navigation.destination.z.toFixed(0) + " cm";
        document.getElementById("indicator_battery_percentage").innerText = data.battery.voltage.toFixed(1) + " V";
        document.getElementById("indicator_latency").innerText = latency + " ms";
        //document.getElementById("indicator_update_frequency").innerText = (1000/latency).toFixed(1) + " Hz";
	}

    function fillMapData(data){
        for(var i = 0; i < data.size * data.size / data.total_parts; i++){
            mapData[Math.floor((data.part * data.size) / data.total_parts) + Math.floor(i / data.size)][i % data.size] = data.chunkData[i];
        }
    }

    function updateCanvas(data){
        var canvas = document.getElementById("indicator_map");
        const c = canvas.getContext('2d');
        c.clearRect(0, 0, canvas.width, canvas.height); // Reseta o canvas
        
        var width = canvas.width;
        var height = canvas.height;
        var squareSize = 4;    // tamanho de cada "pixel" posicional

        for(var i = 0; i < 100; i++){
            for(var j = 0; j < 100; j++){
                if(mapData[j][i] > 0){
                    c.fillStyle = "white";
                    c.fillRect((j-50) * squareSize + canvas.width/2, (50-i) * squareSize + canvas.height/2, squareSize, squareSize);
                }
        
            }
        }
        c.fillStyle = "red";
        c.fillRect((posX * squareSize) + (canvas.width/2), (-posY * squareSize) + (canvas.height/2), squareSize, squareSize);
        
    }
	
</script>
</html>




)rawliteral";