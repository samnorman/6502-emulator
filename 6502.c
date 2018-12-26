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

	program_counter = 0x1000;

	int a;

   	for( a = 0; a < 2; a = a + 1 ){
      switch (memory[program_counter++])  {
		case 0xA9:  
		    accumulator = memory[program_counter];
		    program_counter++;   
	        sign_flag = accumulator & 0x80;
	        zero_flag = !(accumulator);
	 	break;

		case 1:
        
        break;
	}

	printf("%x\n", accumulator);
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