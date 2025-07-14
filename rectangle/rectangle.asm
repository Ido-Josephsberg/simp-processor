init:
    # Initialize stack pointer and save initial registers
    add $sp, $zero, $imm, 4095          # Initialize stack pointer
    add $sp, $sp, $imm, -3              # adjust stack for 3 items
    sw $s0, $sp, $imm, 0                # save initial s0 on stack_0[0]
    sw $s1, $sp, $imm, 1                # save initial s1 on stack_0[1]
    sw $s2, $sp, $imm, 2                # save initial s2 on stack_0[2]
    
    # Load initial values for A and C
    lw $t2, $zero, $imm, 0x100          # $t2 = &A
    and $a0, $t2, $imm, 0xFF            # $t2 & 0xFF -> $a0 = &A[7:0] - X value of A = low_X   (left column)
    srl $a1, $t2, $imm, 8               # $t2 >> 8$  ->  $a1 = &A[15:8] - Y value of A = low_Y (top row)
    lw $t2, $zero, $imm, 0x102          # $t2 = &C
    and $a2, $t2, $imm, 0xFF            # $t2 & 0xFF -> $a2 = &C[7:0] - X value of C = high_X  (right column)
    srl $a3, $t2, $imm, 8               # $t2 >> 8$  -> $a3 = &C[15:8] - Y value of C = high_Y (bottom row)
  
    # Initialize parameters for loops:
    add $s0, $zero, $imm, 1             # $s0 = 1 (monitor command)
    add $s1, $zero, $imm, 255           # $s1 = 255 (white value)
    add $s2, $zero, $imm, 256           # $s2 = 256 (total num of rows and coloumns)
    add $t0, $a1, $zero, 0              # outer loop: $t0 = $a1 (initialize currant row value to top_row)
    
ROW_LOOP:
    bgt $imm, $t0, $a3, DONE            # if curr_row > bottom_row, exit row loop
    add $t1, $a0, $zero, 0              # inner loop: $t1 = $a0 (initialize currant column value to left_col)
    
COL_LOOP:
    bgt $imm, $t1, $a2, INC_ROW         # if curr_col > right_col, exit col loop
    
    # Calculate the address of the current pixel in the monitor
    mul $t2, $t0, $s2, 0                # $t2 = curr_row * total_cols (256)
    add $t2, $t2, $t1, 0                # $t2 = curr_row * total_cols + curr_col (address of pixerl in monitor)
    
    out $t2, $zero, $imm, 20            # write address to monitoraddr
    out $s1, $zero, $imm, 21            # write color (white) to monitordata
    out $s0, $zero, $imm, 22            # write command (1 for write) to monitorcmd

    add $t1, $t1, $imm, 1               # increment column index - curr_col++
    beq $imm, $zero, $zero, COL_LOOP   # continue to next col loop

INC_ROW:
    add $t0, $t0, $imm, 1               # increment row index = curr_row++
    beq $imm, $zero, $zero, ROW_LOOP   # continue to next row loop

DONE:
    lw $s0, $sp, $imm, 0                # restore initial s0 from stack_0[0]
    lw $s1, $sp, $imm, 1                # restore initial s1 from stack_0[1]
    lw $s2, $sp, $imm, 2                # restore initial s2 from stack_0[2]
    add $sp, $sp, $imm, 3                # pop 3 items from stack
    halt $zero, $zero, $zero, 0          # halt the program