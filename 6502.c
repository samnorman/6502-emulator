#include <stdio.h>
#include <stdlib.h>

int    zero_flag;
int    sign_flag;
int    overflow_flag;
int    break_flag;
int    decimal_flag;
int    interrupt_flag;
int    carry_flag;
int    clk;

unsigned int program_counter;
unsigned char stack_pointer,status_register,x_reg,y_reg,accumulator;

unsigned char *memory;

int main() {

	memory = (unsigned char *)malloc(65536);

	memory[0x1000] = 0xA9;
	memory[0x1001] = 0xFF;
	memory[0x1002] = 0xA9;
	memory[0x1003] = 0x55;
	
	memory[0x1004] = 0xAD;
	memory[0x1005] = 0x07;
	memory[0x1006] = 0x10;

	memory[0x1007] = 0x44;

	program_counter = 0x1000;

	int a;
	unsigned short addr;

   	for( a = 0; a < 3; a = a + 1 ){
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

	printf("A %x\n", accumulator);
   }


	return 1;

}

void reset_cpu(void)
{
	status_register = 0x20;       
	zero_flag = sign_flag = overflow_flag = break_flag = 0;
	decimal_flag = interrupt_flag = carry_flag = 0;
	stack_pointer = 0xFF;
	program_counter = (memory[0xFFFD] << 8) | memory[0xFFFC];
	clk=0;
	accumulator=x_reg=y_reg=0;
}