import numpy as np
import matplotlib.pyplot as plt
import glob


files = sorted(glob.glob("gol_output_*.txt"))


ncols = 5
nrows = 2


fig, axes = plt.subplots(nrows=nrows, ncols=ncols, figsize=(15, 6))

plt.tight_layout()


for i, file in enumerate(files[:10]): 
    data = np.loadtxt(file)
    
    
    ax = axes[i // ncols, i % ncols]
    
   
    ax.imshow(data, cmap='binary', origin='lower')
    ax.set_title(f"Game of Life - {file}")
    ax.axis('off') 


plt.savefig("gol_output_combined.png", bbox_inches='tight', dpi=300)


plt.close()
