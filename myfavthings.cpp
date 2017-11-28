#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;



const int READERROR = 100;

const int WRITEERROR = 200;

const int ARRAYSIZE = 100;


//declare my favorite thing class, games for me
class Game

{

	string gameDeveloper;

	string gameName;

	string gameRating;

	double gameLength;

public:

	Game();

	Game(string dev, string name, string r, double l);

	void obtainData();

	void displayData();

	int saveData(ofstream &outfile);

	//establish friend function

	friend bool operator == (const Game& g, const Game& ga);





};

//establish a class for multiple games

class GamesRoster

{

	Game **list;

	int numrecords;

	int listsize;

	int ReallocateArray();

public:

	GamesRoster();

	~GamesRoster();

	void GetUserInput();

	void DisplayGamesRoster();

	int SaveGamesRoster(string filename);

	int ReadGamesRoster(string filename);





};



bool operator== (const Game& g, const Game& ga)

{

	if (g.gameDeveloper == ga.gameDeveloper && g.gameName == ga.gameName && g.gameRating == ga.gameRating && g.gameLength == ga.gameLength)

	{

		return 1;

	}

	else

		return 0;

}



// constructor for game class

Game::Game()

{

	gameDeveloper = ""; gameName = ""; gameRating = ""; gameLength = 0.0;

}

// overload constructor 

Game::Game(string dev, string name, string r, double l)

{

	gameDeveloper = dev;

	gameName = name;

	gameRating = r;

	gameLength = l;

}

// Instructions for user to input game data

void Game::obtainData()

{

	cout << "Who is the Game Developer? ";

		getline(cin, gameDeveloper);

	cout << "What is the name of the game? ";

	getline(cin, gameName);

	cout << "What is the ESRB rating of the game? ";

	getline(cin, gameRating);

	cout << "What is the length of the game in hours? ";

	cin >> gameLength;

	cin.ignore();

	cin.clear();



}

// Show the game data back to user

void Game::displayData()

{

	cout << "The Game’s Developer is: " << gameDeveloper << " and the name of the game is: " << gameName << ". The ESRB rating of this game is " << gameRating << " and its length in hours is " << gameLength << endl;

}

// write game data to file

int Game::saveData(ofstream& outfile)

{

	if (outfile.is_open())

	{

		outfile << gameDeveloper << "|" << gameName << "|" << gameRating << "|" << gameLength << endl;

		return 0;

	}

	else

		return WRITEERROR;

}







// Games roster constructor 

GamesRoster::GamesRoster()

{

	list = new Game*[ARRAYSIZE];

	numrecords = 0;

	listsize = ARRAYSIZE;

}

// deconstructor for deletion caveat

GamesRoster::~GamesRoster()

{

	for (int i = 0; i < numrecords; i++)

	{

		delete list[i];

	}

	delete[]list;

}

// Reallocate memory

int GamesRoster::ReallocateArray()

{

	Game **temp;

	temp = new Game*[listsize + ARRAYSIZE];

	listsize = listsize + ARRAYSIZE;

	for (int i = 0; i < numrecords; i++)

	{

		temp[i] = list[i];

	}

	delete[]list;

	list = temp;

	return 0;

}



// have user input roster data

void GamesRoster::GetUserInput()

{

	bool duplicate = false;

	string answer = "Y";

	cout << "Would you like to enter a game? Y/N?" << endl;

	getline(cin, answer);

	while (answer == "Y" || answer == "y")

	{

		list[numrecords] = new Game();

		list[numrecords]->obtainData();





		for (int i = 0; i < numrecords; i++)

		{

			if (*list[numrecords] == *list[i])

			{

				duplicate = true;

			}

			i++;

		}


		//prevent duplicate entries
		if (duplicate == true)

		{

			cout << "This value is a duplicate and will not be saved.";

			delete list[numrecords];

			cout << "Would you like to enter another game? Y/N?" << endl;

			getline(cin, answer);

			duplicate = false;

			continue;

		}

		numrecords++;

		cout << "Would you like to enter another game? Y/N?" << endl;

		getline(cin, answer);









	}

}



// display roster to user

void GamesRoster::DisplayGamesRoster()

{

	for (int i = 0; i < numrecords; i++)

		list[i]->displayData();

}



// write roster to given txt file

int GamesRoster::SaveGamesRoster(string filename)

{

	ofstream output(filename, ios::trunc);

	if (output)

	{

		for (int i = 0; i < numrecords; i++)

		{

			list[i]->saveData(output);

		}

		output.close();

	}

	else

	{

		return WRITEERROR;

	}

	return 0;

}

// Read Games roster from a file, unless error given


int GamesRoster::ReadGamesRoster(string filename)

{

	string idev, iname, ir, il;

	ifstream infile(filename, ios::in);

	if (!infile)

	{

		cout << "File is unopenable." << endl;

		return READERROR;

	}





	while (!infile.eof())

	{

		if (numrecords == listsize)

		{

			ReallocateArray();

		}

		getline(infile, idev, '|');

		if (!idev.empty())

		{

			getline(infile, iname, '|');

			getline(infile, ir, '|');



			getline(infile, il);

			double l = stoi(il);

			list[numrecords] = new Game(idev, iname, ir, l);

			numrecords++;

		}

	}



	infile.close();

	return 0;

}





int main()

{

	string filename;

	GamesRoster myGame;

	string answer;

	cout << "What is the FULL File Path?" << endl;

	getline(cin, filename);

	int error;

	error = myGame.ReadGamesRoster(filename);

	if (error)

	{

		cout << "Couldn't read in that game(s), want to continue trying? Y/N? ";

		getline(cin, answer);

		if (answer != "Y" && answer != "y")

		{

			return 1;

		}

	}

	myGame.GetUserInput();

	myGame.SaveGamesRoster(filename);

	myGame.DisplayGamesRoster();



	return 0;

}
