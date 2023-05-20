#include "idt.h"
#include "terminal.h"
#include "vga_driver.h"


__attribute__((aligned(0x10))) 
idt_entry_t idt[256]; // Array of Interrupt Descriptors as IDT

// The IDTR register value
idtr_t idtr;

void idt_set_descriptor(uint8_t vector, void* ir, uint8_t flags){
    idt_entry_t* desc = &idt[vector];
    
    desc->ir_low        = (uint32_t)ir & 0xFFFF;
    desc->ir_high       = (uint32_t)ir >> 16;
    desc->cs            = 0x8;
    desc->attributes    = flags;
    desc->reserved      = 0;
}

void default_handler(){
    vga_clear(VGA_COLOR_RED);
    terminal_print("The system has fallen :(");
}

char* status = "0";
struct interrupt_frame;
__attribute__((interrupt)) void interrupt_handler(struct interrupt_frame* frame){
    ++status[0];
    terminal_status_print(status);

}

void idt_init(){
    idtr.base = (uint32_t)&idt;
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

    for (uint16_t vec = 0; vec < IDT_MAX_DESCRIPTORS; vec++)
    {
        idt_set_descriptor(vec, default_handler, 0x8E);
    }

    idt_set_descriptor(0x20, interrupt_handler, 0x8E);

    //Load the new IDT
    asm volatile("lidt %0" : : "m"(idtr));  // LIDT [idtr]

    // Set the interrupt flag
    asm volatile("sti"); // STI
}