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

/* Keys */

// Laser
#define LASER_SW			27

// Media keys
#define MEDIA_PLAY			15

/* Design v0.3
MEDIA_PREV			2 <- pulldown only
MEDIA_NEXT			12 <- pulldown only
*/

#define MEDIA_VOL_UP		19
#define MEDIA_VOL_DOWN		18
#define MEDIA_VOL_MUTE		5

/* Design v0.3
MEDIA_VOL_UP    32
MEDIA_VOL_DOWN    33
MEDIA_VOL_MUTE    25 <- new in replacement of GPIO35 external pullup needed
*/

// Arrow keys
// UP_ARROW			0 <- cannot be used (boot reserved)
#define DOWN_ARROW			17
#define LEFT_ARROW			16
#define RIGHT_ARROW			26
#define OK_RETURN			4

// Switch keys
#define SW_KEY1        32
#define SW_KEY2       33

/* Design v0.3
SW_KEY1				19
SW_KEY2				18
SW_KEY3				5
*/

/* Diodes */
#define LED_BLE 			13
#define LASER_DIODE 		14

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
    pinMode(LASER_SW, INPUT_PULLUP);
	
    pinMode(MEDIA_PLAY, INPUT_PULLUP);
    // pinMode(MEDIA_PREV, INPUT);
    // pinMode(MEDIA_NEXT, INPUT);
    pinMode(MEDIA_VOL_UP, INPUT_PULLUP);
    pinMode(MEDIA_VOL_DOWN, INPUT_PULLUP);
    pinMode(MEDIA_VOL_MUTE, INPUT_PULLUP);

	// pinMode(UP_ARROW, INPUT_PULLUP);
    pinMode(DOWN_ARROW, INPUT_PULLUP);
    pinMode(LEFT_ARROW, INPUT_PULLUP);
    pinMode(RIGHT_ARROW, INPUT_PULLUP);
    pinMode(OK_RETURN, INPUT_PULLUP);

	pinMode(SW_KEY1, INPUT_PULLUP);
	pinMode(SW_KEY2, INPUT_PULLUP);
	//pinMode(SW_KEY3, INPUT_PULLUP); // v0.3
	
	/* OUTPUTS */
    pinMode(LED_BLE, OUTPUT);
    pinMode(LASER_DIODE, OUTPUT);
}

/****************
 *     LOOP     *
 ****************/

void loop() {

    if(Zapette.isConnected())
	{
		if(!digitalRead(LASER_SW)) {
			digitalWrite(LASER_DIODE, HIGH);
		} else {
			digitalWrite(LASER_DIODE, LOW);
		}
		/* Media Keys */
       	sendMediaToBLE(MEDIA_PLAY, KEY_MEDIA_PLAY_PAUSE); 
       	// sendMediaPullDownToBLE(MEDIA_PREV, KEY_MEDIA_PREVIOUS_TRACK); 
       	// sendMediaPullDownToBLE(MEDIA_NEXT, KEY_MEDIA_NEXT_TRACK); 
       	sendMediaToBLE(MEDIA_VOL_UP, KEY_MEDIA_VOLUME_UP); 
       	sendMediaToBLE(MEDIA_VOL_DOWN, KEY_MEDIA_VOLUME_DOWN); 
       	sendMediaToBLE(MEDIA_VOL_MUTE, KEY_MEDIA_MUTE); 
		/* Normal Keys */
       	// sendKeyToBLE(UP_ARROW, KEY_UP_ARROW); 
       	sendKeyToBLE(DOWN_ARROW, KEY_DOWN_ARROW); 
       	sendKeyToBLE(LEFT_ARROW, KEY_LEFT_ARROW); 
       	sendKeyToBLE(RIGHT_ARROW, KEY_RIGHT_ARROW); 
       	sendKeyToBLE(OK_RETURN, KEY_RETURN);
		/* Switch keys */
		sendKeyToBLE(SW_KEY1, 'f');
		sendKeyToBLE(SW_KEY2, KEY_F5);
		// sendKeyToBLE(SW_KEY3, KEY_ESC);
		delay(150);
		Zapette.releaseAll();
		digitalWrite(LED_BLE, LOW);  // BLE LED = OFF
		
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
		Zapette.press(ascii); 			// Send NORMAL key to BLE
	}
}

/* Media keys */
void sendMediaToBLE(uint8_t pin, const MediaKeyReport media)
{
	if (!digitalRead(pin)) { 			// Key pressed
		digitalWrite(LED_BLE, HIGH); 	// BLE LED = ON
		Zapette.press(media); 			// Send MEDIA key to BLE
	}
}

/* Maybe v0.3 */
void sendMediaPullDownToBLE(uint8_t pin, const MediaKeyReport media)
{
	if (digitalRead(pin)) { 			// Key pressed
		digitalWrite(LED_BLE, HIGH); 	// BLE LED = ON
		Zapette.press(media); 			// Send MEDIA key to BLE
	}
}
