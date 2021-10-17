String s_wifi_IP_status = "connect is lost";
unsigned long ulong_IP = 0;

void check_connect()  {
  //
  if ((WiFi.status() != WL_CONNECTED))  {
    s_wifi_IP_status = "connection is lost";
    b_CONNECT = false;
  } else {
    ulong_IP = WiFi.localIP();
    s_wifi_IP_status = "" + String( (ulong_IP&0x000000ff) >> 0 ) +   
                      "." + String( (ulong_IP&0x0000ff00) >> 8) +
                      "." + String( (ulong_IP&0x00ff0000) >> 16) +
                      "." + String( (ulong_IP&0xff000000) >> 24);
    b_CONNECT = true;
  }
  //
}

void wifi_wireless_disconnect() {
  WiFi.disconnect();
  delay(100);
  check_connect();
}

void wifi_wireless_connect() {
  //
  byte byte_counter = 0;

  if ( b_CONNECT == false )  {
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      byte_counter++;
      if ( byte_counter >= 10 ) {
        break;
      }
    }
  }
  check_connect();
  //
}
