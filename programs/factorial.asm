init:
    add $sp, $zero, $imm, 4068          # Initialize stack pointer to top of memory
    add $sp, $sp, $imm, -3              # adjust stack for 3 items
    sw $a0, $sp, $imm, 0                # save initial argument on stack_0[0]
    sw $ra, $sp, $imm, 1                # save initial return address on stack_0[1]
    sw $v0, $sp, $imm, 2                # save initial return value on stack_0[2]
    lw $a0, $zero, $imm, 0x100          # $a0 = n
    lw $ra, $zero, $imm, finalize       # $ra = address of finalize
factorial:
    add $sp, $sp, $imm, -2              # adjust stack for 2 items
    sw $a0, $sp, $imm, 0                # save n on stack_i[0]
    sw $ra, $sp, $imm, 1                # save ra on stack_i[1]
    beq $a0, $zero, $imm, if            # if n == 0, enter if block
else:
    sub $a0, $a0, $imm, 1               # $a0 = n-1
    jal $ra, $imm, $zero, factorial      # recursive call: factorial(n-1)
    lw $a0, $sp, $imm, 0                # restore argument n
    mul $v0, $a0, $v0, 0                   # $v0 = n * factorial(n-1)
    beq $imm, $zero, $zero, done        # jump to done
if:
    add $v0, $zero, $imm, 1             # return 1 (base case)
done:
    lw $ra, $sp, $imm, 1                # restore return address
    add $sp, $sp, $imm, 2               # pop 2 items from stack
    beq $ra, $zero, $zero, 0            # return to caller
finalize:
    sw $v0, $zero, $imm, 0x101          # store result in 0x101
    lw $a0, $sp, $imm, 0                # restore initaial argument from stack_0[0]
    lw $ra, $sp, $imm, 1                # restore initaial return address from stack_0[1]
    lw $v0, $sp, $imm, 2                # restore initaial return value from stack_0[2]
    add $sp, $sp, $imm, 3               # pop 3 items from stack
    halt $zero, $zero, $zero, 0         # halt the program