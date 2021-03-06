/*
 * Graph.h
 *
 *  Created on: 12/03/2018
 *      Author: filipa
 */

#ifndef GRAPH_H_
#define GRAPH_H_

#include <set>
#include <map>
#include <cfloat>
#include <iostream>
#include <utility>
#include <vector>
#include <unordered_map>
#include <utility>
#include <algorithm>
#include <climits>
#include <string>
#include <queue>
#include <cmath>
#include "MutablePriorityQueue.h"

const constexpr double BUS_TIME_MULTIPLIER = 0.025;
const constexpr double SUBWAY_TIME_MULTIPLIER = 0.02;
const constexpr double WALK_TIME_MULTIPLIER = 0.1;

const constexpr double BUS_PRICE = 1.20;
const constexpr double SUBWAY_PRICE = 1.70;
const constexpr double WALK_PRICE = 0.0;

const constexpr double DEFAULT_TRANSBORD_TIME = 5.0;

const string TIME_MODE = "time";
const string PRICE_MODE = "price";

const string BUS = "bus";
const string WALK = "walk";
const string SUBWAY = "subway";

using namespace std;

template<typename T>
class Edge;

//////////////////////////////////////////////////////////////////////////////////
/////							  EXCEPTION								    //////
//////////////////////////////////////////////////////////////////////////////////
class NodeNotFound;

//////////////////////////////////////////////////////////////////////////////////
/////								NODE									 /////
//////////////////////////////////////////////////////////////////////////////////
template<typename T>
class Node {
private:
	unsigned int ID;
	T info;
	vector<Edge<T>> edges;
	bool visited = false;
	double distance = DBL_MAX;
	Node<T> * lastNode;
	int x;
	int y;
	double transbord_time;

	// ---- Needed for Dijkstra with Transbordos ----
	int numTransbords;
	string lastConnection = "";

	// ---- Needed for Dijsktra with Price -----
	double price;
	double walked_time;

public:
	Node(const T &value, unsigned int ID);
	Node(const T &value, unsigned int ID, int x, int y);

	virtual ~Node();

	void addEdge(Edge<T> edge);
	unsigned int getId() const;
	unsigned int getNumberOfEdges() const;
	const vector<Edge<T>> & getEdges() const;
	T getInfo() const;
	int getX() const;
	int getY() const;
	double euclidianDistance(Node<T> * node);
	double getTransbordTime() const;
	void setTransbordTime(double time);

	// ---- Needed for Dijkstra with Transbordos ----
	int getNumTransbords() const;
	void setNumTransbords(int i);
	string getLastConnection() const;
	void setLastConnection(string connect);
	string getLastTypeConnection() const;

	// ---- DIJKSTRA INFO ----
	double getDistance() const;
	void setDistance(double distance);
	Node* getLastNode();
	void setLastNode(Node* lastNode);
	void clearLastNode();
	void setVisited(bool vis);
	bool getVisited() const;
	double getPrice() const;
	void setPrice(double price);
	double getWalkedTime() const;
	void setWalkedTime(double distance);

	// ---- Mutable Priority Queue Info ----
	bool operator<(Node<T> & node) const;
	int queueIndex = 0;

	static bool sortByDistance;

};

template<typename T>
void Node<T>::setWalkedTime(double time) {
	this->walked_time = time;
}

template<typename T>
double Node<T>::getWalkedTime() const {
	return this->walked_time;
}

/**
 * @brief Returns the node's transbord time
 *
 * @return transbord time
 */
template<typename T>
double Node<T>::getTransbordTime() const {
	return this->transbord_time;
}

/**
 * @brief Sets the node's transbord time
 *
 * @param time - the new transbord time
 */
template<typename T>
void Node<T>::setTransbordTime(double time) {
	this->transbord_time = time;
}

/**
 * @Brief Returns the euclidian distance between two nodes
 *
 * @return Euclidian distance
 */
template<typename T>
double Node<T>::euclidianDistance(Node<T> * node) {

	return SUBWAY_TIME_MULTIPLIER
			* sqrt(
					(this->x - node->x) * (this->x - node->x)
							+ (this->y - node->y) * (this->y - node->y));

}

template<typename T>
bool Node<T>::sortByDistance = true;

/**
 * @brief Returns the node's x position
 *
 * @return X position
 */
template<typename T>
int Node<T>::getX() const {
	return this->x;
}

/**
 * @brief Returns the node's y position
 *
 * @return Y position
 */
template<typename T>
int Node<T>::getY() const {
	return this->y;
}

/**
 * @brief Gives information about the type of transportation used to get to that node
 *
 * @see Node<T>::getLastConnection()
 *
 * @return string containing the information
 */
template<typename T>
string Node<T>::getLastTypeConnection() const {

	return this->getLastConnection().substr(0, 4);
}
/**
 * @brief Gives the information about the last edge that connected to this node
 *
 * This allows us to build a more detailed description of the path
 *
 * @see Edge<T>::getEdgeConnection()
 *
 * @return string containing the information
 */
template<typename T>
string Node<T>::getLastConnection() const {
	return this->lastConnection;
}

/**
 * @brief Sets the lastConnection attribute with lastConnection passed by parameter
 *
 * @param connect - a information
 */
template<typename T>
void Node<T>::setLastConnection(string connect) {
	this->lastConnection = connect;
}

/**
 * @brief Gives the number of times a person has to change transports when it reaches this node
 *
 * @return number of transbords
 */
template<typename T>
int Node<T>::getNumTransbords() const {
	return this->numTransbords;
}

/**
 * @brief Sets the numTransbords attribute with numTransbords passed by parameter
 *
 * @param i - the number of Transbords
 */
template<typename T>
void Node<T>::setNumTransbords(int i) {
	this->numTransbords = i;
}

/**
 * @brief Creates a node
 * The node's ID will be its place in the node's vector in graph, for easier access
 *
 * @param info - the node's value
 * @param ID - the node's ID
 */
template<typename T>
Node<T>::Node(const T &info, unsigned int ID) {
	this->info = info;
	this->ID = ID;
	this->distance = DBL_MAX;
	this->lastNode = NULL;
	this->visited = false;
	this->x = 0;
	this->y = 0;
	this->numTransbords = 0;
	this->transbord_time = 0;
	this->price = 0;
}

/**
 * @brief Creates a node
 * The node's ID will be its place in the node's vector in graph, for easier access
 *
 * @param info - the node's value
 * @param ID - the node's ID
 * @param x - the node's x position
 * @param y - the node's y position
 */
template<typename T>
Node<T>::Node(const T &info, unsigned int ID, int x, int y) {
	this->info = info;
	this->ID = ID;
	this->distance = DBL_MAX;
	this->lastNode = NULL;
	this->visited = false;
	this->x = x;
	this->y = y;
	this->numTransbords = 0;
	this->transbord_time = 0;
	this->price = 0;
}

/**
 * @brief Destroys a node
 */
template<typename T>
Node<T>::~Node() {
	edges.clear();
}

/**
 * @brief Adds an edge to a node
 *
 * @param edge - a generic edge to add to the node
 */
template<typename T>
void Node<T>::addEdge(Edge<T> edge) {
	edges.push_back(edge);
}

/**
 * @brief Returns the node's ID
 *
 * @return ID
 */
template<typename T>
unsigned int Node<T>::getId() const {
	return ID;
}

/**
 * @brief Returns the number of edges the node has
 *
 * @return Number of edges
 */
template<typename T>
unsigned int Node<T>::getNumberOfEdges() const {
	return this->edges.size();
}

/**
 * @brief Returns a vector with all the edges of the node
 *
 * @return Vector with the node's edges
 */
template<typename T>
const vector<Edge<T>> & Node<T>::getEdges() const {
	return this->edges;
}

/**
 * @brief Clears the lastNode (for Dijkstra running)
 */
template<typename T>
void Node<T>::clearLastNode() {
	this->lastNode = NULL;
}

/**
 * @brief Returns the LastNode info for Dijkstra
 *
 * @return Last Node
 */
template<typename T>
Node<T>* Node<T>::getLastNode() {
	return this->lastNode;
}

/**
 * @brief Sets the lastNode attribute with lastNode passed by parameter
 *
 * @param lastNode - a Node
 */
template<typename T>
void Node<T>::setLastNode(Node* lastNode) {
	this->lastNode = lastNode;
}

/**
 * @brief Returns the current path distance to get to this Node
 *
 * @return distance
 */
template<typename T>
double Node<T>::getDistance() const {
	return this->distance;
}

/**
 * @brief Sets the distance traveled to get to this Node
 *
 * @param distance
 */
template<typename T>
void Node<T>::setDistance(double distance) {
	this->distance = distance;
}

/**
 * @brief  Returns the node's information
 *
 * @return info
 */
template<typename T>
T Node<T>::getInfo() const {
	return this->info;
}

/**
 * @brief Sets the visited attribute on the Node
 *
 * @param vis - what to set
 */
template<typename T>
void Node<T>::setVisited(bool vis) {
	this->visited = vis;
}

/**
 * @brief Gets the information on wether the Node has been visited or not
 *
 * @return true or false
 */
template<typename T>
bool Node<T>::getVisited() const {
	return this->visited;
}

/**
 * @brief Returns the price of the trip up to that Node
 */
template<typename T>
inline double Node<T>::getPrice() const {
	return price;
}

/**
 * @brief Sets the price of the trip up to that Node
 */
template<typename T>
inline void Node<T>::setPrice(double price) {
	this->price = price;
}

/**
 * @brief Operator < to be able to compare different Nodes
 *
 * @param node - a Node to compare to the current Node
 *
 * @return true or false wether the Node is minor than the one we are comparing to
 */
template<typename T>
bool Node<T>::operator<(Node<T> & node) const {

	if (Node<T>::sortByDistance)
		return this->distance < node.distance;
	else
		return this->price < node.price;
}

//////////////////////////////////////////////////////////////////////////////////
/////								EDGE									 /////
//////////////////////////////////////////////////////////////////////////////////
template<typename T>
class Edge {
protected:
	Node<T>* destiny;
	double weight;
	string type;
	string lineID;

public:

	Edge(Node<T> * destiny, double weight, string type, string lineID);
	virtual ~Edge();
	Node<T>* getDestiny() const;
	double getWeight() const;
	string getType() const;
	string getLineID() const;
	string getEdgeConnection() const;
	double getPriceWeight() const;

};

/**
 * @brief Returns the ticket price of the trip represented by the edge
 *
 * @return the value of the ticket
 */
template<typename T>
double Edge<T>::getPriceWeight() const {
	if (this->type == BUS)
		return BUS_PRICE;
	else if (this->type == SUBWAY)
		return SUBWAY_PRICE;
	else
		return WALK_PRICE;

}

/**
 * @brief Returns a string containing information about the edge
 *
 * The information about a edge is its type, i.e. bus, subway or walk, and the line associated with it, e.g line 204.
 *
 * @return string containing the information
 *
 */
template<typename T>
string Edge<T>::getEdgeConnection() const {
	string result = this->type + " " + this->lineID;

	return result;
}

/**
 * @brief  Creates an Edge
 *
 * @param destiny - the node of destiny of the Edge
 * @param weight - the weight of the Edge
 * @param type - the type of Edge
 * @param lineID - the ID of the line of the Edge
 */
template<typename T>
Edge<T>::Edge(Node<T> * destiny, double weight, string type, string lineID) {
	this->destiny = destiny;
	this->weight = weight;
	this->type = type;
	this->lineID = lineID;
}

/**
 * @brief Destroys an Edge
 */
template<typename T>
Edge<T>::~Edge() {
}

/**
 * @brief Returns the destiny of an Edge
 *
 * @return the Node that is in the end of the Edge
 */
template<typename T>
Node<T>* Edge<T>::getDestiny() const {
	return destiny;
}

/**
 * @brief Returns the type of an Edge
 *
 * @return the type of the Edge ("walk", "edge", "bus")
 */
template<typename T>
string Edge<T>::getType() const {
	return this->type;
}

template<typename T>
string Edge<T>::getLineID() const {
	return this->lineID;
}


/**
 * @brief Returns the weight of the Edge and counts in the multiplier considering which type of transportation is
 *
 * @return the weight of the Edge with the correct multiplier
 */
template<typename T>
double Edge<T>::getWeight() const {
	if (this->type == SUBWAY)
		return this->weight * SUBWAY_TIME_MULTIPLIER;
	else if (this->type == BUS)
		return this->weight * BUS_TIME_MULTIPLIER;
	else
		return this->weight * WALK_TIME_MULTIPLIER;
}

//////////////////////////////////////////////////////////////////////////////////
/////								GRAPH									 /////
//////////////////////////////////////////////////////////////////////////////////
template<typename T>
class Graph {
private:
	vector<Node<T> *> nodes;
	map<string, set<unsigned int>> listStationsByLine;
public:
	Graph();

	virtual ~Graph();

	void addNode(T nodeData, int x, int y);
	unsigned int getNumNodes() const;	//Get the number of nodes in the graph
	Node<T> * getNodeByID(unsigned int ID) const;//Get one node of the graph by its ID
	unsigned int getNumEdges() const; 	// Get the number of edges in the graph
	vector<Node<T> *> getNodes() const;
	map<string, set<unsigned int>> getStationsByLine() const;
	void findInterfaces();
	void insertStation(string lineID, unsigned int sourceNodeID, unsigned int destinyNodeID);

// ---- Edges Types ----
	void addBusEdge(unsigned int sourceNodeID, unsigned int destinyNodeID,
			double weight, string lineID);
	void addSubwayEdge(unsigned int sourceNodeID, unsigned int destinyNodeID,
			double weight, string lineID);
	void addWalkEdge(unsigned int sourceNodeID, unsigned int destinyNodeID,
			double weight, string lineID);

	vector<T> getPath(Node<T> * dest) const;

	vector<Node<T>*> getDetailedPath(Node<T> * dest) const;

// ---- Dijkstra Algorithms ----
	Node<T> * dijkstra_heap(Node<T> * startNode, Node<T> * endNode);

	Node<T> * dijkstra_queue(Node<T> * startNode, Node<T> * endNode);
	Node<T> * dijkstra_queue_NO_WALK(Node<T> * startNode, Node<T> * endNode);
	Node<T> * dijkstra_queue_TRANSBORDS(Node<T> * startNode, Node<T> * endNode,
			int maxNum);
	Node<T> * dijkstra_queue_PRICE(Node<T> * startNode, Node<T> * endNode,
			double walk_distance);

	// Print in the screen
	void presentPath(vector<Node<T>*> invertedPath);

// ---- A Star Algorithms ----
	Node<T> * A_Star(Node<T> * startNode, Node<T> * endNode);
};

/**
 * @brief Checks which nodes of the graph are an interface for the different types of transports.
 *
 * By interface, we mean a single stop where the user can catch the bus or subway.
 * There will be a certain time of exchanging between them.
 *
 */
template<typename T>
void Graph<T>::findInterfaces() {

	for (auto it = this->nodes.begin(); it != this->nodes.end(); it++) {

		set<string> types;

		for (auto i = (*it)->getEdges().begin(); i != (*it)->getEdges().end();
				i++) {

			if (i->getType() != WALK) {
				types.insert(i->getType());

				if (types.size() > 1) {
					(*it)->setTransbordTime(DEFAULT_TRANSBORD_TIME);
					types.clear();
					break;
				}
			}

		}

		types.clear();

	}

}

/**
 * @brief Returns a node by its ID
 * 
 * @tparam T 
 * @param ID Node ID
 * @return Node<T>* A pointer to the node
 * @throw out_of_range If the node doesn't exist
 */
template<typename T>
Node<T> * Graph<T>::getNodeByID(unsigned int ID) const {
	return this->nodes.at(ID);
}

/**
 * @brief Creates a Graph
 */
template<typename T>
Graph<T>::Graph() {
}

/**
 * @brief Destroys a Graph
 */
template<typename T>
Graph<T>::~Graph() {
	for (auto it = this->nodes.begin(); it != this->nodes.end(); it++)
		delete (*it);
	nodes.clear();
}

/**
 * @brief Creates a Node and adds it to the Graph
 *
 * @param nodeData - the data with which we create a Node
 * @param x - the x position of the Node
 * @param y - the y position of the Node
 */
template<typename T>
void Graph<T>::addNode(T nodeData, int x, int y) {
	this->nodes.push_back(new Node<T>(nodeData, nodes.size(), x, y));
}

/**
 * @brief Gets the number of Nodes in the Graph
 *
 * @return The size of the Nodes vector in the Graph
 */
template<typename T>
unsigned int Graph<T>::getNumNodes() const {
	return nodes.size();
}

/**
 * @brief Gets the number of Edges in the Graph
 *
 * @return The sum of all the Edges in all the Nodes in the Graph
 */
template<typename T>
unsigned int Graph<T>::getNumEdges() const {
	unsigned int size = 0;

	for (unsigned int i = 0; i < nodes.size(); i++) {
		size += nodes.at(i)->getNumberOfEdges();
	}
	return size;
}

/**
 * @brief Returns all the Nodes in the Graph
 *
 * @return a vector equal to the Nodes vector in Graph
 */
template<typename T>
vector<Node<T> *> Graph<T>::getNodes() const {
	return this->nodes;
}

template<typename T>
map<string, set<unsigned int>> Graph<T>::getStationsByLine() const {
	return this->listStationsByLine;
}

/**
 * @brief This function fills a container with all stations for each line
 * 
 * @tparam T 
 * @param lineID
 * @param sourceNodeID
 * @param destinyNodeID 
 */
template<typename T>
void Graph<T>::insertStation(string lineID, unsigned int sourceNodeID, unsigned int destinyNodeID) {
	// try to find this line on map
	auto it = this->listStationsByLine.find(lineID);
	
	if(it == listStationsByLine.end()) {
		// this line is still not listed
		set<unsigned int> stationList;

		stationList.insert(sourceNodeID);
		stationList.insert(destinyNodeID);
		
		this->listStationsByLine.insert(pair<string, set<unsigned int>>(lineID, stationList));
	
	} else {

		it->second.insert(sourceNodeID);
		it->second.insert(destinyNodeID);

	}
}

/**
 * @brief Creates an BusEdge and adds it to a certain existing Node
 *
 * @param sourceNodeID - the ID of the source Node of the Edge
 * @param destinyNodeID - the ID of the destiny Node of the Edge
 * @param weight - the weight of the Edge
 * @param lineID - the ID of the bus line
 */
template<typename T>
void Graph<T>::addBusEdge(unsigned int sourceNodeID, unsigned int destinyNodeID, double weight, string lineID) {
	
	Edge<T> edge = Edge<T>(nodes.at(destinyNodeID), weight, BUS, lineID);
	this->nodes.at(sourceNodeID)->addEdge(edge);

	insertStation(lineID, sourceNodeID, destinyNodeID);
}

/**
 * @brief Creates an SubwayEdge and adds it to a certain existing Node
 *
 * @param sourceNodeID - the ID of the source Node of the Edge
 * @param destinyNodeID - the ID of the destiny Node of the Edge
 * @param weight - the weight of the Edge
 * @param lineID - the ID of the subway line
 */
template<typename T>
void Graph<T>::addSubwayEdge(unsigned int sourceNodeID, unsigned int destinyNodeID, double weight, string lineID) {
	
	Edge<T> edge = Edge<T>(nodes.at(destinyNodeID), weight, SUBWAY, lineID);
	this->nodes.at(sourceNodeID)->addEdge(edge);
	
	insertStation(lineID, sourceNodeID, destinyNodeID);
}

/**
 * @brief Creates an WalkEdge and adds it to a certain existing Node
 *
 * @param sourceNodeID - the ID of the source Node of the Edge
 * @param destinyNodeID - the ID of the destiny Node of the Edge
 * @param weight - the weight of the Edge
 * @param lineID - to maintain equality among the other types of edges. In this case, it's always "walk"
 */
template<typename T>
void Graph<T>::addWalkEdge(unsigned int sourceNodeID,
		unsigned int destinyNodeID, double weight, string lineID) {

	Edge<T> edge = Edge<T>(nodes.at(destinyNodeID), weight, WALK, lineID);
	this->nodes.at(sourceNodeID)->addEdge(edge);
}

/*
 * Max heap by default has the highest value on the top of the heap
 * Since we want the minimum, in terms of distance, we must define the operator in the reverse orded
 *
 */
template<typename T>
struct compareDistance {
	bool operator()(Node<T> * rhs, Node<T> * lhs) const {
		return rhs->getDistance() > lhs->getDistance();
	}
};

/**
 * @brief Returns a vector with the path with the "smallest" distance from the startNode to the endNode, implementing Dijkstra, using a heap
 *
 * !!! NOT USED !!!
 *
 * @param startNode - the beginning Node of the path
 * @param endNode - the end Node of the path
 *
 * @return Node * - the final Node of the path, so we can walk it back to get the best path
 */
template<typename T>
Node<T> * Graph<T>::dijkstra_heap(Node<T> * startNode, Node<T> * endNode) {

	vector<Node<T> *> path = { };

	for (auto it = this->nodes.begin(); it != this->nodes.end(); it++) {
		(*it)->setDistance(DBL_MAX);
		(*it)->clearLastNode();
	}

	startNode->setDistance(0);
	path.push_back(startNode);

//making the heap, since it only has one element does not need the function
	make_heap(path.begin(), path.end());

	Node<T> * v;
	Node<T> * w;
	double new_distance;
	double old_distance;

	while (!path.empty()) {

		//the miminum value is always in the top
		v = path.front();

		//putting the min value (considered the max since we swap the operator) in the back
		pop_heap(path.begin(), path.end());

		//removing it
		path.pop_back();

		for (auto it = v->getEdges().begin(); it != v->getEdges().end(); it++) {

			w = it->getDestiny();
			new_distance = v->getDistance() + it->getWeight();
			old_distance = w->getDistance();

			if (old_distance > new_distance) {

				w->setDistance(new_distance);
				w->setLastNode(v);

				if (old_distance == DBL_MAX) {  //aka is not in the path
					path.push_back(w);
				}

				make_heap(path.begin(), path.end(), compareDistance<T>());

			}
		}
	}

	return endNode;
}

/**
 * @brief Calculates the path with the "smallest" distance from the startNode to the endNode, using a mutable priority queue and the A Star algorithm
 *
 *
 * @param startNode - the beginning Node of the path
 * @param endNode - the end Node of the path
 *
 * @return Node * - the final Node of the path, so we can walk it back to get the best path
 */
template<class T>
Node<T> * Graph<T>::A_Star(Node<T> * startNode, Node<T> * endNode) {

	//initial setup to compare by distance
	Node<T>::sortByDistance = true;

	for (auto it = this->nodes.begin(); it != this->nodes.end(); it++) {
		(*it)->setDistance(DBL_MAX);
		(*it)->clearLastNode();
		(*it)->setVisited(false);
		(*it)->setLastConnection("NOT");
		(*it)->setPrice(0);
	}

	startNode->setDistance(0);
	startNode->setLastConnection("FIRST");
	MutablePriorityQueue<Node<T> > q;
	q.insert(startNode);

	Node<T> * v;
	Node<T> * w;
	double old_distance;
	double new_distance;

	while (!q.empty()) {

		v = q.extractMin();

		if (v->getId() == endNode->getId()) {
			endNode->setDistance(
					endNode->getDistance()
							+ startNode->euclidianDistance(endNode));
			break;

		}

		for (auto it = v->getEdges().begin(); it != v->getEdges().end(); it++) {

			w = it->getDestiny();
			old_distance = w->getDistance();
			new_distance = (v->getDistance() + it->getWeight())
					- v->euclidianDistance(endNode)
					+ w->euclidianDistance(endNode);

			if (old_distance > new_distance) {

				/*updating the prices
				 * not important for the queue since it's taking distance as the operator
				 */
				if (v->getLastConnection() != it->getEdgeConnection())
					w->setPrice(v->getPrice() + it->getPriceWeight());

				else
					w->setPrice(v->getPrice());

				/*
				 * adding the transbord time if he changed the type of vehicle
				 * ignoring walking
				 */
				if (v->getLastTypeConnection() != it->getType()
						&& v->getLastTypeConnection() != WALK
						&& it->getType() != WALK) {
					new_distance += v->getTransbordTime();
				}

				w->setDistance(new_distance);
				w->setLastNode(v);
				w->setLastConnection(it->getEdgeConnection());
				if (!w->getVisited())
					q.insert(w);
				else
					q.decreaseKey(w);

				w->setVisited(true);

			}
		}
	}

	return endNode;

}

/**
 * @brief Calculates the path with the "smallest" distance from the startNode to the endNode, implementing Dijkstra, using a mutable priority queue
 *
 * @param startNode - the beginning Node of the path
 * @param endNode - the end Node of the path
 *
 * @return Node * - the final Node of the path, so we can walk it back to get the best path
 */
template<class T>
Node<T> * Graph<T>::dijkstra_queue(Node<T> * startNode, Node<T> * endNode) {

//initial setup to compare by distance
	Node<T>::sortByDistance = true;

	for (auto it = this->nodes.begin(); it != this->nodes.end(); it++) {
		(*it)->setDistance(DBL_MAX);
		(*it)->clearLastNode();
		(*it)->setVisited(false);
		(*it)->setLastConnection("NOT");
		(*it)->setPrice(0);
	}

	startNode->setDistance(0);
	startNode->setLastConnection("FIRST");
	MutablePriorityQueue<Node<T> > q;
	q.insert(startNode);

	Node<T> * v;
	Node<T> * w;
	double old_distance;
	double new_distance;

	while (!q.empty()) {

		v = q.extractMin();

		if (v->getId() == endNode->getId())
			break;

		for (auto it = v->getEdges().begin(); it != v->getEdges().end(); it++) {

			w = it->getDestiny();
			old_distance = w->getDistance();
			new_distance = v->getDistance() + it->getWeight();

			if (old_distance > new_distance) {

				/*updating the prices
				 * not important for the queue since it's taking distance as the operator
				 */
				if (v->getLastConnection() != it->getEdgeConnection())
					w->setPrice(v->getPrice() + it->getPriceWeight());

				else
					w->setPrice(v->getPrice());

				/*
				 * adding the transbord time if he changed the type of vehicle
				 * ignoring walking
				 */
				if (v->getLastTypeConnection() != it->getType()
						&& v->getLastTypeConnection() != WALK
						&& it->getType() != WALK) {
					new_distance += v->getTransbordTime();
				}

				w->setDistance(new_distance);
				w->setLastNode(v);
				w->setLastConnection(it->getEdgeConnection());
				if (!w->getVisited())
					q.insert(w);
				else
					q.decreaseKey(w);

				w->setVisited(true);

			}
		}
	}

	return endNode;
}

/**
 * @brief Calculates the path with the "smallest" distance from the startNode to the endNode, without walking, implementing Dijkstra, using a mutable priority queue
 *
 *
 * @param startNode - the beginning Node of the path
 * @param endNode - the end Node of the path
 *
 * @return Node * - the final Node of the path, so we can walk it back to get the best path
 */
template<class T>
Node<T> * Graph<T>::dijkstra_queue_NO_WALK(Node<T> * startNode,
		Node<T> * endNode) {

//initial setup to compare by distance
	Node<T>::sortByDistance = true;

	for (auto it = this->nodes.begin(); it != this->nodes.end(); it++) {
		(*it)->setDistance(DBL_MAX);
		(*it)->clearLastNode();
		(*it)->setVisited(false);
		(*it)->setLastConnection("NOT");
		(*it)->setPrice(0);
	}

	startNode->setDistance(0);
	startNode->setLastConnection("FIRST");
	MutablePriorityQueue<Node<T> > q;
	q.insert(startNode);

	Node<T> * v;
	Node<T> * w;
	double old_distance;
	double new_distance;

	while (!q.empty()) {

		v = q.extractMin();

		if (v->getId() == endNode->getId())
			break;

		for (auto it = v->getEdges().begin(); it != v->getEdges().end(); it++) {

			w = it->getDestiny();
			old_distance = w->getDistance();
			new_distance = v->getDistance() + it->getWeight();

			if (it->getType() == WALK)
				continue;

			if (old_distance > new_distance) {

				/*updating the prices
				 * not important for the queue since it's taking distance as the operator
				 */
				if (v->getLastConnection() != it->getEdgeConnection())
					w->setPrice(v->getPrice() + it->getPriceWeight());

				else
					w->setPrice(v->getPrice());

				/*
				 * adding the transbord time if he changed the type of vehicle
				 * ignoring walking
				 */
				if (v->getLastTypeConnection() != it->getType()
						&& v->getLastTypeConnection() != WALK
						&& it->getType() != WALK) {
					new_distance += v->getTransbordTime();
				}

				w->setDistance(new_distance);
				w->setLastNode(v);
				w->setLastConnection(it->getEdgeConnection());

				if (!w->getVisited())
					q.insert(w);
				else
					q.decreaseKey(w);

				w->setVisited(true);
			}
		}

	}

	return endNode;
}

/**
 * @brief Calculates the path with the "smallest" distance from the startNode to the endNode, implementing Dijkstra, using a mutable priority queue, and allowing
 * only some exchanges between transports
 *
 *
 *
 * @param startNode - the beginning Node of the path
 * @param endNode - the end Node of the path
 * @param maxNum - the maximum allowed number of transports exchanges
 *
 * @return Node * - the final Node of the path, so we can walk it back to get the best path
 */
template<class T>
Node<T> * Graph<T>::dijkstra_queue_TRANSBORDS(Node<T> * startNode,
		Node<T> * endNode, int maxNum) {

//initial setup to compare by distance
	Node<T>::sortByDistance = true;

	for (auto it = this->nodes.begin(); it != this->nodes.end(); it++) {
		(*it)->setDistance(DBL_MAX);
		(*it)->clearLastNode();
		(*it)->setVisited(false);
		(*it)->setNumTransbords(INT_MAX);
		(*it)->setLastConnection("NOT");
		(*it)->setPrice(0);
	}

	startNode->setDistance(0);
	startNode->setNumTransbords(-1);
	startNode->setLastConnection("FIRST");

	MutablePriorityQueue<Node<T> > q;
	q.insert(startNode);

	Node<T> * v;
	Node<T> * w;
	double old_distance;
	double new_distance;

	while (!q.empty()) {

		v = q.extractMin();

		if (v->getId() == endNode->getId())
			break;

		for (auto it = v->getEdges().begin(); it != v->getEdges().end(); it++) {

			w = it->getDestiny();
			old_distance = w->getDistance();
			new_distance = v->getDistance() + it->getWeight();

			int currentTransbords = v->getNumTransbords();

			/*if the method of transport used or the line has changed
			 * must add another "transbordo"
			 */
			if (v->getLastConnection() != it->getEdgeConnection() && it->getType() != WALK) {

				currentTransbords++;
			}

			if (currentTransbords > maxNum) {
				continue;
			}

			if (old_distance > new_distance) {

				/*updating the prices
				 * not important for the queue since it's taking distance as the operator
				 */
				if (v->getLastConnection() != it->getEdgeConnection())
					w->setPrice(v->getPrice() + it->getPriceWeight());

				else
					w->setPrice(v->getPrice());

				/*
				 * adding the transbord time if he changed the type of vehicle
				 * ignoring walking
				 */
				if (v->getLastTypeConnection() != it->getType()
						&& v->getLastTypeConnection() != WALK
						&& it->getType() != WALK) {
					new_distance += v->getTransbordTime();
				}

				w->setDistance(new_distance);
				w->setLastNode(v);
				w->setLastConnection(it->getEdgeConnection());
				w->setNumTransbords(currentTransbords);

				if (!w->getVisited())
					q.insert(w);
				else
					q.decreaseKey(w);

				w->setVisited(true);
			}
		}
	}

	return endNode;
}

/**
 * @brief Calculates the cheapest path the startNode to the endNode, implementing Dijkstra, using a mutable priority queue
 *
 * Since the cheapest path would obviously be always walking, we ask the user the maximum time he wants to spend walking
 *
 * @param startNode - the beginning Node of the path
 * @param endNode - the end Node of the path
 * @param walk_time - maximum allowed distance
 *
 * @return Node * - the final Node of the path, so we can walk it back to get the best path
 */
template<class T>
Node<T> * Graph<T>::dijkstra_queue_PRICE(Node<T> * startNode, Node<T> * endNode,
		double walk_time) {

//initial setup to compare by distance
	Node<T>::sortByDistance = false;

	for (auto it = this->nodes.begin(); it != this->nodes.end(); it++) {
		(*it)->setDistance(0);
		(*it)->clearLastNode();
		(*it)->setVisited(false);
		(*it)->setNumTransbords(INT_MAX);
		(*it)->setLastConnection("NOT");
		(*it)->setPrice(DBL_MAX);
		(*it)->setWalkedTime(0);
	}

	startNode->setPrice(0);
	startNode->setNumTransbords(-1);
	startNode->setLastConnection("FIRST");

	MutablePriorityQueue<Node<T> > q;
	q.insert(startNode);

	Node<T> * v;
	Node<T> * w;
	double old_price;
	double new_price;

	while (!q.empty()) {

		v = q.extractMin();

		for (auto it = v->getEdges().begin(); it != v->getEdges().end(); it++) {

			w = it->getDestiny();
			old_price = w->getPrice();
			new_price = v->getPrice();

			double time_walked = v->getWalkedTime();


			if (v->getLastConnection() != it->getEdgeConnection())
				new_price += it->getPriceWeight();

			if (it->getType() == WALK)
				time_walked += it->getWeight();

			if (time_walked > walk_time)
				continue;

			if (old_price > new_price) {

				/*updating the distance
				 * not important for the queue since its taking the price as the operator
				 * also checking if he changed vehicles
				 */
				double new_distance = v->getDistance() + it->getWeight();
				if (v->getLastTypeConnection() != it->getType()
						&& v->getLastTypeConnection() != WALK
						&& it->getType() != WALK) {
					new_distance += v->getTransbordTime();
				}
				w->setDistance(new_distance);

				w->setWalkedTime(time_walked);

				w->setPrice(new_price);
				w->setLastNode(v);
				w->setLastConnection(it->getEdgeConnection());

				if (!w->getVisited())
					q.insert(w);
				else
					q.decreaseKey(w);

				w->setVisited(true);
			}
		}
	}

	return endNode;
}

/**
 * @brief get the path to a certain Node
 *
 * @param dest - the destiny Node
 *
 * @return the vector with the full path, ordered
 */
template<class T>
vector<T> Graph<T>::getPath(Node<T> * dest) const {

	vector<T> res;

	while (dest->getLastNode() != NULL) {
		res.push_back(dest->getInfo());
		dest = dest->getLastNode();
	}

	res.push_back(dest->getInfo());
	reverse(res.begin(), res.end());
	return res;
}
/**
 * @brief Gives detailed Information about the path to take
 *
 * @param dest - the destiny Node
 *
 * @return a vector with the path reversed
 */
template<class T>
vector<Node<T>*> Graph<T>::getDetailedPath(Node<T> * dest) const {

	vector<Node<T>*> invertedPath;

	// If there is no way to travel with the constraints, return a vector only with the destiny
	if (dest->getLastNode() == NULL) {
		invertedPath.push_back(dest);
		return invertedPath;
	}

	while (dest->getLastNode() != NULL) {
		invertedPath.push_back(dest);
		dest = dest->getLastNode();
	}

	return invertedPath;
}

/**
 * @brief Presents on the screen the detailed information about the path
 *
 * @param invertedPath - a vector with the Nodes of the path, but reversed
 */
template<class T>
void Graph<T>::presentPath(vector<Node<T>*> invertedPath) {

	double total_distance = invertedPath.at(0)->getDistance();
	double total_price = invertedPath.at(0)->getPrice();

	if (invertedPath.at(0)->getLastNode() == NULL) {
		cout << "It is impossible to travel to "
				<< invertedPath.at(0)->getInfo() << " with those constrains!\n";
		return;
	}

	string previousConnection = "";
	string currentConnection;

	for (int i = invertedPath.size() - 1; i >= 0; i--) {

		currentConnection = invertedPath.at(i)->getLastConnection();

		if (invertedPath.at(i)->getLastConnection() == "walk walk") {
			cout << "At " << invertedPath.at(i)->getLastNode()->getInfo() << " "
				 << invertedPath.at(i)->getLastConnection().substr(0, 4) << " to "
				 << invertedPath.at(i)->getInfo() << endl;
		}

		else {

			if(invertedPath.at(i)->getLastConnection() == previousConnection){
				cout << "At " << invertedPath.at(i)->getLastNode()->getInfo() << " "
				     << " continue on the "
					 << invertedPath.at(i)->getLastConnection() << " until "
					 << invertedPath.at(i)->getInfo() << endl;
			}

			else{
				cout << "At " << invertedPath.at(i)->getLastNode()->getInfo() << " get to the ";

				if(currentConnection.substr(0,1) == "s"){
					cout << invertedPath.at(i)->getLastNode()->getInfo()
						 << "'s subway station" << endl;
				}
				else{
					cout << invertedPath.at(i)->getLastNode()->getInfo()
						 << "'s bus station" << endl;
				}

				cout << "At " << invertedPath.at(i)->getLastNode()->getInfo() << " "
					 << " catch the "
					 << invertedPath.at(i)->getLastConnection() << " to "
					 << invertedPath.at(i)->getInfo() << endl;
			}
		}

		previousConnection = invertedPath.at(i)->getLastConnection();
	}

	string time = to_string(round(total_distance * 100) / 100).substr(0, 5);
	string price = to_string(round(total_price * 100) / 100).substr(0, 5);

	cout << "\nTotal Time: " + time + " minutes.\n";
	cout << "Total Price: " + price + " euros.\n";

}

#endif /* GRAPH_H_ */

