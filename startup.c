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

// The vector table, placed at the beginning of the program in memory
__attribute__((section(".isr_vector")))
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
