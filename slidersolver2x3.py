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
def f(b,p):
    #f is short for file, but like filing away, not a digital file
    if b in reachable_states:
        pass
    elif b == s:
        print(p)
        exit()
    else:
        #print(b)
        reachable_states.append(b)
        paths[b] = p
        f(r(b),p+'r')
        f(l(b),p+'l')
f(board, '')
print(reachable_states)
