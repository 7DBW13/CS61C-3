.text

# Decodes a quadtree to the original matrix
#
# Arguments:
#     quadtree (qNode*)
#     matrix (void*)
#     matrix_width (int)
#
# Recall that quadtree representation uses the following format:
#     struct qNode {
#         int leaf;
#         int size;
#         int x;
#         int y;
#         int gray_value;
#         qNode *child_NW, *child_NE, *child_SE, *child_SW;
#     }

quad2matrix:

        # YOUR CODE HERE #
        # a0: quadtree a1: matrix a2: matrix_width
        addi $sp $sp -8
        sw $ra 0($sp)
        sw $a0 4($sp)
        jal traverse    # traverse the tree!
        lw $ra 0($sp)
        lw $a0 4($sp)
        addi $sp $sp 8
        jr $ra

traverse:
        # a0: node a1: matrix a2: matrix_width
        addi $sp $sp -8
        sw $ra 0($sp)
        sw $a0 4($sp)

        lw $t0 0($a0) # t0: node->leaf
        beq $t0 $0 notLeaf # leaf == 0: not a leaf!

        # now it is a leaf!

        lw $t1 8($a0)    # t1: node->x
        lw $t2 12($a0)   # t2: node->y
        lw $t3 4($a0)    # t3: node->size

        move $t5 $t1     # t5: i # first i = x
        add $t6 $t1 $t3  # t6: node->x + node->size
        outer:
        beq $t5 $t6 outerEnd
        move $t7 $t2     # t7: j
        add $t8 $t2 $t3  # t8: node->y + node->size
        inner:
        beq $t7 $t8 innerEnd


        mul $t9 $t7 $a2
        add $t9 $t9 $t5
        add $a1 $a1 $t9
        lw  $t4 16($a0)
        sb  $t4 0($a1)
        sub $a1 $a1 $t9


        addi $t7 $t7 1
        j inner
        innerEnd:
        addi $t5 $t5 1
        j outer

        outerEnd:
        j end

        ### Above: base case. Below: recursive call ###

        notLeaf:     # recursively go through the children of the tree

        lw $t1 20($a0)
        move $a0 $t1 
        jal traverse
        lw $ra 0($sp)
        lw $a0 4($sp)

        lw $t1 24($a0)
        move $a0 $t1
        jal traverse
        lw $ra 0($sp)
        lw $a0 4($sp)

        lw $t1 28($a0)
        move $a0 $t1
        jal traverse
        lw $ra 0($sp)
        lw $a0 4($sp)

        lw $t1 32($a0)
        move $a0 $t1
        jal traverse
        lw $ra 0($sp)
        lw $a0 4($sp)

        end:
        addi $sp $sp 8
        jr $ra
