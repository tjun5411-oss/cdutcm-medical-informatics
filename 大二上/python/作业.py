import numpy as np
Z = np.arange(10)
Z[4] = 10
Z = Z[(3 < Z) & (Z < 8)]
print(Z)
