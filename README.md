# Dispenser Tap Water Control

## Deskripsi
Sistem dirancang untuk mengontrol Tap secara otomatis ketika ada gelas/benda didekatnya. Sistem ini juga memonitor ketinggian air, ketika air habis, buzzer akan berbunyi. menandakan air sudah habis. informasi ketinggian air ditampilkan di LCD.

## Fitur
- Deteksi ketinggian air menggunakan sensor HC-SR04.
- Membuka dan menutup keran otomatis berdasarkan deteksi benda.
- Tampilan informasi di LCD I2C.
- Notifikasi level air rendah menggunakan buzzer.

## Hardware
- Arduino Uno
- HC-SR04 (2)
- LCD I2C
- Buzzer
- Servo

## Software
- PlatformIO

## Diagram Koneksi
- **HC-SR04 (Sensor 1):**
  - TRIG: Pin 5
  - ECHO: Pin 6
- **HC-SR04 (Sensor 2):**
  - TRIG: Pin 8
  - ECHO: Pin 9
- **Servo Motor:**
  - Sinyal: Pin 3
- **Buzzer:**
  - VCC/IO: Pin 4