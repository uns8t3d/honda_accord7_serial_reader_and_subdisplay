# Honda Accord 7 Android head unit serial reader #

Main goal is to read climate data (e.g. driver and passenger temperature and status of climate (on/off)) and render it
on OEM Navi subdisplay. Also in this project added RTC (real time clock) unit to display current time on a subdisplay
and IR control to set time easily.

Project contains source code, electrical schema, gerber files for order PCB (for example from JLCPCB) and BOM files with
list of required components.

## Wiring ##
PCB has 2 ports, *Input* and *Subdisplay*, here is pin layout, which also described on silk screen on PCB

**Input**
1. Light - (PWM controlled light of display, could be found on ALARM lamp, negative wire)
2. GND (any ground on car)
3. +5V (helper pin, no need to connect, contains 5v from stabilizer)
4. BAT V (RTC battery +, if connect 5V then battery of RTC will charge **IMPORTANT** make sure to use it only with rechargeble batteries, eg LIR 2032)
5. UART RX (receiver of serial data, connect to TX of climate control of head unit)
6. UART TX (transceiver of serial data, connect to RX of head unit)
7. NC
8. NC
9. NC
10. VCC (connect to 12V, better to use ACC to not drown car battery)


**Subdisplay**
1. VCC (connect to subdisplay PIN Light +)
2. Light - (connect to subdisplay PIN Light -)
3. NC
4. PCINT9 D3 (Connect IR signal here)
5. PCINT3 D11 (Connect to subdisplay PIN MOSI)
6. PCINT5 D13 (Connect to subdisplay PIN CLOCK)
7. PCINT20 D4 (Connect to subdisplay PIN SLAVE)
8. NC
9. +5V (Connect to subdisplay PIN +5V and for IR +5V)
10. GND (Connect to subdisplay PIN GND and for IR GND)


## Subdisplay pinout from left to right ##
1. Light +
2. Light -
3. NC
4. SLAVE
5. MOSI
6. MOSI
7. NC
8. NC
9. +5V
10. GND

Some details and video demonstration of work here - https://driver.top/exp/563813