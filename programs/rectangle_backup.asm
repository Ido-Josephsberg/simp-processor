.word 0x0000100 0x0000000
.word 0x0000101 0x0001400
.word 0x0000102 0x0001420
.word 0x0000103 0x0000020
init:
    add $sp, $zero, $imm, 4068          # Initialize stack pointer
    add $sp, $sp, $imm, -4              # adjust stack for 4 items
    sw $a0, $sp, $imm, 0                # save initial a0 on stack_0[0]
    sw $a1, $sp, $imm, 1                # save initial a1 on stack_0[1]
    sw $a2, $sp, $imm, 2                # save initial a2 on stack_0[2]
    sw $a3, $sp, $imm, 3                # save initial a3 on stack_0[3]
    lw $a0, $zero, $imm, 0x100          # $a0 = &A
    and $a1, $a0, $imm, 0xFF00          # $a1 = &A[15:8] = Y*[A] - top row
    and $a0, $a0, $imm, 0xFF            # $a0 = &A[7:0] = X[A]   - left col
    sub $a1, $imm, $a1, 255             # $a1 = 255-Y*[A] = Y[A]  
    lw $a2, $zero, $imm, 0x102          # $a2 = &C
    and $a3, $a2, $imm, 0xFF00          # $a3 = &C[15:8] = Y*[C]
    and $a2, $a2, $imm, 0xFF            # $a2 = &C[7:0] = X[C]
    sub $a3, $imm, $a3, 255             # $a3 = 255-Y*[C] = Y[C]  
    add $t0, $zero, $zero, 0            # $t0 = &P = 0 (first pixel address)
    and $t1, $t0, $imm, 0xFF00          # $t1 = &P[15:8] = Y*[P]
    and $t0, $t0, $imm, 0xFF            # $t0 = &P[7:0] = X[P]
    sub $t1, $imm, $t1, 255             # $t1 = 255-P*[A] = Y[P]  
test_pixel:
    blt $imm, $t0, $a0, set_black      # if X[P] < X[A], set pixel to black
    bgt $imm, $t0, $a2, set_black      # if X[P] > X[C], set pixel to black
    blt $imm, $t1, $a3, set_black      # if Y[P] < Y[C], set pixel to black
    bgt $imm, $t1, $a1, set_black      # if Y[P] > Y[A], set pixel to black
    sub $t2, $imm, $t1, 255            # $t2 = 255-P[A] = Y*[P]  
    sll $t2, $t2, $imm, 8              # shift Y*[P] to the left by 8 bits
    add $t2, $t2, $t0, 0               # $t2 = Y*[P] + X[P] = &P
    out $t2, $zero, $imm, 20           # write &P to monitoraddr
    add $t2, $zero, $imm, 255          # $t2 = 255 (white value)
    out $t2, $zero, $imm, 21           # set pixel color white to monitordata
    add $t2, $zero, $imm, 1            # $t2 = 1 (write value)
    out $t2, $zero, $imm, 22           # write command to monitorcmd
    out $t2, $zero, $imm, 22           # write command to monitorcmd -- ## double line
    beq $imm, $zero, $zero, next_pixel
set_black:
    sub $t2, $imm, $t1, 255            # $t2 = 255-P[A] = Y*[P]  
    sll $t2, $t2, $imm, 8              # shift Y*[P] to the left by 8 bits
    add $t2, $t2, $t0, 0               # $t2 = Y*[P] + X[P] = &P
    out $t2, $zero, $imm, 20           # write &P to monitoraddr
    out $zero, $zero, $imm, 21         # set pixel color black (0) to monitordata
    add $t2, $zero, $imm, 1            # $t2 = 1 (write value)
    out $t2, $zero, $imm, 22           # write command to monitorcmd
next_pixel:
    add $t2, $zero, $imm, 255
    beq $imm, $t0, $t2, next_row        # if X[P] is 255, next P in the next row
    add $t0, $t0, $imm, 1               # increment pixel address: X[P]++
    beq $imm, $zero, $zero, test_pixel  # test the next pixel
next_row:
    add $t2, $zero, $imm, 255
    beq $imm, $t1, $t2, finalize        # if Y[P] is 255, finalize
    add $t1, $t1, $imm, 1               # increment pixel address: Y[P]++
    add $t0, $zero, $imm, 0             # reset pixel address: X[P]=0
    beq $imm, $zero, $zero, test_pixel  # test the next pixel
finalize: 
    lw $a0, $sp, $imm, 0                # restore initial a0 from stack_0[0]
    lw $a1, $sp, $imm, 1                # restore initial a1 from stack_0[1]
    lw $a2, $sp, $imm, 2                # restore initial a2 from stack_0[2]
    lw $a3, $sp, $imm, 3                # restore initial a3 from stack_0[3]
    add $sp, $sp, $imm, 4                # pop 4 items from stack
    halt $zero, $zero, $zero, 0          # halt the program
