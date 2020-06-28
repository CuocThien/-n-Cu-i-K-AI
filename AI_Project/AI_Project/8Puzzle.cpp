#include <iostream>
#include<algorithm>
#include<time.h>
using namespace std;

#define SIZE 9
#define N 3

enum  class actions{ left, right, up, down };

void Print(int A[SIZE])
{
	int count = 0;
	for (int i = 0; i <= N*2; i++)
	{
		if (i % 2 == 0)
		{
			cout << "-------------"<< endl;
		}
		else
		{
			for (int j = 0; j < 3; j++)
			{
				if (A[count] == 0)
				{
					cout << "|   ";
				}
				else
				{
					cout << "| " << A[count] << " ";
				}
				count++;

			}
			cout <<"|"<< endl;
		}
	}
	cout << endl;
}

typedef struct Node_array
{
	int state[SIZE];
	actions action;
	struct Node_array* parent; // tree search

	int path_cost;
	int depth;
	struct Node_array* nextNode; // list
} Node;

struct NodeList {
	unsigned int nodeCount;    //the number of nodes in the list
	Node* head;            //pointer to the first node in the list
	Node* tail;            //pointer to the last node in the list
};


void copy_array(int A[SIZE], int B[SIZE])//copy B to A
{
	for (int i = 0; i < SIZE; i++)
	{
		A[i] = B[i];
	}
}

int is_equal(int A[SIZE], int B[SIZE])
{
	for (int i = 0; i < SIZE; i++)
	{
		if (A[i] != B[i])
			return 0;
	}
	return 1;
}

void swap(int& a, int& b)
{
	int temp;
	temp = a;
	a = b;
	b = temp;
}

bool isLegalAction(Node* node, actions action, int &check) //check the action is legal or illegal
{
	for (int i = 0; i < SIZE; i++)
	{
		if (node->state[i] == 0)
		{
			check = i;
			break;
		}
	}
	if (action == actions::left && check != 0 && check != 3 && check != 6) return true;
	if (action == actions::right && check != 2 && check != 5 && check != 8) return true;
	if (action == actions::up && check != 0 && check != 1 && check != 2) return true;
	if (action == actions::down && check != 6 && check != 7 && check != 8) return true;
	return false;
}

Node* Child_node(Node* node, actions action)
{
	int check;
	Node* current = new Node();
	if (isLegalAction(node, action, check) == true)
	{
		copy_array(current->state, node->state);
		current->action = action;
		current->parent = node;
		current->nextNode = NULL;
		current->depth = node->depth + 1;
		current->path_cost = node->depth + 1;
		if (action == actions::up)
		{
			swap(current->state[check], current->state[check - 3]);
		}
		else if (action == actions::down)
		{
			swap(current->state[check], current->state[check + 3]);
		}
		else if (action == actions::right)
		{
			swap(current->state[check], current->state[check + 1]);
		}
		else if (action == actions::left)
		{
			swap(current->state[check], current->state[check - 1]);
		}
	}
	else
		return NULL;
	return current;
}
int value(int A[], int B[])//B-GOAL
{
	int value = 0;
	for (int i = 0; i < SIZE; i++)
	{
		if (A[i] != B[i])
			value++;
	}
	return value;
}
Node* MinMove(Node* check,Node* GOAL) 
{
	Node* up = new Node();
	Node* down = new Node();
	Node* left = new Node();
	Node* right = new Node();
	up = Child_node(check, actions::up);
	down = Child_node(check, actions::down);
	left = Child_node(check, actions::left);
	right = Child_node(check, actions::right);
	int kq, kq2;
	if (up == NULL && down == NULL)
	{
		kq = 1000;
	}
	else if (up != NULL && down != NULL)
	{
		kq = min(value(up->state, GOAL->state), value(down->state, GOAL->state));		
	}
	else if (up == NULL)
	{
		kq = value(down->state, GOAL->state);
	}
	else if(down==NULL)
	{
		kq = value(up->state, GOAL->state);

	}

	if (left == NULL && right == NULL)
	{
		kq2 = 1000;
	}
	else if(left != NULL && right != NULL)
	{
		kq2 = min(value(left->state, GOAL->state), value(right->state, GOAL->state));
	}
	else if (right == NULL)
	{
		kq2 = value(left->state, GOAL->state);
	}
	else if (left == NULL)
	{
		kq2 = value(right->state, GOAL->state);
	}
	
	
	int Min = min(kq, kq2);
	if (up != NULL && value(up->state, GOAL->state) == Min)
	{
		delete down;
		delete right;
		delete left;
		return up;
	}
		
	if (down != NULL && value(down->state, GOAL->state) == Min)
	{
		delete up;
		delete right;
		delete left;
		return down;
	}
		
	if (left != NULL && value(left->state, GOAL->state) == Min)
	{
		delete down;
		delete right;
		delete up;
		return left;
	}
		
	if (right != NULL && value(right->state, GOAL->state) == Min)
	{
		delete down;
		delete left;
		delete up;
	}
		return right;
}
NodeList* FIFO_initial()
{
	NodeList* list = new NodeList();
	list->head = list->tail = NULL;
	list->nodeCount = 0;
	return list;
}

Node* LIFO_pop(NodeList* list)
{
	Node* p = NULL;
	if (list->head == NULL)
		return NULL;
	else {
		if (list->head->nextNode == NULL)
		{
			p = list->head;
			list->head = p->nextNode;
			list->nodeCount = 0;
			list->head = list->tail = NULL;


		}
		else
		{
			p = list->tail;
			Node* k = list->head;
			while (k->nextNode != p && k->nextNode!=NULL)
			{
				k = k->nextNode;
			}
			list->tail = k;
			list->tail->nextNode = NULL;
			list->nodeCount--;
		}


	}
	return p;
}
void FIFO_add(NodeList* list, Node* node)
{
	if (list->head == NULL)
	{
		list->head = list->tail = node;
	}
	else
	{
		list->tail->nextNode = node;
		list->tail = node;
	}
	list->nodeCount++;
}

Node* FIFO_pop(NodeList* list)
{
	Node* pop = list->head;
	if (list->head == NULL)
	{
		return NULL;
	}
	else
	{
		if (list->head->nextNode == NULL)
		{
			pop = list->head;
			list->head = pop->nextNode;
			list->nodeCount = 0;
			list->head = list->tail = NULL;


		}
		else
		{
			pop = list->head;
			list->head = pop->nextNode;
			list->nodeCount--;
		}
	}
	return pop;
}

bool Goal_test(Node* node, Node* goal) //check GOAL
{
	return is_equal(node->state, goal->state);
}


int Steps(Node* node) {
	NodeList* list = FIFO_initial();
	while (node->parent != NULL)
	{
		FIFO_add(list, node);
		node = node->parent;
	}
	return list->nodeCount;
}

void Solution(Node* node) {
	NodeList* list = FIFO_initial();
	while (node->parent != NULL)
	{
		FIFO_add(list, node);
		node = node->parent;
	}
	while (list->head != NULL)
	{
		Node* pop = LIFO_pop(list);

		Print(pop->state);
		//cout << endl;
	}

}
void Hill_Climbing(Node* start,Node* goal)
{
	Node* current = start;
	Node* neighbor = new Node();
	
	do 
	{

		neighbor = MinMove(current, goal);
		if (value(neighbor->state, goal->state) >= value(current->state, goal->state))
		{
			cout << "START" << endl;
			cout << "Value start: " << value(start->state, goal->state) << endl;
			Print(start->state);
			if (Goal_test(current, goal) == true)
			{
				cout << "HILL IS FOUND" << endl << "Value goal: " << value(current->state, goal->state) <<endl<<endl;
				
				Solution(current);
				return;
			}
			else 
			{
				cout << "HILL IS NOT FOUND"<<endl<<"Value goal: "<<value(current->state,goal->state) << endl<<endl;
				Solution(current);
				return;
			}
			
			//Solution(current);
			
		}
		current = neighbor;
		
	} while (true);
	
}

//First Choice Hill Climbing
Node* random_First_Choice(Node* start, Node* goal)
{
	actions action;
	
	Node* current = start;
	Node* neighbor = new Node();
	int count = 0;
	srand(time(NULL));
	do
	{
		do
		{
			action = actions(rand() % 4);
			neighbor = Child_node(current, action);
		} while (neighbor == NULL);
		count++;
		if(value(neighbor->state, goal->state) < value(current->state, goal->state))
			return neighbor;
		if (count == 10)
			return NULL;
	} while (true);
	
}
void Fisrt_Choice_Hill_Climbing(Node* start, Node* goal)
{
	cout << "\t\t\t\tFIRST CHOICE HILL CLIBMING" << endl;
	cout << "Value start: " << value(start->state, goal->state) << endl;
	
	Node* current = start;
	Node* neighbor = new Node();
	do
	{
		neighbor = random_First_Choice(current, goal);
		if (neighbor == NULL || value(neighbor->state, goal->state)>=value(current->state,goal->state))
			break;
		current = neighbor;

	} while (true);
	if (Goal_test(current, goal) == true)
	{
		cout << "FIRST CHOICE HILL CLIMBING IS FOUND GOAL STATE" << endl << "Value goal: " << value(current->state, goal->state) << endl << endl;
	}
	else 
	{
		cout << "FIRST CHOICE HILL CLIMBING IS NOT FOUND GOAL STATE" << endl << "Value goal: " << value(current->state, goal->state) << endl << endl;
	}
	cout << "START" << endl;
	Print(start->state);
	cout << endl;
	Solution(current);
	cout << endl;
		

}


//Simulated
Node* random_successor_function(Node* start)
{

	srand(time(NULL));
	Node* neighbor=new Node();
	actions action;
	do
	{
		action = actions(rand() % 4);
		neighbor = Child_node(start, action);
	} while (neighbor == NULL);
	
	return neighbor;
}
void Simulated_Annealing(Node* start, Node* goal)
{

	cout << "\t\t\t\tSIMULATED ANNEALING" << endl;
	cout << "Value start: " << value(start->state, goal->state) << endl;
	srand(time(NULL));

	Node* current = new Node();
	Node* neighbor = new Node();
	Node* neighborcp = new Node();
	current = start;
	int t = 0;
	float x, P;
	int deltaE;
	NodeList* Neighbor = FIFO_initial();
	
	FIFO_add(Neighbor, start);
	while (Goal_test(current, goal) == false)
	{
		if (t > 1000)
		{
			break;
		}
		x = float(1000 * t + 100000) / (t * t);
		neighbor = random_successor_function(current);
		
		int COUNT = 0;
		int count;
		if (Neighbor->nodeCount > 1)
		{

			while (COUNT < 50)
			{

				
				neighborcp = neighbor;
				while (neighbor->parent != NULL)
				{
					count = 0;
					if (Goal_test(neighborcp, neighbor->parent) == true)
					{
						count++;
						break;
					}
					neighbor = neighbor->parent;
				}
				if (count != 0)
				{
					neighbor = random_successor_function(current);
					COUNT++;
				}
				else
				{
					FIFO_add(Neighbor, neighborcp);
					break;
				}
			}
		}
		else
		{
			FIFO_add(Neighbor, neighbor);
		}
		neighbor = Neighbor->tail;
		
		deltaE = value(current->state, goal->state) - value(neighbor->state, goal->state);
		if (deltaE > 0)
		{

			current = neighbor;
		}
		else
		{
			P = exp(deltaE / x);
			if ((1.0 * rand() / RAND_MAX) < P)
			{
				current = neighbor;
				
			}
			
		}
		t++;
	}


	if (Goal_test(current, goal) == true)
	{
		cout << "SIMULATED ANNEALING IS FOUND GOAL" << endl;
	}
	else
	{
		cout << "SIMULATED ANNEALING IS NOT FOUND GOAL" << endl;
	}
	cout << "Value goal: " << value(current->state, goal->state) << endl;
	cout << "START" << endl;
	Print(start->state);
	cout << endl;
	Solution(current);
	delete neighbor;
	delete Neighbor;
}

void Choose()
{
	std::cout << "\t\t\t*************************************************\n";
	std::cout << "\t\t\t*\t1. HILL-CLIMBING               \t\t*\n";
	std::cout << "\t\t\t*\t2. FIRST CHOICE HILL-CLIMBING  \t\t*\n";
	std::cout << "\t\t\t*\t3. SIMULATED ANNEALING         \t\t*\n";
	std::cout << "\t\t\t*\t4. INPUT A DIFFERENT POSITION  \t\t*\n";
	std::cout << "\t\t\t*\t0. exit                        \t\t*\n";
	std::cout << "\t\t\t*************************************************\n";
	std::cout << "Select Algorithm: ";
}
void Menu(Node * start, Node* goal)
{
	Choose();
	int x;
	cin >> x;
	while (x>=0)
	{
		if (x == 1)
		{
			cout << "\t\t\t*************************************************\n";
			cout << "\t\t\t\t\tHILL CLIMBING" << endl;
			clock_t s, e;
			double time;
			s = clock();
			Hill_Climbing(start, goal);
			e = clock();
			time = (double)(e - s) / CLOCKS_PER_SEC;
			cout <<"Thoi gian chay: "<< time << endl;
		}
		if (x == 2)
		{
			cout << "\t\t\t*************************************************\n";
			clock_t s, e;
			double time;
			s = clock();
			Fisrt_Choice_Hill_Climbing(start, goal);
			e = clock();
			time = (double)(e - s) / CLOCKS_PER_SEC;
			cout << "Thoi gian chay: " << time << endl;
			
		}
		
		if (x == 3)
		{
			cout << "\t\t\t*************************************************\n";
			clock_t s, e;
			double time;
			s = clock();
			Simulated_Annealing(start, goal);
			e = clock();
			time = (double)(e - s) / CLOCKS_PER_SEC;
			cout << "Thoi gian chay: " << time << endl;
			
			
		}
		if (x == 4)
		{
			return;
		}
		if (x > 4)
		{
			cout << "Select again!"<<endl;
			
		}
		if (x == 0)
		{
			exit(0);
		}
		Choose();
		cin >> x;
	}

}

//so sanh
void Hill_Climbing(Node* start, Node* goal,int &found, int &notfound, int &steps)
{
	Node* current = start;
	Node* neighbor = new Node();

	do
	{

		neighbor = MinMove(current, goal);
		if (value(neighbor->state, goal->state) >= value(current->state, goal->state))
		{
			
			if (Goal_test(current, goal) == true)
			{
				found++;
			}
			else
			{
				notfound++;
				steps = steps + Steps(current);
			}

			//Solution(current);
			return;

		}
		current = neighbor;

	} while (true);

}
void Fisrt_Choice_Hill_Climbing(Node* start, Node* goal, int &found, int &notfound, int& steps)
{
	Node* current = start;
	Node* neighbor = new Node();
	do
	{
		neighbor = random_First_Choice(current, goal);
		if (neighbor == NULL || value(neighbor->state, goal->state) >= value(current->state, goal->state))
			break;
		current = neighbor;

	} while (true);
	if (Goal_test(current, goal) == true)
	{
		found++;
	}
	else
	{
		notfound++;
		steps = steps + Steps(current);
	}
}
void Simulated_Annealing(Node* start, Node* goal, int &found, int &notfound, int& steps)
{
	srand(time(NULL));

	Node* current = new Node();
	Node* neighbor = new Node();
	Node* neighborcp = new Node();
	current = start;
	int t = 0;
	float x, P;
	int deltaE;
	NodeList* Neighbor = FIFO_initial();

	FIFO_add(Neighbor, start);
	while (Goal_test(current, goal) == false)
	{
		if (t > 1000)
		{
			break;
		}
		x = float(1000*t+100000) / (t*t);
		neighbor = random_successor_function(current);

		int COUNT = 0;
		int count;
		if (Neighbor->nodeCount > 1)
		{

			while (COUNT < 50)
			{


				neighborcp = neighbor;
				while (neighbor->parent != NULL)
				{
					count = 0;
					if (Goal_test(neighborcp, neighbor->parent) == true)
					{
						count++;
						break;
					}
					neighbor = neighbor->parent;
				}
				if (count != 0)
				{
					neighbor = random_successor_function(current);
					COUNT++;
				}
				else
				{
					FIFO_add(Neighbor, neighborcp);
					break;
				}
			}
		}
		else
		{
			FIFO_add(Neighbor, neighbor);
		}
		neighbor = Neighbor->tail;

		deltaE = value(current->state, goal->state) - value(neighbor->state, goal->state);
		if (deltaE > 0)
		{
			current = neighbor;
		}
		else
		{
			P = exp(deltaE / x);
			if ((1.0 * rand() / RAND_MAX) < P)
			{
				current = neighbor;
			}
		}
		t++;
	}


	if (Goal_test(current, goal) == true)
	{
		found++;
	}
	else
	{
		notfound++;
		steps = steps + Steps(current);
	}
	
	delete neighbor;
	delete Neighbor;
}
int* RandomStep(int* goal, int steps)
{
	int* Assigment = new int[9];
	for (int i = 0; i < 9; i++)
	{
		Assigment[i] = goal[i];
	}
	int step = 0;

	//0 left
	//1 right
	//2 up
	//3 down
	do
	{
		int index = -1;
		for (int i = 0; i < 9; i++)
		{
			if (Assigment[i] == 0)
			{
				index = i;
				break;
			}
		}
		int k;
		do
		{
			k = rand() % 4;
			if (k == 0)
			{
				//left
				if (index % 3 != 2)
				{
					swap(Assigment[index], Assigment[index + 1]);
					break;
				}

			}
			else if (k == 1)
			{
				if (index % 3 != 0)
				{
					swap(Assigment[index], Assigment[index - 1]);
					break;
				}
			}
			else if (k == 2)
			{
				if (index / 3 != 2)
				{
					swap(Assigment[index], Assigment[index + 3]);
					break;
				}

			}
			else {
				if (index / 3 != 0)
				{
					swap(Assigment[index], Assigment[index - 3]);
					break;
				}
			}
		} while (true);

		step++;

	} while (step < steps);
	return Assigment;
}
int** randomstart(Node* goal)
{
	srand(time(NULL));
	int** start;
	int n = 100;
	start = new int* [n];
	for(int j=0;j<n;j++)
	{
		
		start[j] = new int[SIZE];
		start[j]= RandomStep(goal->state, 30);
	}
	return start;


}
int main()
{
	srand(time(NULL));
	/*int A[SIZE] = { 0,1,2,3,4,5,6,7,8 };
	int A[SIZE] = { 3,1,2,0,4,5,6,7,8 };
	int A[SIZE] = { 4,5,2,3,1,0,8,6,7 };
	Node* Start = new Node();
	copy_array( Start->state , A);
	int GOAL[SIZE] = { 1,4,2,3,5,8,6,7,0 };
	int GOAL[SIZE] = { 0,1,2,3,4,5,6,7,8 };
	Node* Goal = new Node();
	copy_array(Goal->state, GOAL);
	
	cout << "Start" << endl;
	Print(Start->state);
	cout << "GOAL" << endl;
	Print(GOAL);
	Menu(Start, Goal);*/
	int GOAL[SIZE] = { 0,1,2,3,4,5,6,7,8 };
	Node* Goal = new Node();
	copy_array(Goal->state, GOAL);
	int** start = randomstart(Goal);
	/*
	for (int i = 0; i < 1000; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			cout << start[i][j] << " ";
		}
		cout << endl;
	}*/
	int Hillfound = 0; int Hillnotfound = 0; int HillSteps = 0;
	int FChoicefound = 0; int FChoicenotfound = 0; int FChoiceSteps = 0;
	int SAfound = 0; int SAnotfound = 0; int SASteps = 0;
	int i = 0;
	
	do
	{
		Node* Start = new Node();
		copy_array(Start->state, start[i]);
		Hill_Climbing(Start, Goal, Hillfound, Hillnotfound, HillSteps);
		Fisrt_Choice_Hill_Climbing(Start, Goal, FChoicefound, FChoicenotfound,FChoiceSteps);
		Simulated_Annealing(Start, Goal, SAfound, SAnotfound,SASteps);
		i++;
	} while (i < 100);
	cout << "Hill Climbing" << endl;
	cout << "Found: " << Hillfound << endl << "Not Found: " << Hillnotfound << endl << "Max steps if not found: " << HillSteps << endl<<endl;
	cout << "First Choice Hill Climbing" << endl;
	cout << "Found: " << FChoicefound << endl << "Not Found: " << FChoicenotfound << endl << "Max steps if not found: " << FChoiceSteps << endl<<endl;
	cout << "Simulated Annealing" << endl;
	cout << "Found: " << SAfound << endl << "Not Found: " << SAnotfound << endl << "Max steps if not found: " << SASteps << endl <<endl;

	while (true)
	{
		int input;
		do
		{
			cout << "Input the position to check: ";
			cin >> input;
			if (input >= 100 || input < 0)
				cout << "Input again!" << endl;
		} while (input >= 100 || input < 0);
		cout << endl;
		Node* START = new Node();
		copy_array(START->state, start[input]);
		cout << "START" << endl;
		Print(START->state);
		cout << "GOAL" << endl;
		Print(Goal->state);

		Menu(START, Goal);
	}
	return 0;
}