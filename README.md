<h1 align="center" style="color:white; background-color:black">Telemetria OBSat-MCTI</h1>
<h4 align="center">Web Server para testes de telemetria da OBSat</h4>

<p align="center">
	<a href="http://zenith.eesc.usp.br/">
    <img src="https://img.shields.io/badge/Zenith-Embarcados-black?style=for-the-badge"/>
    </a>
    <a href="https://eesc.usp.br/">
    <img src="https://img.shields.io/badge/Linked%20to-EESC--USP-black?style=for-the-badge"/>
    </a>
    <a href="https://github.com/zenitheesc/obsat-telemetry/blob/main/LICENSE">
    <img src="https://img.shields.io/github/license/zenitheesc/obsat-telemetry?style=for-the-badge"/>
    </a>
    <a href="https://github.com/zenitheesc/obsat-telemetry/issues">
    <img src="https://img.shields.io/github/issues/zenitheesc/obsat-telemetry?style=for-the-badge"/>
    </a>
    <a href="https://github.com/zenitheesc/obsat-telemetry/commits/main">
    <img src="https://img.shields.io/github/commit-activity/m/zenitheesc/obsat-telemetry?style=for-the-badge">
    </a>
    <a href="https://github.com/zenitheesc/obsat-telemetry/graphs/contributors">
    <img src="https://img.shields.io/github/contributors/zenitheesc/obsat-telemetry?style=for-the-badge"/>
    </a>
    <a href="https://github.com/zenitheesc/obsat-telemetry/commits/main">
    <img src="https://img.shields.io/github/last-commit/zenitheesc/obsat-telemetry?style=for-the-badge"/>
    </a>
    <a href="https://github.com/zenitheesc/obsat-telemetry/issues">
    <img src="https://img.shields.io/github/issues-raw/zenitheesc/obsat-telemetry?style=for-the-badge" />
    </a>
    <a href="https://github.com/zenitheesc/obsat-telemetry/pulls">
    <img src = "https://img.shields.io/github/issues-pr-raw/zenitheesc/obsat-telemetry?style=for-the-badge">
    </a>
</p>

## Utilização do servidor de teste (Para participantes)

Na IDE do Arduino, certifique-se que a biblioteca ESP32 esteja instalada (clique no [link](https://www.youtube.com/watch?v=Ozr1SXwir9g) caso não saiba como instalar). 

Selecione a placa desejada e  carregue o codigo `obsat-server.ino`. 

Agora você pode fazer o seu satélite se comunicar com a placa escolhida, simulando o servidor embarcado na sonda.

Os parametros de acesso são:
- ssid: `obsat-test-server`
- psk: `obsatkey`

Basta implementar sua logica para acessar o servidor Web pelo seu satélite. 

O servidor apenas coleta dados de objetos chamados `"payload"`.

#### Exemplo
```
{
  ...
  "payload": {
    "Temperatura": 31,
    "Pressao": 234,
    "pH": 4.213,
    "Valido1": true,
    "Valido2": null,
    "Mensagem": "123 testando!!!"
  }
}

```
A transmissão deste JSON resultará na saida abaixo

```
Carga recebida com sucesso!
Números de elementos encontrados na carga: 6;

[ INT: 31][ INT: 234 ][ FLOAT: 4.213 ][ BOOL: true][ NULL ][CHAR: 123 testando!!!]
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
