#include <stdint.h>
#include <uart.hpp>

extern "C" void kernel_main(){
	uart_init();
	uart_write("welcome to kernel!\r\n");


	//init threads, shedular, etc will go here
	
}

