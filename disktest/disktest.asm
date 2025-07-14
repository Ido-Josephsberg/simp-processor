READ_SECTOR0:
	add $a0, $zero, $imm, 1				# $a0 = 1 (set disk command to read)
	jal $ra, $imm, $zero, WAIT_FOR_DISK # jump to wait for disk status
	add $t0, $zero, $zero, 0			# $t0 = 0 (save disk sector)
	out $t0, $zero, $imm, 15			# IORegister[15] = $t0 (disksector = 0)
	add $t1, $zero, $imm, 3456			# $t1 = 3456 (save disk buffer address for sector0)
	out $t1, $zero, $imm, 16			# IORegister[16] = $t1 (diskbuffer = sector0)
	out $a0, $zero, $imm, 14			# IORegister[14] = $a0 (diskcmd = 1; read disk command)

READ_SECTOR1:
	jal $ra, $imm, $zero, WAIT_FOR_DISK # jump to wait for disk status
	add $t0, $zero, $imm, 1				# $t0 = 1 (save disk sector)
	out $t0, $zero, $imm, 15			# IORegister[15] = $t0 (disksector = 1)
	add $t1, $t1, $imm, 128 			# $t1 += 128 (save disk buffer address for sector1)
	out $t1, $zero, $imm, 16			# IORegister[16] = $t1 (diskbuffer = sector1)
	out $a0, $zero, $imm, 14			# IORegister[14] = $a0 (diskcmd = 1; read disk command)

READ_SECTOR2:
	jal $ra, $imm, $zero, WAIT_FOR_DISK # jump to wait for disk status
	add $t0, $zero, $imm, 2				# $t0 = 2 (save disk sector)
	out $t0, $zero, $imm, 15			# IORegister[15] = $t0 (disksector = 2)
	add $t1, $t1, $imm, 128 			# $t1 += 128 (save disk buffer address for sector2)
	out $t1, $zero, $imm, 16			# IORegister[16] = $t1 (diskbuffer = sector2)
	out $a0, $zero, $imm, 14			# IORegister[14] = $a0 (diskcmd = 1; read disk command)

READ_SECTOR3:
	jal $ra, $imm, $zero, WAIT_FOR_DISK # jump to wait for disk status
	add $t0, $zero, $imm, 3				# $t0 = 3 (save disk sector)
	out $t0, $zero, $imm, 15			# IORegister[15] = $t0 (disksector = 3)
	add $t1, $t1, $imm, 128 			# $t1 += 128 (save disk buffer address for sector3)
	out $t1, $zero, $imm, 16			# IORegister[16] = $t1 (diskbuffer = sector3)
	out $a0, $zero, $imm, 14			# IORegister[14] = $a0 (diskcmd = 1; read disk command)

# add each word in sector0-3 to sector4
	add $t0, $zero, $imm, 0				# $t0 = 0 (init index i for loop)
	add $a1, $zero, $imm, 3				# $a1 = 3 (number of lines to sum)
FOR_LOOP_SUM:
	lw $t1, $imm, $t0, 3456				# $t1 = sector0[i] (load word from sector0)
	lw $t2, $imm, $t0, 3584				# $t2 = sector1[i] (load word from sector1)
	add $t1, $t1, $t2, 0				# $t1 = sector0[i] + sector1[i]
	lw $t2, $imm, $t0, 3712				# $t2 = sector2[i] (load word from sector2)
	add $t1, $t1, $t2, 0				# $t1 = sector0[i] + sector1[i] + sector2[i]
	lw $t2, $imm, $t0, 3840				# $t2 = sector3[i] (load word from sector3)
	add $t1, $t1, $t2, 0				# $t1 = sector0[i] + sector1[i] + sector2[i] + sector3[i]
	sw $t1, $imm, $t0, 3968				# store result in sector4[i]
	add $t0, $t0, $imm, 1				# increment index i
	bne $imm, $t0, $a1, FOR_LOOP_SUM	# if i < 3, continue loop

# Write sector4 to disk
	jal $ra, $imm, $zero, WAIT_FOR_DISK # jump to wait for disk status
	add $a0, $zero, $imm, 2				# $a0 = 2 (set disk command to write)
	add $t0, $zero, $imm, 4				# $t0 = 4 (save disk sector)
	out $t0, $zero, $imm, 15			# IORegister[15] = $t0 (disksector = 4)
	add $t1, $zero, $imm, 3968 			# $t1 = 3968 (save disk buffer address for sector4)
	out $t1, $zero, $imm, 16			# IORegister[16] = $t1 (diskbuffer = sector4)
	out $a0, $zero, $imm, 14			# IORegister[14] = $a0 (diskcmd = 2; write disk command)
	halt $zero, $zero, $zero, 0			# halt the program






WAIT_FOR_DISK:
	in $t0, $zero, $imm, 17				# read disk status
	bne $imm, $zero, $t0, WAIT_FOR_DISK # wait until disk status is 0
	beq $ra, $zero, $zero, 0			# return from WAIT_FOR_DISK
	