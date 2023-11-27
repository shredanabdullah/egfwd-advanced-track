// File: startup.c

#include <stdint.h>
#include <stm32f4xx.h>  // Include the CMSIS header for your specific microcontroller

// Import the symbols defined in the linker script
extern uint32_t _estack;  // Defined in the linker script
extern uint32_t _etext, _sdata, _edata, _sbss, _ebss;

// Default weak interrupt handlers
void Reset_Handler(void);
void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));

// More interrupt handlers as needed...

// Function prototypes for the Cortex-M interrupt vectors
void Default_Handler(void);
/*
__attribute__((section(".isr_vector"))): This is an attribute that specifies 
that the following variable (g_pfnVectors) should be placed in the section named .isr_vector. 
This section typically contains the interrupt vector table.

void (*const g_pfnVectors[])(void): This declares g_pfnVectors as an array of function pointers to functions 
that take no arguments and return void. 
This array represents the interrupt vector table.

_estack: This is the initial stack pointer. 
The first entry in the vector table points to the initial stack pointer value. 
This value is defined in the linker script and represents the top of the stack memory.

Reset_Handler: This is the function that will be called on a system reset. 
It initializes the system and then calls the main function.

NMI_Handler: This is the Non-Maskable Interrupt handler. 
It's a weak function, meaning it can be overridden by a stronger definition elsewhere.
HardFault_Handler: This is the Hard Fault handler. Like NMI_Handler, it's a weak function.*/

// The vector table, placed at the beginning of the program in memory
__attribute__((section(".isr_vector")))
/*g_pfnVectors as an array of function pointers to functions that take no arguments and return void. 
This array represents the interrupt vector table.*/
void (*const g_pfnVectors[])(void) = {
    // Core Level - CM4
    &_estack,               // The initial stack pointer
    Reset_Handler,          // Reset Handler
    NMI_Handler,            // NMI Handler
    HardFault_Handler,      // Hard Fault Handler
    // More interrupt vectors...
};

// Initialize the system clock and other hardware
void SystemInit(void) {
    // Configure system clock using the microcontroller-specific CMSIS functions
    SystemCoreClockUpdate();
    // Initialize other peripherals as needed
}

// Default interrupt handler
void Default_Handler(void) {
    while (1) {
        // Default handler for unexpected interrupts or faults
    }
}

// Reset handler: called on reset
void Reset_Handler(void) {
    // Copy data from Flash to RAM
    uint32_t *pSrc, *pDest;

    // Copy the data segment initializers from flash to RAM
    pSrc = &_etext;
    pDest = &_sdata;

    while (pDest < &_edata) {
        *(pDest++) = *(pSrc++);
    }

    // Initialize the .bss section to zero in RAM
    pDest = &_sbss;

    while (pDest < &_ebss) {
        *(pDest++) = 0;
    }

    // Call CMSIS SystemInit function for microcontroller-specific setup
    SystemInit();

    // Call the application's entry point (main function)
    main();

    // Infinite loop to prevent execution from reaching undefined memory
    while (1) {
    }
}
