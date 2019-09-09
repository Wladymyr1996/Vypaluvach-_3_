EESchema Schematic File Version 4
LIBS:Vypaluvach_3_-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 2
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
L Connector_Generic:Conn_02x05_Odd_Even J?
U 1 1 5D70BFAC
P 2000 2800
AR Path="/5D70BFAC" Ref="J?"  Part="1" 
AR Path="/5D70299B/5D70BFAC" Ref="J7"  Part="1" 
F 0 "J7" H 2050 3217 50  0000 C CNN
F 1 "PANEL" H 2050 3126 50  0000 C CNN
F 2 "Connector_IDC:IDC-Header_2x05_P2.54mm_Horizontal" H 2000 2800 50  0001 C CNN
F 3 "~" H 2000 2800 50  0001 C CNN
	1    2000 2800
	1    0    0    -1  
$EndComp
Wire Wire Line
	1800 2700 1250 2700
Wire Wire Line
	2950 3000 2300 3000
Wire Wire Line
	2300 2900 2950 2900
Wire Wire Line
	2950 2600 2300 2600
Wire Wire Line
	2300 2700 2950 2700
$Comp
L Switch:SW_Push SW1
U 1 1 5D70C16F
P 4650 2050
F 0 "SW1" H 4650 2335 50  0000 C CNN
F 1 "BTN_BACK" H 4650 2244 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH-12mm_Wuerth-430476085716" H 4650 2250 50  0001 C CNN
F 3 "" H 4650 2250 50  0001 C CNN
	1    4650 2050
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW4
U 1 1 5D70C3F5
P 4650 3200
F 0 "SW4" H 4650 3485 50  0000 C CNN
F 1 "BTN_OK" H 4650 3394 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH-12mm_Wuerth-430476085716" H 4650 3400 50  0001 C CNN
F 3 "" H 4650 3400 50  0001 C CNN
	1    4650 3200
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW2
U 1 1 5D70C41F
P 4650 2400
F 0 "SW2" H 4650 2685 50  0000 C CNN
F 1 "BTN_UP" H 4650 2594 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH-12mm_Wuerth-430476085716" H 4650 2600 50  0001 C CNN
F 3 "" H 4650 2600 50  0001 C CNN
	1    4650 2400
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW3
U 1 1 5D70C45B
P 4650 2800
F 0 "SW3" H 4650 3085 50  0000 C CNN
F 1 "BTN_DOWN" H 4650 2994 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH-12mm_Wuerth-430476085716" H 4650 3000 50  0001 C CNN
F 3 "" H 4650 3000 50  0001 C CNN
	1    4650 2800
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5D70D04A
P 4350 3400
AR Path="/5D70D04A" Ref="R?"  Part="1" 
AR Path="/5D70299B/5D70D04A" Ref="R11"  Part="1" 
F 0 "R11" V 4250 3400 50  0000 C CNN
F 1 "4.7K" V 4350 3400 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 4280 3400 50  0001 C CNN
F 3 "~" H 4350 3400 50  0001 C CNN
	1    4350 3400
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 5D70D2E1
P 4150 3400
AR Path="/5D70D2E1" Ref="R?"  Part="1" 
AR Path="/5D70299B/5D70D2E1" Ref="R10"  Part="1" 
F 0 "R10" V 4050 3400 50  0000 C CNN
F 1 "4.7K" V 4150 3400 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 4080 3400 50  0001 C CNN
F 3 "~" H 4150 3400 50  0001 C CNN
	1    4150 3400
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 5D70D33A
P 3950 3400
AR Path="/5D70D33A" Ref="R?"  Part="1" 
AR Path="/5D70299B/5D70D33A" Ref="R9"  Part="1" 
F 0 "R9" V 3850 3400 50  0000 C CNN
F 1 "4.7K" V 3950 3400 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 3880 3400 50  0001 C CNN
F 3 "~" H 3950 3400 50  0001 C CNN
	1    3950 3400
	-1   0    0    1   
$EndComp
$Comp
L Device:R R?
U 1 1 5D70D35E
P 3750 3400
AR Path="/5D70D35E" Ref="R?"  Part="1" 
AR Path="/5D70299B/5D70D35E" Ref="R8"  Part="1" 
F 0 "R8" V 3650 3400 50  0000 C CNN
F 1 "4.7K" V 3750 3400 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 3680 3400 50  0001 C CNN
F 3 "~" H 3750 3400 50  0001 C CNN
	1    3750 3400
	-1   0    0    1   
$EndComp
Wire Wire Line
	4350 3250 4350 3200
Wire Wire Line
	4350 3200 4450 3200
Wire Wire Line
	4450 2800 4150 2800
Wire Wire Line
	4150 2800 4150 3250
Wire Wire Line
	3950 3250 3950 2400
Wire Wire Line
	3950 2400 4450 2400
Wire Wire Line
	3750 3250 3750 2050
Wire Wire Line
	3750 2050 4450 2050
Text Label 2950 2700 2    50   ~ 0
PNL_GND
Text Label 1250 2600 0    50   ~ 0
PNL_LCD_SCL
Wire Wire Line
	1250 3000 1800 3000
Wire Wire Line
	1250 2900 1800 2900
Text Label 1250 2700 0    50   ~ 0
PNL_LCD_SDA
Text Label 2950 2600 2    50   ~ 0
PNL_5V
Wire Wire Line
	1250 2600 1800 2600
Text Label 1250 3000 0    50   ~ 0
PNL_BTN_BACK
Text Label 2950 3000 2    50   ~ 0
PNL_BTN_UP
Text Label 1250 2800 0    50   ~ 0
PNL_BTN_DOWN
Text Label 1250 2900 0    50   ~ 0
PNL_BTN_OK
Entry Wire Line
	2950 2600 3050 2700
Entry Wire Line
	2950 2700 3050 2800
Entry Wire Line
	2950 2900 3050 3000
Entry Wire Line
	2950 3000 3050 3100
Entry Wire Line
	1150 2500 1250 2600
Entry Wire Line
	1150 2600 1250 2700
Entry Wire Line
	1150 2800 1250 2900
Entry Wire Line
	1150 2900 1250 3000
Wire Bus Line
	1150 1450 3050 1450
Connection ~ 3050 1450
Wire Bus Line
	3050 1450 5300 1450
Entry Wire Line
	3050 3100 3150 3200
Entry Wire Line
	3050 2700 3150 2800
Entry Wire Line
	3050 2300 3150 2400
Entry Wire Line
	3050 1950 3150 2050
Text Label 3150 2050 0    50   ~ 0
PNL_BTN_BACK
Text Label 3150 2400 0    50   ~ 0
PNL_BTN_UP
Wire Wire Line
	3150 2050 3750 2050
Connection ~ 3750 2050
Wire Wire Line
	3950 2400 3150 2400
Connection ~ 3950 2400
Text Label 3150 3200 0    50   ~ 0
PNL_BTN_OK
Text Label 3150 2800 0    50   ~ 0
PNL_BTN_DOWN
Wire Wire Line
	3150 3200 4350 3200
Connection ~ 4350 3200
Wire Wire Line
	3150 2800 4150 2800
Connection ~ 4150 2800
Entry Wire Line
	5200 3200 5300 3300
Entry Wire Line
	5200 2800 5300 2900
Entry Wire Line
	5200 2400 5300 2500
Entry Wire Line
	5200 2050 5300 2150
Wire Wire Line
	5200 2050 4850 2050
Wire Wire Line
	4850 2400 5200 2400
Wire Wire Line
	5200 2800 4850 2800
Wire Wire Line
	4850 3200 5200 3200
Text Label 5200 2050 2    50   ~ 0
PNL_5V
Text Label 5200 2400 2    50   ~ 0
PNL_5V
Text Label 5200 2800 2    50   ~ 0
PNL_5V
Text Label 5200 3200 2    50   ~ 0
PNL_5V
Entry Wire Line
	5200 3650 5300 3750
Text Label 5200 3650 2    50   ~ 0
PNL_GND
Wire Wire Line
	5200 3650 4350 3650
Wire Wire Line
	3750 3650 3750 3550
Wire Wire Line
	3950 3550 3950 3650
Connection ~ 3950 3650
Wire Wire Line
	3950 3650 3750 3650
Wire Wire Line
	4150 3550 4150 3650
Connection ~ 4150 3650
Wire Wire Line
	4150 3650 3950 3650
Wire Wire Line
	4350 3550 4350 3650
Connection ~ 4350 3650
Wire Wire Line
	4350 3650 4150 3650
$Comp
L Connector:Conn_01x04_Male J8
U 1 1 5D715776
P 6200 2100
F 0 "J8" H 6172 2073 50  0000 R CNN
F 1 "LCD_Connector" H 6172 1982 50  0000 R CNN
F 2 "Connector_Wire:SolderWirePad_1x04_P5.08mm_Drill1.5mm" H 6200 2100 50  0001 C CNN
F 3 "~" H 6200 2100 50  0001 C CNN
	1    6200 2100
	-1   0    0    -1  
$EndComp
Text Label 5400 2000 0    50   ~ 0
PNL_5V
Text Label 5400 2200 0    50   ~ 0
PNL_LCD_SDA
Text Label 5400 2300 0    50   ~ 0
PNL_LCD_SCL
Text Label 5400 2100 0    50   ~ 0
PNL_GND
Entry Wire Line
	5300 1900 5400 2000
Entry Wire Line
	5300 2000 5400 2100
Entry Wire Line
	5300 2100 5400 2200
Entry Wire Line
	5300 2200 5400 2300
Wire Wire Line
	5400 2000 6000 2000
Wire Wire Line
	6000 2100 5400 2100
Wire Wire Line
	5400 2200 6000 2200
Wire Wire Line
	6000 2300 5400 2300
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J?
U 1 1 5D71A4FD
P 1850 4900
AR Path="/5D71A4FD" Ref="J?"  Part="1" 
AR Path="/5D70299B/5D71A4FD" Ref="J5"  Part="1" 
F 0 "J5" H 1900 5317 50  0000 C CNN
F 1 "OUT_X" H 1900 5226 50  0000 C CNN
F 2 "Connector_IDC:IDC-Header_2x05_P2.54mm_Horizontal" H 1850 4900 50  0001 C CNN
F 3 "~" H 1850 4900 50  0001 C CNN
	1    1850 4900
	1    0    0    1   
$EndComp
NoConn ~ 1650 4900
NoConn ~ 2150 4900
Text Label 1050 4700 0    50   ~ 0
ADPTR_OUT1_X
Text Label 1050 4800 0    50   ~ 0
ADPTR_OUT2_X
Text Label 1050 5000 0    50   ~ 0
ADPTR_OUT3_X
Text Label 1050 5100 0    50   ~ 0
ADPTR_OUT4_X
Wire Wire Line
	1050 5100 1650 5100
Wire Wire Line
	1050 5000 1650 5000
Wire Wire Line
	1050 4800 1650 4800
Wire Wire Line
	1050 4700 1650 4700
Text Label 2850 4700 2    50   ~ 0
ADPTR_OUT1_X
Text Label 2850 4800 2    50   ~ 0
ADPTR_OUT2_X
Text Label 2850 5000 2    50   ~ 0
ADPTR_OUT3_X
Text Label 2850 5100 2    50   ~ 0
ADPTR_OUT4_X
Wire Wire Line
	2850 5100 2150 5100
Wire Wire Line
	2850 5000 2150 5000
Wire Wire Line
	2850 4800 2150 4800
Wire Wire Line
	2850 4700 2150 4700
Entry Wire Line
	2850 4700 2950 4800
Entry Wire Line
	2850 4800 2950 4900
Entry Wire Line
	2850 5100 2950 5200
Entry Wire Line
	2850 5000 2950 5100
Entry Wire Line
	950  4600 1050 4700
Entry Wire Line
	950  4700 1050 4800
Entry Wire Line
	950  4900 1050 5000
Entry Wire Line
	950  5000 1050 5100
$Comp
L Connector_Generic:Conn_01x04 J6
U 1 1 5D72699F
P 1850 5700
F 0 "J6" H 1930 5692 50  0000 L CNN
F 1 "Motor_Connector" H 1930 5601 50  0000 L CNN
F 2 "Connector_Wire:SolderWirePad_1x04_P5.08mm_Drill1.5mm" H 1850 5700 50  0001 C CNN
F 3 "~" H 1850 5700 50  0001 C CNN
	1    1850 5700
	1    0    0    -1  
$EndComp
Text Label 1050 5600 0    50   ~ 0
ADPTR_OUT1_X
Text Label 1050 5700 0    50   ~ 0
ADPTR_OUT2_X
Text Label 1050 5800 0    50   ~ 0
ADPTR_OUT3_X
Text Label 1050 5900 0    50   ~ 0
ADPTR_OUT4_X
Wire Wire Line
	1050 5900 1650 5900
Wire Wire Line
	1050 5800 1650 5800
Wire Wire Line
	1050 5700 1650 5700
Wire Wire Line
	1050 5600 1650 5600
Entry Wire Line
	950  5500 1050 5600
Entry Wire Line
	950  5600 1050 5700
Entry Wire Line
	950  5700 1050 5800
Entry Wire Line
	950  5800 1050 5900
Wire Bus Line
	2950 4400 950  4400
Text Notes 650  750  0    50   ~ 0
Передня панель\nFront panel
Wire Notes Line
	600  700  550  700 
Wire Notes Line
	550  700  550  3900
Wire Notes Line
	550  3900 7000 3900
Wire Notes Line
	7000 3900 7000 700 
Wire Notes Line
	7000 700  1300 700 
Wire Notes Line
	1300 550  600  550 
Wire Notes Line
	600  550  600  800 
Wire Notes Line
	600  800  1300 800 
Wire Notes Line
	1300 550  1300 800 
Text Notes 650  4150 0    50   ~ 0
Адаптер двигуна X\nX Motor adapter
Wire Notes Line
	600  4100 550  4100
Wire Notes Line
	550  4100 550  6350
Wire Notes Line
	550  6350 3750 6350
Wire Notes Line
	3750 6350 3750 4100
Wire Notes Line
	3750 4100 1400 4100
Wire Notes Line
	1400 3950 600  3950
Wire Notes Line
	600  3950 600  4200
Wire Notes Line
	600  4200 1400 4200
Wire Notes Line
	1400 3950 1400 4200
Entry Wire Line
	2950 2800 3050 2900
Wire Wire Line
	2950 2800 2300 2800
Text Label 2950 2600 2    50   ~ 0
PNL_5V
Text Label 2950 2800 2    50   ~ 0
PNL_GND
Text Label 2950 2900 2    50   ~ 0
PNL_GND
Entry Wire Line
	1150 2700 1250 2800
Wire Wire Line
	1250 2800 1800 2800
Wire Bus Line
	1150 1450 1150 2900
Wire Bus Line
	2950 4400 2950 5200
Wire Bus Line
	3050 1450 3050 3100
Wire Bus Line
	950  4400 950  5900
Wire Bus Line
	5300 1450 5300 3750
$EndSCHEMATC
