#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define CHRG_STAT   19       
#define BTN         17 
#define SELF_TURN   5
#define LED         18
#define REF_2V5     27
#define BATT        4

unsigned long   //ulong_time_millis = 0,
                ulong_time_log_millis = 0,
                ulong_time_turn_off_millis = 0;

int int_time_to_send_log = 2000,
    int_time_to_turn_on = 5000,
    int_time_for_turn_off = 120000,
    int_time_to_turn_off = 0;

boolean b_CHARGE = false,
        b_BTN = false;

float f_aref_2v5 = 2.5,
      f_analog_koef = 1.0,
      f_aref_bird = 0,
      f_self_pwr = 0,
      f_ain_batt = 0,
      f_batt_koeff = 2;

String s_input_buf = "",
       s_input = "";

//BluetoothSerial SerialBT;
//boolean confirmRequestPending = true;

void setup() {
  //
  init_gpio_pins();
  
  ulong_time_log_millis = millis();

  Serial.begin(115200);
  Serial.println("ITR-1, Start!");

  init_blink( 4 );

  ulong_time_turn_off_millis = millis();
  digitalWrite( SELF_TURN, HIGH );

  Serial.println("SELF-PWR TURN ON");
  Serial.println( "'help' for list of commands" );
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
  read_gpios_inputs();
  adc_read();
  
  if ( (millis() - ulong_time_log_millis) > int_time_to_send_log ) {
      //send_logs();
      ulong_time_log_millis = millis();
  }

  serial_word_read();
  s_input = serial_comand_exe( s_input );
  
  //led_blink(5, 2000);
  led_work_indication();
  
  //
  condition_for_turn_off();
  //
}

void init_gpio_pins()  {
  //
  pinMode( CHRG_STAT, INPUT_PULLUP );

  pinMode( BTN, INPUT_PULLUP );

  pinMode( SELF_TURN, OUTPUT );
  digitalWrite( SELF_TURN, LOW );

  pinMode( LED, OUTPUT );
  digitalWrite( LED, LOW );
  //
}

void read_gpios_inputs()  {
  //
  if ( (digitalRead(CHRG_STAT)) == HIGH )  {
    b_CHARGE = false;
  } else {
    b_CHARGE = true;
  }

  if ( (digitalRead(BTN)) == HIGH )  {
    b_BTN = false;
  } else {
    b_BTN = true;
  }

  if (b_BTN == true)  {
    ulong_time_turn_off_millis = millis();
  }
  //
}

void send_device_status()  {
  Serial.print("TIME(SEC): ");
  Serial.println( (int)( millis()/1000 ) );

  Serial.print("TIME_TO_OFF(SEC): ");
  Serial.println( int_time_to_turn_off );
  
  Serial.print("BTN: ");
  Serial.println( b_BTN );

  Serial.print("CHARGE_STATUS: ");
  Serial.println( b_CHARGE );

  Serial.print("SELF_PWR (V): ");
  Serial.println( f_self_pwr );

  Serial.print("Uref (bird): ");
  Serial.println( f_aref_bird );

  Serial.print("U_BATT(V): ");
  Serial.println( f_ain_batt );
  
  Serial.println();
}


void send_help()  {
  Serial.println( "'help' for list of commands" );
  Serial.println( "'dstatus' for device status" );
  
  Serial.println();
}

void adc_read() {
  f_aref_bird = analogRead( REF_2V5 );
  f_analog_koef = f_aref_2v5 / f_aref_bird;

  f_self_pwr = 4096 * f_aref_2v5 / f_aref_bird;
  
  f_ain_batt = f_batt_koeff * f_analog_koef * analogRead( BATT );
}

void init_blink(int num) {
  //
  for (int i=0; i<num; i++) {
    digitalWrite( LED, HIGH );
    delay(500);
    digitalWrite( LED, LOW );
    delay(500);
  }

  delay(500);
  //
}

void led_blink(float f_fill, int int_period)  {
  
  int relay_time_now = (millis()) % int_period;
  int step_time = float(int_period)*f_fill/100;

  if (relay_time_now < step_time) {
    digitalWrite( LED, HIGH );
  } else {
    digitalWrite( LED, LOW );
  }
}

void condition_for_turn_off() {
  int_time_to_turn_off = (int)(int_time_for_turn_off/1000) - (int)( (millis() - ulong_time_turn_off_millis)/1000 );

  if (int_time_to_turn_off <= 0)  {
    digitalWrite( SELF_TURN, LOW );
  }
}

void led_work_indication()  {
  //
  if (int_time_to_turn_off <= 10)  {
    led_blink(10, 500);
  } else {
    led_blink(5, 2000);
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

String serial_comand_exe( String s_command )  {
  //
  boolean b_fail_command = true;
  
  if (s_command != "") {
    Serial.print(">>");
    Serial.println( s_command );
    
    if (s_command == "dstatus") {
      send_device_status();
      b_fail_command = false;
      //s_input = "";
    }

    if (s_command == "help") {
      send_help();
      b_fail_command = false;
      //s_input = "";
    }

    if ( b_fail_command == true )  {
      Serial.print( "COMMAND '" );
      Serial.print( s_command );
      Serial.println( "' NOT FOUND" );
      Serial.println();
      b_fail_command = false;
      //s_input = "";
    }
  }

  return "";
  //
}
