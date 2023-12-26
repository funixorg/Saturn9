#include <stdbool.h>

#ifndef __I8259_H__
#define __I8259_H__

#define PIC_REMAP_OFFSET 0x20

void i8259_Configure(uint8_t offsetPic1, uint8_t offsetPic2, bool autoEoi);
void i8259_Disable();
void i8259_Mask(int irq);
void i8259_Unmask(int irq);
void i8259_SendEndOfInterrupt(int irq);

uint16_t i8259_ReadIrqRequestRegister();
uint16_t i8259_ReadInServiceRegister();

#endif // __I8259_H__