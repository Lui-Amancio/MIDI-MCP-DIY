#include <MIDI.h>
#include <AD_Mux.h>

AD_Mux mux(16); // multiplexador de 16 canais conectado ao pino 16
const int controlPin0 = 2;
const int controlPin1 = 3;
const int controlPin2 = 4;
const int controlPin3 = 5; // adicionando mais uma porta de controle
int sensorValues[10][2]; // matriz para armazenar os valores de cada sensor

void setup() {
  MIDI.begin();
  pinMode(controlPin0, OUTPUT);
  pinMode(controlPin1, OUTPUT);
  pinMode(controlPin2, OUTPUT);
  pinMode(controlPin3, OUTPUT);
  mux.setChannelCount(16); // definindo o número de canais do multiplexador
}

void loop() {
  for (int i = 0; i < 10; i++) {
    // Select the channel to read
    mux.setChannel(i);

    // Read the sensor value
    int sensorValue = mux.analogRead();

    // Store the sensor value in the array
    sensorValues[i][0] = i;
    sensorValues[i][1] = sensorValue;

    // Send the sensor value as a MIDI message
    MIDI.sendControlChange(i, sensorValue >> 7, 1);
    MIDI.sendControlChange(i + 32, sensorValue & 0x7F, 1);
  }

  // Send the sensor values as a MIDI Sysex message
  MIDI.sendSysEx(2 + 20 * 2, (byte*)sensorValues);

  delay(10); // Wait for a short time before reading the sensors again
}
