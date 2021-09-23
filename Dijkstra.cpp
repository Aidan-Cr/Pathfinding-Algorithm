#include "iostream"
#include "cmath"
#include "queue"
#include <chrono>
#include <thread>

using namespace std;
using std::chrono::duration_cast;
using std::chrono::nanoseconds;
typedef std::chrono::steady_clock the_clock;

struct node {
	int coord[2];//location of node
	int type = 0;
	float g = 0;   //distance traveled
	float h = 0;   //distance left
	float f = 0;   //total
	node* parent = nullptr;

};

struct outer_node {

	node* n;

	bool operator<(const outer_node& rhs) const {
		return n->f > rhs.n->f;
	}

	bool operator==(const outer_node& rhs) const {
		return ((n->coord[0] == rhs.n->coord[0]) && (n->coord[1] == rhs.n->coord[1]));
	}

	bool equals(const outer_node& rhs) const {
		return ((n->coord[0] == rhs.n->coord[0]) && (n->coord[1] == rhs.n->coord[1]));
	}

};

bool found = false;

int counter = 0;

const int len = 10;
const int start[2] = { 1,1 };
int fin[2] = { 9,9 };
outer_node current;

priority_queue<outer_node> list;
int listSize = 0;

queue<outer_node> visited;

node ary[len][len];

void setup() {//Cycle through array and setup coordinates
	for (int y = 0; y < len; y++) {
		for (int x = 0; x < len; x++) {
			ary[y][x].coord[1] = x;
			ary[y][x].coord[0] = y;

		}
	}
}
void display() {//Cycle through array and display results
	for (int y = 0; y < len; y++) {
		for (int x = 0; x < len; x++) {
			cout << ary[y][x].type;
		}
		cout << endl;
	}
}

float distS(int x2, int y2) { //Distance from Start
	float x = abs(start[1] - x2);
	float y = abs(start[0] - y2);
	return x + y;//calculate and return manhattan distance
}

bool is_in_list(priority_queue<outer_node> list, queue<outer_node> visited, outer_node Newn) {

	while (!list.empty()) {
		if (list.top().equals(Newn)) {
			return true;
		}
		list.pop();
	}

	while (!visited.empty()) {
		if (visited.front().equals(Newn)) {
			return true;
		}
		visited.pop();
	}

	return false;
}

void add(outer_node parent, int x, int y) { //add new struct to array item

	outer_node node_to_add;
	node_to_add.n = &ary[y][x];

	if (!is_in_list(list, visited, node_to_add) && ary[y][x].type != 3) {

		if (parent.n != nullptr) {
			node_to_add.n->g = ary[y][x].g = parent.n->g + 1;
		}
		node_to_add.n->h = ary[y][x].h = 1;
		node_to_add.n->f = ary[y][x].f = node_to_add.n->g + node_to_add.n->h;
		node_to_add.n->parent = parent.n;
		list.push(node_to_add);
	}
	else if (parent.n->g + 1 < node_to_add.n->g) {
		node_to_add.n->g = parent.n->g + 1;
		node_to_add.n->parent = parent.n;
	}
}

void get_adj_nodes(outer_node x) {
	int x2 = x.n->coord[1];
	int y2 = x.n->coord[0];

	int coordN = y2 - 1;
	int coordS = y2 + 1;
	int coordE = x2 + 1;
	int coordW = x2 - 1;
	if (coordN < len && coordN >= 0) {
		add(x, x2, coordN);
	}
	if (coordS < len && coordS >= 0) {
		add(x, x2, coordS);
	}
	if (coordE < len && coordE >= 0) {
		add(x, coordE, y2);
	}
	if (coordW < len && coordW >= 0) {
		add(x, coordW, y2);
	}
}

void trace(int parentx, int parenty) {
	int size = visited.size();
	node traceNode;

	traceNode = *visited.back().n;
	traceNode.coord[1] = parentx;
	traceNode.coord[0] = parenty;
	int x = visited.back().n->g;

	while (traceNode.parent != nullptr) {

		int x = traceNode.coord[1];
		int y = traceNode.coord[0];


		ary[y][x].type = 7;
		traceNode = *traceNode.parent;
	}
}
int main() {


	int x = start[1];
	int y = start[0];

	current.n = &ary[y][x];
	ary[y][x].type = 1;
	outer_node null_node;
	null_node.n = nullptr;
	add(null_node, x, y);

	x = fin[1];
	y = fin[0];
	ary[y][x].type = -1;

	setup();

	for (size_t i = 0; i < len; i++)
	{
		ary[4][i].type = 3;
	}

	ary[4][1].type = 0;

	display();
	cout << endl;

	the_clock::time_point start = the_clock::now();

	while (!found) {
		int next_g = current.n->g + 1;

		get_adj_nodes(current);


		/*if (next_g < ) {

		}*/

		outer_node low_f = list.top();
		visited.push(low_f);

		//cout << visited.back().n->type << endl;
		int x = visited.back().n->coord[1];
		int y = visited.back().n->coord[0];
		if (ary[y][x].type == -1) {
			found = true;
			break;
		}
		else {
			ary[y][x].type = 2;
			visited.back().n->type = 2;
		}
		list.pop();
		current = low_f;

		/*	trace();*/
		//display();
	}
	the_clock::time_point end = the_clock::now();
	outer_node low_f = list.top();
	visited.push(low_f);

	visited.pop();
	trace(x, y);
	display();
	double time_taken = duration_cast<nanoseconds>(end - start).count();
	cout << "It took " << (round(time_taken* 100 /1000000)/100) << " ms.\n";
}

