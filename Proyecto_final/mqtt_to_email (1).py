import paho.mqtt.client as mqtt
import smtplib
from email.mime.text import MIMEText

# Configuración de tu correo
SMTP_SERVER = "smtp.gmail.com"
SMTP_PORT = 587
EMAIL_USER = "abrilvasstyles@gmail.com"        # tu correo
EMAIL_PASS = "raeu lqux rdfg tyvx"           # contraseña de app (Gmail)
TO_EMAIL = "armandocp2407@gmail.com"      # a quién enviar el mail

# Funciones de MQTT
def on_connect(client, userdata, flags, rc):
    print("Conectado con código", rc)
    client.subscribe("pico/email")

def on_message(client, userdata, msg):
    mensaje = msg.payload.decode()
    print("Mensaje recibido:", mensaje)
    
    # Preparar correo
    email_msg = MIMEText(mensaje)
    email_msg['Subject'] = 'Alerta desde Pico i.MX8'
    email_msg['From'] = EMAIL_USER
    email_msg['To'] = TO_EMAIL

    # Enviar correo
    with smtplib.SMTP(SMTP_SERVER, SMTP_PORT) as server:
        server.starttls()
        server.login(EMAIL_USER, EMAIL_PASS)
        server.send_message(email_msg)
        print("Correo enviado!")

# Conectar al broker MQTT
client = mqtt.Client("EmailBridge")
client.on_connect = on_connect
client.on_message = on_message

client.connect("test.mosquitto.org", 1883, 60)
client.loop_forever()
