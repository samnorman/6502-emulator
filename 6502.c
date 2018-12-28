#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>


// 6502 Registers, Flags, Memory.
int zero_flag;
int sign_flag;
int overflow_flag;
int break_flag;
int decimal_flag;
int interrupt_flag;
int carry_flag;
int clk;

unsigned int program_counter;
unsigned char stack_pointer,status_register,x_reg,y_reg,accumulator;

unsigned char *memory;

// All the Ncurses debug output stuff.
void outputregisters();
void outputmemory();
void updatescreen();

int pages = 0;

int main()
{	
	memory = (unsigned char *)malloc(65536);

	for(int i=0; i<65536; i++)
		memory[i] = 0xFF;
	

	// Dummy test memory.
	memory[0x1000] = 0xA9;
	memory[0x1001] = 0xFF;
	memory[0x1002] = 0xA9;
	memory[0x1003] = 0x55;
	
	memory[0x1004] = 0xAD;
	memory[0x1005] = 0x07;
	memory[0x1006] = 0x10;

	memory[0x1007] = 0x44;

	program_counter = 0x1000;

	unsigned short addr;

	int ch;

	initscr();

	keypad(stdscr, TRUE);

	while(1){

		// Nucures - wait for user input. 
		ch = getch();

		switch (ch) {
			case KEY_RIGHT: /* Exit */
				goto EndWhile;
				break;
			case KEY_LEFT: /* Increment memory pages  */
				pages++;
				break;
		}

		switch (memory[program_counter++])  {
			case 0xA9:  
			    accumulator = memory[program_counter];
			    program_counter++;   
		        sign_flag = accumulator & 0x80;
		        zero_flag = !(accumulator);
		 	break;

			case 0x4c: /* JMP absolute */
	      		program_counter = (memory[program_counter+1] << 8) | memory[program_counter];     
			break;

	        case 0xAD: /* LDA absolute */
	    		addr = (memory[program_counter+1] << 8) | memory[program_counter];     
	    		accumulator = memory[addr];
	    		program_counter += 2; 
	    		sign_flag = accumulator & 0x80;
	    		zero_flag = !(accumulator);
	    		printf("Address %x\n", addr);
 			break;
	}

	updatescreen();
	
	}

	EndWhile:

	endwin();
	return 0;
}

void updatescreen()
{
		clear();
		outputregisters();
		outputmemory();
}

void outputregisters(){
	printw("Accumulator ");
	printw("0x%x", accumulator);
	printw("\n");
	printw("PC ");
	printw("0x%x", program_counter);
}

void outputmemory() 
{
	mvprintw(0,25,"%s","0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F");
	mvprintw(1,20,"%s","0x00");
	mvprintw(2,20,"%s","0x10");
	mvprintw(3,20,"%s","0x20");
	mvprintw(4,20,"%s","0x30");
	mvprintw(5,20,"%s","0x40");
	mvprintw(6,20,"%s","0x50");
	mvprintw(7,20,"%s","0x60");
	mvprintw(8,20,"%s","0x70");
	mvprintw(9,20,"%s","0x80");
	mvprintw(10,20,"%s","0x90");
	mvprintw(11,20,"%s","0xA0");
	mvprintw(12,20,"%s","0xB0");
	mvprintw(13,20,"%s","0xC0");
	mvprintw(14,20,"%s","0xD0");
	mvprintw(15,20,"%s","0xE0");
	mvprintw(16,20,"%s","0xF0");

	mvprintw(18,20,"Page %x", pages);

	// column spacer.
	int ci = 25;

	//row spacer.
	int rj = 1;

	// multiple the page number to display the correct page. 
	int memorylocation = pages * 256;

	for(int i=0; i<16; i++) {
		for(int j=0; j<16; j++) {
			mvprintw(rj,ci,"0x%x", memory[memorylocation]);

			// move column to the next space.
			ci+=5;

			// increment the memory location.
			memorylocation++;
		}
		// reset the column spacer.
		ci = 25;

		// increment the row.
		rj++;
	}

}