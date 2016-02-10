; Print.s
; Student names: Ryan Syed, Brandon Nguyen
; Last modification date: change this to the last modification date or look very silly
; Runs on LM4F120 or TM4C123
; EE319K lab 7 device driver for any LCD
;
; As part of Lab 7, students need to implement these LCD_OutDec and LCD_OutFix
; This driver assumes two low-level LCD functions
; ST7735_OutChar   outputs a single 8-bit ASCII character
; ST7735_OutString outputs a null-terminated string 

    IMPORT   ST7735_OutChar
    IMPORT   ST7735_OutString
    EXPORT   LCD_OutDec
    EXPORT   LCD_OutFix

    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB
;Define Constants
POINT	equ 46

;-----------------------LCD_OutDec-----------------------
; Output a 32-bit number in unsigned decimal format
; Uses recursive magic by essetially doing output = LCD_OutDec(input / 10)
; If input < 10, then a number is actually printed and the calling function can continue
; Reminder: PUSH and POP even number of registers at a time
; Input: R0 (call by value) 32-bit unsigned number
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutDec
LCD_OutDec_input	EQU		0
									; Allocate 2 uint32_t's (....alignment damn you.....)
    PUSH	{R0,LR}					; alloc for input (in R0), push LR for the function call's 8 byte alignment

    CMP 	R0,#10					;
    BLO		LCD_OutDec_done			; If input is less than ten, end the recursion
    MOV		R2,#10					;
    UDIV	R0,R2					; Divide input by 10, call self
    BL LCD_OutDec
LCD_OutDec_done
	LDR R0,[SP,#LCD_OutDec_input]	; Load the local var 'input'
									; Calc input%10
	MOV		R1,#10					; 10 constant
	UDIV	R2,R0,R1				; Divide input by 10, put into R1
	MLS		R0,R2,R1,R0				; Calculate modulo from R0 = R0 - R1*R2 --- output = input - (input/div) * div
	ADD		R0,#0x30				; Add 0x30 for the ASCII offset
	BL	ST7735_OutChar
	POP {R0,PC}						; Deallocate the local var while bringing in the LR
;* * * * * * * * End of LCD_OutDec * * * * * * * *

PlaceValueStart	equ	1000
PlaceValue	equ 0
OverflowString	DCB	"*.***",0		;What to output if overflow, 0 is null character
; -----------------------LCD _OutFix----------------------
; Output characters to LCD display in fixed-point format
; unsigned decimal, resolution 0.001, range 0.000 to 9.999
; Inputs:  R0 is an unsigned 32-bit number
; Outputs: none
; E.g., R0=0,    then output "0.000 "
;       R0=3,    then output "0.003 "
;       R0=89,   then output "0.089 "
;       R0=123,  then output "0.123 "
;       R0=9999, then output "9.999 "
;       R0>9999, then output "*.*** "
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutFix
	PUSH {R4, LR}
	
	MOV R1, #PlaceValueStart
	PUSH {R1, R12}
	
	LDR R1, =9999
	CMP R0, R1
	BHI LCD_OutFix_Overflow			;Check overflow, R0 > 9999

	MOV R4, R0						;Gets & prints one's place
	LDR R1, [SP, #PlaceValue]
	UDIV R0, R4, R1
	ADD R0, #48	
	BL 	ST7735_OutChar
	
	LDR R0, =POINT
	BL 	ST7735_OutChar				;Print decimal point
	
	MOV R0, R4
	LDR R1, [SP, #PlaceValue]
	UDIV R2, R4, R1
	MLS R4, R2, R1, R4				;Remove most significant digit to print rest of number
	
	CMP R4, #100
	BLO LCD_OutFix_Zeros			;Check for leading zero's

LCD_OutFix_Print
	MOV R0, R4
	BL	LCD_OutDec
	
	POP	{R1, R12}
	POP	{R4, PC}

LCD_OutFix_Zeros				;Print leading zeros
	MOV R0, #48
	BL	ST7735_OutChar
	
	CMP R4, #9
	BHI LCD_OutFix_Print		;Check if another leading zero is needed
	
	MOV R0, #48
	BL	ST7735_OutChar
	B	LCD_OutFix_Print

LCD_OutFix_Overflow
	LDR R0, =OverflowString
	BL 	ST7735_OutString
	POP {R1, R12}
	POP {R4, PC}

     ALIGN
;* * * * * * * * End of LCD_OutFix * * * * * * * *


     ALIGN                           ; make sure the end of this section is aligned
     END                             ; end of file
