import numpy as np
import matplotlib.pyplot as plt

# Carga la matriz que hemos calculado con C

# Flujo con C
psi = np.loadtxt("psitau.dat")

# Malla con C
XX = np.loadtxt("xxtau.dat") 
YY = np.loadtxt("yytau.dat")


plt.figure("flujo perfil", figsize=(12,12))

plt.contour(XX, YY, psi, np.linspace(-5,5,40), colors=['blue', 'blue'])

plt.xlim(-8,8)
plt.ylim(-3,3)
plt.grid()
plt.gca().set_aspect(1)
plt.show()