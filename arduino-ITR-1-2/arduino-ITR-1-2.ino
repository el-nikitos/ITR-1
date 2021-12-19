/*
#include "BluetoothSerial.h"
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
*/
#include <WiFi.h>
#include <driver/adc.h>

#define CHRG_STAT   19       
#define BTN_TURN    4 
#define BTN_UP      17
#define BTN_DOWN    16
#define SELF_TURN   5
#define LED_GREEN   19
#define LED_RED     18
#define REF_2V5     32
#define BATT        35
//#define ZERO_REF    34

unsigned long   //ulong_time_millis = 0,
                ulong_time_log_millis = 0,
                ulong_time_for_turn_off = 300000,
                ulong_time_turn_off_millis = 0;

int int_time_to_send_log = 2000,
    int_time_to_turn_on = 5000,
    int_time_to_turn_off = 0;

boolean b_CHARGE = false,
        b_BTN_TURN = true,
        b_BTN_UP = false,
        b_BTN_DOWN = false,
        b_BTN_TURN_free = false,
        b_CONNECT = false,
        b_command_to_start = true;
int int_BTN_TURN_counter = 3000,
    int_BTN_UP_counter = 3000,
    int_BTN_DOWN_counter = 3000;

float f_aref = 2.5,
      f_analog_koef = 1.0,
      f_aref_bird = 0,
      f_batt_bird = 0,
      f_zero_bird = 0,
      f_self_pwr = 0,
      f_ain_batt = 0,
      f_batt_koeff = 2.0;

String s_input_buf = "",
       s_input = "",
       s_alarm_msg = "";

//BluetoothSerial SerialBT;
//boolean confirmRequestPending = true;
//String ssid     = "TxBrom_point",
//       password = "164txbrom";

const char* ssid     = "TxBrom_point";
const char* password = "164txbrom";
const char* host = "192.168.1.20";

IPAddress local_IP(192, 168, 1, 51);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);

WiFiClient client;
const int httpPort = 80;


void setup() {
  //
  adc1_config_width( ADC_WIDTH_BIT_10 );
  adc1_config_channel_atten( ADC1_CHANNEL_4, ADC_ATTEN_DB_11 );
  //adc1_config_channel_atten( ADC1_CHANNEL_6, ADC_ATTEN_DB_11 );
  adc1_config_channel_atten( ADC1_CHANNEL_7, ADC_ATTEN_DB_11 );
  
  init_gpio_pins();
  
  ulong_time_log_millis = millis();

  Serial.begin(115200);
  Serial.println("ITR-1, Start!");

  //init_blink( 4 );
  wifi_wireless_connect();

  ulong_time_turn_off_millis = millis();
  digitalWrite( SELF_TURN, HIGH );

  Serial.println("SELF-PWR TURN ON");
  Serial.println( "'help' for list of commands" );

  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }

  //WiFi.begin(ssid, password);
  //
  /*
  SerialBT.enableSSP();
  SerialBT.onConfirmRequest(BTConfirmRequestCallback);
  SerialBT.onAuthComplete(BTAuthCompleteCallback);
  SerialBT.begin("EL-Nikitos BT test"); //Bluetooth device name
  Serial.println("The BT device started, now you can pair it with bluetooth!");
  */
}

void loop() {
  //
  s_alarm_msg = "";
  
  read_gpios_inputs();
  adc_read();
  check_connect();
  
  if ( (millis() - ulong_time_log_millis) > int_time_to_send_log ) {
      
      ulong_time_log_millis = millis();
  }

  serial_word_read();
  s_input = serial_comand_exe( s_input );
  
  //led_blink(5, 2000);
  //client.connect( host, httpPort );
  if (b_BTN_TURN == false) {
    b_BTN_TURN_free = true;
  }
  if ( (b_BTN_TURN == true)&(b_BTN_TURN_free == true) )  {
    b_BTN_TURN_free = false;

    if (b_command_to_start == true) {
      send_motor_start();
    } else  {
      send_motor_stop();
    }
    
  }
  
  led_work_indication();
  
  //
  condition_for_turn_off();
  //
  
}

void init_gpio_pins()  {
  //
  pinMode( CHRG_STAT, INPUT_PULLUP );

  pinMode( BTN_TURN, INPUT_PULLUP );
  pinMode( BTN_UP, INPUT_PULLUP );
  pinMode( BTN_DOWN, INPUT_PULLUP );
  

  pinMode( SELF_TURN, OUTPUT );
  digitalWrite( SELF_TURN, LOW );

  pinMode( LED_GREEN, OUTPUT );
  digitalWrite( LED_GREEN, LOW );

  pinMode( LED_RED, OUTPUT );
  digitalWrite( LED_RED, LOW );

  //pinMode( ZERO_REF, INPUT_PULLDOWN );
  //
}

void read_gpios_inputs()  {
  //
  if ( (digitalRead(CHRG_STAT)) == HIGH )  {
    b_CHARGE = false;
  } else {
    b_CHARGE = true;
  }
// обработка нажатия кнопки ВКЛ/ВЫКЛ
  if ( (digitalRead(BTN_TURN)) == HIGH )  {
    int_BTN_TURN_counter--;
  } else {
    int_BTN_TURN_counter++;
  }
  if (int_BTN_TURN_counter > 3000)  {  int_BTN_TURN_counter = 3000;  }
  if (int_BTN_TURN_counter < 0)  {  int_BTN_TURN_counter = 0;  }

  if ( int_BTN_TURN_counter >= 2500 )  {
    b_BTN_TURN = true;
  } else {
    b_BTN_TURN = false;
  }
// обработка нажатия кнопки ВВЕРХ
  if ( (digitalRead(BTN_UP)) == HIGH )  {
    int_BTN_UP_counter--;
  } else {
    int_BTN_UP_counter++;
  }
  if (int_BTN_UP_counter > 3000)  {  int_BTN_UP_counter = 3000;  }
  if (int_BTN_UP_counter < 0)  {  int_BTN_UP_counter = 0;  }

  if ( int_BTN_UP_counter >= 2500 )  {
    b_BTN_UP = true;
  } else {
    b_BTN_UP = false;
  }
// обработка нажатия кнопки ВНИЗ
  if ( (digitalRead(BTN_DOWN)) == HIGH )  {
    int_BTN_DOWN_counter--;
  } else {
    int_BTN_DOWN_counter++;
  }
  if (int_BTN_DOWN_counter > 3000)  {  int_BTN_DOWN_counter = 3000;  }
  if (int_BTN_DOWN_counter < 0)  {  int_BTN_DOWN_counter = 0;  }

  if ( int_BTN_DOWN_counter >= 2500 )  {
    b_BTN_DOWN = true;
  } else {
    b_BTN_DOWN = false;
  }
//


  if (b_BTN_TURN == true)  {
    ulong_time_turn_off_millis = millis();
  }
  //
}

void adc_read() {
  //f_aref_bird = analogRead( REF_2V5 );
  f_aref_bird = adc1_get_raw( ADC1_CHANNEL_4 );
  //f_zero_bird = adc1_get_raw( ADC1_CHANNEL_6 );
  f_batt_bird = adc1_get_raw( ADC1_CHANNEL_7 );

  if ( f_aref_bird <= 0 ) {
    s_alarm_msg = s_alarm_msg + "uncorrect AREF level; ";
    f_aref_bird = 3100;
  }
  
  //f_analog_koef = f_aref / f_aref_bird;

  f_self_pwr = (1023 * f_aref) / f_aref_bird;
  
  //f_ain_batt = f_batt_koeff * f_analog_koef * analogRead( BATT );
  f_ain_batt = (f_batt_koeff * f_self_pwr * f_batt_bird) / 1023;

  if ( f_ain_batt <= 0 ) {
    s_alarm_msg = s_alarm_msg + "uncorrect BATTERY level; ";
  }
}

void init_blink(int num) {
  //
  for (int i=0; i<num; i++) {
    digitalWrite( LED_GREEN, HIGH );
    delay(500);
    digitalWrite( LED_GREEN, LOW );
    delay(500);
  }

  delay(500);
  //
}

void led_blink(int int_color, float f_fill, int int_period)  {
  
  int relay_time_now = (millis()) % int_period;
  int step_time = float(int_period)*f_fill/100;

  if (relay_time_now < step_time) {
    digitalWrite( int_color, HIGH );
  } else {
    digitalWrite( LED_GREEN, LOW );
    digitalWrite( LED_RED, LOW );
  }
}

void condition_for_turn_off() {
  int_time_to_turn_off = (int)(ulong_time_for_turn_off/1000) - (int)( (millis() - ulong_time_turn_off_millis)/1000 );

  if (int_time_to_turn_off <= 0)  {
    digitalWrite( SELF_TURN, LOW );
  }
}

void led_work_indication()  {
  //
  int int_color_pin = LED_GREEN;
  
  if (b_CONNECT == true)  {
    int_color_pin = LED_GREEN;
  } else {
    int_color_pin = LED_RED;
  }
  
  if (int_time_to_turn_off <= 10)  {
    led_blink( LED_GREEN, 10, 500 );
  } else {
    led_blink( int_color_pin, 5, 2000 );
  }
  //
}

void serial_word_read() {
  //
  if (Serial.available() > 0) {  //если есть доступные данные
    
    // считываем байт
    //int size_buffer = Serial.available();
    char incomingByte = Serial.read();

    if (incomingByte != 10) {
      s_input_buf = s_input_buf + incomingByte;
    } else {
      //Serial.println( s_input_buf );
      s_input = s_input_buf;
      s_input_buf = "";
    }
  }
  //
}
