#include <stddef.h>
#include <stdint.h>

struct state {
	int pc;
	int prod_p;
	int cons_p;
	int *arr;
};

static void outb(uint16_t port, uint8_t value)
{
	asm("outb %0,%1" : /* empty */ : "a"(value), "Nd"(port) : "memory");
}

static void outl(uint16_t port, uint32_t value)
{
	asm("outl %0,%1" : /* empty */ : "a"(value), "Nd"(port) : "memory");
}

void HC_print8bit(uint8_t val)
{
	outb(0xE9, val);
}

void HC_print32bit(uint32_t val)
{
	outl(0xF9, val);
	/* Write code here */
}

void HC_setState(struct state *st)
{
	uint32_t ptr = (uint32_t)((uintptr_t)st);
	outl(0x39, ptr);
}

void HC_getState(struct state *st)
{
	uint32_t ptr = (uint32_t)((uintptr_t)st);
	outl(0x29, ptr);
}



void
	__attribute__((noreturn))
	__attribute__((section(".start")))
	_start(void)
{
	
	/* Write code here */
	//PRODUCER
	
	int buff[20] = {0};
	struct state st_m;
	struct state *st = &st_m;
	st->cons_p = 0;
	st->prod_p = -1;
	st->arr = buff;
	st->pc = 0;

	int ctr = 1;

	while(1){
		//Update state
		HC_getState(st);
		//Generate Random number between [0, 10] using inline rdtsc
		unsigned int lo, hi;
		asm volatile("rdtsc" : "=a"(lo), "=d"(hi));
		int random_num = (lo ^ hi) % 11;;
		// HC_print32bit(random_num);
		//Produce Random number
		for(int i = 0; i < random_num && (st->prod_p + 1) % 20 != st->cons_p; i++){
			st->prod_p = (st->prod_p + 1) % 20;
			st->arr[st->prod_p] = ctr++;
		}
		//HC_print32bit(st->prod_p);
		//Inform state to hypervisor
		HC_setState(st);
	}

	*(long *)0x400 = 42;

	for (;;)
		asm("hlt" : /* empty */ : "a"(42) : "memory");
}
