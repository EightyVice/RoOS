#include "pic_driver.h"
#include "hal.h"


#define ICW1_ICW4       0x01    /* Indicates that ICW4 will be present */
#define ICW1_SINGLE     0x02    /* Single (cascade) mode */
#define ICW1_INTERVAL4  0x04    /* Call address interval 4 (8) */
#define ICW1_LEVEL      0x08    /* Level triggered (edge) mode */
#define ICW1_INIT       0x10    /* Initialization - required! */
 
#define ICW4_8086       0x01    /* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO       0x02    /* Auto (normal) EOI */
#define ICW4_BUF_SLAVE  0x08    /* Buffered mode/slave */
#define ICW4_BUF_MASTER 0x0C    /* Buffered mode/master */
#define ICW4_SFNM       0x10    /* Special fully nested (not) */

void pic_remap_vectors(int from_offset, int to_offset){
    uint8_t a1, a2;

    a1 = inb(PIC1_DATA);
    a2 = inb(PIC2_DATA);

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4); // Starts the initialization in sequence (cascade mode)
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT| ICW1_ICW4);
    io_wait();

    outb(PIC1_DATA, from_offset);   // ICW2: Master PIC vector offset
    io_wait();
    outb(PIC2_DATA, to_offset);     // ICW2: Slave PIC vector offset
    io_wait();

    outb(PIC1_DATA, 4);             // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
    io_wait();
    outb(PIC2_DATA, 2);             // ICW3: tell Slave PIC its cascade identity (0000 0010)

    outb(PIC1_DATA, ICW4_8086);     // ICW4: have the PICs use 8086 mode (and not 8080 mode)
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    outb(PIC1_DATA, a1);
    outb(PIC2_DATA, a2);
}

void pic_send_eoi(uint8_t irq){
    if(irq >= 8)
        outb(PIC2_COMMAND, 0x20);
        
    outb(PIC1_COMMAND, 0x20);
}