#include <stddef.h>
#include <stdint.h>

static void outl(uint16_t port, uint32_t value)
{
	asm("outl %0,%1" : /* empty */ : "a"(value), "Nd"(port) : "memory");
}

void HC_get(int *str)
{
	//consider only lower 32bits of int* str
	uint32_t ptr = (uint32_t)((uintptr_t)str);
	outl(0x39, ptr);
}

void HC_consumed(int *str)
{
	//consider only lower 32bits of int* str
	uint32_t ptr = (uint32_t)((uintptr_t)str);
	outl(0x49, ptr);
}

void
	__attribute__((noreturn))
	__attribute__((section(".start")))
	_start(void)
{
	
	/* Write code here */
	int arr[5] = {0};
	while(1){
		HC_get(arr);
		HC_consumed(arr);
	}
	
	*(long *)0x400 = 42;

	for (;;)
		asm("hlt" : /* empty */ : "a"(42) : "memory");
}
