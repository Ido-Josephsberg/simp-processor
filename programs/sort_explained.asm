init: 
    
    # init_sort - save previous state of registers to stack:
    add $sp, $sp, $imm, -3	    	# adjust stack for 3 items
    sw $s0, $sp, $imm, 0            # save $s0 on stack[0]
    sw $s1, $sp, $imm, 1            # save $s1 on stack[1] 
    sw $s2, $sp, $imm, 2            # save $s2 on stack[2]
    
    # init sort - set up registers for bubble sort:
    add $s0, $zero, $imm, 0x100     # $s0 = base address of the array
    add $s1, $zero, $zero, 0        # $s1 = i = 0

for_i:
    add $t0, zero, $imm, 16         # $t0 = 16 (break limit for i)
    beq $s1, $t0, $imm, done        # if i==16, break loop (finish)
    add $s2, $zero, $zero, 0        # $s2 = j = 0
for_j:
    sub $t0, $imm, $t0, 15          # $t0 = 15-i (break limit for j)
    beq $s2, $t0, $imm, inc_i       # if j == 15-i, break loop (next i)
    lw $t0, $s0, $s2, 0             # $t0 = arr[j]
    add $t1, $s2, $imm, 1           # $t1 = j+1
    lw $t1, $s0, $t1, 0             # $t1 = arr[j+1]
    ble $imm, $t0, $t1, inc_j       # if arr[i] <= arr[j+1], dont swap
swap:
    add $t2, $t0, $zero, 0          # $t2 = temp copy of arr[j]
    add $t0, $s0, $s2, 0            # $t0 = address arr[j]
    sw $t1, $t0, $zero, 0           # arr[j] = arr[j+1]
    sw $t2, $t0, $imm, 1            # arr[j+1] = temp
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



- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

Execute a bubble sort algorithm for 16 Numbers
- The numbers are stored in the adresses 0x100 to 0x10f
- The result is stored in the same addresses (sort in-place)

follow the bubble sort algorithm implementation in python:

def bubble_sort(arr):
    n = len(arr)
    for i in range(n):  #break out in i==n
        # Last i elements are already sorted
        for j in range(0, n-i-1):
            if arr[j] > arr[j+1]:
                # Swap if the element found is greater than the next element
                arr[j], arr[j+1] = arr[j+1], arr[j]

