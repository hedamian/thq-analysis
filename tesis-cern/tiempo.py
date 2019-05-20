from  math import *

print("escribe tiempo")
b=float(input())
gamma=1.0/(sqrt(1.0-0.998**2))

t=b*gamma

d=0.998*t*3.0e8

print("distancia",'{:.2e}'.format(d))
print("tiempo de la particula",t)

