SECTIONS {
    . = 0x10000;
    
    /* Code Segments */
    .text : 
    {
        *(.entry)
        *(.text*)
    }

    .rodata :
    {
        *(.rodata)
    }

    .data :
    {
        *(.data)
    }

    .bss :
    {
        *(COMMON)
        *(.bss)
    }
}