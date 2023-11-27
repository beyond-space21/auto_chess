// canvas = document.getElementsByTagName('canvas')[0]
// canvas.width = window.innerHeight-100
// canvas.height = window.innerHeight-100
// cs = canvas.getContext("2d");



class Graph {
    constructor() {
        this.vertices = [];
        this.adjacencyList = {};
    }
    show_data(){
        console.log(this.vertices);
        console.log(this.adjacencyList);
    }
    addVertex(vertex) {
        this.vertices.push(vertex);
        this.adjacencyList[vertex] = {};
        // console.log(vertex);
    }

    addEdge(vertex1, vertex2, weight) {
        this.adjacencyList[vertex1][vertex2] = weight;
    }

    changeWeight(vertex1, vertex2, weight) {
        this.adjacencyList[vertex1][vertex2] = weight;
    }

    dijkstra(source) {
        let distances = {},
            parents = {},
            visited = new Set();
        for (let i = 0; i < this.vertices.length; i++) {
            if (this.vertices[i] === source) {
                distances[source] = 0;
            } else {
                distances[this.vertices[i]] = Infinity;
            }
            parents[this.vertices[i]] = null;
        }

        let currVertex = this.vertexWithMinDistance(distances, visited);

        while (currVertex !== null) {
            let distance = distances[currVertex],
                neighbors = this.adjacencyList[currVertex];
            for (let neighbor in neighbors) {
                let newDistance = distance + neighbors[neighbor];
                if (distances[neighbor] > newDistance) {
                    distances[neighbor] = newDistance;
                    parents[neighbor] = currVertex;
                }
            }
            visited.add(currVertex);
            currVertex = this.vertexWithMinDistance(distances, visited);
        }

        console.log(parents);
        console.log(distances);
    }

    vertexWithMinDistance(distances, visited) {
        let minDistance = Infinity,
            minVertex = null;
        for (let vertex in distances) {
            let distance = distances[vertex];
            if (distance < minDistance && !visited.has(vertex)) {
                minDistance = distance;
                minVertex = vertex;
            }
        }
        return minVertex;
    }
}


 

let g = new Graph();

var mat = [
    [0, 1, 0, 0, 1, 0, 0, 1],
    [1, 1, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0],
    [1, 1, 0, 1, 1, 1, 0, 0],
    [1, 1, 1, 1, 0, 1, 0, 0],
    [1, 1, 1, 1, 1, 1, 0, 0],
    [1, 1, 1, 1, 1, 1, 1, 1],
    [1, 1, 1, 1, 1, 1, 1, 1]
]

//add chess grid vertices
for (y = 0; y < 9; y++) {
    for (x = 0; x < 9; x++) {
        g.addVertex(x + ',' + y);
    }
}

//add chess grid diagnal vertices
for (y = 0; y < 8; y++) {
    for (x = 0; x < 8; x++) {
        if (mat[y][x] == 0) {
            g.addVertex((x + 0.5) + ',' + (y + 0.5));
        }
    }
}

//add chess grid edges
//y
for (y = 0; y < 9; y++) {
    for (x = 0; x < 8; x++) {
        g.addEdge((x + ',' + y), ((x + 1) + ',' + y), 100);
    }
}
// //x 
for (x = 0; x < 9; x++) {
    for (y = 0; y < 8; y++) {
        g.addEdge((x + ',' + y), (x + ',' + (y + 1)), 100);
    }
}

// add chess grid diagonal edges
for (y = 0; y < 8; y++) {
    for (x = 0; x < 8; x++) {
        if (mat[y][x] == 0) {
            g.addEdge((x + ',' + y),(x + 0.5) + ',' + (y + 0.5), 75);
            g.addEdge(((x + 1) + ',' + (y + 1)),(x + 0.5) + ',' + (y + 0.5), 75);
            g.addEdge((x + ',' + (y + 1)),(x + 0.5) + ',' + (y + 0.5), 75);
            g.addEdge(((x + 1) + ',' + y),(x + 0.5) + ',' + (y + 0.5), 75);
            // console
        }
    }
}
// }
 
g.show_data();
g.dijkstra('0,0');
