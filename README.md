
<h1 align="center" style="color:white; background-color:black">Telemetria OBSat-MCTI</h1>
<h4 align="center">Web Server para testes de telemetria da OBSat</h4>

<p align="center">
	<a href="http://zenith.eesc.usp.br/">
    <img src="https://img.shields.io/badge/Zenith-Embarcados-black?style=for-the-badge"/>
    </a>
    <a href="https://eesc.usp.br/">
    <img src="https://img.shields.io/badge/Linked%20to-EESC--USP-black?style=for-the-badge"/>
    </a>
    <a href="https://github.com/zenitheesc/Telemetria-OBSat-MCTI/blob/main/LICENSE">
    <img src="https://img.shields.io/github/license/zenitheesc/Telemetria-OBSat-MCTI?style=for-the-badge"/>
    </a>
    <a href="https://github.com/zenitheesc/Telemetria-OBSat-MCTI/issues">
    <img src="https://img.shields.io/github/issues/zenitheesc/Telemetria-OBSat-MCTI?style=for-the-badge"/>
    </a>
    <a href="https://github.com/zenitheesc/Telemetria-OBSat-MCTI/commits/main">
    <img src="https://img.shields.io/github/commit-activity/m/zenitheesc/Telemetria-OBSat-MCTI?style=for-the-badge">
    </a>
    <a href="https://github.com/zenitheesc/Telemetria-OBSat-MCTI/graphs/contributors">
    <img src="https://img.shields.io/github/contributors/zenitheesc/Telemetria-OBSat-MCTI?style=for-the-badge"/>
    </a>
    <a href="https://github.com/zenitheesc/Telemetria-OBSat-MCTI/commits/main">
    <img src="https://img.shields.io/github/last-commit/zenitheesc/Telemetria-OBSat-MCTI?style=for-the-badge"/>
    </a>
    <a href="https://github.com/zenitheesc/Telemetria-OBSat-MCTI/issues">
    <img src="https://img.shields.io/github/issues-raw/zenitheesc/Telemetria-OBSat-MCTI?style=for-the-badge" />
    </a>
    <a href="https://github.com/zenitheesc/Telemetria-OBSat-MCTI/pulls">
    <img src = "https://img.shields.io/github/issues-pr-raw/zenitheesc/Telemetria-OBSat-MCTI?style=for-the-badge">
    </a>
</p>

### Formatação JSON e validação
#### Passo 1:
 Verificar se o JSON é valido, sites de analise de json como [este](https://jsonformatter.org/json-viewer) verificam se a estrutura é valida.
#### Passo 2:
 Tendo uma versão valida, use uma ferramenta de comunicação HTTP como [esta](https://reqbin.com/post-online) com a URL: https://obsat.org.br/teste_post/envio.php para validar especificidades.
##### Exemplo:
![http-post](image/http-post-example.png)

Para mais informações sobre os status de validação, veja a sessão `Deu erro, e agora?` neste [link](https://github.com/OBSAT-MCTI/OBSAT-MCTI/tree/main/servidor_testes)

### Utilização do servidor de teste
Para testar a interação com o servidor embarcado da Sonda e os modulos programados no BIPES, é necessario ter uma ESP32 que irá agir como servidor

![http-arch](image/http-arch.png)

Na IDE do Arduino, certifique-se que a biblioteca ESP32 esteja instalada (clique no [link](https://www.youtube.com/watch?v=Ozr1SXwir9g) caso não saiba como instalar). 

Selecione a placa desejada e  carregue o codigo `obsat-server.ino`.

Para fazer testes pelo BIPES, [aqui](https://bipes.net.br/ide/?lang=pt-br#6kgy85) um exemplo de implementaçao, basta estar na rede do servidor e deteminar `http://192.168.4.1/` como rota. Com isso você poderá fazer o seu satélite se comunicar com a placa escolhida, simulando o servidor embarcado na sonda.

Os parametros do ponto de Wi-Fi de acesso são:
- ssid: `obsat-test-server`
- psk: `obsatkey`


### Exemplo de transmissão
```
{
  "equipe": 41,
  "bateria": 24,
  "temperatura": 30,
  "pressao": 1,
  "giroscopio": [42, 90, 30],
  "acelerometro": [10, 3, 4],
  "payload": {
    "sensor_status": "work work work",
    "temperature": 22.8,
    "humidity": 47.5,
    "gyroscope": {
      "x": 1,
      "y": 2,
      "z": 3
    },
    "accelerometer": {
      "x": 0.98,
      "y": 0.05,
      "z": 9.72
    },
    "motion_detected": true
  }
}


```
A transmissão deste JSON resultará na resposta abaixo

```
[STRING: work work work][FLOAT: 22.800][FLOAT: 47.500][INT: 1][INT: 2][INT: 3][FLOAT: 0.980][FLOAT: 0.050][FLOAT: 9.720][BOOL: true](68/90)
```
---
<p align="center">
    <a href="http://zenith.eesc.usp.br">
    <img src="https://img.shields.io/badge/Check%20out-Zenith's Oficial Website-black?style=for-the-badge" />
    </a> 
    <a href="https://www.facebook.com/zenitheesc">
    <img src="https://img.shields.io/badge/Like%20us%20on-facebook-blue?style=for-the-badge"/>
    </a> 
    <a href="https://www.instagram.com/zenith_eesc/">
    <img src="https://img.shields.io/badge/Follow%20us%20on-Instagram-red?style=for-the-badge"/>
    </a>

</p>
<p align = "center">
<a href="zenith@usp.br">zenith@usp.br</a>
</p>
