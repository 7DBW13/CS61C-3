from pyspark import SparkContext
import Sliding, argparse

def get_level(state):
    """Abstraction for obtaining the level of a
    state representation
    """
    return state[1]

def get_board(state):
    """Abstraction for obtaining the board of a
    state representation
    """
    return Sliding.hash_to_board(WIDTH, HEIGHT, state[0])

def make_state(level, board):
    """Abstraction for making a state wher
    level and board are represented
    """
    return (Sliding.board_to_hash(WIDTH, HEIGHT, board), level)

def unhash_board(state):
    return str(get_level(state)) + ' ' + str(state[0])

def bfs_flat_map(state):
    """Takes in a state (level and board) and
    returns a list containing the original state
    with the addition of the children if the state is
    at the current level (in the frontier)
    """
    self_list = (state,)
    if get_level(state) == level-1:
        children = Sliding.children(WIDTH, HEIGHT, get_board(state))
        return tuple(make_state(level, board) for board in children) + self_list
    return self_list

def bfs_reduce(lvl1, lvl2):
    """Sets level for each state to minimum"""
    return min(lvl1, lvl2)

def solve_puzzle(master, output, height, width, slaves):
    global HEIGHT, WIDTH, level
    HEIGHT=height
    WIDTH=width
    level = 0

    sc = SparkContext(master, "python")

    """ YOUR CODE HERE """
    NUM_WORKERS = slaves

    sol = Sliding.solution(WIDTH, HEIGHT)
    """ MAP REDUCE PROCESSING CODE HERE """
    level_pos = sc.parallelize((make_state(level, sol),))
    prev_size, size = 0, 1

    while prev_size != size:
        level += 1
        if level % 10 == 0:
            level_pos = level_pos.partitionBy(PARTITION_COUNT)
        level_pos = level_pos.flatMap(bfs_flat_map).reduceByKey(bfs_reduce)
        prev_size = size
        size = level_pos.count()

    """ OUTPUT CODE HERE """
    level_pos = level_pos.map(unhash_board)
    level_pos.coalesce(NUM_WORKERS).saveAsTextFile(output)

    sc.stop()

""" DO NOT EDIT PAST THIS LINE

You are welcome to read through the following code, but you
do not need to worry about understanding it.
"""

def main():
    """
    Parses command line arguments and runs the solver appropriately.
    If nothing is passed in, the default values are used.
    """
    parser = argparse.ArgumentParser(
            description="Returns back the entire solution graph.")
    parser.add_argument("-M", "--master", type=str, default="local[8]",
            help="url of the master for this job")
    parser.add_argument("-O", "--output", type=str, default="solution-out",
            help="name of the output file")
    parser.add_argument("-H", "--height", type=int, default=2,
            help="height of the puzzle")
    parser.add_argument("-W", "--width", type=int, default=2,
            help="width of the puzzle")
    parser.add_argument("-S", "--slaves", type=int, default=6,
            help="number of slaves executing the job")
    args = parser.parse_args()

    global PARTITION_COUNT
    PARTITION_COUNT = args.slaves * 16

    # call the puzzle solver
    solve_puzzle(args.master, args.output, args.height, args.width, args.slaves)

# begin execution if we are running this file directly
if __name__ == "__main__":
    main()
