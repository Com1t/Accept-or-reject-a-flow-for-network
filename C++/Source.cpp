#include <iostream>
#include <vector>
#include <cmath>
#include <climits>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

// an ordinary node with val, and pointer to the next
typedef struct intNode {
	int val;
	struct intNode *next;
} node_int;
// headerNode is a special node, in order to store the size of list 
typedef struct intHeaderNode {
	int size;
	struct intNode *first;
} int_headerNode;

// push will push an element to the tail of the list
void int_push(int_headerNode *head, int val) {
	if (head->size == 0) {	// size 0 need another way to do since headerNode only contains a pointer to node
		head->first = (node_int *)malloc(sizeof(node_int));
		head->first->val = val;
		head->first->next = NULL;
	}
	else {
		node_int *current = head->first;
		while (current->next != NULL) {
			current = current->next;	// finding the tail
		}
		current->next = (node_int *)malloc(sizeof(node_int));
		current->next->val = val;
		current->next->next = NULL;
	}
	head->size++;
}
// pop will remove the last node of the list and return the value of last node
int int_pop(int_headerNode *head) {
	int val = 0;
	if (head->size == 1) {	// size 1 need another way to do since headerNode only contains a pointer to node
		val = head->first->val;
		free(head->first);
		head->first = NULL;
		head->size--;
	}
	else if (head->size > 1) {
		node_int *current = head->first;
		while (current->next->next != NULL) {
			current = current->next;	// finding the element before tail
		}
		val = current->next->val;
		free(current->next);
		current->next = NULL;
		head->size--;
	}
	return val;
}
// clear, clears whole list
void int_clear(int_headerNode *head) {
	int eles = head->size;
	for (int i = 0; i < eles; i++)
		int_pop(head);
}
// assign, lhs = rhs
void int_assign(int_headerNode *lhs, const int_headerNode *rhs) {
	int_clear(lhs);
	node_int *rhsCurrent = rhs->first;
	while (rhsCurrent != NULL) {
		int_push(lhs, rhsCurrent->val);
		rhsCurrent = rhsCurrent->next;
	}
}
// determine if two lists are equal and return true, otherwise return false
int int_equal(const int_headerNode *lhs, const int_headerNode *rhs)
{
	if (lhs->size != rhs->size)
		return 0;
	else {
		node_int *lhsCurrent = lhs->first;
		node_int *rhsCurrent = rhs->first;
		while (lhsCurrent != NULL) {
			if (lhsCurrent->val != rhsCurrent->val)
				return 0;
			lhsCurrent = lhsCurrent->next;
			rhsCurrent = rhsCurrent->next;
		}
		return 1;
	}
}
// to access the list like an array
node_int* int_get(const int_headerNode *head, int key)
{
	node_int *current = head->first;
	for (int i = 0; i < key; i++)
		current = current->next;
	return current;
}



// an ordinary node with cotent, and pointer to the next
typedef struct intlistNode {
	struct intHeaderNode content;
	struct intlistNode *next;
} node_intlist;
// intlistHeaderNode is a special node, in order to store the size of intlist 
typedef struct intlistHeaderNode {
	int size;
	struct intlistNode *first;
} intlist_headerNode;

// push will push an element to the tail of the list
// not using pass by point is in order to create new copy of the content
void intlist_push(intlist_headerNode *head, intHeaderNode *content) {
	if (head->size == 0) {	// size 0 need another way to do since headerNode only contains a pointer to node
		head->first = (node_intlist *)malloc(sizeof(node_intlist));
		head->first->content.size = 0;
		for (int i = 0; i < content->size; i++) {
			int_push(&head->first->content, int_get(content, i)->val);
		}
		head->first->next = NULL;
	}
	else {
		node_intlist *current = head->first;
		while (current->next != NULL) {
			current = current->next;	// finding the tail
		}
		current->next = (node_intlist *)malloc(sizeof(node_intlist));
		current->next->content.size = 0;
		for (int i = 0; i < content->size; i++) {
			int_push(&current->next->content, int_get(content, i)->val);
		}
		current->next->next = NULL;
	}
	head->size++;
}
// pop will remove the last node of the list and return the value of last node
intHeaderNode intlist_pop(intlist_headerNode *head) {
	intHeaderNode content = { 0, NULL };
	if (head->size == 1) {	// size 1 need another way to do since headerNode only contains a pointer to node
		content = head->first->content;
		int_clear(&head->first->content);
		free(head->first);
		head->first = NULL;
	}
	else if (head->size > 1) {
		node_intlist *current = head->first;
		while (current->next->next != NULL) {
			current = current->next;	// finding the element before tail
		}
		content = current->next->content;
		int_clear(&current->next->content);
		free(current->next);
		current->next = NULL;
	}
	head->size--;
	return content;
}
// clear, clears whole list
void intlist_clear(intlist_headerNode *head) {
	int eles = head->size;
	for (int i = 0; i < eles; i++)
		intlist_pop(head);
}
// to access the list like an array
node_intlist* intlist_get(const intlist_headerNode *head, int key)
{
	node_intlist *current = head->first;
	for (int i = 0; i < key; i++)
		current = current->next;
	return current;
}


struct dir {
	int hops = 0;
	int_headerNode routes;
};

struct edge_status {
	float cap = 0;
	float load = 0;
	float weight = 0;
};

int choose(int distance[], int n, int checked[]) {
	int min = INT_MAX, minIdx = -1;	// to the minimum index
	for (int i = 0; i < n; i++)
		if (distance[i] < min && !checked[i]) {
			min = distance[i];
			minIdx = i;
		}
	return minIdx;
}

void shortest_route(int **routeDirectory, int startNode, int nodeNumbers, dir* route_exist) {
	int* checked = new int[nodeNumbers]();
	int* distance = new int[nodeNumbers]();
	for (int i = 0; i < nodeNumbers; i++) {
		if (routeDirectory[startNode][i])
			distance[i] = routeDirectory[startNode][i];
		else
			distance[i] = INT_MAX;
	}

	distance[startNode] = 0;
	checked[startNode] = 1;
	int_push(&route_exist[startNode].routes, startNode);

	int_headerNode runTime = {0, NULL};
	int_push(&runTime, startNode);

	for (int i = 0; i < nodeNumbers - 2; i++) {
		int minIdx = choose(distance, nodeNumbers, checked);
		if (!route_exist[minIdx].routes.size) {
			// if this route is hasn't been parsed, save it, and identify as parsed(first time)
			// if this route is result of other route, we don't need to save it again(since they are already saved)
			// such as if we parse 1 get 0 1 3 route, while we pass 3 get an 0 3, which we don't need it
			int_push(&runTime, minIdx);
			int_assign(&route_exist[minIdx].routes, &runTime);
		}
		else {
			// change 0 3 to 0 1 3
			int_assign(&runTime, &route_exist[minIdx].routes);
		}
		
		checked[minIdx] = 1;

		for (int j = 0; j < nodeNumbers; j++) {
			if (!checked[j] && distance[minIdx] + routeDirectory[minIdx][j] <= distance[j] && routeDirectory[minIdx][j]) {
				// the condition "graph[minIdx][j]" is in order to avoid the path that doesn't exist
				int_push(&runTime, j);
				if (route_exist[j].routes.size && distance[j] == distance[minIdx] + routeDirectory[minIdx][j]){
					// if the new distance is equal to original, we examing whole route to see which go through a smaller nodeID
					for (int k = 0; k < runTime.size - 1; k++) {
						if (int_get(&runTime, k)->val > int_get(&route_exist[j].routes, k)->val) {
							break;
						}
						else if (int_get(&runTime, k)->val < int_get(&route_exist[j].routes, k)->val) {
							int_assign(&route_exist[j].routes, &runTime);
							break;
						}
					}
				}
				else {
					// if the new distance is smaller to original, replace it with new one
					// or it doesn't have an old one, just store it
					int_assign(&route_exist[j].routes, &runTime);
				}

				distance[j] = distance[minIdx] + routeDirectory[minIdx][j];
				int_pop(&runTime);
			}
		}
		int_pop(&runTime);
	}
	int_clear(&runTime);
	for (int i = 0; i < nodeNumbers; i++)
		route_exist[i].hops = distance[i];
}

float weight_calculator(edge_status* edge, float loading) {
	return edge->load / (edge->cap - edge->load);
}

int main() {
	int nodeNumbers = 0, linkNumbers = 0;
	cin >> nodeNumbers >> linkNumbers;

	int** routeDirectory = new int*[nodeNumbers]();
	for (int i = 0; i < nodeNumbers; i++)
		routeDirectory[i] = new int[nodeNumbers]();

	edge_status** edgeDirectory = new edge_status*[nodeNumbers]();
	for (int i = 0; i < nodeNumbers; i++)
		edgeDirectory[i] = new edge_status[nodeNumbers]();

	dir** neighborMap = new dir*[nodeNumbers]();
	for (int i = 0; i < nodeNumbers; i++){
		neighborMap[i] = new dir[nodeNumbers]();
		for (int j = 0; j < nodeNumbers; j++)
			neighborMap[i][j].routes = { 0, NULL };
	}
	for (int i = 0; i < linkNumbers; i++) {
		int linkID, firstNodeID, secondNodeID, linkCapacity;
		cin >> linkID >> firstNodeID >> secondNodeID >> linkCapacity;

		edgeDirectory[firstNodeID][secondNodeID].cap = linkCapacity;
		routeDirectory[firstNodeID][secondNodeID] = 1;

		edgeDirectory[secondNodeID][firstNodeID].cap = linkCapacity;
		routeDirectory[secondNodeID][firstNodeID] = 1;
	}

	cout << endl;
	for (int i = 0; i < nodeNumbers; i++) {
		shortest_route(routeDirectory, i, nodeNumbers, neighborMap[i]);
		cout << "\nFrom " << i << endl;
		cout << "Distance\tRoute\n";
		for (int j = 0; j < nodeNumbers; j++) {
			cout << neighborMap[i][j].hops << "\t\t";
			for (int k = 0; k < neighborMap[i][j].routes.size; k++)
				cout << int_get(&neighborMap[i][j].routes, k)->val << " ";
			cout << endl;
		}
		cout << endl;
	}

	int flowNumbers = 0, total_flow = 0;
	cin >> flowNumbers;
	intlist_headerNode accepted = { 0, NULL };
	for (int i = 0; i < flowNumbers; i++) {
		int flowID, firstNodeID, secondNodeID;
		float flowSize;
		cin >> flowID >> firstNodeID >> secondNodeID >> flowSize;

		// generating a route and its total weight
		int best_hops = neighborMap[firstNodeID][0].hops + neighborMap[0][secondNodeID].hops;
		int_headerNode best_path = { 0, NULL };
		int_assign(&best_path, &neighborMap[firstNodeID][0].routes);		// copy the first segment
		for (int j = 1; j < neighborMap[0][secondNodeID].routes.size; j++)
			int_push( &best_path, int_get(&neighborMap[0][secondNodeID].routes, j)->val);	// copy the second segment
		float best_weight = 0;
		for (int j = 0; j + 1 < best_path.size; j++)
			best_weight += edgeDirectory[int_get(&best_path, j)->val][int_get(&best_path, j+1)->val].weight;
		
		for (int j = 1; j < nodeNumbers; j++) {
			// generating a route and its total weight
			int temp_hops = neighborMap[firstNodeID][j].hops + neighborMap[j][secondNodeID].hops;
			int_headerNode temp_path = { 0, NULL };
			int_assign(&temp_path, &neighborMap[firstNodeID][j].routes);	// copy the first segment
			for (int k = 1; k < neighborMap[j][secondNodeID].routes.size; k++)
				int_push(&temp_path, int_get(&neighborMap[j][secondNodeID].routes, k)->val);
			float temp_weight = 0;
			for (int j = 0; j + 1 < temp_path.size; j++)
				temp_weight += edgeDirectory[int_get(&temp_path, j)->val][int_get(&temp_path, j + 1)->val].weight;
			// decide a best route by condition of priority from weight, hops, nodeID
			if (!int_equal(&best_path ,&temp_path)) {
				if (temp_weight < best_weight) {
					best_weight = temp_weight;
					best_hops = temp_hops;
					int_assign(&best_path, &temp_path);
				}
				else if (temp_weight == best_weight) {
					if (temp_hops < best_hops) {
						best_weight = temp_weight;
						best_hops = temp_hops;
						int_assign(&best_path, &temp_path);
					}
					else if (best_hops == temp_hops) {
						for (int k = 0; k < best_hops; k++) {
							if (int_get(&temp_path, k)->val < int_get(&best_path, k)->val) {
								int_assign(&best_path, &temp_path);
								break;
							}
							else if (int_get(&temp_path, k)->val > int_get(&best_path, k)->val)
								break;
						}
					}
				}
			}
		}
		int drop = 0;
		for (int j = 0; j + 1 < best_path.size; j++) {
			// if any part of route is already inf we won't go through there
			if (isinf(edgeDirectory[int_get(&best_path, j)->val][int_get(&best_path, j + 1)->val].weight)) {
				drop = 1;
				break;
			}
		}
		if (!drop) {
			total_flow += flowSize;
			// updating weight here
			for (int j = 0; j + 1 < best_path.size; j++) {
				edgeDirectory[int_get(&best_path, j)->val][int_get(&best_path, j + 1)->val].load = flowSize;
				edgeDirectory[int_get(&best_path, j)->val][int_get(&best_path, j + 1)->val].weight = weight_calculator(&edgeDirectory[int_get(&best_path, j)->val][int_get(&best_path, j + 1)->val], flowSize);
			}
			intlist_push(&accepted, &best_path);
		}
	}
	cout << endl;
	cout << accepted.size << " " << total_flow << endl;
	for (int i = 0; i < accepted.size; i++){
		cout << i << " ";
		for (int k = 0; k < intlist_get(&accepted, i)->content.size; k++)
			cout << int_get(&intlist_get(&accepted, i)->content, k)->val << " ";
		cout << endl;
	}
	for (int i = 0; i < nodeNumbers; i++)
		delete[] edgeDirectory[i];
	delete[] edgeDirectory;

	for (int i = 0; i < nodeNumbers; i++)
		delete[] routeDirectory[i];
	delete[] routeDirectory;

	return 0;
}