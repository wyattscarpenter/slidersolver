# Solves a 2x3 slide puzzle

# arrangement:
# 0 1
# 234

board  = [0,2,1,3,4]
s = [0,1,2,3,4]
def l(b):
    return [b[2],b[1],b[3],b[0],b[4]]

def r(b):
    return [b[0],b[4],b[2],b[1],b[3]]

assert(board == l(l(l(board))) and board == r(r(r(board))) and board != l(r(board)))
       
bs = [[board, ""]]
while True:
    tb = []
    for b in bs:
        print(b)
        if b[0] == s:
            print(b[1])
            break
        elif "rrr" in b[1] or "lll" in b[1]:
            #do nothing, loop detected
            print()
        else:
            tb.append([l(b[0]),b[1]+"l"])
            tb.append([r(b[0]),b[1]+"r"])
    bs = tb
