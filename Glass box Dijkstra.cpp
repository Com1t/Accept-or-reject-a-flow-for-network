#include <iostream>
#include <vector>
#include <cfloat>

using namespace std;

struct dir {
	int nodeID = 0;
	vector<int> routes;
};

int choose(int distance[], int n, int checked[])
{
	int min = INT_MAX, minIdx = -1;	// to the minimum index
	for (int i = 0; i < n; i++)
		if (distance[i] < min && !checked[i]) {
			min = distance[i];
			minIdx = i;
		}
	return minIdx;
}

void shortest_route(int **graph, int startNode, int nodeNumbers) {
	int* checked = new int[nodeNumbers]();
	int* route_exist = new int[nodeNumbers]();		// in oreder to identify undirect routes
	int* distance = new int[nodeNumbers]();
	for (int i = 0; i < nodeNumbers; i++) {
		if (graph[startNode][i]) {
			distance[i] = graph[startNode][i];
		}
		else {
			distance[i] = INT_MAX;
		}
	}

	distance[startNode] = 0;
	checked[startNode] = 1;
	route_exist[startNode] = 1;

	vector< vector<int> > whole_route;
	vector<int> runTime;
	runTime.push_back(startNode);

	for (int i = 0; i < nodeNumbers - 2; i++) {
		int minIdx = choose(distance, nodeNumbers, checked);
		cout << minIdx << endl;

		if (!route_exist[minIdx]) {
			// if this route is hasn't been parsed, save it, and identify as parsed(first time)
			// if this route is result of other route, we don't need to save it againc(since they are already saved)
			runTime.push_back(minIdx);
			whole_route.push_back(runTime);
			route_exist[minIdx] = 1;
		}
		else {
			for (int i = 0; i < whole_route.size(); i++) {
				if ((whole_route[i])[whole_route[i].size() - 1] == minIdx) {
					runTime = whole_route[i];
					break;
				}
			}
		}

		checked[minIdx] = 1;

		for (int i = 0; i < nodeNumbers; i++)
			cout << checked[i] << " ";
		cout << endl;
		for (int i = 0; i < nodeNumbers; i++)
			cout << distance[i] << " ";
		cout << endl;

		for (int j = 0; j < nodeNumbers; j++) {
			if (!checked[j] && distance[minIdx] + graph[minIdx][j] <= distance[j] && graph[minIdx][j]) {
				// the condition "graph[minIdx][j]" is in order to avoid the path that doesn't exist
				runTime.push_back(j);
				whole_route.push_back(runTime);

				if (route_exist[j]) {
					int comparing_route = 0;
					for (int k = 0; k < whole_route.size(); k++) {
						if ((whole_route[k])[whole_route[k].size() - 1] == j) {
							comparing_route = k;
							break;
						}
					}
					if (distance[j] == distance[minIdx] + graph[minIdx][j]) {
						// if the new distance is equal to original, we examing whole route to see which go through a smaller nodeID
						for (int k = 0; k < (whole_route[whole_route.size() - 1]).size() - 1; k++) {
							if ((whole_route[whole_route.size() - 1])[k] < (whole_route[comparing_route])[k]) {
								whole_route[comparing_route] = whole_route[whole_route.size() - 1];
								whole_route.pop_back();
								break;
							}
							if ((whole_route[whole_route.size() - 1])[k] > (whole_route[comparing_route])[k]) {
								whole_route.pop_back();
								break;
							}
						}
					}
					else {
						whole_route[comparing_route] = whole_route[whole_route.size() - 1];
						whole_route.pop_back();
						break;
					}
				}
				distance[j] = distance[minIdx] + graph[minIdx][j];
				route_exist[j] = 1;
				runTime.pop_back();
			}
		}
		runTime.pop_back();
		for (int i = 0; i < nodeNumbers; i++)
			cout << distance[i] << " ";
		cout << endl;

	}

	for (int i = 0; i < whole_route.size(); i++) {
		for (int j = 0; j < whole_route[i].size(); j++)
			cout << (whole_route[i])[j] << " ";
		cout << endl;

	}
	cout << "Vertex\t\tDistance from Source\n";
	for (int i = 0; i < nodeNumbers; i++)
		cout << i << "\t\t" << distance[i] << endl;
}

int main() {
	int nodeNumbers = 0, linkNumbers = 0;
	cin >> nodeNumbers >> linkNumbers;

	int** addrDirectory = new int*[nodeNumbers]();
	for (int i = 0; i < nodeNumbers; i++)
		addrDirectory[i] = new int[nodeNumbers]();

	for (int i = 0; i < linkNumbers; i++) {
		int linkID, firstNodeID, secondNodeID, linkCapacity;
		cin >> linkID >> firstNodeID >> secondNodeID >> linkCapacity;
		addrDirectory[firstNodeID][secondNodeID] = 1;
		addrDirectory[secondNodeID][firstNodeID] = 1;
	}

	cout << endl;
	for (int i = 0; i < nodeNumbers; i++) {
		for (int j = 0; j < nodeNumbers; j++)
			cout << addrDirectory[i][j] << " ";
		cout << endl;
	}
	cout << endl;

	int startNode;
	cin >> startNode;
	cout << endl;

	shortest_route(addrDirectory, startNode, nodeNumbers);

	for (int i = 0; i < nodeNumbers; i++)
		delete[] addrDirectory[i];
	delete[] addrDirectory;

	return 0;
}
