#ifndef _MOVIE_NODE_HPP_
#define _MOVIE_NODE_HPP_

#include <iostream>
#include "Movie.hpp"
using namespace std;


class MovieNode
{

private:
	//Create a new movie that will contain the information of the node
	Movie* _film;
	//create a new node that point to the next node in the list
	MovieNode* _next;
	
public:
	//constructor of the movienode
	MovieNode(Movie peli):_next(NULL){_film = new Movie(); *_film = peli;}
	
	//accesor and modifier of the movie
	Movie* value() const {return _film;}
	Movie* & value() {return _film;}
	
	//accesor and modifier of the next node
	MovieNode* next() const {return _next;}
	MovieNode* & next(){return _next;}
	
	//<< operator overloading
	friend std::ostream& operator<<(std::ostream &os, const MovieNode &node);
};

#endif 
