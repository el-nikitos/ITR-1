String serial_comand_exe( String s_command )  {
  //
  boolean b_fail_command = true;
  
  if (s_command != "") {
    Serial.print(">>");
    Serial.println( s_command );

    if (s_command == "help") {
      send_help();
      b_fail_command = false;
      //s_input = "";
    }
    
    if (s_command == "dstatus") {
      send_device_status();
      b_fail_command = false;
      //s_input = "";
    }

    if (s_command == "wstatus") {
      send_wireless_status();
      b_fail_command = false;
    }

    if (s_command == "disconnect") {
      wifi_wireless_disconnect();
      send_wireless_status();
      b_fail_command = false;
    }

    if (s_command == "connect") {
      wifi_wireless_connect();
      send_wireless_status();
      b_fail_command = false;
    }

    if (s_command == "mstart") {
      send_motor_start();
      b_fail_command = false;
    }

    if (s_command == "mstop") {
      send_motor_stop();
      b_fail_command = false;
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

void send_device_status()  {
  Serial.print("TIME(SEC): ");
  Serial.println( (int)( millis()/1000 ) );

  Serial.print("TIME_TO_OFF(SEC): ");
  Serial.println( int_time_to_turn_off );
  
  Serial.print("BTN: ");
  Serial.println( b_BTN );

  Serial.print("BTN if free: ");
  Serial.println( b_BTN_free );

  Serial.print("CHARGE_STATUS: ");
  Serial.println( b_CHARGE );

  Serial.print("COMMAND TO START: ");
  Serial.println( b_command_to_start );

  Serial.print("ALARMS: ");
  if (s_alarm_msg == "")  {
    Serial.println( "no alarms" );
  } else  {
    Serial.println( s_alarm_msg );
  }

  Serial.print("U_REF(V): ");
  Serial.println( f_self_pwr );

  Serial.print("U_REF(bird): ");
  Serial.println( f_aref_bird );

  Serial.print("U_BATT(V): ");
  Serial.println( f_ain_batt );

  Serial.print("U_BATT(bird): ");
  Serial.println( f_batt_bird );
  /*
  Serial.print("U_ZERO(bird): ");
  Serial.println( f_zero_bird );
  */
  
  Serial.println();
}

void send_help()  {
  Serial.println( "'help' for list of commands" );
  Serial.println( "'dstatus' for device status" );
  Serial.println( "'wstatus' for wireless connect status" );
  Serial.println( "'disconnect' disconnect from current wireless connection" );
  Serial.println( "'connect' connect to current SSID" );
  Serial.println( "'mstart' start the motor" );
  Serial.println( "'mstop' stop the motor" );
  
  Serial.println();
}

void send_wireless_status()  {
  
  Serial.print("SSID_FOR_CONNECT: ");
  Serial.println( ssid );

  Serial.print("PASSWORD_FOR_CONNECT: ");
  Serial.println( password );

  Serial.print("IP_STATUS: ");
  Serial.println( s_wifi_IP_status );

  Serial.println();
}
