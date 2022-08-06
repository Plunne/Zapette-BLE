/***************************************************************************************
 *
 * ███████╗ █████╗ ██████╗ ███████╗████████╗████████╗███████╗    ██████╗ ██╗     ███████╗
 * ╚══███╔╝██╔══██╗██╔══██╗██╔════╝╚══██╔══╝╚══██╔══╝██╔════╝    ██╔══██╗██║     ██╔════╝
 *   ███╔╝ ███████║██████╔╝█████╗     ██║      ██║   █████╗      ██████╔╝██║     █████╗  
 *  ███╔╝  ██╔══██║██╔═══╝ ██╔══╝     ██║      ██║   ██╔══╝      ██╔══██╗██║     ██╔══╝  
 * ███████╗██║  ██║██║     ███████╗   ██║      ██║   ███████╗    ██████╔╝███████╗███████╗
 * ╚══════╝╚═╝  ╚═╝╚═╝     ╚══════╝   ╚═╝      ╚═╝   ╚══════╝    ╚═════╝ ╚══════╝╚══════╝
 *
 * Repo : https://gitlab.com/Plunne/zapette_ble
 *
 * BleKeyboard by @T-vK : https://github.com/T-vK/ESP32-BLE-Keyboard
 *
 ***************************************************************************************/

#include <BleKeyboard.h>

#define USE_NIMBLE

/* Diodes */
#define LED_BLE 			2

/* Media keys */
#define MEDIA_PLAYPAUSE 	15

/* Zapette object setup */
BleKeyboard Zapette("Zapette BLE", "PlunneCeleste", 100);

/*****************
 *     SETUP     *
 *****************/

void setup() {

	/* Limit F_CPU to reduce Power Consumption */
	setCpuFrequencyMhz(80);

	/* Start BLE */
	Zapette.begin();

	/* INPUTS */
    pinMode(MEDIA_PLAYPAUSE, INPUT_PULLUP);
	
	/* OUTPUTS */
    pinMode(LED_BLE, OUTPUT);
}

/****************
 *     LOOP     *
 ****************/

void loop() {

    if(Zapette.isConnected())
	{	
		/* Media Keys */
       	sendMediaToBLE(MEDIA_PLAYPAUSE, KEY_MEDIA_PLAY_PAUSE); 
    }
	else
	{
		/* BLE Disconnected */
		digitalWrite(LED_BLE, HIGH);
		delay(100);
		digitalWrite(LED_BLE, LOW);
		delay(100);
		digitalWrite(LED_BLE, HIGH);
		delay(100);
		digitalWrite(LED_BLE, LOW);
		delay(2500);
	}
}

/********************
 *     SEND KEY     *
 *      TO BLE      *
 ********************/

/* Normal Keys */
void sendKeyToBLE(uint8_t pin, uint8_t ascii)
{
	if (!digitalRead(pin)) { 			// Key pressed
		digitalWrite(LED_BLE, HIGH); 	// BLE LED = ON
		Zapette.write(ascii); 			// Send NORMAL key to BLE
		while (!digitalRead(pin)); 		// While key pressed do nothing
		delay(250); 					// 250ms Delay to avoid debounce
		digitalWrite(LED_BLE, LOW); 	// BLE LED = OFF
	}
}

/* Media keys */
void sendMediaToBLE(uint8_t pin, const MediaKeyReport media)
{
	if (!digitalRead(pin)) { 			// Key pressed
		digitalWrite(LED_BLE, HIGH); 	// BLE LED = ON
		Zapette.write(media); 			// Send MEDIA key to BLE
		while (!digitalRead(pin)); 		// While key pressed do nothing
		delay(250); 					// 250ms Delay to avoid debounce
		digitalWrite(LED_BLE, LOW); 	// BLE LED = OFF
	}
}
