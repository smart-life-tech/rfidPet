#include <SPI.h>
#include <MFRC522.h>
#include <AccelStepper.h>

#define SS_PIN 10
#define RST_PIN 5
#define STEP_PIN 3
#define DIR_PIN 2
#define ENABLE_PIN 5
#define DELAY_TIME 5000 // Time delay in milliseconds

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

void setup()
{
    Serial.begin(9600);
    SPI.begin();        // Init SPI bus
    mfrc522.PCD_Init(); // Init MFRC522
    pinMode(ENABLE_PIN, OUTPUT);
    digitalWrite(ENABLE_PIN, LOW); // Enable the motor driver
    stepper.setMaxSpeed(1000);
    stepper.setAcceleration(500);
    Serial.println("Place your RFID tag near the reader...");
}

void loop()
{
    // Look for new cards
    if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
    {
        return;
    }

    Serial.println("RFID tag detected!");

    // Open the gate (rotate motor to open position)
    stepper.moveTo(200); // Adjust according to your gate mechanism
    while (stepper.distanceToGo() != 0)
    {
        stepper.run();
    }

    delay(DELAY_TIME); // Wait for the delay time

    // Close the gate (rotate motor back to close position)
    stepper.moveTo(0); // Adjust according to your gate mechanism
    while (stepper.distanceToGo() != 0)
    {
        stepper.run();
    }

    // Halt the motor after operation
    digitalWrite(ENABLE_PIN, HIGH); // Disable the motor driver to save power
}
