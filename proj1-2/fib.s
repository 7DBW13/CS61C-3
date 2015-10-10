main:
    li $a0 4
    jal fib

fib:
    li $t1 2
    blt $a0 $t1 less
    j more
    less:
    move $v0 $a0
    j ends
    more:
    addi $a0 $a0 -1
    jal fib
    

    ends:
    jr $ra
