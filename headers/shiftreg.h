#ifndef SHIFTREG_H
#define SHIFTREG_H
//taken from External_Registers documentation
//modified to simplify project use
//all credit to the original author and all affiliated with the functions

//uses shift registers on upper 4 pins of PORTX
//send 8 bits
#define REG_LOW8 PORTC
void transmit_data_lower ( unsigned char data ) {
	int i;
	
	unsigned char Hold_Reg = REG_LOW8 & 0xF0;	
	
	for ( i = 7; i >= 0; --i){
		// Sets SRCLR to 1 allowing data to be set
		// Also clears SRCLK in preparation of sending data
		REG_LOW8 = 0x08;
		// set SER = next bit of data to be sent.
		REG_LOW8 |= (( data >> i ) & 0x01);
		// set SRCLK = 1. Rising edge shifts next bit of data into the shift register
		REG_LOW8 |= 0x04;
	}
	/* set RCLK = 1. Rising edge copies data from the “Shift” register to the
	“Storage” register*/
	REG_LOW8 |= 0x02;
	// clears all lines in preparation of a new transmission
	REG_LOW8 = 0x00;
	
	REG_LOW8 = Hold_Reg;
}

//uses shift registers on lower 4 pins of PORTX
//send 8 bits
#define REG_UP8 PORTC
void transmit_data_upper ( unsigned char data ) {
	int i;

	unsigned char Hold_Reg = REG_UP8 & 0xF0;
		
	unsigned short up_shift = (short)data << 4;
	for ( i = 7; i >= 0; --i){
		// Sets SRCLR to 1 allowing data to be set
		// Also clears SRCLK in preparation of sending data
		REG_UP8 = 0x80;
		// set SER = next bit of data to be sent.
		REG_UP8 |= (char)(( up_shift >> i ) & 0x10);
		// set SRCLK = 1. Rising edge shifts next bit of data into the shift register
		REG_UP8 |= 0x40;
	}
	/* set RCLK = 1. Rising edge copies data from the “Shift” register to the
	“Storage” register*/
	REG_UP8 |= 0x20;
	// clears all lines in preparation of a new transmission
	REG_UP8 = 0x00;
	
	REG_UP8 = Hold_Reg;
}

//used to daisy chain 2 shift registers
//uses shift registers on Upper 4 pins of PORTX
//send 16 bits
#define REG_UP16 PORTD
void transmit_chain_upper ( unsigned short data ) {
	int i;
	unsigned char Hold_Reg = REG_UP16;
	//sends over to shift
	unsigned long shift_over = (long)data << 4;
	for ( i = 15; i >= 0; --i){
		// Sets SRCLR to 1 allowing data to be set
		// Also clears SRCLK in preparation of sending data
		REG_UP16 = 0x80;
		// set SER = next bit of data to be sent.
		REG_UP16 |= (char)(( shift_over >> i ) & 0x0010);
		// set SRCLK = 1. Rising edge shifts next bit of data into the shift register
		REG_UP16 |= 0x40;
	}
	/* set RCLK = 1. Rising edge copies data from the “Shift” register to the
	“Storage” register*/
	REG_UP16 |= 0x20;
	// clears all lines in preparation of a new transmission
	
	REG_UP16 = Hold_Reg;
}

//used to daisy chain 2 shift registers
//uses shift registers on lower 4 pins of PORTX
//send 16 bitsu
#define REG_LOW16 PORTC
void transmit_chain_lower(unsigned short data){
	int i ;
	unsigned char Hold_Reg = REG_LOW16 & 0xF0;
	for( i = 15; i >= 0; --i){
		// Sets SRCLR to 1 allowing data to be set
		// Also clears SRCLK in preparation of sending data
		REG_LOW16 = 0x08;
		// set SER = next bit of data to be sent.
		REG_LOW16 |= (( data >> i ) & 0x01);
		// set SRCLK = 1. Rising edge shifts next bit of data into the shift register
		REG_LOW16 |= 0x04;
	}
	/* set RCLK = 1. Rising edge copies data from the “Shift” register to the
	“Storage” register*/
	REG_LOW16 |= 0x02;
	// clears all lines in preparation of a new transmission
	REG_LOW16 = 0x00;
	
	REG_LOW16 = Hold_Reg;		
}


#endif // SHIFT_REGISTERS_H