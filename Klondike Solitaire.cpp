#include<iostream>
#include<Windows.h>
#include<vector>
#include <cstdlib>  
#include <ctime>
#include<string>
using namespace std;
// Color codes using Windows API
#define RED     12
#define RESET   7


template <class T>
class DLList {

	template<class T>
	class Node {
	public:
		T data;
		Node<T>* next;
		Node<T>* prev;

		Node()
		{
			//data = 0;
			prev = 0;
			next = 0;
		}
		Node(T val, Node<T>* nextPtr = 0, Node<T>* prevPtr = 0)
		{
			data = val;
			next = nextPtr;
			prev = prevPtr;
		}
		~Node()
		{
			next = NULL;
			prev = NULL;
		}
	};

	Node<T>* head;
	Node<T>* tail;
	int size;

	class ListIterator {
	public:
		friend class DLList;
		ListIterator(Node<T>* ptr = NULL)
		{
			iptr = ptr;
		}

		ListIterator& operator++()
		{//prefix ++
			if (iptr)
				iptr = iptr->next;
			return (*this);
		}

		ListIterator operator++(int)
		{
			// postfix ++
			ListIterator old = *this;
			++(*this);
			return old;
		}
		ListIterator& operator--() //pre dec

		{
			if (iptr)
				iptr = iptr->prev;
			return (*this);
		}

		ListIterator operator--(int)
		{
			ListIterator old = *this;
			--(*this);
			return old;
		}

		T& operator* () const
		{

			if (iptr)
				return iptr->data;
		}

		bool operator==(const ListIterator& rhs)
		{
			if (iptr == rhs.iptr)
				return true;
			return false;
		}

		bool operator !=(const ListIterator& rhs)
		{
			return !(iptr == rhs.iptr);
		}



	private:
		typename DLList<T>::Node<T>* iptr;

	};

public:

	typedef ListIterator Iterator;
	Iterator begin()
	{
		Iterator I(head);
		return I;
	}
	Iterator end()
	{
		Iterator I(tail);
		return I;
	}

	DLList()
	{
		head = new Node<T>();
		tail = new Node<T>();
		tail->prev = head;
		head->next = tail;
		size = 0;

	}
	void InsertAtEnd(DLList<T>& list, int n)
	{
		Node<T>* ptr, * ptr2 = list.tail->prev;
		ptr = ptr2;
		int i = n - 1;
		while (i > 0)
		{
			ptr = ptr->prev;
			i--;

		}
		Node<T>* temp = tail->prev;
		temp->next = ptr;
		tail->prev = ptr2;
		ptr->prev->next = list.tail;
		list.tail->prev = ptr->prev;
		ptr2->next = tail;
		ptr->prev = temp;
		size = size + n;
		list.size -= n;


	}

	// used by tableaus to move card to their end through a given iterator
	bool InsertAtEnd(typename DLList<T>::Iterator ptr)
	{
		Node<T>* newNode = ptr.iptr;;

		bool inserted = false;
		if (!inserted)
		{
			tail->prev->next = newNode;
			newNode->prev = tail->prev;
			newNode->next = tail;
			tail->prev = newNode;
			size++;
			inserted = true;
		}

		return inserted;
	}

	int Size() const
	{
		return size;
	}

	bool isEmpty()
	{
		if (tail == NULL)
			return true;
		return false;

	}

	bool DeleteFromStart()
	{
		bool deleted = false;
		if (head->next != tail)// if not pointing to each other
		{
			head->next = head->next->next;
			delete head->next->prev;
			head->next->prev = head;
			size--;
			deleted = true;

		}

		return deleted;
	}

	//this is used to shuffle the initially dll based deck of 52 cards in the shuffle funtion
	bool Swap(int leftInd, int rightInd)
	{
		if (leftInd < 1 || rightInd>size)
		{
			return false;
		}
		else
		{
			Node<T>* t1 = head->next, * t2 = head->next;
			int count = 1;
			for (; count != leftInd; count++)
			{
				t1 = t1->next;
			}

			count = 1;
			for (; count != rightInd && t2 != NULL; count++)
			{
				t2 = t2->next;
			}
			if (t1->next != t2)// non adjacent
			{
				Node<T>* t3 = t2->prev, * t4 = t1->next;
				t3->next = t1;
				t2->prev = t1->prev;
				t1->prev->next = t2;
				t1->prev = t3;
				t1->next = t2->next;
				t2->next->prev = t1;
				t2->next = t4;
				t4->prev = t2;
			}
			else// adjacent
			{
				t1->next = t2->next;
				t2->prev = t1->prev;
				t1->prev->next = t2;
				t1->prev = t2;
				t2->next->prev = t1;
				t2->next = t1;

			}


			return true;
		}
	}


	//this returns the iterator to the n-1th node from the tail of a tableau to shift n number of cards from one list to another, that is , moving sublist
	typename DLList<T>::Iterator ReturnNth(int n)
	{
		typename DLList<T>::Iterator ret = --end();
		int i = n - 1;
		while (i > 0)
		{
			--ret;
			i--;
		}

		return ret;
	}

	//this returns it to the top of list, used by stack that is implemented by dll to peek
	typename DLList<T>::Iterator ReturnItToTop()
	{
		typename DLList<T>::Iterator topNode = ++begin();

		return topNode;


	}
	void PrintHead()
	{
		cout << head->next->data;
	}

	//Used by stack to add to its head by the given iterator

	bool PushAtStart(typename DLList<T>::Iterator& ptr) {
		if (ptr.iptr == nullptr || head == nullptr)
		{
			return false; // Check if the iterator or the head is invalid
		}


		Node<T>* newNode = ptr.iptr; // Get the actual node pointer from the iterator


		newNode->next = head->next;

		// If there's a node after head, update its previous pointer
		if (head->next != nullptr) {
			head->next->prev = newNode; // Update the next node's previous to the new node
		}

		// Update head's next to point to the new node
		head->next = newNode;
		newNode->prev = head;

		size++;

		return true;
	}

	//Also used by the stack implemented by dll to remove the node through itr, so it doesnt delete the node but just returns the it by ending its previous links 
	typename DLList<T>::Iterator PopFromStart()
	{
		typename DLList<T>::Iterator it = NULL;
		if (isEmpty()) return it;

		it = this->begin();
		it++;

		Node<T>* temp = head->next;
		head->next = temp->next;

		// Update the previous pointer of the new first node (if it exists)
		if (head->next != nullptr) {
			head->next->prev = head;
		}

		// Nullify the popped node's pointers (to avoid dangling references)
		temp->next = nullptr;
		temp->prev = nullptr;

		size--;

		return it; // Successfully popped the node
	}

	//used in initilialization of the deck, to create the nodes only once
	void InsertAtStart(T const value)
	{
		Node<T>* temp = new Node<T>(value);
		head->next->prev = temp;
		temp->next = head->next;
		head->next = temp;
		temp->prev = head;
		size++;

	}

	// shuffling deck when game starts
	void shuffle()
	{
		srand(time(0));
		for (int i = Size() - 1; i > 0; i--)
		{
			int j = rand() % (i + 1);
			if (i != 0 && j != 0 && i != j)
			{
				bool swapDone = Swap(i, j);
			}

		}
	}

	T top()
	{
		if (!isEmpty())
			return head->next->data;
	}

	//detaches the last node by ending previourz
	typename DLList::Iterator DetachLastNode()
	{
		typename DLList::Iterator lastNode = end();
		--lastNode;


		lastNode.iptr->prev->next = tail;
		tail->prev = lastNode.iptr->prev;

		lastNode.iptr->next = NULL;
		lastNode.iptr->prev = NULL;

		size--;

		return lastNode;
	}

	~DLList()
	{

		Node<T>* temp = head;
		for (int i = 0; i < size + 2; i++)
		{
			head = temp->next;
			delete temp;
			temp = head;

		}
	}

};


class Card;

template<class T>
class Stack
{
private:
	DLList<T>* List;


public:
	Stack()
	{
		List = new DLList<T>();

	}

	bool isEmpty()
	{
		if (List->Size() == 0)
			return true;
		else
			return false;
	}

	void PushIntoStack(typename DLList<T>::Iterator I)
	{
		List->PushAtStart(I);

	}

	void PushIntoStack(T const val)
	{
		List->InsertAtStart(val);
	}

	//remove and return data 
	typename DLList<T>::Iterator PopFromStack()
	{
		typename DLList<T>::Iterator Data = List->PopFromStart();
		return Data;
	}

	bool PopFromStack(int)
	{
		if (List->DeleteFromStart())
			return true;
		return false;

	}

	typename DLList<T>::Iterator topItem(int)
	{
		return List->ReturnItToTop();
	}

	void PrintTop() const
	{
		if (!List->isEmpty())
			List->PrintHead();
		else
		{
			cout << "[   ]";
		}
	}

	~Stack()
	{
		delete List;
	}
};

class Card {
	friend ostream& operator<<(ostream& os, const Card& card);
private:
	string suit;
	int rank;

public:
	// Constructor
	Card()
	{
		rank = 0;
	}
	Card(string _suit, int _rank) : suit(_suit), rank(_rank) {}

	bool Compatible(const Card& obj)
	{
		bool compatible = false;
		if ((suit == "Hearts" || suit == "Diamonds") && (obj.suit == "Clubs" || obj.suit == "Spades") && (obj.rank - rank == 1))
			compatible = true;
		else if ((suit == "Spades" || suit == "Clubs") && (obj.suit == "Hearts" || obj.suit == "Diamonds") && (obj.rank - rank == 1))
			compatible = true;

		return compatible;

	}
	bool CanMoveToFoundation(const Card& obj)
	{
		bool CanMove = false;
		if (suit == obj.suit && (rank - obj.rank == 1))
			CanMove = true;
		return CanMove;
	}

	bool isAce()
	{
		if (rank == 1)
			return true;

		return false;

	}

	bool isKing()
	{
		if (rank == 13)
			return true;
		return false;
	}
};

// Overload the << operator so that our card data can be printed
ostream& operator<<(ostream& os, const Card& card) {

	// Set color for red suits
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (card.suit == "Hearts" || card.suit == "Diamonds") {
		SetConsoleTextAttribute(hConsole, RED);
	}

	// Print rank
	if (card.rank == 13) os << "K";
	else if (card.rank == 12) os << "Q";
	else if (card.rank == 11) os << "J";
	else if (card.rank == 1) os << "A";
	else os << card.rank;

	// Print suit symbol
	if (card.suit == "Spades") os << "\x06";
	else if (card.suit == "Hearts") os << "\x03";
	else if (card.suit == "Clubs") os << "\x05";
	else if (card.suit == "Diamonds") os << "\x04";

	if (card.rank % 10 != 0)
	{
		cout << " ";
	}
	else
	{
		cout << "  ";
	}

	// Reset color
	SetConsoleTextAttribute(hConsole, RESET);

	return os; // Return the stream for chaining
}

class Command {
	string command;
	string source;
	string dest;
	int number;

public:
	// Function to parse the command from a string
	bool ParseCommand(const string& str)
	{
		command.clear();
		int pos = 0;
		bool valid = true;

		// Check for empty input
		if (str.empty()) {
			return false;
		}

		// Check for empty input
		if (str.empty()) {
			return false; // Return false for empty input
		}

		// Ignore leading spaces
		while (pos < str.length() && str[pos] == ' ') {
			pos++; // Skip spaces
		}

		// Read command character until a space or end of string
		while (pos < str.length() && str[pos] != ' ') {
			command += str[pos]; // Append to command string
			pos++;
		}

		// Ignore leading spaces
		while (pos < str.length() && str[pos] == ' ') {
			pos++; // Skip spaces
		}

		// If the command is 's', no further processing is needed
		if (command[0] == 's' || command[0] == 'z') {
			source = "";
			dest = "";
			number = 0;
			if (command[1] != '\0')
			{
				return false;
			}
			return true;
		}

		else if (command[0] == 'm')
		{

			while (pos < str.length() && str[pos] != ',' && str[pos] != ' ') {
				source += str[pos++];
			}


			// Ignore spaces or check for a comma
			while (pos < str.length() && (str[pos] == ' ' || str[pos] == ',')) pos++;

			// Parse destination
			while (pos < str.length() && str[pos] != ',' && str[pos] != ' ') {
				dest += str[pos++];
			}

			// Ignore spaces or check for a comma
			while (pos < str.length() && (str[pos] == ' ' || str[pos] == ',')) pos++;

			// Parse number
			if (pos < str.length()) {
				if (isdigit(str[pos])) {
					number = str[pos] - '0'; // Convert char to int
					pos++;
				}
				else {
					valid = false; // Number is not valid
				}
			}
		}
		else
		{
			valid = false;
		}

		return valid; //print error in game class loop in play function
	}

	string getType()
	{
		return command;
	}

	string getSource()
	{
		return source;
	}

	string getDest()
	{
		return dest;
	}

	int getNum()
	{
		return number;
	}

};


class Game {
private:
	DLList<Card> tableau[7];
	Stack<Card> foundations[4];
	Stack<Card> stock;
	Stack<Card> waste;
	Stack<Command> commandStack;
	int stockSize;
	int wasteSize;
	int foundationSizes[4]{ 0 };
	int ColSizes[7];
	int exposedCards[7];

public:
	Game()
	{
		stockSize = 52;
		wasteSize = 0;

		// Define a vector to hold names of the four suits
		vector<string> suits(4);

		// Assign names to the suits
		suits[0] = "Hearts";
		suits[1] = "Diamonds";
		suits[2] = "Clubs";
		suits[3] = "Spades";

		DLList<Card>pile;

		for (int i = 0; i < 4; i++)
		{
			for (int j = 1; j <= 13; j++)
			{

				pile.InsertAtStart(Card(suits[i], j));
			}
		}

		pile.shuffle();
		int j;


		for (int i = 0; i < stockSize; i++)
		{
			// Assuming DLList has a method to access elements
			typename DLList<Card>::Iterator it = pile.DetachLastNode(); // Method to get the last card from pile
			stock.PushIntoStack(it);     // Push the card onto the stock stack

		}


		for (int i = 0; i < 7; i++)
		{
			for (j = 0; j <= i; j++)
			{
				typename DLList<Card>::Iterator ptrToStockTop = stock.PopFromStack();
				tableau[i].PushAtStart(ptrToStockTop);
				stockSize--;
			}
			ColSizes[i] = j;
			exposedCards[i] = 1;

		}

		PrintInterface();

	}

	void PrintInterface()
	{

		cout << "Stock\t\tWaste\t\tFoundation 1\t\tFoundation 2\t\tFoundation3\t\tFoundation 4\t\t " << endl;
		cout << "[  ]";
		cout << "  \t\t";
		waste.PrintTop();
		cout << "  \t\t";


		for (int i = 0; i < 4; i++)
		{
			foundations[i].PrintTop();
			cout << "\t\t\t";
		}

		cout << endl << "(" << stockSize << " cards)\t" << "(" << wasteSize << " cards)\t";

		for (int i = 0; i < 4; i++)
		{
			cout << "(" << foundationSizes[i] << " cards)\t\t";
		}

		cout << "\n\n";
		for (int i = 0; i < 7; i++)
		{
			cout << "Column " << i + 1 << "\t";
		}

		cout << endl;

		for (int i = 0; i < 7; i++)
		{
			cout << "( " << ColSizes[i] << " )\t\t";
		}

		cout << endl;

		// Find the maximum column size
		int maxRows = 0;
		for (int i = 0; i < 7; ++i) {
			if (tableau[i].Size() > maxRows)
				maxRows = tableau[i].Size();
		}

		cout << "Tableau:\n";
		// Print each row one by one
		int hiddencardsToPrint[7]{ 0 };
		for (int row = 0; row < maxRows; ++row)
		{
			for (int col = 0; col < 7; ++col)
			{
				int i = 0;
				DLList<Card>::Iterator iter = tableau[col].begin();
				iter++;
				hiddencardsToPrint[col] = ColSizes[col] - exposedCards[col]; // Number of hidden cards

				int currentRow = 0;

				// Move the iterator to the current row position
				while (iter != (--tableau[col].end()) && currentRow < row) {
					++iter;
					++currentRow;
				}
				if (currentRow == row && iter != tableau[col].end() && iter != NULL)
				{
					if (iter == (--tableau[col].end()) || (currentRow >= hiddencardsToPrint[col] && hiddencardsToPrint[col] > -1))
					{
						cout << *iter << "\t\t";
						i++;
					}
					else if (hiddencardsToPrint[col] > currentRow)
					{
						cout << "[  ]\t\t";
					}
					else if (tableau[col].Size() == 0)
					{
						cout << "\t\t";
					}


				}
				else
					cout << "\t\t";

			}
			cout << endl;
		}
		cout << "\n\n\n\n";
	}

	void Play()
	{
		string command;
		bool gameHasEnded = false;
		while (!gameHasEnded)
		{
			cout << "------------------------------------------------------------------------------------------------------------" << endl;
			cout << "Enter Move: ";
			getline(cin, command);

			if (command == "q")
			{
				gameHasEnded = true;
			}
			else
			{
				Command c;
				while (!c.ParseCommand(command))
				{
					cout << "Enter Valid Move:" << endl;
					getline(cin, command);
				}

				commandStack.PushIntoStack(c);
				if (!ProcessCmd())
				{
					cout << "Move Cannot be made!" << endl;
				}

				PrintInterface();

				if (GameIsWon())
				{
					gameHasEnded = true;
					cout << "You have Won the Game, Congratulations!";
				}
			}



		}
	}
	bool GameIsWon()
	{
		for (int i = 0; i < 4; i++)
		{
			if (!((*foundations[i].topItem(0)).isKing()))
				return false;


		}

		return true;
	}
	bool ProcessCmd()
	{
		//this function will break down the instructions and call the public functions of the classes to do what is required
		typename DLList<Command>::Iterator c = commandStack.topItem(0);

		if ((*c).getType()[0] == 's')
		{
			if (stockSize)
			{
				typename DLList<Card>::Iterator ItTodraw = stock.PopFromStack();
				waste.PushIntoStack(ItTodraw);
				wasteSize++;
				stockSize--;
				return true;
			}
			else
			{
				while (!waste.isEmpty())
				{
					typename DLList<Card>::Iterator ItTowasteTop = waste.PopFromStack();
					stock.PushIntoStack(ItTowasteTop);
					stockSize++;
					wasteSize--;

				}
				return true;
			}
		}

		else if ((*c).getType()[0] == 'z')
		{
			commandStack.PopFromStack(0); //remove the z operation from stack top by deletion

			undoOperation();
			return true;
		}

		else if ((*c).getType()[0] == 'm')
		{
			string source = (*c).getSource();
			string dest = (*c).getDest();
			int numberOfcards = (*c).getNum();
			bool success = move(source, dest, numberOfcards);
			if (success)
			{
				return true;
			}
			else
			{
				cout << "Move Cannot be Made!" << endl;
				commandStack.PopFromStack(0); //invalid move so not keeping it in our history command

			}


		}

	}

	void undoOperation()
	{
		if (!commandStack.isEmpty())
		{
			typename DLList<Command>::Iterator undo = commandStack.PopFromStack();
			//waste to stock
			if ((*undo).getType()[0] == 's')
			{
				if (!waste.isEmpty())
				{
					typename DLList<Card>::Iterator ItTowasteTop = waste.PopFromStack();
					stock.PushIntoStack(ItTowasteTop);
					wasteSize--;
					stockSize++;

				}

			}
			//move semantics 
			else if ((*undo).getType()[0] == 'm')
			{
				string source = (*undo).getDest();
				string dest = (*undo).getSource();
				int numberOfcards = (*undo).getNum();
				bool success = Undomove(source, dest, numberOfcards);

				if (!success)
				{
					cout << "Move Cannot be Made!" << endl;

				}


			}
			commandStack.PushIntoStack(undo);
			commandStack.PopFromStack(0); //forever deletion of the undid part in any case

		}
	}

	bool move(string s, string d, int n)
	{
		if (n <= 0)
			return false; // Invalid move request if n is less than 1

		// Source is a tableau column
		if (s[0] == 'c')
		{
			int srcCol = s[1] - '1'; // Convert '1'..'7' to index 0..6 for tableau
			if (tableau[srcCol].Size() < n)
				return false; // Not enough cards to move

			//Destination is a tableau column
			if (d[0] == 'c')
			{
				typename DLList<Card>::Iterator destCard, toMove;
				bool CanMove = false;
				int destCol = d[1] - '1'; // Convert '1'..'7' to index 0..6
				if (n > 1)
				{
					toMove = tableau[srcCol].ReturnNth(exposedCards[srcCol]); // Get the top exposed card to move
				}
				else
					toMove = tableau[srcCol].ReturnNth(1);
				if (tableau[destCol].Size() > 0)
					destCard = tableau[destCol].ReturnNth(1);// Get the top card of the destination column
				else
				{
					if ((*toMove).isKing())
						CanMove = true;
				}

				if ((*toMove).Compatible(*destCard) || CanMove) {
					// Move n cards from srcCol to destCol
					tableau[destCol].InsertAtEnd(tableau[srcCol], n);
					ColSizes[srcCol] -= n;
					ColSizes[destCol] += n;
					if (exposedCards[srcCol] == 1 && ColSizes[srcCol] == 0 || (exposedCards[srcCol] > 1))
					{
						exposedCards[srcCol] -= n;
						if (exposedCards[srcCol] == 0 && ColSizes[srcCol] != 0)
						{
							exposedCards[srcCol] = 1;
						}
						else if (exposedCards[srcCol] == 0 && ColSizes[srcCol] == 0)
						{
							exposedCards[srcCol] = 0;
						}
					}

					if (ColSizes[destCol] != 1) //abhi khali tha and abhi abhi card dala hai tou keep it 1
					{
						exposedCards[destCol] += n;
					}

					return true;
				}

			}
			// Destination is a foundation
			else if (d[0] == 'f') {
				bool CanMove = false;
				typename DLList<Card>::Iterator destCard, toMove;
				int foundationIdx = d[1] - '1'; // Convert '1'..'4' to index 0..3 for foundations
				if (tableau[srcCol].Size() > 0)
					toMove = tableau[srcCol].DetachLastNode();// Get the top card ptr from the source column
				if (!foundations[foundationIdx].isEmpty())
					destCard = foundations[foundationIdx].topItem(0);
				else
				{
					if ((*toMove).isAce())
						CanMove = true;
				}


				if ((*toMove).CanMoveToFoundation(*destCard) || CanMove) {
					// Move the card to the foundation
					int i = 0;
					while (i < n)
					{
						foundations[foundationIdx].PushIntoStack(toMove);
						ColSizes[srcCol]--;
						if (exposedCards[srcCol] == 1 && ColSizes[srcCol] == 0 || (exposedCards[srcCol] > 1))
						{
							exposedCards[srcCol] -= n;
							if (exposedCards[srcCol] == 0 && ColSizes[srcCol] != 0)
							{
								exposedCards[srcCol] = 1;
							}
							else if (exposedCards[srcCol] == 0 && ColSizes[srcCol] == 0)
							{
								exposedCards[srcCol] = 0;
							}

						}

						foundationSizes[foundationIdx]++;
						i++;
					}
					return true;
				}
				else
				{
					tableau[srcCol].InsertAtEnd(toMove);
				}
			}
		}
		// Source is the waste pile
		else if (s == "w")
		{
			typename DLList<Card>::Iterator destCard, toMove;
			toMove = waste.PopFromStack();

			// Destination is a tableau column
			if (d[0] == 'c') {
				int destCol = d[1] - '1'; // Convert '1'..'7' to index 0..6 for tableau
				bool CanMove = false;
				if (tableau[destCol].Size() > 0)
					destCard = tableau[destCol].ReturnNth(1);// Get the top card of the destination column
				else
				{
					if ((*toMove).isKing())
						CanMove = true;
				}
				if ((*toMove).Compatible(*destCard) || CanMove)
				{
					// Move the card from waste to the destination column
					bool done = tableau[destCol].InsertAtEnd(toMove);
					wasteSize--;
					ColSizes[destCol]++;
					exposedCards[destCol]++;
					return true;
				}
				else
				{
					tableau[destCol].InsertAtEnd(destCard);
					waste.PushIntoStack(toMove);
				}
			}
			//Destination is a foundation
			else if (d[0] == 'f')
			{
				bool CanMove = false;
				int foundationIdx = d[1] - '1'; // Convert '1'..'4' to index 0..3 for foundations
				if (!foundations[foundationIdx].isEmpty())
					destCard = foundations[foundationIdx].topItem(0);
				else
				{
					if ((*toMove).isAce())
						CanMove = true;
				}

				if ((*toMove).CanMoveToFoundation((*destCard)) || CanMove)
				{
					// Move the card from waste to the foundation
					foundations[foundationIdx].PushIntoStack(toMove);
					wasteSize--;
					foundationSizes[foundationIdx]++;
					return true;
				}
				else
				{
					waste.PushIntoStack(toMove);
				}
			}

		}

		//Invalid move
		return false;

	}
	bool Undomove(string s, string d, int n)
	{
		typename DLList<Card>::Iterator destCard, toMove;
		if (n <= 0)
			return false; // Invalid move request if n is less than 1

		if (s[0] == 'c')
		{
			int srcCol = s[1] - '1'; // Convert '1'..'7' to index 0..6 for tableau

			//Destination is a tableau column
			if (d[0] == 'c')
			{
				int destCol = d[1] - '1'; // Convert '1'..'7' to index 0..6
				if (n > 1)
				{
					toMove = tableau[srcCol].ReturnNth(exposedCards[srcCol]); // Get the top exposed card to move
				}
				else
					toMove = tableau[srcCol].ReturnNth(1);

				if (tableau[destCol].Size() > 0)
					destCard = tableau[destCol].ReturnNth(1);// Get the top card of the destination column

				tableau[destCol].InsertAtEnd(tableau[srcCol], n);
				int destExposedPrev = exposedCards[destCol];
				ColSizes[srcCol] -= n;
				ColSizes[destCol] += n;
				if (exposedCards[srcCol] == 1 && ColSizes[srcCol] == 0 || (exposedCards[srcCol] > 1))
				{
					exposedCards[srcCol] -= n;
					if (exposedCards[srcCol] == 0 && ColSizes[srcCol] != 0)
					{
						exposedCards[srcCol] = 1;
					}
					else if (exposedCards[srcCol] == 0 && ColSizes[srcCol] == 0)
					{
						exposedCards[srcCol] = 0;
					}
				}
				exposedCards[destCol] = n;


				return true;


			}
			//destination is waste
			else if (d[0] == 'w')
			{
				toMove = tableau[srcCol].DetachLastNode();
				waste.PushIntoStack(toMove);
				wasteSize++;
				ColSizes[srcCol] -= n;
				if (exposedCards[srcCol] == 1 && ColSizes[srcCol] == 0 || (exposedCards[srcCol] > 1))
				{
					exposedCards[srcCol] -= n;
					if (exposedCards[srcCol] == 0 && ColSizes[srcCol] != 0)
					{
						exposedCards[srcCol] = 1;
					}
				}
				return true;
			}

		}
		//source is foundation
		if (s[0] == 'f')
		{
			int foundationIdx = s[1] - '1';
			toMove = foundations[foundationIdx].PopFromStack();
			foundationSizes[foundationIdx]--;

			//destination is waste
			if (d[0] == 'w')
			{
				waste.PushIntoStack(toMove);
				wasteSize++;
				return true;

			}
			//destination is column
			else if (d[0] == 'c')
			{

				int destCol = d[1] - '1'; // Convert '1'..'7' to index 0..6 for tableau

				tableau[destCol].InsertAtEnd(toMove);
				ColSizes[destCol]++;
				if (exposedCards[destCol] >= 0)
				{
					exposedCards[destCol]++;//otherwise they remain the same(1)
				}


				return true;
			}
		}

		return false;
	}
};



int main()
{

	Game Solataire;

	Solataire.Play();




	return 0;
}