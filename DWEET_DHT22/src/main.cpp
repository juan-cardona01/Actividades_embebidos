#include <Arduino.h>
#include <DHT.h>
#include <WiFi.h>

#define DHTPIN 4
#define DHTTYPE DHT22

const char * SSID = "DESKTOP-JLSKF2D 9986" ;
const char * PASS = "7n6363>Y" ;
const char * HOST = "dweet.io" ;
const char * SERVER = "uceva07" ;
const int PORT = 80 ;

DHT dht(DHTPIN, DHTTYPE); // Construyo un objeto de la clase DHT que representa el sensor

void setup() {
  Serial.begin(115200);
  Serial.println("Prueba del DHT22");
  dht.begin(); // Inicializo el sensor
  WiFi.begin(SSID, PASS);

  Serial.print("Intentando conexion: ");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("Conectado");
  Serial.print("Direccion IP: ");
  Serial.println(WiFi.localIP());

  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
}

void loop() {
  delay(2000);
  float Humedad = dht.readHumidity();
  float Temperatura = dht.readTemperature();

  if(isnan(Humedad) || isnan(Temperatura)) // Si hay un error en la lectura
  {
    Serial.println("Error al leer el sensor DHT22");
    return ;
  }

  Serial.print("Humedad: ");
  Serial.println(Humedad);
  Serial.print("Temperatura: ");
  Serial.println(Temperatura);

  WiFiClient Cliente ; // Objeto que realiza peticiones a un socket servidor
  if (!Cliente.connect(HOST, PORT))
  {
    Serial.println("Conexion fallida al servidor");
    delay(1000);
    return ;
  }

  String Request = String ("GET /dweet/for/") + SERVER + "?" + "Temperatura=" + Temperatura + "&Humedad=" + Humedad + "\r\n\r\n" ; // Parámetro de cabecera

  Cliente.print(Request);

  unsigned long Inicio = millis();
  while (Cliente.available() == 0)
  {
    if (millis() - Inicio > 5000)
    {
      Serial.println("Tiempo de espera del servidor agotado");
      Cliente.stop();
      return ;
    }
  }

  while (Cliente.available())
  {
    String Linea = Cliente.readStringUntil('\r'); // Leer string hasta encontrar el caracter enter
    Serial.println(Linea);
  }

  Serial.println("Fin de conexion al servidor");
  Cliente.stop();
  delay(5000);
}