init:
    add $t0, $zero, $imm, 1         # $t0 = 1
    out $zero, $zero, $t0, 1        # IORegister[1] = 1 (enable irq1)
    add $t1, $zero, $imm, irq_handler # $t1 = address of irq_handler
    out $t1, $zero, $imm, 6        # IORegister[6] = $t1

    # sector4[i] = sector0[i];
    add $a3, $zero, $imm, 512       # $a3 = 512 (&sector4)
    out $zero, $zero, $a3, 16       # IORegister[16] = $a3 (diskbuffer = sector4)
    add $a1, $zero, $zero, 0        # $a1 = 0 (sector0)
    out $zero, $zero, $a1, 15       # IORegister[15] = $a1 (disksector = 0)
    add $t0, $zero, $imm, 1         # $t0 = 1 (read)
    out $zero, $zero, $t0, 14       # IORegister[14] = $t0 (diskcmd = 1)

    .word 0x0000100 0x0000000 # DEBUG
    .word 0x0000101 0x0000100
    .word 0x0000102 0x0000101
    .word 0x0000103 0x0000001 # END DEBUG

wait_sector0:
    in  $t2, $zero, $imm, 17       # $t2 = IORegister[17] (diskstatus)
    beq $zero, $t2, $zero, wait_sector0 # if $t2 == 0, loop
    out $t2, $zero, $imm, 17     # IORegister[17] = 0 (clear diskstatus)

    add $a1, $zero, $imm, 1         # $a1 = 1 (start with sector1)
    add $a2, $zero, $imm, 640       # $a2 = 640 (&buffer)
sector_sum_loop:
    out $a2, $zero, $imm, 16       # IORegister[16] = $a2 (diskbuffer = buffer)
    out $a1, $zero, $imm, 15       # IORegister[15] = $a1 (disksector = sectorX)
    add $t0, $zero, $imm, 1         # $t0 = 1 (read)
    out $t0, $zero, $imm, 14       # IORegister[14] = $t0 (diskcmd = 1)

wait_sectorX:
    in  $t2, $zero, $imm, 17       # $t2 = IORegister[17] (diskstatus)
    beq $zero, $t2, $zero, wait_sectorX # if $t2 == 0, loop
    out $zero, $zero, $imm, 17     # IORegister[17] = 0 (clear diskstatus)

    add $a0, $zero, $zero, 0        # $a0 = i = 0
sum_words_loop:
    lw  $t0, $a2, $a0, 0            # $t0 = buffer[i]
    lw  $t1, $a3, $a0, 0            # $t1 = sector4[i]
    add $t2, $t0, $t1, 0            # $t2 = buffer[i] + sector4[i]
    sw  $a3, $a0, $t2, 0            # sector4[i] = $t2
    add $a0, $a0, $imm, 1           # i++
    blt $a0, $zero, $imm, 128, sum_words_loop # if i < 128, loop

    add $a1, $a1, $imm, 1           # sector++
    blt $a1, $zero, $imm, 4, sector_sum_loop # if sector < 4, loop

    out $a3, $zero, $imm, 16       # IORegister[16] = $a3 (diskbuffer = sector4)
    add $a1, $zero, $imm, 4         # $a1 = 4 (sector4)
    out $a1, $zero, $imm, 15       # IORegister[15] = $a1 (disksector = 4)
    add $t0, $zero, $imm, 2         # $t0 = 2 (write)
    out $t0, $zero, $imm, 14       # IORegister[14] = $t0 (diskcmd = 2)

wait_write:
    in  $t2, $zero, $zero, 17       # $t2 = IORegister[17] (diskstatus)
    beq $zero, $t2, $zero, wait_write # if $t2 == 0, loop
    out $zero, $zero, $zero, 17     # IORegister[17] = 0 (clear diskstatus)

    halt $zero, $zero, $zero, 0     # End of program

irq_handler:
    add $t0, $zero, $imm, 1         # $t0 = 1
    out $zero, $zero, $t0, 17       # IORegister[17] = 1 (set diskstatus)
    reti $zero, $zero, $zero, 0     # Return from interrupt
