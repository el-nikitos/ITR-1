EESchema Schematic File Version 4
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L modules:модуль_esp32_wroom_32_SMD_noSD A1
U 1 1 613347E3
P 3450 1750
F 0 "A1" H 4975 2024 50  0000 C CNN
F 1 "модуль_esp32_wroom_32_SMD_noSD" H 4975 1931 50  0000 C CNN
F 2 "N_A:ESP32_WROOM32_pnp_noSD" H 3450 1750 50  0001 C CNN
F 3 "" H 3450 1750 50  0001 C CNN
	1    3450 1750
	1    0    0    -1  
$EndComp
$Comp
L elements_2020:Светодиод_сдвоенный_KPB-3025SURKCGKC HL1
U 1 1 613366EC
P 8750 2500
F 0 "HL1" H 8900 2849 60  0000 C CNN
F 1 "Светодиод_сдвоенный_KPB-3025SURKCGKC" H 8900 2739 60  0000 C CNN
F 2 "N_VD_HL:Светодиод_SMD_m3025" H 9050 2000 60  0001 C CNN
F 3 "" H 9050 2000 60  0001 C CNN
	1    8750 2500
	1    0    0    -1  
$EndComp
$Comp
L elements:кнопка K1
U 1 1 6133A484
P 8600 1500
F 0 "K1" H 8425 1824 60  0000 C CNN
F 1 "кнопка" H 8425 1714 60  0000 C CNN
F 2 "N_X:Кнопка_FSMSM" H 8475 1550 60  0001 C CNN
F 3 "" H 8475 1550 60  0001 C CNN
	1    8600 1500
	1    0    0    -1  
$EndComp
$Comp
L elements:стабилизатор_нерегулируемый_LM1117S33-SOT223 G1
U 1 1 6133BBFC
P 9750 1250
F 0 "G1" H 10150 1524 50  0000 C CNN
F 1 "стабилизатор_нерегулируемый_LM1117S33-SOT223" H 10150 1431 50  0000 C CNN
F 2 "N_VD_HL:Транзистор_NPN_SOT223" H 9800 1250 50  0001 C CNN
F 3 "" H 9800 1250 50  0001 C CNN
	1    9750 1250
	1    0    0    -1  
$EndComp
$EndSCHEMATC
