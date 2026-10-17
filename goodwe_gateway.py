import json
import time


def process_goodwe_telemetry(data_json):
    data = json.loads(data_json)

    co2_evitado_kg = data['injecaoGoodWe'] * 0.085
    economia_reais = data['injecaoGoodWe'] * (data['precoKwh'] - 0.40)

    print("\n--- [ GOODWE SEMS PORTAL - TELEMETRIA EM TEMPO REAL ] ---")
    print(f"Status Inversor GoodWe: OPERACIONAL | Protocolo: Modbus TCP/REST")
    print(f"Estado de Carga Bateria Lynx (SoC): {data['bateriaSoC']:.1f}%")
    print(f"Injeção BESS (Peak Shaving): {data['injecaoGoodWe']:.2f} kW")
    print(f"Demanda Total dos EVs: {data['demandaTotal']:.2f} kW | Exigência da Rede: {data['demandaRede']:.2f} kW")
    print(f"Métricas de Impacto: R$ {economia_reais:.2f} economizados | {co2_evitado_kg:.2f} kg CO2 evitado")

    if data['smartChargingActive']:
        print("ALERT: Smart Charging ativado via GoodWe Management System! Carga rebalancada.")


sample_payload = '{"horarioPico":true,"precoKwh":2.40,"demandaTotal":60.0,"cargaEntregue":45.0,"injecaoGoodWe":15.0,"bateriaSoC":84.95,"geracaoSolar":12.5,"demandaRede":30.0,"carro1_req":30.0,"carro1_real":22.5,"carro2_req":30.0,"carro2_real":22.5,"smartChargingActive":true}'

if __name__ == "__main__":
    process_goodwe_telemetry(sample_payload)