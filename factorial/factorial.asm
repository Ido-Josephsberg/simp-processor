init:
    add $sp, $zero, $imm, 4095          # Initialize stack pointer
    lw $a0, $zero, $imm, 0x100          # Load input n
    add $ra, $zero, $imm, finalize      # Set return address to finalize

factorial:
    add $sp, $sp, $imm, -2              # Allocate 2 words
    sw $a0, $sp, $imm, 0                # Save n
    sw $ra, $sp, $imm, 1                # Save ra
    beq $imm, $a0, $zero, if            # if n == 0,
    sub $a0, $a0, $imm, 1               # n = n - 1
    jal $ra, $imm, $zero, factorial     # recursive call
    lw $a0, $sp, $imm, 0                # restore n
    mul $v0, $a0, $v0, 0                # v0 = n * factorial(n-1)
    beq $imm, $zero, $zero, ret         # jump to return

if:
    add $v0, $zero, $imm, 1             # base case: return 1

ret:
    lw $ra, $sp, $imm, 1                # restore ra
    add $sp, $sp, $imm, 2               # pop 2 words
    beq $ra, $zero, $zero, 0            # return to caller

finalize:
    sw $v0, $zero, $imm, 0x101          # write result
    halt $zero, $zero, $zero, 0         # halt