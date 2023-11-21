#ifndef IRQS_H
#define IRQS_H

__attribute__((interrupt)) void pe_handler(void *) {
	pit_int();
	i8259_SendEndOfInterrupt(0x20);
}	

void init_periodic_event() {
	set_idt_gate(0x20, (uint64_t)&pe_handler, 0x28, 0x8E);
	i8259_Unmask(0);
}

#endif