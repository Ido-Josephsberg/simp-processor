init:
    add $sp, $zero, $imm, 4068      # Initialize stack pointer to top of memory
    add $sp, $sp, $imm, -3	    	# adjust stack for 3 items
    sw $s0, $sp, $imm, 0            # save $s0 on stack[0]
    sw $s1, $sp, $imm, 1            # save $s1 on stack[1] 
    sw $s2, $sp, $imm, 2            # save $s2 on stack[2]
    add $s0, $zero, $imm, 0x100     # $s0 = base address of the array
    add $s1, $zero, $zero, 0        # $s1 = i = 0
for_i:
    add $t0, $zero, $imm, 15        # $t0 = 15 (break limit for i)
    beq $imm, $s1, $t0, done        # if i==15, break loop (finish)
    add $s2, $zero, $zero, 0        # $s2 = j = 0
for_j:
    sub $t0, $imm, $s1, 15          # $t0 = 15-i (break limit for j)
    beq $imm, $s2, $t0, inc_i       # if j == 14-i, break loop (next i)
    lw $t0, $s0, $s2, 0             # $t0 = arr[j]
    add $t1, $s2, $imm, 1           # $t1 = j+1
    lw $t2, $s0, $t1, 0             # $t2 = arr[j+1]
    ble $imm, $t0, $t2, inc_j       # if arr[j] <= arr[j+1], dont swap
swap:
    lw $t0, $s0, $s2, 0             # $t0 = arr[j]
    add $t1, $s2, $imm, 1           # $t1 = j+1
    lw $t2, $s0, $t1, 0             # $t2 = arr[j+1]
    sw $t2, $s0, $s2, 0             # arr[j] = arr[j+1]
    sw $t0, $s0, $t1, 0             # arr[j+1] = arr[j]
inc_j:
    add $s2, $s2, $imm, 1           # j++
    beq $imm, $zero, $zero, for_j   # jump to for_j    
inc_i:
    add $s1, $s1, $imm, 1           # i++
    beq $imm, $zero, $zero, for_i   # jump to for_i
done:
    lw $s0, $sp, $imm, 0            # restore $s0 from stack[0]
    lw $s1, $sp, $imm, 1            # restore $s1 from stack[1]
    lw $s2, $sp, $imm, 2            # restore $s2 from stack[2]
    add $sp, $sp, $imm, 3           # pop 3 items from stack
    halt $zero, $zero, $zero, 0     # halt the program