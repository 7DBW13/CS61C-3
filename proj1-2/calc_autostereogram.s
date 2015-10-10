.text

# Generates an autostereogram inside of buffer
#
# Arguments:
#     autostereogram (unsigned char*)
#     depth_map (unsigned char*)
#     width
#     height
#     strip_size
calc_autostereogram:

        # Allocate 5 spaces for $s0-$s5
        # (add more if necessary)
        addiu $sp $sp -20
        sw $s0 0($sp)
        sw $s1 4($sp)
        sw $s2 8($sp)
        sw $s3 12($sp)
        sw $s4 16($sp)

        # autostereogram
        lw $s0 20($sp)
        # depth_map
        lw $s1 24($sp)
        # width
        lw $s2 28($sp)
        # height
        lw $s3 32($sp)
        # strip_size
        lw $s4 36($sp)

        # YOUR CODE HERE #
        move $t9 $ra
        li $t0 0    # i = 0
        li $t1 0    # j = 0
        outerloop:
        beq $t0 $s2 done
        bgt $t0 $s2 done

        innerloop:
        beq $t1 $s3 innerdone
        bgt $t1 $s3 innerdone

        blt $t0 $s4 true # i < S

        mul $t3 $t1 $s2
        add $t3 $t3 $t0
        add $s1 $s1 $t3
        lbu $t5 0($s1)
        sub $s1 $s1 $t3

        add $t6 $t0 $t5
        sub $t6 $t6 $s4 
        mul $t7 $t1 $s2
        add $t7 $t7 $t6 

        add $s0 $s0 $t7
        lbu $t8 0($s0) 
        sub $s0 $s0 $t7

        add $s0 $s0 $t3
        sb  $t8 0($s0)
        sub $s0 $s0 $t3


        j fi

        true:
        addi $sp $sp -12
        sw $t0 0($sp)
        sw $t1 4($sp)
        sw $t9 8($sp)
        jal lfsr_random
        and $v0 $v0 0xff
        lw $t0 0($sp)
        lw $t1 4($sp)
        lw $t9 8($sp)
        addi $sp $sp 12
        mul $t3 $t1 $s2
        add $t3 $t3 $t0
        add $s0 $s0 $t3
        sb  $v0 0($s0)
        sub $s0 $s0 $t3

        fi:

        addi $t1 $t1 1
        j innerloop

        innerdone:
        addi $t0 $t0 1
        li $t1 0
        j outerloop

        done:

        lw $s0 0($sp)
        lw $s1 4($sp)
        lw $s2 8($sp)
        lw $s3 12($sp)
        lw $s4 16($sp)
        addiu $sp $sp 20
        move $ra $t9
        jr $ra
