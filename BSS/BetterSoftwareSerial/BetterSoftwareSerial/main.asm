;
; BetterSoftwareSerial.asm
;
; Created: 30.05.2021 12:16:45
; Author : Michael Hohenstein
;

.def temp = r16

.org 0x0000							; interrupt-vector-table
	rjmp init						; RESET
	reti							; INT0
	rjmp pcint0_handler				; PCINT0
	reti							; TIMER1_COMPA
	reti							; TIMER1_OVF
	reti							; TIMER0_OVF
	reti							; EE_RDY
	reti							; ANA_COMP
	reti							; ADC
	reti							; TIMER1_COMPB
	reti							; TIMER0_COMPA
	reti							; TIMER0_COMPB
	reti							; WDT
	reti							; USI_START
	reti							; USI_OVF

.org 0x000F
init:
									; stack-pointer initialisieren
	ldi temp, HIGH(RAMEND)			; HIGH-Byte der obersten RAM-Adresse
	out SPH, temp
	ldi temp, LOW(RAMEND)			; LOW-Byte der obersten RAM-Adresse
	out SPL, temp

	ldi temp, 0b00011110			; Setze PORTB: PB0 Input (RX), PB1 bis PB4 OUTPUT (TX, RCK, SCK, SER), Rest gleichgültig
	out DDRB, temp
	clr temp						; Setze alle IO-Pins auf 0 (default behaviour)
	out PORTB, temp

	ldi temp, (1<<PCINT0)			; enable PCINT0 on Pin PB0
	out PCMSK, temp

	ldi temp, (1<<PCIF)				; clear pin-change interrupt flag before enabeling interrupts by setting it to a logical one (1)
	out GIFR, temp

	ldi temp, (1<<PCIE)				; activate Pin-Change interrupt
	out GIMSK, temp

	clr temp						; clear Timer counter register 0 
	out TCNT0, temp


; Replace with your application code
start:
    inc r16
    rjmp start

pcint0_handler:
	push temp						; lege den Wert vom Temp-Register auf dem Stack ab
	in temp, SREG					; nimm den Zustand des SREG und ...
	push temp						; ... lege ihn auf den Stack ab
	cli								; deaktiviere globale interrupts
	
	pop temp						; hole den zuvor gesicherten Zustand des SREG vom Stack ab und ...
	out SREG, temp					; ... setze das SREG auf den Zustand vor der Ausführung des Interrupts zurück
	pop temp						; nimm den vor der Ausführung des Interrupts zurückgelegten Wertes vom Temp-Register und setzt es zurück
	reti							; springe zurück an die Stelle vor der Auslösung des Interrupts