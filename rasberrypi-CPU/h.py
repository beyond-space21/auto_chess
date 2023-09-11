# You are using Python
a=int(input())

for i in range(a):
    st=''
    for e in range(i+1,a):
        st += str(e)
    st += str(a)*(i+1)
    print(st)