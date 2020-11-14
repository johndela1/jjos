import operator

def merge(l1, l2):
    merged = []
    while l1 and l2:
        if l1[0] < l2[0]:
            merged.append(l1[0])
            l1 = l1[1:]
        else:
            merged.append(l2[0])
            l2 = l2[1:]
    return merged + l1 + l2


def merge2(l1, l2):
    merged = []
    while l1 or l2:
        if not l1:
            merged.append(l2[0])
            l2 = l2[1:]
        elif not l2:
            merged.append(l1[0])
            l1 = l1[1:]
        elif l1[0] < l2[0]:
            merged.append(l1[0])
            l1 = l1[1:]
        else:
            merged.append(l2[0])
            l2 = l2[1:]
    return merged


def sort(l):
    if len(l) <= 1: return l
    mid = len(l) >> 1
    l1, l2 = (l[:mid], l[mid:])
    return merge2(sort(l1), sort(l2))


l = [4, 3, 9, 1, 0, 3]
print(sort(l))
assert sorted(l) == sort(l), f"expected {sorted(l)}, got {sort(l)}"
