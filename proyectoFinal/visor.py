import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import pandas as pd

# Lee el archivo .xyz o .ply (si es .ply, cambia la extensión)
# Asumiendo que el archivo tiene columnas X, Y, Z
df = pd.read_csv('tu_archivo.xyz', sep=' ', header=None, names=['X', 'Y', 'Z'])

fig = plt.figure(figsize=(10, 8))
ax = fig.add_subplot(111, projection='3d')

# Grafica los puntos
ax.scatter(df['X'], df['Y'], df['Z'], c=df['Z'], cmap='viridis', s=1)

ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
plt.title('Mi Nube de Puntos')
plt.show()