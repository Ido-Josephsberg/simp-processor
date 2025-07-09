.word 1024 0                # flag = 0 (synchronization flag for interrupts)

init:
    add $t0, $zero, $imm, 1         # $t0 = 1
    out $zero, $zero, $t0, 0        # IORegister[0] = 1 (enable irq0)
    out $zero, $zero, $t0, 1        # IORegister[1] = 1 (enable irq1)
    add $t1, $zero, $imm, irq_handler # $t1 = address of irq_handler
    out $zero, $zero, $t1, 6        # IORegister[6] = $t1

    # sector4[i] = sector0[i];
    add $a2, $zero, $imm, 512       # $a2 = 512 (&sector4)
    out $zero, $zero, $a2, 16       # IORegister[16] = $a2 (diskbuffer = sector4)
    add $a1, $zero, $zero, 0        # $a1 = 0 (sector0)
    out $zero, $zero, $a1, 15       # IORegister[15] = $a1 (disksector = 0)
    add $a3, $zero, $t0, 0          # $a3 = 1 (read)
    out $zero, $zero, $a3, 14       # IORegister[14] = $a3 (diskcmd = 1)

wait_sector0:
    add $s0, $zero, $imm, 1024      # $s0 = 1024 (&flag)
    lw  $t2, $s0, $zero, 0          # $t2 = MEM[flag]
    beq $zero, $t2, $zero, wait_sector0 # if $t2 == 0, loop
    sw  $s0, $zero, $zero, 0        # MEM[flag] = 0

    add $a1, $zero, $t0, 0          # $a1 = 1 (start with sector1)
sector_sum_loop:
    add $a2, $zero, $imm, 640       # $a2 = 640 (&buffer)
    out $zero, $zero, $a2, 16       # IORegister[16] = $a2 (diskbuffer = buffer)
    out $zero, $zero, $a1, 15       # IORegister[15] = $a1 (disksector = sectorX)
    out $zero, $zero, $t0, 14       # IORegister[14] = 1 (read)

wait_sectorX:
    add $s0, $zero, $imm, 1024      # $s0 = 1024 (&flag)
    lw  $t2, $s0, $zero, 0          # $t2 = MEM[flag]
    beq $zero, $t2, $zero, wait_sectorX # if $t2 == 0, loop
    sw  $s0, $zero, $zero, 0        # MEM[flag] = 0

    add $a0, $zero, $zero, 0        # $a0 = i = 0
sum_words_loop:
    add $s1, $zero, $imm, 640       # $s1 = 640 (&buffer)
    add $s2, $zero, $imm, 512       # $s2 = 512 (&sector4)
    lw  $t1, $s1, $a0, 0            # $t1 = buffer[i]
    lw  $t2, $s2, $a0, 0            # $t2 = sector4[i]
    add $t0, $t1, $t2, 0            # $t0 = buffer[i] + sector4[i]
    sw  $s2, $a0, $t0, 0            # sector4[i] = $t0
    add $a0, $a0, $t0, 0            # i++
    blt $a0, $zero, $imm, 128, sum_words_loop # if i < 128, loop

    add $a1, $a1, $t0, 0            # sector++
    blt $a1, $zero, $imm, 4, sector_sum_loop # if sector < 4, loop

    add $a2, $zero, $imm, 512       # $a2 = 512 (&sector4)
    out $zero, $zero, $a2, 16       # IORegister[16] = $a2 (diskbuffer = sector4)
    add $a3, $zero, $imm, 4         # $a3 = 4 (sector4)
    out $zero, $zero, $a3, 15       # IORegister[15] = $a3 (disksector = 4)
    add $t0, $zero, $imm, 2         # $t0 = 2 (write)
    out $zero, $zero, $t0, 14       # IORegister[14] = $t0 (diskcmd = 2)

wait_write:
    add $s0, $zero, $imm, 1024      # $s0 = 1024 (&flag)
    lw  $t2, $s0, $zero, 0          # $t2 = MEM[flag]
    beq $zero, $t2, $zero, wait_write # if $t2 == 0, loop
    sw  $s0, $zero, $zero, 0        # MEM[flag] = 0

    halt $zero, $zero, $zero, 0     # End of program

irq_handler:
    add $s0, $zero, $imm, 1024      # $s0 = 1024 (&flag)
    add $t0, $zero, $imm, 1         # $t0 = 1
    sw  $s0, $zero, $t0, 0          # MEM[flag] = 1
    reti $zero, $zero, $zero, 0     # Return from interrupt