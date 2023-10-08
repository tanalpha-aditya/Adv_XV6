import matplotlib.pyplot as plt
from matplotlib.cm import get_cmap

# Define the file containing the data
data_file = 'plots.txt'

# Create dictionaries to store data for each queue
queue_data = {}

# Read the data from the file and store it in the dictionaries
with open(data_file, 'r') as f:
    for line in f:
        if line.startswith('GRAPH'):
            parts = line.strip().split()
            pid = int(parts[3])
            ticks = int(parts[2])
            queue = int(parts[1])
            
            if queue not in queue_data:
                queue_data[queue] = {'x': [], 'y': [], 'label': f'P{queue-2}'}
            
            queue_data[queue]['x'].append(pid)
            queue_data[queue]['y'].append(ticks)

# Create a color map for queues
cmap = get_cmap('tab10', len(queue_data))


# Plot the data for each queue and connect them
for queue, data in queue_data.items():
    plt.plot(data['y'], data['x'], label=data['label'], marker='_', markersize=5, color=cmap(queue-5), linestyle='-')

# Customize the plot
plt.xlabel('Time Elapsed')
plt.ylabel('Queue')
plt.title('MLFQ Scheduler Timeline')
plt.legend()

# Show the plot
plt.grid(True)
plt.show()
