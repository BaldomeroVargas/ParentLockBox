#ifndef MEMORY_H
#define MEMORY_H

/* Notes for eeprom

password stored at 75-78
time A stored at 100-103
time B stored at 200-203

*/

unsigned char password[4] = {'0', '0', '0', '0'};
unsigned char timeA[6] = {'x','x','x','x','0','0'};
unsigned char timeB[6] = {'x','x','x','x','0','0'};

//set for demo V1
long timeASeconds = 100;
long timeBSeconds = 36050;

void system_setup(){
	eeprom_write_byte((uint8_t*)75, '0');
	eeprom_write_byte((uint8_t*)76, '0');
	eeprom_write_byte((uint8_t*)77, '0');
	eeprom_write_byte((uint8_t*)78, '0');
	
	eeprom_write_byte((uint8_t*)100, 'x');
	eeprom_write_byte((uint8_t*)101, 'x');
	eeprom_write_byte((uint8_t*)102, 'x');
	eeprom_write_byte((uint8_t*)103, 'x');
	eeprom_write_byte((uint8_t*)104, 'x');
	eeprom_write_byte((uint8_t*)105, 'x');
	
	eeprom_write_byte((uint8_t*)200, 'x');
	eeprom_write_byte((uint8_t*)201, 'x');
	eeprom_write_byte((uint8_t*)202, 'x');
	eeprom_write_byte((uint8_t*)203, 'x');
	eeprom_write_byte((uint8_t*)204, 'x');
	eeprom_write_byte((uint8_t*)205, 'x');
}

void retrieve_all_memory(){
	password[0] = eeprom_read_byte((uint8_t*)75);
	password[1] = eeprom_read_byte((uint8_t*)76);
	password[2] = eeprom_read_byte((uint8_t*)77);
	password[3] = eeprom_read_byte((uint8_t*)78);
	
	timeA[0] = eeprom_read_byte((uint8_t*)100);
	timeA[1] = eeprom_read_byte((uint8_t*)101);
	timeA[2] = eeprom_read_byte((uint8_t*)102);
	timeA[3] = eeprom_read_byte((uint8_t*)103);
	timeA[4] = eeprom_read_byte((uint8_t*)104);
	timeA[5] = eeprom_read_byte((uint8_t*)105);
	
	timeB[0] = eeprom_read_byte((uint8_t*)200);
	timeB[1] = eeprom_read_byte((uint8_t*)201);
	timeB[2] = eeprom_read_byte((uint8_t*)202);
	timeB[3] = eeprom_read_byte((uint8_t*)203);
	timeB[4] = eeprom_read_byte((uint8_t*)204);
	timeB[5] = eeprom_read_byte((uint8_t*)205);
}

void retrieve_password(){
	password[0] = eeprom_read_byte((uint8_t*)75);
	password[1] = eeprom_read_byte((uint8_t*)76);
	password[2] = eeprom_read_byte((uint8_t*)77);
	password[3] = eeprom_read_byte((uint8_t*)78);
}

void overwrite_password(){
	eeprom_write_byte((uint8_t*)75, password[0]);
	eeprom_write_byte((uint8_t*)76, password[1]);
	eeprom_write_byte((uint8_t*)77, password[2]);
	eeprom_write_byte((uint8_t*)78, password[3]);
}

void retrieve_timeA(){
	timeA[0] = eeprom_read_byte((uint8_t*)100);
	timeA[1] = eeprom_read_byte((uint8_t*)101);
	timeA[2] = eeprom_read_byte((uint8_t*)102);
	timeA[3] = eeprom_read_byte((uint8_t*)103);
	timeA[4] = eeprom_read_byte((uint8_t*)104);
	timeA[5] = eeprom_read_byte((uint8_t*)105);
}

void retrieve_timeB(){
	timeB[0] = eeprom_read_byte((uint8_t*)200);
	timeB[1] = eeprom_read_byte((uint8_t*)201);
	timeB[2] = eeprom_read_byte((uint8_t*)202);
	timeB[3] = eeprom_read_byte((uint8_t*)203);
	timeB[4] = eeprom_read_byte((uint8_t*)204);
	timeB[5] = eeprom_read_byte((uint8_t*)205);
}

#endif