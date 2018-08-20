# Solves a 2x3 slide puzzle

# arrangement:
# 0 1
# 234

board  = (0,2,1,3,4)
s = (0,1,2,3,4)
def l(b):
    return (b[2],b[1],b[3],b[0],b[4])

def r(b):
    return (b[0],b[4],b[2],b[1],b[3])

assert(board == l(l(l(board))) and board == r(r(r(board))) and board != l(r(board)))
       
reachable_states = []
paths = {}
gen = [board]
next_gen = []
def f(b,p):
    #f is short for file, but like filing away, not a digital file
    #print(p)
    if b in reachable_states:
        return False
    elif b == s:
        print("We did it, Reddit!")
        print(p)
        exit()
    else:
        #print(b)
        reachable_states.append(b)
        paths[b] = p
        return True

f(board, '')
while len(gen) != 0:
    print(gen)
    next_gen = []
    for b in gen:
        f(b,paths[b])
        if f(r(b),paths[b]+'r'):
            next_gen.append(r(b))
        if f(l(b),paths[b]+'l'):
            next_gen.append(l(b))
    gen = next_gen
print(reachable_states)
print(len(reachable_states))
