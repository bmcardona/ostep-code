import matplotlib.pyplot as plt
import pandas as pd

# Read the data
data = pd.read_csv('tlb_data.csv')

# Filter to start at 4 pages or higher
data = data[data['num_pages'] >= 4]

# Convert microseconds to nanoseconds
data['cost_nanoseconds'] = data['cost_microseconds'] * 1000

# Create the plot
plt.figure(figsize=(10, 6))
plt.plot(data['num_pages'], data['cost_nanoseconds'], 
         marker='o', linewidth=2, markersize=8, color='steelblue')

plt.xlabel('Number of Pages', fontsize=12)
plt.ylabel('Time per Access (nanoseconds)', fontsize=12)
plt.title('TLB Performance', fontsize=14)

# Use log scale but show actual values as tick labels
plt.xscale('log', base=2)
plt.xticks(data['num_pages'], data['num_pages'])  # Show actual page numbers

plt.grid(True, alpha=0.3, linestyle='--')

plt.tight_layout()
plt.savefig('tlb_graph.png', dpi=300, bbox_inches='tight')
print("Graph saved as tlb_graph.png")
plt.show()