.data

lfsr:
        .align 4
        .half
        0x1

.text

# Implements a 16-bit lfsr
#
# Arguments: None
lfsr_random:

        la $t0 lfsr
        lhu $v0 0($t0)

        # YOUR CODE HERE #
        li $t1 0
        li $t2 16

        loop:
        beq $t1 $t2 end
        srl $t3 $v0 0
        srl $t4 $v0 2
        srl $t5 $v0 3
        srl $t6 $v0 5
        xor $t7 $t3 $t4
        xor $t7 $t7 $t5
        xor $t7 $t7 $t6
        sll $t7 $t7 15
        srl $v0 $v0 1
        or $v0 $v0 $t7
        and $v0 $v0 0x0000FFFF

        addi $t1 $t1 1
        j loop

        end:

        la $t0 lfsr
        sh $v0 0($t0)
        jr $ra
