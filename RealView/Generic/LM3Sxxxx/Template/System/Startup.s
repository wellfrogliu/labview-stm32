; <<< Use Configuration Wizard in Context Menu >>>
;******************************************************************************
;
; Startup.s - Startup code for Stellaris.
;
; Copyright (c) 2006-2007 Luminary Micro, Inc.  All rights reserved.
;
; Software License Agreement
;
; Luminary Micro, Inc. (LMI) is supplying this software for use solely and
; exclusively on LMI's microcontroller products.
;
; The software is owned by LMI and/or its suppliers, and is protected under
; applicable copyright laws.  All rights are reserved.  Any use in violation
; of the foregoing restrictions may subject the user to criminal sanctions
; under applicable laws, as well as to civil liability for the breach of the
; terms and conditions of this license.
;
; THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
; OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
; MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
; LMI SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
; CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
;
; This is part of revision 1387 of the Stellaris Peripheral Driver Library.
;
;******************************************************************************

;        IMPORT    __use_no_semihosting

;******************************************************************************
;
; <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
;
;******************************************************************************
Stack   EQU     0x00000200

;******************************************************************************
;
; <o> Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
;
;******************************************************************************
Heap    	EQU 0x00008000
HeapSmall	EQU	0x00001000
HeapMax		EQU	0x00010000
HeapMin		EQU	0x00001000

;******************************************************************************
;
; Allocate space for the stack.
;
;******************************************************************************
        AREA    STACK, NOINIT, READWRITE, ALIGN=3
StackMem
        SPACE   Stack
__initial_sp

                EXPORT  Stack_Top
Stack_Top       EQU     StackMem + Stack


;******************************************************************************
;
; Allocate space for the heap.
;
;******************************************************************************
        AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
HeapMem
	IF :DEF: __HeapOpt
		IF __HeapOptSize > 0
			IF __HeapOptSize > HeapMax
				SPACE	HeapMax
			ELIF __HeapOptSize < HeapMin
				SPACE	HeapMin
			ELSE
				SPACE	__HeapOptSize
			ENDIF
		ELSE
			SPACE	HeapSmall
		ENDIF
	ELSE
        SPACE   Heap
	ENDIF
__heap_limit

;******************************************************************************
;
; Indicate that the code in this file preserves 8-byte alignment of the stack.
;
;******************************************************************************
        PRESERVE8

;******************************************************************************
;
; Place code into the reset code section.
;
;******************************************************************************
        AREA    RESET, CODE, READONLY
        THUMB

;******************************************************************************
;
; The vector table.
;
;******************************************************************************
        EXPORT  __Vectors
__Vectors
        DCD     StackMem + Stack            ; Top of Stack
        DCD     Reset_Handler               ; Reset Handler
        DCD     NmiSR                       ; NMI Handler
        DCD     FaultISR                    ; Hard Fault Handler
        DCD     IntDefaultHandler           ; MPU Fault Handler
        DCD     IntDefaultHandler           ; Bus Fault Handler
        DCD     IntDefaultHandler           ; Usage Fault Handler
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        DCD     0                           ; Reserved
        EXTERN  SVC_Handler
        DCD     SVC_Handler                 ; SVCall Handler
        DCD     IntDefaultHandler           ; Debug Monitor Handler
        DCD     0                           ; Reserved
        EXTERN  PendSV_Handler
        DCD     PendSV_Handler              ; PendSV Handler
        EXTERN  SysTick_Target_Handler
        DCD     SysTick_Target_Handler      ; SysTick Handler
;        EXTERN  SysTick_Handler
;        DCD     SysTick_Handler             ; SysTick Handler
        EXTERN  GPIOAHandler
        DCD     GPIOAHandler                ; GPIO Port A
        EXTERN  GPIOBHandler
        DCD     GPIOBHandler                ; GPIO Port B
        EXTERN  GPIOCHandler
        DCD     GPIOCHandler                ; GPIO Port C
        EXTERN  GPIODHandler
        DCD     GPIODHandler                ; GPIO Port D
        EXTERN  GPIOEHandler
        DCD     GPIOEHandler                ; GPIO Port E
        EXTERN  ser_irq_0                   ; LabView serial interface
        DCD     ser_irq_0                   ; UART0
        EXTERN  ser_irq_1
        DCD     ser_irq_1                   ; UART1
        EXTERN  SSI0IntHandler
        DCD     SSI0IntHandler              ; SSI
        DCD     IntDefaultHandler           ; I2C
		EXTERN  PWM_FaultHandler
        DCD     PWM_FaultHandler            ; PWM Fault
		EXTERN  PWM0Handler
        DCD     PWM0Handler		            ; PWM Generator 0
		EXTERN  PWM1Handler
        DCD     PWM1Handler		            ; PWM Generator 1
		EXTERN  PWM2Handler
        DCD     PWM2Handler		            ; PWM Generator 2
		EXTERN  QEI0Handler
        DCD     QEI0Handler		            ; Quadrature Encoder 0
		EXTERN  ADC0Handler
        DCD     ADC0Handler		            ; ADC Sequence 0
		EXTERN  ADC1Handler
        DCD     ADC1Handler           		; ADC Sequence 1
		EXTERN  ADC2Handler
        DCD     ADC2Handler          	    ; ADC Sequence 2
        DCD     IntDefaultHandler           ; ADC Sequence 3
		EXTERN  WatchdogHandler
        DCD     WatchdogHandler	            ; Watchdog
        EXTERN  Timer0AHandler
        DCD     Timer0AHandler              ; Timer 0A
        DCD     IntDefaultHandler           ; Timer 0B
        EXTERN  Timer1AHandler
        DCD     Timer1AHandler              ; Timer 1A
        DCD     IntDefaultHandler           ; Timer 1B
        EXTERN  Timer2AHandler
        DCD     Timer2AHandler              ; Timer 2A
        DCD     IntDefaultHandler           ; Timer 2B
		EXTERN  Comp0Handler
        DCD     Comp0Handler	            ; Comp 0
        DCD     IntDefaultHandler           ; Comp 1
        DCD     IntDefaultHandler           ; Comp 2
		EXTERN  SysCtlHandler
        DCD     SysCtlHandler	            ; System Control
        DCD     IntDefaultHandler           ; Flash Control
        EXTERN  GPIOFHandler
        DCD     GPIOFHandler                ; GPIO Port F
        EXTERN  GPIOGHandler
        DCD     GPIOGHandler                ; GPIO Port G
        DCD     IntDefaultHandler           ; GPIO Port H
        EXTERN  ser_irq_2
        DCD     ser_irq_2                   ; UART2 Rx and Tx
        DCD     IntDefaultHandler           ; SSI1 Rx and Tx
        EXTERN  Timer3AHandler
        DCD     Timer3AHandler              ; Timer 3 subtimer A
        DCD     IntDefaultHandler           ; Timer 3 subtimer B
        DCD     IntDefaultHandler           ; I2C1 Master and Slave
		EXTERN  QEI1Handler
        DCD     QEI1Handler		            ; Quadrature Encoder 1
        EXTERN  CAN0_IRQHandler
        DCD     CAN0_IRQHandler             ; CAN0
        DCD     IntDefaultHandler           ; CAN1
        DCD     0                           ; Reserved
        EXTERN  interrupt_ethernet          ; EthernetIntHandler
        DCD     interrupt_ethernet          ; Ethernet
        DCD     IntDefaultHandler           ; Hibernate

;******************************************************************************
;
; This is the code that gets called when the processor first starts execution
; following a reset event.
;
;******************************************************************************
        EXPORT  Reset_Handler
Reset_Handler
        ;
        ; Call the C library enty point that handles startup.  This will copy
        ; the .data section initializers from flash to SRAM and zero fill the
        ; .bss section.  It will then call __rt_entry, which will be either the
        ; C library version or the one supplied here depending on the
        ; configured startup type.
        ;
        IMPORT  __main
        IMPORT SysCtlClockSet

        ; set the system clock to run at 50 MHz
        ; SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL |
        ;                SYSCTL_XTAL_8MHZ | SYSCTL_OSC_MAIN);

        LDR        R0, =0x01C00380
        BL        SysCtlClockSet

        B       __main

;******************************************************************************
;
; This is the code that gets called when the processor receives a NMI.  This
; simply enters an infinite loop, preserving the system state for examination
; by a debugger.
;
;******************************************************************************
NmiSR
        B       NmiSR

;******************************************************************************
;
; This is the code that gets called when the processor receives a fault
; interrupt.  This simply enters an infinite loop, preserving the system state
; for examination by a debugger.
;
;******************************************************************************
FaultISR
        B       FaultISR

;******************************************************************************
;
; This is the code that gets called when the processor receives an unexpected
; interrupt.  This simply enters an infinite loop, preserving the system state
; for examination by a debugger.
;
;******************************************************************************
IntDefaultHandler
        B       IntDefaultHandler

;******************************************************************************
;
; Make sure the end of this section is aligned.
;
;******************************************************************************
        ALIGN

;******************************************************************************
;
; Some code in the normal code section for initializing the heap and stack.
;
;******************************************************************************
        AREA    |.text|, CODE, READONLY

;******************************************************************************
;
; The function expected of the C library startup code for defining the stack
; and heap memory locations.  For the C library version of the startup code,
; provide this function so that the C library initialization code can find out
; the location of the stack and heap.
;
;******************************************************************************
    IF :DEF: __MICROLIB
        EXPORT  __initial_sp
        EXPORT  __heap_base
        EXPORT __heap_limit
    ELSE
        IMPORT  __use_two_region_memory
        EXPORT  __user_initial_stackheap
__user_initial_stackheap
        LDR     R0, =HeapMem
        LDR     R1, =(StackMem + Stack)
        LDR     R2, =(__heap_limit)
        LDR     R3, =StackMem
        BX      LR
    ENDIF

;******************************************************************************
;
; Make sure the end of this section is aligned.
;
;******************************************************************************
        ALIGN

;******************************************************************************
;
; Tell the assembler that we're done.
;
;******************************************************************************
        END
