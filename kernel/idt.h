/*
    idt.h
    Handling Interrupt Descriptor Table and Interrupts
*/

#pragma once

#include "../crt/stdint.h"

#define IDT_MAX_DESCRIPTORS 256

typedef struct {
    uint16_t ir_low;    // The lower 16 bits of the interrupt routine address
    uint16_t cs;        // The GDT segment selector (will be used to select the code segment that contains the IR)
    uint8_t reserved;   // Set to zero
    uint8_t attributes; // IR Attributes
    uint16_t ir_high;   // The higher 16 bits of the interrupt routine address
} __attribute__((packed)) idt_entry_t;


typedef struct {
    uint16_t limit;     // size of the IDT
    uint32_t base;      // Base address of IDT
} __attribute((packed)) idtr_t; // IDTR regsiter structure

/// @brief Sets a descriptor entity in the IDT
/// @param vector The vector to lookup in the IDT
/// @param ir The interrupt routine address
/// @param flags Interrupt attributes
void idt_set_descriptor(uint8_t vector, void* ir, uint8_t flags);

/// @brief Initializes the Interrupt Descriptor Table (IDT)
void idt_init();
