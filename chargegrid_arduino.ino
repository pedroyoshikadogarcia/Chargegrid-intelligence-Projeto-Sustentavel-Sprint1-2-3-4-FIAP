const int POT_CARRO1 = A0;
const int POT_CARRO2 = A1;
const int BOTAO_PICO = 2;

const int LED_AZUL = 10;
const int LED_VERDE = 11;
const int LED_AMARELO = 12;
const int LED_VERMELHO = 13;

const float LIMITE_CONTRATO = 45.0;
const float BATERIA_MAX_POTENCIA = 15.0;

float bateriaSoC = 85.0; 
float geracaoSolar = 12.5; 

bool horarioPico = false;
int estadoBotaoAnt = LOW;

unsigned long tempoAnterior = 0;
const long intervaloExibicao = 3000;

void setup() {
  Serial.begin(9600);
  pinMode(BOTAO_PICO, INPUT);
  pinMode(LED_AZUL, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  
  Serial.println("{\"system\":\"ChargeGrid Intelligence v4.0\",\"goodwe_status\":\"SEMS_CONNECTED\"}");
}

void loop() {
  int estadoBotao = digitalRead(BOTAO_PICO);
  if (estadoBotao == HIGH && estadoBotaoAnt == LOW) {
    horarioPico = !horarioPico;
    delay(150);
  }
  estadoBotaoAnt = estadoBotao;

  float demandaCarro1 = map(analogRead(POT_CARRO1), 0, 1023, 0, 30);
  float demandaCarro2 = map(analogRead(POT_CARRO2), 0, 1023, 0, 30);
  float totalSolicitado = demandaCarro1 + demandaCarro2;

  float cargaRealCarro1 = demandaCarro1;
  float cargaRealCarro2 = demandaCarro2;
  float totalEntregue = totalSolicitado;
  bool controleAtivoAcionado = false;
  
  if (totalSolicitado > LIMITE_CONTRATO) {
    controleAtivoAcionado = true;
    float fatorCorte = LIMITE_CONTRATO / totalSolicitado;
    cargaRealCarro1 = demandaCarro1 * fatorCorte;
    cargaRealCarro2 = demandaCarro2 * fatorCorte;
    totalEntregue = LIMITE_CONTRATO;
  }

  float injecaoBateria = 0.0;
  bool bateriaAtiva = false;

  if (horarioPico && totalEntregue > 0 && bateriaSoC > 20.0) {
    bateriaAtiva = true;
    injecaoBateria = (totalEntregue < BATERIA_MAX_POTENCIA) ? totalEntregue : BATERIA_MAX_POTENCIA;
  }

  float cargaRede = totalEntregue - injecaoBateria;
  float precoKwh = horarioPico ? 2.40 : 0.90;

  digitalWrite(LED_AZUL, bateriaAtiva ? HIGH : LOW);
  digitalWrite(LED_AMARELO, horarioPico ? HIGH : LOW);
  digitalWrite(LED_VERMELHO, controleAtivoAcionado ? HIGH : LOW);
  digitalWrite(LED_VERDE, (!horarioPico && !controleAtivoAcionado) ? HIGH : LOW);

  unsigned long tempoAtual = millis();
  if (tempoAtual - tempoAnterior >= intervaloExibicao) {
    tempoAnterior = tempoAtual;

    if (bateriaAtiva) {
      bateriaSoC -= 0.5;
      if (bateriaSoC < 0) bateriaSoC = 0;
    }

    Serial.print("{\"horarioPico\":"); Serial.print(horarioPico ? "true" : "false");
    Serial.print(",\"precoKwh\":"); Serial.print(precoKwh);
    Serial.print(",\"demandaTotal\":"); Serial.print(totalSolicitado);
    Serial.print(",\"cargaEntregue\":"); Serial.print(totalEntregue);
    Serial.print(",\"injecaoGoodWe\":"); Serial.print(injecaoBateria);
    Serial.print(",\"bateriaSoC\":"); Serial.print(bateriaSoC);
    Serial.print(",\"geracaoSolar\":"); Serial.print(geracaoSolar);
    Serial.print(",\"demandaRede\":"); Serial.print(cargaRede);
    Serial.print(",\"carro1_req\":"); Serial.print(demandaCarro1);
    Serial.print(",\"carro1_real\":"); Serial.print(cargaRealCarro1);
    Serial.print(",\"carro2_req\":"); Serial.print(demandaCarro2);
    Serial.print(",\"carro2_real\":"); Serial.print(cargaRealCarro2);
    Serial.print(",\"smartChargingActive\":"); Serial.print(controleAtivoAcionado ? "true" : "false");
    Serial.println("}");
  }
}
