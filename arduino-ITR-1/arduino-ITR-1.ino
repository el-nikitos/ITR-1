#define CHRG_STAT   19       
#define BTN         17 
#define SELF_TURN   5
#define LED         18

unsigned long   ulong_time_millis = 0,
                ulong_time_log_millis = 0;

int int_time_to_send_log = 2000,
    int_time_to_turn_on = 5000;

boolean b_CHARGE = false,
        b_BTN = false;

void setup() {
  //
  init_gpio_pins();
  
  ulong_time_log_millis = millis();

  Serial.begin(115200);
  Serial.println("ITR-1, Start!");

  delay( int_time_to_turn_on );

  digitalWrite( SELF_TURN, HIGH );

  Serial.println("SELF-PWR TURN ON");
  //
}

void loop() {
  //
  read_gpios_inputs();
  
  if ( (millis() - ulong_time_log_millis) > int_time_to_send_log ) {
      send_logs();
      ulong_time_log_millis = millis();
  }
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
  //
}

void send_logs()  {
  Serial.print("BTN: ");
  Serial.println( b_BTN );

  Serial.print("CHARGE_STATUS: ");
  Serial.println( b_CHARGE );
  
  Serial.println();
}
