def shallow_rec(n):
    if n == 0:
        return 0
    first = shallow_rec(n - 1)
    second = shallow_rec(n - 1)
    return first + second

def main():
    print(shallow_rec(3))
main()
