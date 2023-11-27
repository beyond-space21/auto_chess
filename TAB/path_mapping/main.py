import sys
 
class Graph(object):
    def __init__(self, nodes, init_graph):
        self.nodes = nodes
        self.graph = self.construct_graph(nodes, init_graph)
        
    def construct_graph(self, nodes, init_graph):
        '''
        This method makes sure that the graph is symmetrical. In other words, if there's a path from node A to B with a value V, there needs to be a path from node B to node A with a value V.
        '''
        graph = {}
        for node in nodes:
            graph[node] = {}
        
        graph.update(init_graph)
        
        for node, edges in graph.items():
            for adjacent_node, value in edges.items():
                if graph[adjacent_node].get(node, False) == False:
                    graph[adjacent_node][node] = value
                    
        return graph
    
    def get_nodes(self):
        "Returns the nodes of the graph."
        return self.nodes
    
    def get_outgoing_edges(self, node):
        "Returns the neighbors of a node."
        connections = []
        for out_node in self.nodes:
            if self.graph[node].get(out_node, False) != False:
                connections.append(out_node)
        return connections
    
    def value(self, node1, node2):
        "Returns the value of an edge between two nodes."
        return self.graph[node1][node2]

############################################################################################

def dijkstra_algorithm(graph, start_node):
    unvisited_nodes = list(graph.get_nodes())
 
    # We'll use this dict to save the cost of visiting each node and update it as we move along the graph   
    shortest_path = {}
 
    # We'll use this dict to save the shortest known path to a node found so far
    previous_nodes = {}
 
    # We'll use max_value to initialize the "infinity" value of the unvisited nodes   
    max_value = sys.maxsize
    for node in unvisited_nodes:
        shortest_path[node] = max_value
    # However, we initialize the starting node's value with 0   
    shortest_path[start_node] = 0
    
    # The algorithm executes until we visit all nodes
    while unvisited_nodes:
        # The code block below finds the node with the lowest score
        current_min_node = None
        for node in unvisited_nodes: # Iterate over the nodes
            if current_min_node == None:
                current_min_node = node
            elif shortest_path[node] < shortest_path[current_min_node]:
                current_min_node = node
                
        # The code block below retrieves the current node's neighbors and updates their distances
        neighbors = graph.get_outgoing_edges(current_min_node)
        for neighbor in neighbors:
            tentative_value = shortest_path[current_min_node] + graph.value(current_min_node, neighbor)
            if tentative_value < shortest_path[neighbor]:
                shortest_path[neighbor] = tentative_value
                # We also update the best path to the current node
                previous_nodes[neighbor] = current_min_node
 
        # After visiting its neighbors, we mark the node as "visited"
        unvisited_nodes.remove(current_min_node)
    
    return previous_nodes, shortest_path


nodes = [] # ["Reykjavik", "Oslo", "Moscow", "London", "Rome", "Berlin", "Belgrade", "Athens"]
init_graph = {}

def add_Vertex(v):
    nodes.append(v)
def add_edge(a,b,w):
    init_graph[a][b] = w

mat = [
    [0, 1, 0, 0, 1, 0, 0, 1],
    [1, 1, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0],
    [1, 1, 0, 1, 1, 1, 0, 0],
    [1, 1, 1, 1, 0, 1, 0, 0],
    [1, 1, 1, 1, 1, 1, 0, 0],
    [1, 1, 1, 1, 1, 1, 1, 1],
    [1, 1, 1, 1, 1, 1, 1, 1]
]

# //add chess grid vertices
for y in range(9):
    for x in range(9):
        add_Vertex(str(x)+','+str(y))


# //add chess grid diagnal vertices
for y in range(8):
    for x in range(8):
        if(mat[y][x] == 0):
            add_Vertex(str(x+0.5) + ',' + str(y+0.5))

for node in nodes:
    init_graph[node] = {}

# //add chess grid edges
#y
for y in range(9):
    for x in range(8):
        add_edge(str(x)+','+str(y),str(x+1)+','+str(y),100)
#x
for x in range(9):
    for y in range(8):
        add_edge(str(x)+','+str(y),str(x)+','+str(y+1),100)


# // add chess grid diagonal edges
for y in range(8):
    for x in range(8):
        if(mat[x][y] == 0):
            add_edge(str(x)+','+str(y),str(x+0.5)+','+str(y+0.5),75)
            add_edge(str(x+1)+','+str(y+1),str(x+0.5)+','+str(y+0.5),75)
            add_edge(str(x)+','+str(y+1),str(x+0.5)+','+str(y+0.5),75)
            add_edge(str(x+1)+','+str(y),str(x+0.5)+','+str(y+0.5),75)
        

graph = Graph(nodes, init_graph)

# print(init_graph)
# dijkstra_algorithm(graph,'0,0') 