#include <Arduino.h>

// HTML web page
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<title>AutoCar V2 Inteface</title>
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.7.1/jquery.min.js"></script>
<style>
.motor_controls {
	border: solid black 5px;
    width: 150px;
	height: 594px;
}

.motor_controls_divider {
	border: solid gray 2px;
}

.slider {
  -webkit-appearance: none;
  width: 20%;
  height: 400px;
  background: #d3d3d3;
  outline: none;
  opacity: 0.7;
  -webkit-transition: .2s;
  transition: opacity .2s;
}

.slider:hover {
  opacity: 1;
}

.slider::-webkit-slider-thumb {
  -webkit-appearance: none;
  appearance: none;
  width: 25px;
  height: 25px;
  background: #04AA6D;
  cursor: pointer;
}

.slider::-moz-range-thumb {
  width: 25px;
  height: 25px;
  background: #04AA6D;
  cursor: pointer;
}


.ultrassound_readings {
	text-align: left;
	margin-right: 10px ;
	width: 300
}

.modeButton {
	margin-top: 0px;
	margin-bottom: 0px;
	width:140px;
	height: 50px;
}

.motor_configs {
	border: solid black 5px;
	width: 170px;
}



</style>
</head>
<body>
<table style="margin-left:30px;">
	<tr>
		<th>
			<div class="motor_configs">
				<table align="center">
					<tr>
						<th colspan="2">
							<button class="modeButton" id="motor_mode_auto" type="button"> AUTOMATIC </button>
						</th>
					</tr>
					<tr>
						<th colspan="2">
							<button class="modeButton" id="motor_mode_manual" type="button"> MANUAL </button>
						</th>
					</tr>
					<tr>
						<th>
							<p id="motor_speedIndicator"> Speed: 0.0 cm/s </p>
						</th>
						<th>
							<p id="motor_throttleIndicator"> Throttle: 0% </p>
						</th>
					</tr>
					<tr>
						<th>
							<input class="slider" id="motor_speedSlider" orient="vertical" type="range" step="0.1" value="0" min="0" max="15">
						</th>
						<th>
							<input class="slider" id="motor_throttleSlider"orient="vertical" type="range" step="1" value="0" min="0" max="255">
						</th>
					</tr>
				</table>
			</div>
		</th>
		<th>
			<div class="motor_controls">
				<table align="center">
					<tr>
						<p style="margin-top: 40px; margin-bottom: 35px; text-align: center; font-size: 25px;"> Left Motor </p>
					</tr>
					<tr>
						<th>
							<p id="motorL_speedIndicator"> Speed: 0.0 cm/s </p>
						</th>
						<th>
							<p id="motorL_throttleIndicator"> Throttle: 0% </p>
						</th>
					</tr>
					<tr>
						<th>
							<input class="slider" id="motorL_speedSlider" orient="vertical" type="range" step="0.1" value="0" min="0" max="15">
						</th>
						<th>
							<input class="slider" id="motorL_throttleSlider"orient="vertical" type="range" step="1" value="0" min="0" max="255">
						</th>
					</tr>
				</table>
			</div>
		</th>
		<th>
			<div class="motor_controls">
				<table align="center">
					<tr>
						<p style="margin-top: 40px; margin-bottom: 35px; text-align: center; font-size: 25px;"> Right Motor </p>
					</tr>
					<tr>
						<th>
							<p id="motorR_speedIndicator"> Speed: 0.0 cm/s </p>
						</th>
						<th>
							<p id="motorR_throttleIndicator"> Throttle: 0% </p>
						</th>
					</tr>
					<tr>
						<th>
							<input class="slider" id="motorR_speedSlider" orient="vertical" type="range" step="0.1" value="0" min="0" max="15">
						</th>
						<th>
							<input class="slider" id="motorR_throttleSlider"orient="vertical" type="range" step="1" value="0" min="0" max="255">
						</th>
					</tr>
				</table>
			</div>
		</th>
		<th>
			<div class="ultrassound_readings">
				<p style="margin:0px;" id="ultrassound_front_value"> Sensor Frente: 0.0 cm </p>
				<p style="margin:0px;" id="ultrassound_front_left_value"> Sensor Frente-Esquerda: 0.0 cm </p>
				<p style="margin:0px;" id="ultrassound_front_right_value"> Sensor Frente-Direita: 0.0 cm </p>
				<p style="margin:0px;" id="ultrassound_left_value"> Sensor Esquerda: 0.0 cm </p>
				<p style="margin:0px;" id="ultrassound_right_value"> Sensor Direita: 0.0 cm </p>
				<p style="margin:0px;" id="ultrassound_back_left_value"> Sensor Traseira-esquerda: 0.0 cm </p>
				<p style="margin:0px;" id="ultrassound_back_right_value"> Sensor Traseira-direita: 0.0 cm </p>
				<p style="margin:0px;" id="ultrassound_back_value"> Sensor Traseira: 0.0 cm </p>
			</div>
		</th>
	</tr>
</table>
</body>
<script>
	
	// Configuração de WebSocket

	
	var gateway = `ws://${window.location.hostname}/ws`;
	var websocket;	
	// Init web socket when the page loads
	window.addEventListener('load', onload);

	function onload(event) {
	    initWebSocket();
	}
	
	function initWebSocket() {
	    console.log('Trying to open a WebSocket connection…');
	    websocket = new WebSocket(gateway);
	    websocket.onopen = onOpen;
	    websocket.onclose = onClose;
	    websocket.onmessage = onMessage;
	}

	// When websocket is established, call the getReadings() function
	function onOpen(event) {
	    console.log('Connection opened');
	}

	function onClose(event) {
	    console.log('Connection closed');
	    setTimeout(initWebSocket, 2000);
	}

	// Function that receives the message from the ESP32 with the readings
	function onMessage(event) {
		var data = JSON.parse(event.data);
		updateIndicators(data);
	}
	


	// Sliders do motor esquerdo
    var motorL_slider_speed = document.getElementById("motorL_speedSlider");
	var motorL_slider_throttle = document.getElementById("motorL_throttleSlider");
	// Sliders do motor direito
	var motorR_slider_speed = document.getElementById("motorR_speedSlider");
	var motorR_slider_throttle = document.getElementById("motorR_throttleSlider");
	// Indicadores do motor esquerdo
	var motorL_indicator_speed = document.getElementById("motorL_speedIndicator");
	var motorL_indicator_throttle = document.getElementById("motorL_throttleIndicator");
	// Indicadores do motor direito
	var motorR_indicator_speed = document.getElementById("motorR_speedIndicator");
	var motorR_indicator_throttle = document.getElementById("motorR_throttleIndicator");
	
	// Botoes de modo manual e automatico
	var mode_button_auto = document.getElementById("motor_mode_auto");
	var mode_button_manual = document.getElementById("motor_mode_manual");
	
	// ---------------------------------------------------------------------
	// Atualização e requisição de valores dos motores esquerdo e direito
	// ---------------------------------------------------------------------
	
	// Atualizar visualmente o valor no GUI
	motorL_slider_speed.oninput = function() {
		motorL_indicator_speed.innerHTML = "Speed: " + (this.value*1).toFixed(1) + " cm/s";
	} 
	
	// Atualizar visualmente o valor no GUI
	motorL_slider_throttle.oninput = function() {
		motorL_indicator_throttle.innerHTML = "Throtle: " + ((this.value)*100/255).toFixed(2) + " %";
	} 
	
	// Atualizar visualmente o valor no GUI
	motorR_slider_speed.oninput = function() {
		motorR_indicator_speed.innerHTML = "Speed: " + (this.value*1).toFixed(1) + " cm/s";
	} 
	
	// Atualizar visualmente o valor no GUI
	motorR_slider_throttle.oninput = function() {
		motorR_indicator_throttle.innerHTML = "Throtle: " + ((this.value)*100/255).toFixed(2) + " %";
	} 
	
	// Enviar para o ESP quando soltar o slider
	motorL_slider_speed.onclick = function() {
		// Função de enviar o dado
		websocket.send(JSON.stringify({"toChange":"leftSetpoint","changeTo":motorL_slider_speed.value}));
		console.log(JSON.stringify({"toChange":"leftSetpoint","changeTo":motorL_slider_speed.value}));
	} 
	
	// Enviar para o ESP quando soltar o slider
	motorL_slider_throttle.onclick = function() {
		// Função de enviar o dado
		websocket.send(JSON.stringify({"toChange":"leftThrottle","changeTo":motorL_slider_throttle.value}));
		console.log(JSON.stringify({"toChange":"leftThrottle","changeTo":motorL_slider_throttle.value}));
	} 
	
	// Enviar para o ESP quando soltar o slider
	motorR_slider_speed.onclick = function() {
		// Função de enviar o dado
		websocket.send(JSON.stringify({"toChange":"rightSetpoint","changeTo":motorR_slider_speed.value}));
		console.log(JSON.stringify({"toChange":"rightSetpoint","changeTo":motorR_slider_speed.value}));
		
	} 
	
	// Enviar para o ESP quando soltar o slider
	motorR_slider_throttle.onclick = function() {
		// Função de enviar o dado
		websocket.send(JSON.stringify({"toChange":"rightThrottle","changeTo":motorR_slider_throttle.value}));
		console.log(JSON.stringify({"toChange":"rightThrottle","changeTo":motorR_slider_throttle.value}));
		
	} 
	
	// Enviar para o ESP caso o botão de modo seja pressionado
	mode_button_auto.onclick = function(){
		websocket.send(JSON.stringify({"toChange":"motorMode","changeTo":"auto"}));
		console.log(JSON.stringify({"toChange":"motorMode","changeTo":"auto"}));
	}
	
	// Enviar para o ESP caso o botão de modo seja pressionado
	mode_button_manual.onclick = function(){
		websocket.send(JSON.stringify({"toChange":"motorMode","changeTo":"manual"}));
		console.log(JSON.stringify({"toChange":"motorMode","changeTo":"manual"}));
	}
	
	
	
	
	// ---------------------------------------------------------------------
	// Função de atualização de todos os indicadores após receber uma mensagem
	// ---------------------------------------------------------------------
	
	function updateIndicators(data){
		if(data.motor.mode == "auto"){
				document.getElementById("motor_mode_auto").style.backgroundColor   = "#808080";
				document.getElementById("motor_mode_manual").style.backgroundColor = "#d13d3d";
		} else {
				document.getElementById("motor_mode_auto").style.backgroundColor   = "#5dd13d";
				document.getElementById("motor_mode_manual").style.backgroundColor = "#808080";
		}
		motorL_indicator_speed.innerHTML = "Speed: " + (data.motor.left.speed*1).toFixed(1) + " cm/s";
		motorR_indicator_speed.innerHTML = "Speed: " + (data.motor.right.speed*1).toFixed(1) + " cm/s";
		motorL_indicator_throttle.innerHTML = "Throtle: " + ((data.motor.left.throttle)*100/255).toFixed(2) + " %";
		motorR_indicator_throttle.innerHTML = "Throtle: " + ((data.motor.right.throttle)*100/255).toFixed(2) + " %";
			
		// Atualizar sliders
		motorL_slider_speed.value = data.motor.left.speed;
		motorR_slider_speed.value = data.motor.right.speed;
		motorL_slider_throttle.value = data.motor.left.throttle;
		motorR_slider_throttle.value = data.motor.right.throttle;
		
		// Atualizar ultrassons
		document.getElementById("ultrassound_front_left_value").innerText =  "Sensor Frente Esquerda: " + data.ultrassound.front_left + " cm";
		document.getElementById("ultrassound_front_value").innerText = 		 "Sensor Frente: " + data.ultrassound.front + " cm";
		document.getElementById("ultrassound_front_right_value").innerText = "Sensor Frente Direita: " + data.ultrassound.front_right + " cm";
		document.getElementById("ultrassound_left_value").innerText = 		 "Sensor Esquerda: " + data.ultrassound.left + " cm";
		document.getElementById("ultrassound_right_value").innerText = 		 "Sensor Direita: " + data.ultrassound.right + " cm";
		document.getElementById("ultrassound_back_left_value").innerText =   "Sensor Traseira Esquerda: " + data.ultrassound.back_left + " cm";
		document.getElementById("ultrassound_back_value").innerText = 		 "Sensor Traseira: " + data.ultrassound.back + " cm";
		document.getElementById("ultrassound_back_right_value").innerText =  "Sensor Traseira Direita: " + data.ultrassound.back_right + " cm";
		
	}
	
	
</script>
</html>




)rawliteral";