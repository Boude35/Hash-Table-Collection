#include "MovieCollection.hpp"
#include <iostream>


using namespace std;

int
MovieCollection::maxSearchLenght()
{
	int length = 0;
	int max = 0;
	for(int i = 0; i < MAX_SIZE; i++)
	{
		MovieNode* curr = hashTab[i];
		if(curr != NULL)
		{
			while(curr->next())
			{	
				length++;
				curr = curr->next();
			}
			if(length>max)
			{
				max = length;
				length = 0;
			}
		}
	}	
	if(max == 0)
		max = 1;
	
	
	return max;//return the length of the largest chain
}

//returns the size of the tree by calling the previous function
int
MovieCollection::size() const
{
	int tamaño = 0;//variable that count the size
	
	for(int i = 0; i < MAX_SIZE; i++) //go through all the values in the hash table
	{
		MovieNode* curr = hashTab[i];//start at the beginnig
		if(curr != NULL) //if the node is not null
			while(curr)
				{	
					tamaño++; //add one to the size
					curr = curr->next();
				}
	}	
	
	
	
	return tamaño;//return the size
}
//retunr the rating of a given film
double
MovieCollection::getRating(string film, int year)
{
	
	Movie *comp = new Movie(film, year, 0);//create a new pointer with the parameters
	
	for(int i = 0; i < MAX_SIZE; i++)//go through all the values in the hash table
	{
		MovieNode* curr = hashTab[i];//start at the beginnig
		if(curr != NULL)//if the node is not null
			while(curr)
				{	
					if(*curr->value() == *comp) //if it is the movie
						return curr->value()->rating(); //return his rating
					curr = curr->next();
				}
	}	
	return -1;
	
}

//hash and compression funtion
int hash(Movie m)
{
	MovieNode *newNode = new MovieNode(m); //new node with the movie
	string title = newNode->value()->title(); //create a string with the title
	int sum = 0; //new variable to store the sum
	for(int i =0; title[i]!='\0'; i++)
	{
		sum += title[i]; //add the ascii code of that letter
	}
	sum +=  newNode->value()->year();//add the year of the movie
	return sum % MAX_SIZE;//allocate the place for that movie
	
}
//insert a new movie into the hash table
void
MovieCollection::insert(Movie m)
{
	int place = hash(m);//find the place of the movie
	MovieNode* prev = NULL; //new node that starts at null
	MovieNode* curr = hashTab[place]; //current node
	
	while(curr!=NULL)//while current node is not null
	{
		prev = curr;//got to the next node
		curr = curr->next();
	}
	
	if(curr == NULL)//if current is null
	{
		curr = new MovieNode(m);//curr equals a new node
		if(prev == NULL) //if prev is null
		{
			hashTab[place] = curr;
		}
		else
		{
			prev->next() = curr;//prev points to the current value
		}
	}
	else
	{
		*curr->value() = m;//current value equlls m
	}
}

//erase a node from the hash table
void 
MovieCollection::erase(Movie m)
{
	int place = hash(m);//find the place to the movie
	
	if(hashTab[place] != NULL) //if it is null
	{
		MovieNode *prev = NULL; //new node prev equals null
		MovieNode *curr = hashTab[place];//new node current equals the place of the movie
		while ( curr->next() != NULL && *curr->value() != m) //while the next node is not null and the movie does not equal current value
		{
			prev = curr; //go to the next node
			curr = curr->next();
		}
		if(*curr->value() == m)//If the current value equals the movie
		{
			if(prev == NULL) //and prev is nul
			{
				MovieNode *next = curr->next(); //new node
				delete curr;//delete current node
				hashTab[place] = next;
			}
			else
			{
				MovieNode *next2 = curr->next();
				delete curr;
				prev->next() = next2;
			}
		}
	}
		
	
	
}

//find a node
MovieNode*
MovieCollection::find(Movie m) const
{
	int place = hash(m);//find the place of the movie
	MovieNode* curr = hashTab[place];//new node 
	
	while(curr != NULL) //while it is null
	{
		if(*curr->value() == m)//if current valu equals the movie
		{
			return curr; //return it
		}
		curr = curr->next();//go to the next node
	}
	return NULL;
}

//constructor from a file
MovieCollection::MovieCollection(ifstream &ifile)
{
	hashTab = new MovieNode*[MAX_SIZE];//inizialize the table
	for(int i = 0; i < MAX_SIZE; i++)
			hashTab[i] = NULL;	
	
	//new movie
	Movie m;
	//pass the movie from the file to the new just created
	ifile >> m;
	//while the document has text
	while(ifile)
	{
		insert(m);//add it to the list
		ifile >> m;//add the next values
	}
}

//copy constructor
MovieCollection::MovieCollection(const MovieCollection & other)
{
	hashTab = new MovieNode*[MAX_SIZE];//inizialize the table
	for(int i = 0; i < MAX_SIZE; i++)
			hashTab[i] = NULL;
			
	for(int i = 0; i < MAX_SIZE; i++)//copy all the values in it
	{
		MovieNode* curr = other.hashTab[i];
		if(curr != NULL)
			while(curr)
				{	
					Movie *comp = curr->value();
					insert(*comp);
					curr = curr->next();
				}
	}	
}

//overloading od + operator
const MovieCollection& 
MovieCollection::operator+(const MovieCollection &other) const
{
	MovieCollection *answer = new MovieCollection(); //new moviecollection
	

	for(int i = 0; i < MAX_SIZE; i++)//add all the values of the first collection
	{
		MovieNode* curr = hashTab[i];
		if(curr != NULL)
			while(curr)
				{	
					Movie *comp = curr->value();
					answer->insert(*comp); //insert the value
					curr = curr->next();
				}
	}	
	
	
	//add all the values of the RHS that are not already in the new collection
	for(int i = 0; i < MAX_SIZE; i++)
	{
		MovieNode* curr2 = other.hashTab[i];
		if(curr2 != NULL)
			while(curr2)
				{	
					if(!answer->find(*curr2->value()))
						{
							Movie *comp = curr2->value();
							answer->insert(*comp);//insert the values if theya re not already in answer
						}
					curr2 = curr2->next();
				}
	}	
	return *answer; //return the new collection
}

// overloading of / operator
const MovieCollection& 
MovieCollection::operator/(const MovieCollection &other) const
{
	MovieCollection *answer = new MovieCollection(); //new collection

	for(int i = 0; i < MAX_SIZE; i++)//pass the common values
	{
		MovieNode* curr = other.hashTab[i];// go over the values of other
		if(curr != NULL)
			while(curr)
				{	
					if(find(*curr->value()))//can be find in this
					{
						answer->insert(*curr->value());//insert it to answer
					}
					curr = curr->next();
				}
	}	
	return *answer;
}

//overloading of += operator with a movie
const MovieCollection& 
MovieCollection::operator+=(const Movie &movieToAdd)
{	
        //add the movie to this only if it is not already in it
	if(!find(movieToAdd))
	{
		insert(movieToAdd);//insert it o this
	}

	MovieCollection *answer = new MovieCollection(*this);//copy this in a new moviecollection
	return *answer;//return the new moviecollection
}
//overloading of += operator with a moviecollection
const MovieCollection& 
MovieCollection::operator+=(const MovieCollection &other)
{

	for(int i = 0; i < MAX_SIZE; i++)//add the values that are not already in this
	{
		MovieNode* curr = other.hashTab[i];
		if(curr != NULL)
			while(curr)
				{	
					operator+=(*curr->value());//add it and it is not repeated
					curr = curr->next();
				}
	}	

	MovieCollection *answer = new MovieCollection(*this);//copy this 
	return *answer;
}
//overloading of -= operator with a movie
const MovieCollection& 
MovieCollection::operator-=(const Movie &movieToDel)
{
	if(find(movieToDel))//if it is in the collection
	{
		erase(movieToDel);//delete it
	}

	MovieCollection *answer = new MovieCollection(*this);//copy this
	return *answer;
}
//overloading of -= operator with a moviecollection
const MovieCollection& 
MovieCollection::operator-=(const MovieCollection &other)
{
	
	MovieCollection *answer = new MovieCollection();//new Moviecollection
	
	for(int i = 0; i < MAX_SIZE; i++)//remove the values that are in both using -= operator
	{
		MovieNode* curr = other.hashTab[i];
		if(curr != NULL)
			while(curr)
				{	
					if(find(*curr->value()))
						this -> operator-=(*curr->value());
					curr = curr->next();
				}
	}	
	
	
	//use assigment operator overloaded
	*answer = *this;
	return *answer;
}

//overloading of assigment operator
const MovieCollection& 
MovieCollection::operator=(const MovieCollection &other)
{
	for(int i = 0; i < MAX_SIZE; i++) //copy all the values in the hash table
	{
		MovieNode* curr = other.hashTab[i];
		if(curr != NULL)
			while(curr)
				{	
					operator+=(*curr->value());
					curr = curr->next();
				}
	}	
	return *this;
}
//clear the hash table
void
MovieCollection::clear()
{
	//if head is null the rest is not printed
	hashTab = new MovieNode*[MAX_SIZE];
		for(int i = 0; i < MAX_SIZE; i++)
			hashTab[i] = NULL;
}
//print the hash table
std::ostream&
MovieCollection::print(std::ostream &os) const
{
	for(int i = 0; i<MAX_SIZE; i++)
	{
		MovieNode* curr = hashTab[i];
		if(curr != NULL)
			while(curr)
			{
				
				os<<*curr<<endl;
				curr = curr->next();
			}
	}
	return os;
}
// << operator overloading
ostream& operator<<(ostream &os, const MovieCollection &table)
{
	return table.print(os); //call the method print
}








