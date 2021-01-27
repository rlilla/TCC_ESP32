const char index_html[] = R"rawliteral(
<!DOCTYPE HTML><html>
<HEAD>
	<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css">
	<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js"></script>
	<script src="https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.16.0/umd/popper.min.js"></script>
	<script src="https://maxcdn.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"></script>
 </HEAD>

<SCRIPT>
var teste;
var simulaVolEntrada=0;
var estados=[];
estados[0] = "INICIO";
estados[1] = "AGUARDA TEMP";
estados[2] = "AGUARDA VOL INICIAL";
estados[3] = "MONITORA DIFERENCA";
estados[4] = "AGUARDA ENTRADA";
estados[5] = "FINAL";

function mudaTela(xx){
	var api=new XMLHttpRequest();
	console.log(api);
	api.open("POST", "http://192.168.15.35/changeScreen", true);
	api.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	api.send("tela=" + xx);
}

function alteraModo(modo){
	let modoAtual = "";
	var url="";
	switch(modo){
		case 0:
			url = "http://192.168.15.35/modoManual";
			break;
		case 1:
			url = "http://192.168.15.35/modoAuto";
			break;
		case 2:
			url = "http://192.168.15.35/modoSemi";
			break;
		default:
			url = "http://192.168.15.35/modoNeutro";
	}
	var api=new XMLHttpRequest();
	api.onreadystatechange = function() {
		if (this.readyState == 4 && this.status == 200){
			document.getElementById("modoAtual").innerHTML = modoAtual;
		}
	};
	api.open("GET", url, true);
	api.send();
	
}

function lerParametro(){
	var api=new XMLHttpRequest();
	api.onreadystatechange = function() {
		if (this.readyState == 4 && this.status == 200){
			teste=api.responseText;
			console.log(teste);
			var parAtual=JSON.parse(teste);
			document.getElementById("volEntrada").value = parAtual["volEntrada"];
			document.getElementById("volSaidaIni").value = parAtual["volSaida"];
			document.getElementById("diferenca").value = parAtual["diferenca"];
			document.getElementById("temperatura").value = parAtual["temp"];
		}
	};
	api.open("GET", "http://192.168.15.35/readPar", true);
	api.send();
}

function lerValoresAtuais(){
	var api=new XMLHttpRequest();
	api.onreadystatechange = function() {
		if (this.readyState == 4 && this.status == 200){
			resposta=api.responseText;
			console.log(teste);
			var valoresAtuais=JSON.parse(resposta);
			indice = valoresAtuais["estadoAtual"];
			if(valoresAtuais["modo"]=="AUTOMATICO"){
				document.getElementById("modoAtual").innerHTML = valoresAtuais["modo"] +"\n" + estados[indice];
			}else{
				document.getElementById("modoAtual").innerHTML = valoresAtuais["modo"];
			}
			
			document.getElementById("volEntradaAtual").innerHTML = valoresAtuais["volEntrada"].toFixed(2) + " L";
			document.getElementById("volSaidaAtual").innerHTML = valoresAtuais["volSaida"].toFixed(2) + " L";
			document.getElementById("diferencaAtual").innerHTML = valoresAtuais["diferenca"].toFixed(2) + " L";
			document.getElementById("temperaturaAtual").innerHTML = valoresAtuais["temp"].toFixed(2) + " C";
			console.log(valoresAtuais);	
		}
	}
	api.open("GET", "http://192.168.15.35/readActValues", true);
	api.send();
}

function valvulaEntrada(estado){
	var api=new XMLHttpRequest();
	var url="";
	if(estado==0) url="http://192.168.15.35/desligaEntrada";
	if(estado==1) url="http://192.168.15.35/ligaEntrada";
	api.open("GET", url, true);
	api.send();
}
function valvulaSaida(estado){
	var api=new XMLHttpRequest();
	var url="";
	if(estado==0) url="http://192.168.15.35/desligaSaida";
	if(estado==1) url="http://192.168.15.35/ligaSaida";
	api.open("GET", url, true);
	api.send();
}
function iniciaAuto(){
	var api=new XMLHttpRequest();
	var url="";
	url="http://192.168.15.35/iniciaTrocaAuto";
	api.open("GET", url, true);
	api.send();
}
function interrompe(){
	var api=new XMLHttpRequest();
	var url="";
	url="http://192.168.15.35/interrompeTroca";
	api.open("GET", url, true);
	api.send();
}
function simulacao(estado){
	var api=new XMLHttpRequest();
	var url="";
	if(estado==0) url="http://192.168.15.35/desabilitaSimulacao";
	if(estado==1) url="http://192.168.15.35/habilitaSimulacao";
	api.open("GET", url, true);
	api.send();
}
function semiEntrada(estado){
	var api=new XMLHttpRequest();
	var url="";
	if(estado==0) url="http://192.168.15.35/finalizaEntradaSemi";
	if(estado==1) url="http://192.168.15.35/iniciaEntradaSemi";
	api.open("GET", url, true);
	api.send();
}
function semiSaida(estado){
	var api=new XMLHttpRequest();
	var url="";
	if(estado==0) url="http://192.168.15.35/finalizaSaidaSemi";
	if(estado==1) url="http://192.168.15.35/iniciaSaidaSemi";
	api.open("GET", url, true);
	api.send();
}
function enviarParametro(){
	var parametros={};
	parametros["volEntrada"] = Number(document.getElementById("volEntrada").value);
	parametros["volSaida"] = Number(document.getElementById("volSaidaIni").value);
	parametros["diferenca"] = Number(document.getElementById("diferenca").value);
	parametros["temp"] = Number(document.getElementById("temperatura").value);
	console.log(JSON.stringify(parametros));
	var api=new XMLHttpRequest();
	api.open("POST", "http://192.168.15.35/sendPar", true);
	api.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	api.send("parametros=" + JSON.stringify(parametros));	
}

setInterval(function(){
	lerValoresAtuais();
},1000);



</SCRIPT>

<BODY>
<nav class="navbar navbar-expand-sm bg-dark navbar-light">
	<ul class="navbar-nav">
		<a class="navbar-brand">SmartLub</a>
		<li class="nav-item active">
			<a class="nav-link" onclick=mudaTela(0) href="#">Tela Principal</a>
		</li>
		<li class="nav-item active">
			<a class="nav-link" onclick=mudaTela(1) href="#">Tela Parametros</a>
		</li>
	</ul>
</nav>

<div class="row bg-dark text-white pl-3">
	<div class="col">
		Volume de Entrada<BR>
		<INPUT TYPE=TEXT id="volEntrada"><BR>
		Volume inicial de saida<BR>
		<INPUT TYPE=TEXT id="volSaidaIni"><BR>
		Diferenca Maxima<BR>
		<INPUT TYPE=TEXT id="diferenca"><BR>
		Temperatura<BR>
		<INPUT TYPE=TEXT id="temperatura"><BR><BR><BR>
		<INPUT TYPE=button class="btn btn-warning" value="ENVIAR" onclick="enviarParametro()"> &nbsp; 
		<INPUT TYPE=button class="btn btn-warning" value="RECEBER" onclick="lerParametro()"><BR><BR>
		<BR><BR>
	</div>
	<div class="col">
		<BR><BR><BR>
		<p id="modoAtual" style="font-size:48px">SEM MODO</p>
	</div>
	<div class="col">
		Volume Entrada Atual
		<p id="volEntradaAtual">???????</p>
		Volume Saida Atual
		<p id="volSaidaAtual">???????</p>
		Diferenca Atual
		<p id="diferencaAtual">???????</p>
		Temperatura Atual
		<p id="temperaturaAtual">???????</p>
	</div>
</div>
<div class="row bg-dark">
	<div class="col-sm">
		<INPUT TYPE=button class="btn btn-primary" value="Manual" onclick="alteraModo(0)">
	</div>
	<div class="col-sm">
		<INPUT TYPE=button class="btn btn-primary" value="Abre Valvula Entrada" onclick="valvulaEntrada(1)">
	</div>
	<div class="col-sm">
		<INPUT TYPE=button class="btn btn-primary" value="Fecha Valvula Entrada" onclick="valvulaEntrada(0)">
	</div>
	<div class="col-sm">
		<INPUT TYPE=button class="btn btn-primary" value="Abre Valvula Saida" onclick="valvulaSaida(1)">
	</div>
	<div class="col-sm">
		<INPUT TYPE=button class="btn btn-primary" value="Fecha Valvula Saida" onclick="valvulaSaida(0)">
	</div>	
</div>
<div class="row bg-dark">
	<div class="col-sm">
		<INPUT TYPE=button class="btn btn-primary" value="Auto" onclick="alteraModo(1)">
	</div>
	<div class="col-sm">
		<INPUT TYPE=button class="btn btn-primary" value="Iniciar Auto" onclick="iniciaAuto()">
	</div>
	<div class="col-sm">
		<INPUT TYPE=button class="btn btn-primary" value="Habilita Simulacao" onclick="simulacao(1)">
	</div>
	<div class="col-sm">
		<INPUT TYPE=button class="btn btn-primary" value="Desabilita Simulacao" onclick="simulacao(0)">
	</div>
	<div class="col-sm">
		<INPUT TYPE=button class="btn btn-primary" value="Interrompe" onclick="interrompe()">
	</div>
</div>
<div class="row bg-dark">
	<div class="col-sm">
		<INPUT TYPE=button class="btn btn-primary" value="Semi Auto" onclick="alteraModo(2)">
	</div>
	<div class="col-sm">
		<INPUT TYPE=button class="btn btn-primary" value="Iniciar Semi Entrada" onclick="semiEntrada(1)">
	</div>
	<div class="col-sm">
		<INPUT TYPE=button class="btn btn-primary" value="Iniciar Semi Saida" onclick="semiSaida(1)">
	</div>
	<div class="col-sm">
		
	</div>
	<div class="col-sm">
		
	</div>
</div>
 
</BODY>
</html>
)rawliteral";