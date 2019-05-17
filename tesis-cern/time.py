from math import *

print("Escribe tiempo la particula")
t1=float(input())
t=t1/sqrt(1-(0.99**2))

v=3.0e8
d=v*t
print("tiempo del segundo observador",t)
print("distancia recorrida ",d,"    metros")
