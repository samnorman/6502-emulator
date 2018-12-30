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

unsigned char pages = 0;

unsigned char wrapnumber;

int main()
{	
	memory = (unsigned char *)malloc(65536);

	for(int i=0; i<65536; i++)
		memory[i] = 0xFF;
	
	// Dummy test memory.
	memory[0x00EE] = 0x04;

	memory[0x1000] = 0xE6;
	memory[0x1001] = 0xEE;

	memory[0x1002] = 0xE6;
	memory[0x1003] = 0xEE;

	memory[0x1004] = 0xE6;
	memory[0x1005] = 0xEE;

	program_counter = 0x1000;

	unsigned short addr;

	int ch;

	initscr();

	keypad(stdscr, TRUE);

	while(1){

		// Nucures - wait for user input. 
		ch = getch();

		switch (ch) {
			case KEY_F(1): /* Exit */
				goto EndWhile;
				break;
			case 'a': /* Increment memory pages  */
				pages++;
				break;
			case 's': /* Increment memory pages  */
				pages--;
				break;
		}

		if(ch == 'z') {

			switch (memory[program_counter++])  {

				/* LDX Instrutions */

			 	case 0xA2:  /* LDX Immediate */
				    x_reg = memory[program_counter];
				    program_counter++;   
			        sign_flag = x_reg & 0x80;
			        zero_flag = !(x_reg);
			 	break;

			 	case 0xA6: /* LDX zero page */
	 				// Concatenate number for zero page addressing.
		    		addr = (0x00 << 8) | memory[program_counter];     
		    		x_reg = memory[addr];
		    		program_counter ++; 
		    		sign_flag = x_reg & 0x80;
		    		zero_flag = !(x_reg);
		    	break;

		    	case 0xB6: { /* LDX Zero Page,Y */
		    		// Concatenate number for zero page addressing.
		    		unsigned char yoffset = (memory[program_counter] + y_reg);
		    		addr = (0x00 << 8) | yoffset;     
		    		x_reg = memory[addr];
		    		program_counter ++; 
		    		sign_flag = x_reg & 0x80;
		    		zero_flag = !(x_reg);
	 			break;
	 			}

	 			case 0xAE: /* LDX absolute */
		    		addr = (memory[program_counter+1] << 8) | memory[program_counter];     
		    		x_reg = memory[addr];
		    		program_counter += 2; 
		    		sign_flag = x_reg & 0x80;
		    		zero_flag = !(x_reg);
	 			break;

	 			case 0xBE: { /* LDX Absolute,Y  */

	 				addr = (memory[program_counter+1] << 8) | memory[program_counter];     
	 				addr += y_reg;
		    		x_reg = memory[addr];
		    		program_counter += 2; 
		    		sign_flag = x_reg & 0x80;
		    		zero_flag = !(x_reg);

	 			break;
	 			}

	 			/* LDY Instrutions */

			 	case 0xA0:  /* LDY Immediate */
				    y_reg = memory[program_counter];
				    program_counter++;   
			        sign_flag = y_reg & 0x80;
			        zero_flag = !(y_reg);
			 	break;

			 	case 0xA4: /* LDY zero page */
	 				// Concatenate number for zero page addressing.
		    		addr = (0x00 << 8) | memory[program_counter];     
		    		y_reg = memory[addr];
		    		program_counter ++; 
		    		sign_flag = y_reg & 0x80;
		    		zero_flag = !(y_reg);
		    	break;

		    	case 0xB4: { /* LDY Zero Page,X */
		    		// Concatenate number for zero page addressing.
		    		unsigned char xoffset = (memory[program_counter] + x_reg);
		    		addr = (0x00 << 8) | xoffset;     
		    		y_reg = memory[addr];
		    		program_counter ++; 
		    		sign_flag = y_reg & 0x80;
		    		zero_flag = !(y_reg);
	 			break;
	 			}

	 			case 0xAC: /* LDY absolute */
		    		addr = (memory[program_counter+1] << 8) | memory[program_counter];     
		    		y_reg = memory[addr];
		    		program_counter += 2; 
		    		sign_flag = y_reg & 0x80;
		    		zero_flag = !(y_reg);
	 			break;

	 			case 0xBC: { /* LDY Absolute,X  */

	 				addr = (memory[program_counter+1] << 8) | memory[program_counter];     
	 				addr += x_reg;
		    		y_reg = memory[addr];
		    		program_counter += 2; 
		    		sign_flag = y_reg & 0x80;
		    		zero_flag = !(y_reg);

	 			break;
	 			}

				/* LDA Instrutions */

				case 0xA9:  /* LDA Immediate */
				    accumulator = memory[program_counter];
				    program_counter++;   
			        sign_flag = accumulator & 0x80;
			        zero_flag = !(accumulator);
			 	break;

		        case 0xAD: /* LDA absolute */
		    		addr = (memory[program_counter+1] << 8) | memory[program_counter];     
		    		accumulator = memory[addr];
		    		program_counter += 2; 
		    		sign_flag = accumulator & 0x80;
		    		zero_flag = !(accumulator);
	 			break;

	 			case 0xA5: /* LDA zero page */
	 				// Concatenate number for zero page addressing.
		    		addr = (0x00 << 8) | memory[program_counter];     
		    		accumulator = memory[addr];
		    		program_counter ++; 
		    		sign_flag = accumulator & 0x80;
		    		zero_flag = !(accumulator);
		    	break;

		    	case 0xB5: { /* LDA Zero Page,X */
		    		// Concatenate number for zero page addressing.
		    		unsigned char xoffset = (memory[program_counter] + x_reg);
		    		addr = (0x00 << 8) | xoffset;     
		    		accumulator = memory[addr];
		    		program_counter ++; 
		    		sign_flag = accumulator & 0x80;
		    		zero_flag = !(accumulator);
	 			break;
	 			}

	 			case 0xBD: { /* LDA Absolute,X  */

	 				addr = (memory[program_counter+1] << 8) | memory[program_counter];     
	 				addr += x_reg;
		    		accumulator = memory[addr];
		    		program_counter += 2; 
		    		sign_flag = accumulator & 0x80;
		    		zero_flag = !(accumulator);

	 			break;
	 			}

	 			case 0xB9: { /* LDA Absolute,Y  */

	 				addr = (memory[program_counter+1] << 8) | memory[program_counter];     
	 				addr += y_reg;
		    		accumulator = memory[addr];
		    		program_counter += 2; 
		    		sign_flag = accumulator & 0x80;
		    		zero_flag = !(accumulator);

	 			break;
	 			}

	 			case 0xA1: { /* LDA (Indirect,X)  */

	 				// Add X to the value of A. 
	 				unsigned char xoffset = (memory[program_counter] + x_reg);

	 				// Get the address in 16bit. 
		    		addr = (0x00 << 8) | xoffset;

		    		// Get the address of the value we require.
		    		addr = (memory[addr+1] << 8) | memory[addr];

		    		accumulator = memory[addr];
		    		program_counter++; 
		    		sign_flag = accumulator & 0x80;
		    		zero_flag = !(accumulator);

	 			break;
	 			}

	 			case 0xB1: { /* LDA (Indirect,X)  */

	 				// Zero page address. 
		    		addr = (0x00 << 8) | memory[program_counter];

		    		// Get the address of the value we require.
		    		addr = (memory[addr+1] << 8) | memory[addr];

		    		accumulator = memory[addr + y_reg];
		    		program_counter++; 
		    		sign_flag = accumulator & 0x80;
		    		zero_flag = !(accumulator);

	 			break;
	 			}

	 			/* INC Instrution */
	 			case 0xE6: { /* INC Zero Page  */
	 				// Zero page address. 
		    		addr = (0x00 << 8) | memory[program_counter];
		    		memory[addr]++;
		    		program_counter++; 
		    		sign_flag = accumulator & 0x80;
		    		zero_flag = !(accumulator);
	 			break;
	 			}

	 			case 0x4C: /* JMP absolute */
		      		program_counter = (memory[program_counter+1] << 8) | memory[program_counter];     
				break;
			}

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
	printw("A ");
	printw("0x%02X", accumulator);
	printw("\n");
	printw("\n");
	printw("X ");
	printw("0x%02X", x_reg);
	printw("\n");
	printw("\n");
	printw("Y ");
	printw("0x%02X", y_reg);
	printw("\n");
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

	mvprintw(18,20,"Page 0x%02X", pages);

	// column spacer.
	int ci = 25;

	//row spacer.
	int rj = 1;

	// multiple the page number to display the correct page. 
	int memorylocation = pages * 256;

	for(int i=0; i<16; i++) {
		for(int j=0; j<16; j++) {
			mvprintw(rj,ci,"0x%02X", memory[memorylocation]);

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