/*
    pic_driver.h
    A driver for 8259A Programmable Interrupt Controller

    Resources:
    https://wiki.osdev.org/8259_PIC
    http://www.brokenthorn.com/Resources/OSDevPic.html
*/

#pragma once

#define PIC1            0x20    /* IO base address for master PIC */
#define PIC2            0xA0    /* IO base address for slave PIC */
#define PIC1_COMMAND    PIC1
#define PIC1_DATA       (PIC1+1)
#define PIC2_COMMAND    PIC2
#define PIC2_DATA       (PIC2+1)


void pic_remap_vectors(int offset1, int offset2);
void pic_send_eoi(unsigned char irq);