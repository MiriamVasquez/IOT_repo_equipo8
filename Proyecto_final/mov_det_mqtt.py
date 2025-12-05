import cv2
import time
import paho.mqtt.client as mqtt

# ============================
# CONFIG MQTT
# ============================
BROKER = "test.mosquitto.org"
PORT = 1883
TOPIC = "pico/email"

client = mqtt.Client("PicoSender")
client.connect(BROKER, PORT, 60)

def enviar_mqtt(mensaje):
    client.publish(TOPIC, mensaje)
    print(f"[MQTT] Enviado: {mensaje}")


# ============================
# FUNCIÓN TIMESTAMP
# ============================
def get_timestamp():
    return time.strftime("%Y-%m-%d %H:%M:%S")


# ============================
# INICIO DE OPENCV
# ============================
cap = cv2.VideoCapture(0)
if not cap.isOpened():
    print("Error: No se pudo abrir la cámara")
    exit()

pBackSub = cv2.createBackgroundSubtractorMOG2(
    history=100,
    varThreshold=8,
    detectShadows=False
)

log_file = open("movimiento.log", "a")
print("Detectando movimiento... Presiona ESC para salir.")

frame_count = 0
ruido_promedio = 0
umbral = 0
contador_quieto = 0
cooldown_mqtt = 0   # evita enviar muchos mensajes repetidos


# ============================
# LOOP PRINCIPAL
# ============================
while True:
    ret, frame = cap.read()
    if not ret:
        break

    fgmask = pBackSub.apply(frame, learningRate=0.005)
    frame_count += 1

    _, fgmask = cv2.threshold(fgmask, 70, 255, cv2.THRESH_BINARY)
    movimiento = cv2.countNonZero(fgmask)

    # ============================
    # CALIBRACIÓN DE RUIDO
    # ============================
    if frame_count <= 50:
        ruido_promedio += movimiento
        if cv2.waitKey(30) & 0xFF == 27:
            break
        continue

    if frame_count == 51:
        umbral = int((ruido_promedio / 50) * 1)
        print(f"Umbral calibrado: {umbral}")

    print(f"Movimiento actual: {movimiento}  Umbral: {umbral}")

    # ============================
    # DETECTA MOVIMIENTO
    # ============================
    if movimiento > umbral:
        print(f"Movimiento detectado! ({movimiento} píxeles)")
        log_file.write(f"{get_timestamp()} - Movimiento detectado ({movimiento} píxeles)\n")
        contador_quieto = 0

        # ============================
        # ENVÍA MQTT SOLO CADA 3 SEGUNDOS
        # ============================
        if time.time() > cooldown_mqtt:
            enviar_mqtt(f"Movimiento detectado a las {get_timestamp()}")
            cooldown_mqtt = time.time() + 3  # evita spam de mensajes

    else:
        contador_quieto += 1
        if contador_quieto > 10:
            print("Escena estable nuevamente.")
            log_file.write(f"{get_timestamp()} - Escena estable\n")
            contador_quieto = 0

    if cv2.waitKey(30) & 0xFF == 27:
        break

# ============================
# AL SALIR
# ============================
cap.release()
log_file.close()
client.disconnect()
cv2.destroyAllWindows()
