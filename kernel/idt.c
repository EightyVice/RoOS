#include "idt.h"
#include "terminal.h"
#include "vga_driver.h"
#include "pic_driver.h"
#include "hal.h"
#include "kernel.h"

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
    asm volatile("cli; hlt"); // Completely hangs the computer

}

struct interrupt_frame;
#define INTERRUPT(name) __attribute__((interrupt)) void name(struct interrupt_frame* frame)

INTERRUPT(pit_timer){
    
    // Increament kernel ticks counter
    ++kernel_tick;

    char anim_set[4] = {'\\', '|', '/', '-'};
    char* anim = "    ";

    for (size_t i = 0; i < 4; i++)
        anim[i] = ' ';

    anim[kernel_tick % 4] = 219; // █
    
    vga_puts_c(anim, VGA_WIDTH - 4, VGA_HIEGHT - 1, COLOR_ATTR(VGA_COLOR_RED, VGA_COLOR_WHITE));
    
    pic_send_eoi(0);
}

INTERRUPT(keyboard){
    terminal_println("Keyboard key is pressed");
    uint8_t scancode = inb(0x60);
    pic_send_eoi(1);
} 

void idt_init(){
    idtr.base = (uint32_t)&idt;
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

    for (uint16_t vec = 0; vec < IDT_MAX_DESCRIPTORS; vec++)
    {
        idt_set_descriptor(vec, default_handler, 0x8E);
    }

    idt_set_descriptor(0x20, pit_timer, 0x8E);
    idt_set_descriptor(0x21, keyboard, 0x8E);

    //Load the new IDT
    asm volatile("lidt %0" : : "m"(idtr));  // LIDT [idtr]

    // Set the interrupt flag
    asm volatile("sti"); // STI
}