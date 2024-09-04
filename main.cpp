#include <iostream>
#include <string>
#include <vector>

using namespace std;

#define INF  99999

// Estructura de datos para almacenar una arista de un grafo
struct Edge {
	int src, dest;
};

// Una clase para representar un objeto de grafo
class Graph {
public:
	// un vector de vectores para representar una lista de adyacencia
	vector<vector<int>> adjList;

	// almacena el grado de entrada de un vértice
	vector<int> indegree;

	// Constructor del grafo
	Graph(vector<Edge> const& edges, int n) {
		// redimensiona el vector para contener `n` elementos de tipo `vector<int>`
		adjList.resize(n);

		// inicializa el grado de entrada
		vector<int> temp(n, 0);
		indegree = temp;

		// agrega aristas al grafo dirigido
		for (auto& edge : edges) {
			// agrega una arista desde la fuente hasta el destino
			adjList[edge.src].push_back(edge.dest);

			// incrementa el grado de entrada del vértice destino en 1
			indegree[edge.dest]++;
		}
	}
};

void printSolution(int** dist, int V, int** local, string* nameLocations)
{
	cout << "La siguiente matriz muestra la distancia mas corta entre cada par de nodos " << endl;
	for (int i = 0; i < V; i++) {
		for (int j = 0; j < V; j++) {
			if (dist[i][j] == INF)
				cout << "INF"
				<< " ";
			else
				cout << dist[i][j] << "   ";
		}
		cout << endl;
	}


	// Imprimir matriz locations
	cout << "La matriz de recorrido es: " << endl;
	int aux = 0;
	for (int i = 0; i < V; i++) {
		for (int j = 0; j < V; j++) {
			aux = local[i][j];
			cout << nameLocations[aux - 1] << " ";
		}
		cout << endl;
	}
}

// Funcion para usar algoritmo de Floyd-Warshall
void floydWarshall(int** dist, int V, int** local, string* nameLocations)
{
	int i, j, k;

	for (k = 0; k < V; k++) {
		for (i = 0; i < V; i++) {
			for (j = 0; j < V; j++) {
				if (dist[i][j] > (dist[i][k] + dist[k][j])
					&& (dist[k][j] != INF
						&& dist[i][k] != INF)) {
					dist[i][j] = dist[i][k] + dist[k][j];
					if (i == j) {
						continue;
					}
					local[i][j] = local[i][k];
				}
			}
		}
	}
	printSolution(dist, V, local, nameLocations);
}

// Funcion para usar algoritmo de Dijkstra
void dijkstra(int src, int finalNode, int** graph, int V, string* nameLocations) {
	// Declaración de arrays necesarios para la lógica
	int* finalDistance = new int[V];
	bool* visited = new bool[V];

	// Inicialización de arrays, sin incluir el nodo 0 (inicializado debajo)
	for (int i = 0; i < V; i++) {
		finalDistance[i] = INF;
		visited[i] = false;
	}

	// Inicialización nodo 0
	finalDistance[0] = src;

	bool finished = false;

	int u = src;

	while (!finished) {
		int minDist = INF;

		// Localizar el nodo de menor distancia temporal de los nodos no visitados
		for (int i = 0; i < V; i++) {
			if (!visited[i] && finalDistance[i] < minDist && finalDistance[i]) {
				minDist = finalDistance[i];
				u = i;
			}
		}

		// Confirmar distancia final de u
		visited[u] = true;

		// Actualizar distancia temporal de nodos adyacentes aún no visitados
		for (int i = 0; i < V; i++) {
			if (graph[u][i] != INF && !visited[i]) {
				if (finalDistance[i] > finalDistance[u] + graph[u][i]) {
					finalDistance[i] = finalDistance[u] + graph[u][i];
				}
			}
		}

		// Verificar si ya se visitaron todos los nodos
		finished = true; // Suponemos que hemos terminado
		for (int i = 0; i < V; i++) {
			if (!visited[i]) { // Si hay un nodo no visitado, continuamos
				finished = false;
				break;
			}
		}
	}

	// Imprimir recorrido
	int recorrido = finalDistance[finalNode];
	int recorridoTotal = finalDistance[finalNode];
	cout << "Recorrido: ";
	while (finalNode != src) {
		for (int i = 0; i < V; i++) {
			// Verificamos si la resta coincide con alguna distancia
			if (recorridoTotal - graph[finalNode][i] == finalDistance[i]) {
				cout << nameLocations[finalNode] << " <- ";
				recorridoTotal = finalDistance[i];
				finalNode = i;
			}
		}
	}
	cout << nameLocations[src] << endl;
	cout << "La distancia minima es: " << recorrido << endl;

	// Liberar memoria
	delete[] finalDistance;
	delete[] visited;
}

// Función para realizar un ordenamiento topológico en un DAG dado
vector<int> doTopologicalSort(Graph const& graph) {
	vector<int> L;

	// obtiene el número total de nodos en el grafo
	int n = graph.adjList.size();

	vector<int> indegree = graph.indegree;

	// Conjunto de todos los nodos sin aristas entrantes
	vector<int> S;
	for (int i = 0; i < n; i++) {
		if (!indegree[i]) {
			S.push_back(i);
		}
	}

	while (!S.empty()) {
		// elimina el nodo `n` de `S`
		int n = S.back();
		S.pop_back();

		// agrega `n` al final de `L`
		L.push_back(n);

		for (int m : graph.adjList[n]) {
			// elimina una arista de `n` a `m` del grafo
			indegree[m] -= 1;

			// si `m` no tiene otras aristas entrantes, inserta `m` en `S`
			if (!indegree[m]) {
				S.push_back(m);
			}
		}
	}

	// si un grafo tiene aristas, entonces el grafo tiene al menos un ciclo
	for (int i = 0; i < n; i++) {
		if (indegree[i]) {
			return {};
		}
	}

	return L;
}

// Función para agregar una nueva asignatura
void addSubject(vector<Edge>& subjects, string subjectName, vector<string>& subjectNames, int& totalSubjects) {
	int numDependencies;

	cout << "Ingrese el numero de dependencias para la asignatura " << subjectName << ": ";
	cin >> numDependencies;

	// Agrega la nueva asignatura
	subjectNames.push_back(subjectName);
	totalSubjects++;

	// Agrega dependencias
	for (int j = 0; j < numDependencies; j++) {
		cout << "Ingrese la dependencia " << j + 1 << " para la asignatura " << subjectName << ": ";
		string dependency;
		cin >> dependency;
		// Encuentra el índice de la asignatura
		int k = 0;
		while (subjectNames[k] != dependency) {
			k++;
		}
		Edge edge = { k, totalSubjects - 1 };
		subjects.push_back(edge);
	}

	// Crea un nuevo grafo con las asignaturas actualizadas
	Graph graph(subjects, totalSubjects);
	vector<int> L = doTopologicalSort(graph);

	// Imprime el orden topológico
	cout << "El orden de las asignaturas es el siguiente: " << endl;
	if (L.size()) {
		for (int i : L) {
			cout << subjectNames[i] << " --> ";
		}
		cout << "Fin" << endl;
	}
	else {
		cout << "El orden topologico no es posible" << endl;
	}
}

int main() {
	// Logica menu
	int selector = 0, i = 0, j = 0, k = 0;
	bool seguir = true;
	cout << "---------MENU---------" << endl;
	cout << "1. Usar Floyd-Warshall" << endl;
	cout << "2. Usar Dijkstra" << endl;
	cout << "3. Orden Topologico" << endl;
	cout << "4. Usar recorrido en profundidad" << endl;
	cout << "5. Usar recorrido por amplitud" << endl;
	cout << "6. Usar recorrido por conectividad" << endl;
	cout << "9. Salir" << endl;
	cin >> selector;

	// Variables case 1
	int nodes = 0, auxNumber = 0;
	bool correct = true;
	string* nameLocations = new string[nodes];

	// Variables case 2
	int finalNode = 0, src = 0;
	int** graph1 = new int* [nodes];


	// Variables case 3
	vector<Edge> subjects;
	int totalSubjects, numDependencies = 0, option;
	//Definir subjectNames
	vector<string> subjectNames;
	vector<int> L;

	while (seguir) {

		switch (selector)
		{
		case 1:
		{
			// Se pide al usuario ingresar cantidad de nodos
			cout << "Ingrese la cantidad de nodos: ";
			cin >> nodes;

			int** matrix = new int* [nodes];
			// Declaracion de la matriz
			for (i = 0; i < nodes; i++) {
				matrix[i] = new int[nodes];
			}

			// Declaracion de la matriz de recorrido
			int** locations = new int* [nodes];
			for (i = 0; i < nodes; i++) {
				locations[i] = new int[nodes];
			}

			nameLocations = new string[nodes];
			for (i = 0; i < nodes; i++) {
				cout << "Ingrese el nombre de la ciudad " << i << endl;
				cin >> nameLocations[i];
			}

			// Inicializacion de la matriz de recorrido
			for (i = 0; i < nodes; i++) {
				for (j = 0; j < nodes; j++) {
					locations[i][j] = j + 1;
				}
			}

			// Insercion de datos segun corresponda en matriz de adyacencia

			for (i = 0; i < nodes; i++) {
				for (j = 0; j < nodes; j++) {

					if (i == j) {
						matrix[i][j] = 0;
						continue;
					}

					// Consideraciones al ingresar los datos de aristas
					cout << "Ingrese el numero de arista entre los nodos " << i << " " << j << endl;
					cout << "En caso de no existir arista, ingrese 0" << endl;

					// Manejo de excepciones tipo de dato
					try {
						cin >> auxNumber;
					}
					catch (exception e) {
						cout << "Error: " << e.what() << endl;
						cout << "Intente nuevamente" << endl;
					}

					// Verificacion de casos especiales (bucle o no arista)
					if (auxNumber == 0) {
						matrix[i][j] = INF;
						continue;
					}

					matrix[i][j] = auxNumber;
				}
			}

			floydWarshall(matrix, nodes, locations, nameLocations);
		}
		break;
		case 2:
		{
			// Se pide al usuario ingresar cantidad de nodos
			cout << "Ingrese la cantidad de nodos: ";
			cin >> nodes;

			// Declaracion de la matriz
			for (i = 0; i < nodes; i++) {
				graph1[i] = new int[nodes];
			}

			nameLocations = new string[nodes];
			for (i = 0; i < nodes; i++) {
				cout << "Ingrese el nombre de la ciudad " << i << endl;
				cin >> nameLocations[i];
			}


			// Inicializar matriz con valores 0
			for (i = 0; i < nodes; i++)
			{
				for (j = 0; j < nodes; j++)
				{
					graph1[i][j] = 0;
				}
			}

			// Llenar la matriz por encima de la diagonal principal

			for (i = 0; i < nodes; i++)
			{
				for (j = 0; j < nodes; j++)
				{
					if (i != j)
					{
						if (graph1[i][j] == 0)
						{
							int distancia;
							cout << "Ingrese la distancia entre el nodo " << nameLocations[i] << " y el nodo " << nameLocations[j] << ": ";
							cin >> distancia;
							if (distancia == 0)
							{
								graph1[i][j] = INF;
								graph1[j][i] = INF;
							}
							else
							{
								graph1[i][j] = distancia;
								graph1[j][i] = distancia;
							}
						}
					}
				}
			}

			// Imprimir grafo

			cout << "Indica el indice de tu nodo raiz" << endl;
			cin >> src;
			cout << "Indica el indice del nodo final" << endl;
			cin >> finalNode;

			dijkstra(src, finalNode, graph1, nodes, nameLocations);
			break;
		}
		case 3:
		{
			// Orden topologico
			cout << "Ingrese el numero de asignaturas: ";
			cin >> totalSubjects;
			subjectNames.resize(totalSubjects);

			// Array dinámico para asignaturas
			for (i = 0; i < totalSubjects; i++) {
				cout << "Ingrese el nombre de la asignatura " << i + 1 << ": ";
				cin >> subjectNames[i];
			}

			// Inicializa las dependencias de las asignaturas
			for (int i = 0; i < totalSubjects; i++) {
				cout << "Ingrese el numero de dependencias para la asignatura " << subjectNames[i] << ": ";
				cin >> numDependencies;
				for (int j = 0; j < numDependencies; j++) {
					cout << "Ingrese la dependencia " << j + 1 << " para la asignatura " << subjectNames[i] << ": ";
					string dependency;
					cin >> dependency;
					// Encuentra el índice de la asignatura
					int k = 0;
					while (subjectNames[k] != dependency) {
						k++;
					}
					Edge edge = { k, i };
					subjects.push_back(edge);
				}
			}

			Graph graph(subjects, totalSubjects);


			// Realiza el ordenamiento topológico
			L = doTopologicalSort(graph);

			// Imprime el orden topológico
			cout << "El orden de las asignaturas es el siguiente: " << endl;
			if (L.size()) {
				for (int i : L) {
					cout << subjectNames[i] << " --> ";
				}
				cout << "Fin" << endl;
			}
			else {
				cout << "El orden topologico no es posible" << endl;
			}

			// Opción para agregar y eliminar asignaturas

			while (true) {
				cout << "Ingrese 1 para agregar una asignatura" << endl;
				cout << "Ingrese 2 para eliminar una asignatura" << endl;
				cout << "Ingrese 0 para salir " << endl;
				cin >> option;

				if (option == 1) {
					string subjectName;
					cout << "Ingrese el nombre de la asignatura: ";
					cin >> subjectName;
					addSubject(subjects, subjectName, subjectNames, totalSubjects);
				}
				else if (option == 2) {
					string subjectName;
					cout << "Ingrese el nombre de la asignatura para eliminar: ";
					cin >> subjectName;
					// Encuentra el índice de la asignatura
					k = 0;
					while (subjectNames[k] != subjectName) {
						k++;
					}
					subjectNames.erase(subjectNames.begin() + k);
					totalSubjects--;
					// Reconstruye el grafo después de eliminar la asignatura
					vector<Edge> updatedSubjects;
					for (Edge e : subjects) {
						if (e.src != k && e.dest != k) {
							// Ajusta los índices para la lista de asignaturas actualizada
							if (e.src > k) e.src--;
							if (e.dest > k) e.dest--;
							updatedSubjects.push_back(e);
						}
					}
					subjects = updatedSubjects;
					Graph graph(subjects, totalSubjects);
					L = doTopologicalSort(graph);
					// Imprime el orden topológico
					cout << "El orden de las asignaturas después de eliminar es el siguiente: " << endl;
					if (L.size()) {
						for (int i : L) {
							cout << subjectNames[i] << " --> ";
						}
						cout << "Fin" << endl;
					}
					else {
						cout << "El orden topologico no es posible" << endl;
					}
				}
				else if (option == 0) {
					break;
				}
			}
			break;
		}
		case 4:

		}
		if (selector == 9)
		{
			seguir = false;
		}
		else
		{
			cout << endl << endl;
			cout << "---------MENU---------" << endl;
			cout << "1. Usar Floyd-Warshall" << endl;
			cout << "2. Usar Dijkstra" << endl;
			cout << "3. Orden Topologico" << endl;
			cout << "4. Usar recorrido en profundidad" << endl;
			cout << "5. Usar recorrido por amplitud" << endl;
			cout << "6. Usar recorrido por conectividad" << endl;
			cout << "9. Salir" << endl;
			cin >> selector;
		}
	}
}