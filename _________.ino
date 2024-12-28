#include "DHT.h"                    // Library ของ DHT
DHT dht;
#include <Wire.h>
#include <LiquidCrystal_I2C.h>      // Library ของ จอแสดงผล
int analogPin = A0;                 // กำหนดตัวแปรสำหรับเซ็นเซอร์วัดความชื้นในดินในฝั่ง Analog A0
int val = 0;                        // กำหนดตัวแปรเก็บข้อมูล
int buzzer = 2;                     // กำหนดตัวแปรสำหรับลำโพงสัญญาณในฝั่ง Digital 2
int MotorPin2 = 3;                  // กำหนดตัวแปรสำหรับรีเลย์ในฝั่ง Digital 3


// Set the LCD address to 0x27 or 0x3F for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);  // กำหนดหน้าจอแสดงผล Adress 0x27, จำนวนคอร์ลัม, จำนวนแถว

void setup()
{`````````

  pinMode(MotorPin2, OUTPUT);       // กำหนดโหมดการใช้งาน ให้ โมดูลขับมอเตอร์ เป็น OUTPUT (สัญญาณส่งออก)
  pinMode(buzzer , OUTPUT);        // กำหนดโหมดการใช้งาน ให้ ลำโพงสัญญาณ เป็น OUTPUT (สัญญาณส่งออก)
  digitalWrite (buzzer, HIGH);      // สั้งใช้งานในขั้นเตรียมการให้ ลำโพงสัญญาณ มีสถานะเป็น HIGH (ลอจิก 1) ส่งผลให้ลำโพงสัญญาณยังไม่ทำงาน
  Serial.begin(9600);               // กำหนดค่าการอ่านข้อมูลเป็น 9600 baud
  Serial.println();                 // สร้างข้อความเว้นบรรทัด ในกรณีนี้จะไม่ปรากฏข้อความใดๆ จึงทำให้เกิดการเว้นบรรทัดปกติ
  Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F)");   // สร้างข้อความเว้นบรรทัด ในกรณีนี้จะปรากฏข้อความตามที่กำหนดขึ้นมา

  dht.setup(4);                     // กำหนดช่องรับสัญญาณสำหรับ DHT เป็นฝั่ง Digital 4

}
void loop() {
  delay (100);
  delay(dht.getMinimumSamplingPeriod());
  float humidity = dht.getHumidity();             // ดึงค่าความชื้น
  float temperature = dht.getTemperature();       // ดึงค่าอุณหภูมิ
  Serial.print(dht.getStatusString());            // สร้างชุดข้อความ
  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.print(temperature, 1);
  Serial.print("\t\t");
  Serial.println(dht.toFahrenheit(temperature), 1);

  // initialize the LCD
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.setCursor(0, 0); // ไปที่ตัวอักษรที่ 0 แถวที่ 1
  lcd.print("Temp : ");
  lcd.print(temperature);
  lcd.print(" C");

  lcd.setCursor(0, 1); // ไปที่ตัวอักษรที่ 7 แถวที่ 2
  lcd.print("Humi : ");
  lcd.print(humidity);
  lcd.print(" %");
  delay (100);
  val = analogRead(analogPin);                // กำหนดให้ตัวแปร vall อ่านค่า Analog จากช่อง Analog(ในที่นี้คือ A0)
  Serial.print("val = ");                     // สร้างข้อความ val =
  Serial.println(val);                        // สร้างข้อความแบบเว้นบรรทัด โดยจะเป็นการอ่านข้อความจากตัวแปร val
  Serial.print("Humi");
  
  if (val > 600) {                            // สร้างคำสัง if ถ้าตัวแปร val มีค่ามากกว่า 800 ให้ทำตามคำสั่งต่อไปนี้...
    digitalWrite(MotorPin2, HIGH);            // สั่งใช้งานคำสั่ง digital ให้โมดูลขับมอเตอร์มีสถานะเป็น HIGH (ลอจิก 1) ส่งผลให้โมดูลขับมอเตอร์ทำงาน
    digitalWrite(buzzer, LOW);                // สั่งใช้งานคำสั่ง digital ให้ลำโพงสัญญาณมีสถานะเป็น LOW (ลอจิก 0) ส่งผลให้รีเลย์ทำงาน
    delay (300);
    digitalWrite(buzzer, HIGH);               // สั่งใช้งานคำสั่ง digital ให้ลำโพงสัญญาณมีสถานะเป็น HIGH (ลอจิก 1) ส่งผลให้รีเลย์ไม่ทำงาน
    delay (300);
    digitalWrite(buzzer, LOW);
    delay (300);
    digitalWrite(buzzer, HIGH);
    delay (300);
    digitalWrite(buzzer, LOW);
    delay (300);
    digitalWrite(buzzer, HIGH);
    delay (300);
    digitalWrite(buzzer, LOW);
    delay (300);
    digitalWrite(buzzer, HIGH);
    delay (300);
  }
  else {                                      // ถ้าไม่ใช่ (ในที่นี้คือหาก val มีค่าน้อยกว่า 800) ให้ทำตามคำสั่งต่อไปนี้....
    digitalWrite(MotorPin2, LOW);             // สั่งใช้งานคำสั่ง digital ให้โมดูลขับมอเตอร์มีสถานะเป็น LOW (ลอจิก 0) ส่งผลให้โมดูลขับมอเตอร์ไม่ทำงาน
    digitalWrite(buzzer, HIGH);               // สั่งใช้งานคำสั่ง digital ให้ลำโพงสัญญาณมีสถานะเป็น HIGH (ลอจิก 1) ส่งผลให้รีเลย์ไม่ทำงาน
  }
  Serial.println();

  delay (500);

}
