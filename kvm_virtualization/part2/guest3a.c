#include <stddef.h>
#include <stdint.h>

static void outl(uint16_t port, uint32_t value)
{
	asm("outl %0,%1" : /* empty */ : "a"(value), "Nd"(port) : "memory");
}

void HC_produced(int *str)
{
	//consider only lower 32bits of int* str
	uint32_t ptr = (uint32_t)((uintptr_t)str);
	outl(0x29, ptr);
}

void
	__attribute__((noreturn))
	__attribute__((section(".start")))
	_start(void)
{

	int arr[5] = {0};
	/* write code here */
	for(int i = 0;;i++){
		if(i%5 == 0 && i>0){
			HC_produced(arr);
		}
		arr[i%5] = i;
	}

	*(long *)0x400 = 42;

	for (;;)
		asm("hlt" : /* empty */ : "a"(42) : "memory");
}
