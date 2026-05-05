#include <Arduino.h>
#include <SD.h>
#include <SPI.h>
#include "AudioGeneratorWAV.h"
#include "AudioOutputI2S.h"
#include "AudioFileSourceSD.h"

#define SD_CS     10
#define SPI_MOSI  11
#define SPI_MISO  13
#define SPI_SCK   12

#define I2S_BCLK  5
#define I2S_LRC   6
#define I2S_DOUT  7

AudioFileSourceSD *file;
AudioGeneratorWAV *wav;
AudioOutputI2S    *out;

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("Iniciant...");

    pinMode(SD_CS, OUTPUT);
    digitalWrite(SD_CS, HIGH);
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);

    if (!SD.begin(SD_CS, SPI, 4000000)) {
        Serial.println("Error: SD no detectada");
        while (true) delay(1000);
    }
    Serial.println("SD OK!");

    file = new AudioFileSourceSD("/audio.wav");
    wav  = new AudioGeneratorWAV();
    out  = new AudioOutputI2S();
    out->SetPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    out->SetGain(0.125);
    wav->begin(file, out);
}

void loop() {
    if (wav->isRunning()) {
        if (!wav->loop()) wav->stop();
    } else {
        Serial.println("WAV finalitzat");
        delay(1000);
    }
}