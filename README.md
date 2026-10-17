# ChargeGrid Intelligence

---

Equipe Grupo 03
* Pedro Andreassa Zamai - RM 569318
* Pedro Yoshikado Garcia - RM 570449
* Thiago Maluf Hofmann - RM 569852
* Rafael Ferreirinha Quaresma - RM 571949

---

**Problema e Justificativa**

* **O aumento da frota de veículos elétricos gera um paradoxo:** como carregar todos simultaneamente sem derrubar a rede ou pagar multas altíssimas?
* **Picos de Demanda:** O custo da energia e o estresse na rede não são constantes. Sem gestão, o eletroposto paga caro pela "Ultrapassagem de Demanda".
* **Subutilização:** Em horários de baixo movimento, o capital investido no eletroposto fica parado e sem gerar receita.

---

## Sprint 1

**Proposta: ChargeGrid Intelligence**

Nossa solução atua em duas frentes principais para garantir a viabilidade do negócio:

* **Tarifa Dinâmica:** Implementamos um modelo econômico onde o preço do kWh flutua conforme a demanda. Isso incentiva o motorista a carregar em horários ociosos (preços baixos) e gerencia a carga em horários de pico (preços premium).
* **Controle Ativo (Smart Charging):** Através de algoritmos de software, o sistema limita a potência de saída de cada carregador em tempo real. Se os carros conectados passarem do limite contratado, o sistema distribui a carga para nunca estourar o contrato com a concessionária.

---

**Tecnologias e Sustentabilidade**

* **Hardware (GoodWe):** Uso de inversores e baterias para armazenar energia barata e injetar no sistema durante a Tarifa Premium, maximizando o lucro e a eficiência.
* **Software (Orquestração):** Camada de inteligência que processa os dados de consumo e ajusta os preços e a distribuição de carga automaticamente.
* **Sustentabilidade:** Ao "achatar" a curva de consumo através da tarifa dinâmica, evitamos a sobrecarga da rede urbana e otimizamos o uso da infraestrutura existente, evitando desperdício de energia.

---

## Sprint 2 — Prova de Conceito Funcional (Tinkercad)

Para esta sprint, montamos um protótipo funcional no Tinkercad usando um Arduino Uno para simular o cérebro do nosso eletroposto e provar que a lógica de software funciona.

**O que usamos no circuito:**
* **2 Potenciômetros:** Simulam dois carros na tomada pedindo potência (de 0 a 30 kW cada).
* **1 Botão:** Simula o relógio da concessionária mudando o horário (Normal vs. Pico).
* **LED Verde:** Sistema operando normal com energia barata.
* **LED Amarelo:** Horário de Pico ativo (Tarifa Premium).
* **LED Vermelho:** Alerta de Controle Ativo acionado (Cortando potência para salvar a rede).

---

**Cenários de Teste e Dados Gerados (Monitor Serial)**

O sistema roda a lógica a cada 3 segundos baseado no limite contratado de **45 kW**. Validamos 3 cenários práticos:

**Cenário 1: Horário Normal (Suave)**
Carros puxando pouca carga e fora do horário de pico. Tarifa barata.
* **LED:** Verde
```text
STATUS DA REDE: HORÁRIO NORMAL (Tarifa Reduzida)
Preço Atual do kWh: R$ 0.90
Carro 1 -> Solicitado: 12.00kW | Injetado: 12.00kW
Carro 2 -> Solicitado: 15.00kW | Injetado: 15.00kW
Carga Total do Posto: 27.00 / 45.00 kW
```

**Cenário 2: Tarifa Dinâmica Ativada**
Botão pressionado simulando o horário de pico. O preço do kWh sobe automaticamente.
* **LED:** Amarelo
```text
STATUS DA REDE: HORÁRIO DE PICO (Tarifa Elevada)
Preço Atual do kWh: R$ 2.40
Carro 1 -> Solicitado: 10.00kW | Injetado: 10.00kW
Carro 2 -> Solicitado: 18.00kW | Injetado: 18.00kW
Carga Total do Posto: 28.00 / 45.00 kW
```

**Cenário 3: Controle Ativo Acionado (Limite de Carga)**
Os dois carros vão pro máximo (30 kW cada = 60 kW). Como passa do limite de 45 kW, o sistema corta a potência de cada um para 22.5 kW e salva a rede.
* **LED:** Vermelho (Segurança máxima)
```text
STATUS DA REDE: HORÁRIO DE PICO (Tarifa Elevada)
Preço Atual do kWh: R$ 2.40
Carro 1 -> Solicitado: 30.00kW | Injetado: 22.50kW
Carro 2 -> Solicitado: 30.00kW | Injetado: 22.50kW
Carga Total do Posto: 45.00 / 45.00 kW
ALERTA: CONTROLE ATIVO ACIONADO! EVITANDO MULTA POR ULTRAPASSAGEM DE DEMANDA.
```

---

**Imagem/Diagrama do Circuito**

<img width="1463" height="583" alt="image" src="https://github.com/user-attachments/assets/b7403838-5503-4674-a71a-9da428a7960e" />

---

## Sprint 3 — Prototipagem Funcional e Integração

**1. Diagrama de Blocos de Integração (Hardware e Comunicação)**

    [ Entrada de Dados ]          [ Controlador Central ]               [ Atuadores e Sinalização ]
    +-------------------+        +-----------------------+              +---------------------------+
    | Potenciômetro 1   |------->|                       |------------->| LED Verde (Rede OK)       |
    | (Simula Carro 1)  |        |                       |------------->| LED Amarelo (Hora Pico)   |
    +-------------------+        |                       |------------->| LED Azul (Bateria GoodWe) |
    | Potenciômetro 2   |------->|      Arduino Uno      |------------->| LED Vermelho (Corte/Alert)|
    | (Simula Carro 2)  |        |  (Controlador de Carga|              +---------------------------+
    +-------------------+        |    e Orquestração)    |
    | Botão de Pico     |------->|                       |              [ Ecossistema de Energia ]
    | (Simula Concession|        |                       |              +---------------------------+
    +-------------------+        +-----------------------+              | Inversor Híbrido GoodWe   |
                                     ^               ^                  | Bateria Lynx (Peak Shaving|
                                     |               |                  | Painel Solar Fotovoltaico |
                                    (Modbus/OCPP)   (Sensor TC)         +---------------------------+
                                     v               v                                |
                                 [ Rede Elétrica Concessionária ]<--------------------+

**2. Fluxograma de Decisão do Algoritmo (Lógica de Automação)**

              +-----------------------------------+
               |    Início do Loop de Leitura      |
               +-----------------------------------+
                                 |
                                 v
               +-----------------------------------+
               | Lê Demanda dos EVs (Potenciômetros)|
               |   Calcula Carga Total Solicitada  |
               +-----------------------------------+
                                 |
                     /-----------------------\
                    /   É Horário de Pico?    \
                    \  (Botão Pressionado)   /
                     \-----------------------/
                       /                   \
                 [SIM] /                     \ [NÃO]
                      /                       \
                     v                         v
    +-------------------------------+   +-------------------------------+
    | Ativa Injeção Bateria GoodWe  |   | Injeção Bateria = 0 kW        |
    | (Peak Shaving até 15 kW)      |   | Tarifa Normal = R$ 0.90/kWh   |
    | Tarifa Elevada = R$ 2.40/kWh  |   | LED Verde ACESO               |
    | LED Amarelo + LED Azul ACESOS |   +-------------------------------+
    +-------------------------------+                   |
                   |                                    |
                   +-----------------+------------------+
                                     |
                                     v
               +-------------------------------------------+
               | Calcula Carga Exigida da Concessionária:  |
               |  (Carga Total - Injeção da Bateria)       |
               +-------------------------------------------+
                                     |
                       /---------------------------\
                      /  Carga Exigida > 45 kW ?    \
                      \    (Limite do Contrato)     /
                       \---------------------------/
                         /                       \
                   [SIM] /                         \ [NÃO]
                        /                           \
                       v                             v
    +-----------------------------------+   +-------------------------------+
    | EXECUTA SMART CHARGING:           |   | Carga entregue integralmente  |
    | Aplica fator de corte nos VEs     |   | aos veículos elétricos.       |
    | Trava demanda da rede em 45 kW    |   +-------------------------------+
    | LED Vermelho ACESO + Alerta Serial|
    +-----------------------------------+


**Imagem/Diagrama atualizado do Circuito**

<img width="1521" height="585" alt="image" src="https://github.com/user-attachments/assets/30d30c85-8f3c-4ece-bae9-d2a33b2c0f58" />

---

**Link da Simulação no TinkerCad**

https://www.tinkercad.com/things/itsZgUB0G57-chargegrid-intelligence?sharecode=rEB-arvSr9X3oIVoZrVsbl6MePTDojcdYvYG1lKRfcA

---

**Conexão com os Conteúdos da Disciplina**

* **IoT e Edge Computing:** Programação embarcada em C++ no Arduino Uno manipulando entradas analógicas (potenciômetros) e saídas digitais/PWM sem o uso de rotinas bloqueantes (`millis`).
* **Sustentabilidade e Gestão Energética:** Aplicação prática de *Peak Shaving* e BESS (Battery Energy Storage Systems) para alívio da malha elétrica urbana e redução da pegada de carbono.
* **Automação de Processos:** Tomada de decisão autônoma em milissegundos substituindo a intervenção humana no controle de demanda de potência.

---

## Sprint 4 — Solução Final Integrada GoodWe
---

**1. Visão Geral do Projeto & Desafio GoodWe

O **ChargeGrid Intelligence** é uma plataforma de orquestração energética para eletropostos urbanos de recarga rápida. A solução integra o ecossistema de armazenamento de energia (BESS) e inversores fotovoltaicos da **GoodWe** com algoritmos proprietários de **Smart Charging** e **Tarifa Dinâmica**.

A arquitetura resolve gargalos críticos da mobilidade elétrica urbana:
* **Prevenção de sobrecarga elétrica:** Elimina riscos de queda na rede e multas por ultrapassagem de demanda contratada com as concessionárias.
* **Otimização do ecossistema GoodWe (Inversores Híbridos + Bateria Lynx):** Aplicação automatizada de *Peak Shaving* para injeção de energia barata nos horários de ponta.
* **Telemetria e Ingestão Cloud:** Geração de dados estruturados em formato JSON para sincronização remota via API com o **GoodWe SEMS Portal**.

---

**2. Arquitetura de Integração End-to-End

```text
[ ENTRADAS DE DADOS ]            [ CONTROLADOR LOCAL (EDGE) ]             [ ECOSSISTEMA GOODWE & CLOUD ]
+---------------------+          +--------------------------+             +-------------------------------+
| Potenciômetros (EVs)|--------->|                          |------------>| GoodWe SEMS Portal (Cloud)   |
| Demanda 0 a 60 kW   |          |       Arduino Uno        | (JSON/Serial| Monitoramento de SoC % e BESS |
+---------------------+          | (Algoritmo Smart Charging|  Modbus/REST)-------------------------------+
| Botão Concessionária|--------->|    e Peak Shaving C++)   |                            |
| (Horário Pico/Normal|          +--------------------------+                            v
+---------------------+                       |                   +-------------------------------+
                                              v                   | Gateway Python (Terminal)     |
                                 +--------------------------+     |  - Economia em R$             |
                                 | Actuadores & Sinalização |     |  - Redução de CO₂ (kg)        |
                                 | LEDs (Verde, Amarelo,    |     |  - Processamento da Telemetria|
                                 |       Azul e Vermelho)   |     +-------------------------------+
                                 +--------------------------+
```

---

**3. Integração com Ecossistema GoodWe

1. Inversores Híbridos & Bateria Lynx Home/Commercial: O sistema opera com injeção direta de até 15 kW no barramento durante tarifas elevadas, reduzindo a solicitação junto à concessionária.
2. Proteção Ativa de Ativos (SoC > 20%): O algoritmo C++ implementa uma trava de segurança que bloqueia o Peak Shaving caso o Estado de Carga (SoC) da Bateria Lynx atinja 20%, preservando a química e a vida útil das células de Lítio.
3. Gateway de Telemetria IoT: O controlador gera payloads JSON serializados para permitir a ingestão direta por gateways de borda e integração com serviços Cloud da GoodWe via REST API ou Modbus TCP.

---

**4. Resultados Quantitativos e Qualitativos

Matriz de Comparação Operacional (Cenário de Sobecarga - 60kW solicitado)

| Métrica | Sem ChargeGrid / GoodWe | Com ChargeGrid Intelligence + GoodWe |
| :--- | :--- | :--- |
| **Demanda Exigida da Rede** | 60,00 kW (Ultrapassagem de limite) | **30,00 kW** (Dentro do contrato de 45 kW) |
| **Injeção BESS GoodWe Lynx** | 0,00 kW | **15,00 kW** (Peak Shaving Ativo) |
| **Carga Entregue por Carro** | 30,00 kW (Risco de blackout) | **22,50 kW** (Smart Charging rebalanceado) |
| **Tarifa Ponderada kWh** | R$ 2,40 | **R$ 0,90** (Com uso da bateria/solar) |
| **Multa por Ultrapassagem** | Aplicada (Alto Custo) | **R$ 0,00** (Eliminada 100%) |
| **Redução de CO₂ por Carga** | 0 kg | **~3,82 kg de CO₂ evitado** por ciclo |

---

** 6. Links do Projeto

- Simulação do Circuito no Tinkercad: https://www.tinkercad.com/things/itsZgUB0G57-chargegrid-intelligence?sharecode=rEB-arvSr9X3oIVoZrVsbl6MePTDojcdYvYG1lKRfcA
- Vídeo Demonstrativo no Youtube: https://youtu.be/qGNm51wwaHI
- Repositório do Projeto no GitHub: https://github.com/pedroyoshikadogarcia/Chargegrid-intelligence-Projeto-Sustentavel-Sprint1-2-3-4-FIAP
