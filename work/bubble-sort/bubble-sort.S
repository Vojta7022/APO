// bubble-sort.S file template, rename and implement the algorithm
// Test algorithm in qtrvsim_gui program
// Select the CPU core configuration with delay-slot
// This setups requires (for simplicity) one NOP instruction after
// each branch and jump instruction (more during lecture about pipelining)
// The code will be compiled and tested by external riscv64-unknown-elf-gcc
// compiler by teachers, you can try make in addition, but testing
// by internal assembler should be enough

// copy directory with the project to your repository to
// the directory work/bubble-sort
// critical is location of the file work/bubble-sort/bubble-sort.S
// which is checked by the scripts

// Directives to make interesting windows visible
#pragma qtrvsim show registers
#pragma qtrvsim show memory

.option norelax

.globl    array_size
.globl    array_start

.text
.globl _start

_start:

    la   a0, array_start  
    la   a1, array_size   
    lw   a1, 0(a1)        # number of elements in the array

    addi t0, a1, -1      
    blt  t0, zero, end_loop 

outer_loop:
    addi t1, t0, 0        
    la   t3, array_start  
    li   t2, 0            

inner_loop:
    lw   t4, 0(t3)        
    lw   t5, 4(t3)        

    blt  t4, t5, no_swap 
    
    sw   t5, 0(t3)        
    sw   t4, 4(t3)        
    li   t2, 1            

no_swap:
    addi t3, t3, 4        
    addi t1, t1, -1       
    bne  t1, zero, inner_loop 

   
    beq  t2, zero, end_loop 

    addi t0, t0, -1       
    bne  t0, zero, outer_loop 

end_loop:
    fence           # Flush cache memory
    ebreak          # Stop the simulator
    j end_loop

.data

array_size:
.word   15
array_start:
.word   5, 3, 4, 1, 15, 8, 9, 2, 10, 6, 11, 1, 6, 9, 12

#pragma qtrvsim focus memory array_size
