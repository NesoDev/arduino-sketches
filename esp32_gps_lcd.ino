#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <HardwareSerial.h>
#include <TinyGPSPlus.h>

// Dirección I2C del LCD
#define LCD_I2C_ADDRESS 0x27  // Cambia esto a la dirección correcta si es diferente

// Inicializa el LCD con la dirección I2C
LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, 16, 2);  // 16 columnas y 2 filas

#define RXD2 16
#define TXD2 17
HardwareSerial NEO6M(1);
TinyGPSPlus gps;
int isConnected = 0;

void setup() {
  Wire.begin(21, 22);  // Inicia la comunicación I2C con pines SDA en D21 y SCL en D22
  Serial.begin(115200);  // Inicia la comunicación serial con el monitor serial
  NEO6M.begin(9600, SERIAL_8N1, RXD2, TXD2);

  lcd.begin(16, 2);  // Inicializa el LCD
  lcd.backlight();   // Enciende la retroiluminación del LCD
  lcd.clear();       // Borra el contenido inicial del LCD
  lcd.setCursor(0, 0);
  lcd.print("--- Starting ---");
}

void loop() {
  // Espera a que se establezca la conexión con los satélites
  while (NEO6M.available() > 0) {
    if (gps.encode(NEO6M.read())) {
      // Si se ha recibido un nuevo conjunto de datos, muestra la información
      if (gps.location.isUpdated()) {
        isConnected = 1;
        lcd.clear();  // Borra el contenido anterior del LCD
        lcd.setCursor(1, 0);
        lcd.print("Lat: ");
        lcd.print(gps.location.lat(), 6);
        lcd.setCursor(1, 1);
        lcd.print("Lon: ");
        lcd.print(gps.location.lng(), 6);
      }
    }
  }

  // Si no se ha establecido la conexión, muestra el mensaje de espera
  if (isConnected == 0) {
    // No es necesario imprimir nada en el LCD en este caso
    lcd.setCursor(0, 0);
    lcd.print("Connecting......");
    isConnected = -1;
  }
}
