# Solves a 2x3 slide puzzle

h = 3
w = 3

board  = (1,2,3,4,8,6,5,0,9)
s = (1,2,3,4,0,5,6,7,8)

def v(b, i, j):
    # b is a board, i is from, j is to (0-indexed!)
    #012
    #345
    #678
    return (b[j] == 0 and i >= 0 and j >= 0 and i < h*w and j<h*w and (
        (j == i+1 and i+1%w != 0) or
        (j == i+1 and i%w != 0) or
        (j == i+w) or
        (j == i-w)
    ))
def swappedcopy(b, i, j):
    l = list(b)
    tmp = l[j]
    l[j] = l[i]
    l[i] = tmp
    return tuple(l)
reachable_states = []
parents = {}
gen = [board]
next_gen = []
def f(b, parent=False):
    global parents
    #f is short for file, but like filing away, not a digital file
    #print(b)
    if b in reachable_states:
        return False
    elif b == s:
        print("We did it, Reddit!")
        exit()
    else:
        #print(b)
        reachable_states.append(b)
        parents[b] = parent
        return True

f(board)
while len(gen) != 0:
    #print(gen)
    next_gen = []
    for b in gen:
        for i, val in enumerate(b):
            j = b.index(0)
            if v(b, i, j):
                c = swappedcopy(b,i,j)
                f(c,b)
                next_gen.append(c)
    gen = next_gen
    print(len(reachable_states))
print(reachable_states)
print(len(reachable_states))
