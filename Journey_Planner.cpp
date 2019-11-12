#include "pch.h"
#include "car.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
using namespace std;

int path_matrix[50][50], nodes, edges; //the nodes represent the cities; the edges represent the paths between the cities
const int inf = 60000; //inf=infinity
int intermediar_nodes = 0, shortest_path_distance = 0, direct_path_distance = 0;
/*intermediar_nodes-stores the number of intermediary nodes between two destinations
  shortest_path_distance-stores the distance of the shortest path between two destinations
  direct_path_distance-stores the distance of the direct direct path between two destinations*/

int input_check(string input);
int valid_input(string input);
int car_input_check(string input);

void initialize();
void readPathFromFile();
void RoyFloyd();

void path(int start_node, int finish_node);
int numberOfIntermediarNodes(int start_node, int finish_node);
void printRoad(int start_node, int finish_node);
int check(int start_node, int finish_node);
void directPath(int start_node, int finish_node);

int input_trunk_tires();
int AC_on();
void time(double distance, int intermediar_nodes);

int main()
{
	const int carNumber = 26; //stores the number of car in the carFile.txt

	int start, finish, choose_car_number; //start/finish-will store the number that is corresponds to the initial/final destination entered by the user, choose_car_number allows user to enter the car number from the list displayed 
	string choose_again = "y"; //condition for the while loop
	string start_check, finish_check; //will store the name for the initial/final destination

	while (choose_again == "y" || choose_again == "Y")
	{
		cout << "\t!Beaware that the distance is calculated in km! The speed limit from most European countries is considered." << endl;
		cout << "\t\tPlease choose two cities from the list below: " << endl;
		cout << "\t\t\tLiverpool" << endl;
		cout << "\t\t\tLondon" << endl;
		cout << "\t\t\tManchester" << endl;
		cout << "\t\t\tNorwich" << endl;
		cout << "\t\t\tNottingham" << endl << endl;

		readPathFromFile();
		cout << endl;

		cout << "Start: "; cin >> start_check;
		//check if user input is valid
		while (!valid_input(start_check))
		{
			cout << "Name for the starting point NOT valid! Please choose a name from the list." << endl;
			cout << "Start: "; cin >> start_check;
		}

		cout << "Finish: "; cin >> finish_check;
		//check if user input is valid
		while (!valid_input(finish_check))
		{
			cout << "Destination name NOT valid! Please choose a name from the list." << endl;
			cout << "Finish: "; cin >> finish_check;
		}

		//check if the chosen cities are different
		while (start_check == finish_check)
		{
			cout << "Choose different cities!" << endl;
			cout << "Start: "; cin >> start_check;
			cout << "Finish: "; cin >> finish_check;
		}

		ifstream CarFile("carFile.txt");

		car myCar[carNumber]; //create objects for car class

		//load information from file into myCar
		for (int i = 1; i < carNumber; i++)
		{
			myCar[i].load(CarFile);
		}

		cout << endl;

		cout << setfill(' ') << setw(10) << "Car number" << setfill(' ') << setw(15) << "Car brand" << setfill(' ') << setw(15) << "Car model" << setfill(' ') << setw(15) << "Engine size" << setfill(' ') << setw(23) << "Average consumption" << endl;
		cout << endl;

		//display the list with cars
		for (int i = 1; i < carNumber; i++)
		{
			myCar[i].display();
		}

		cout << endl << "Choose your car from the list. Type the number here: "; cin >> choose_car_number;
		//check if user input is valid
		while ((choose_car_number < 1) || (choose_car_number > 25))
		{
			cout << "Car number out of range. Please type again: "; cin >> choose_car_number;
		}

		cout << "Consumption for: ";
		myCar[choose_car_number].display();
		cout << endl;

		myCar[choose_car_number].consumption();

		start = input_check(start_check); //converts the name of the city into an integer
		finish = input_check(finish_check); //converts the name of the city into an integer
		cout << endl << "From: " << start_check << endl << "  To: " << finish_check << endl;
		cout << "Journey options:" << endl;

		cout << "\tFirst option: direct path" << endl;
		directPath(start, finish);
		if (check(start, finish) != 0)
		{
			myCar[choose_car_number].consumptionWithAC(direct_path_distance);
			cout << "\t" << "Average consumption for " << direct_path_distance << " km: " << myCar[choose_car_number].totalAverageConsumption(direct_path_distance) << " L" << endl;
			cout << "\t" << "Here is how much time it takes to reach your destination: ";
			time(path_matrix[start][finish], intermediar_nodes);
		}

		cout << endl;
		cout << endl << "\tSecond option: shortest path" << endl;
		cout << "***It is recommended to take this route into account when planning your journey.***" << endl;
		RoyFloyd();
		printRoad(start, finish);
		if (path_matrix[start][finish] < inf)
		{
			myCar[choose_car_number].consumptionWithAC(shortest_path_distance);
			cout << "\t" << "Average consumption for " << shortest_path_distance << " km: " << myCar[choose_car_number].totalAverageConsumption(shortest_path_distance) << " L" << endl;
			cout << "\t" << "Here is how much time it takes to reach your destination: ";
			time(path_matrix[start][finish], intermediar_nodes);
		}

		cout << endl;

		cout << "Do you want to choose again? To continue, enter 'Y'. To exit, enter 'N': "; cin >> choose_again;
		//check if user input is valid
		while (car_input_check(choose_again) == 0)
		{
			cout << "Your answer is not valid. Try again! "; cin >> choose_again;
		}
		cout << endl;
	}

	cout << endl;
	return 0;
}

int input_check(string input)
{
	//converts the user's input (a string) into an integer
	int city = 0; //city=0 because we do not have the number 0 representing a city
	//converts the string entered by the user into capital letters. SOURCE: http://www.cplusplus.com/reference/locale/toupper/
	for (int i = 0; i < input.length(); i++)
	{
		input[i] = toupper(input[i]);
	}
	if (input == "NOTTINGHAM")
	{
		city = 1;
	}
	else
		if (input == "MANCHESTER")
		{
			city = 2;
		}
		else
			if (input == "LIVERPOOL")
			{
				city = 3;
			}
			else
				if (input == "LONDON")
				{
					city = 4;
				}
				else
					if (input == "NORWICH")
					{
						city = 5;
					}
	return city;
}

int valid_input(string input)
{
	//checks if the user input is valid. Here, the user input will be the name of the city
	int valid = 0; //user input is not correct

	//converts the string entered by the user into capital letters. SOURCE: http://www.cplusplus.com/reference/locale/toupper/
	for (int i = 0; i < input.length(); i++)
	{
		input[i] = toupper(input[i]);
	}
	if ((input == "NOTTINGHAM") || (input == "MANCHESTER") || (input == "LIVERPOOL") || (input == "LONDON") || (input == "NORWICH"))
	{
		valid = 1; //user input is valid
	}
	return valid;
}


int car_input_check(string input)
{
	//checks if user input is valid
	int valid = 0;//input is not valid
	if ((input == "Y") || (input == "N") || (input == "y") || (input == "n"))
	{
		valid = 1;//input is valid
	}
	return valid;
}

void initialize()
{
	//initializes the path matrix
	int start_node, finish_node; //start_node and final_node represent the initial/final destination
	//path matrix has dimension nodes*edges
	for (start_node = 1; start_node <= nodes; start_node++)
		for (finish_node = 1; finish_node <= edges; finish_node++)
			if (start_node == finish_node)
			{
				path_matrix[start_node][finish_node] = 0; //if the starting node is the same as the final node, we cannot have a path between
			}
			else
			{
				path_matrix[start_node][finish_node] = inf;
			}
}

void readPathFromFile()
{
	//load data from file
	int start_node, finish_node, i;//distance=path legth
	float distance;
	ifstream f("distance.txt");
	if (!f)
	{
		cout << "No file was found.";
	}
	f >> nodes >> edges;
	initialize();
	for (i = 1; i <= edges; i++)
	{
		f >> start_node >> finish_node >> distance;//(start_node,finish_node)=edge, distance=path legth (start_node,finish_node)
		//filling in the path matrix with the correct distances between 2 nodes
		path_matrix[start_node][finish_node] = distance;
	}
	f.close();
}

void RoyFloyd()
{
	//determines the shortest path between 2 nodes
	//SOURCE: https://atestatbas.weebly.com/algoritmul-roy-floyd.html
	int middle_node, start_node, finish_node;
	for (middle_node = 1; middle_node <= nodes; middle_node++)
		for (start_node = 1; start_node <= nodes; start_node++)
			for (finish_node = 1; finish_node <= nodes; finish_node++)
				if ((start_node != middle_node) && (finish_node != middle_node) && (path_matrix[start_node][finish_node] > path_matrix[start_node][middle_node] + path_matrix[middle_node][finish_node]))
				{
					path_matrix[start_node][finish_node] = path_matrix[start_node][middle_node] + path_matrix[middle_node][finish_node];
				}
}

void path(int start_node, int finish_node)
{
	//determine number of edges between nodes and intermediar node
	int ok = 0, middle_node; //ok=0-we do not have an intermediar node, middle_node-represents the node between start_node and finish_node 
	for (middle_node = 1; middle_node <= nodes && ok == 0; middle_node++)
	{
		if ((start_node != middle_node) && (finish_node != middle_node) && (path_matrix[start_node][finish_node] == path_matrix[start_node][middle_node] + path_matrix[middle_node][finish_node]))
		{
			path(start_node, middle_node);
			path(middle_node, finish_node);
			ok = 1;//we found an edge between a node and an intermediar node
		}
	}
	if (ok == 0)
	{
		intermediar_nodes++;
	}
}

int numberOfIntermediarNodes(int start_node, int finish_node)
{
	//return the number of intermediar nodes
	if (path_matrix[start_node][finish_node] < inf)
	{
		intermediar_nodes = 0;
		path(start_node, finish_node);
		intermediar_nodes = intermediar_nodes - 1; //intermediar_nodes-1 because if we have 2 nodes and an intermediar one between those two, then we will have 2 edges and 1 intermediar node
	}
	return intermediar_nodes;
}

void printRoad(int start_node, int finish_node)
{
	//prints the shortest path
	if (path_matrix[start_node][finish_node] < inf) //we have a path between nodes
	{
		intermediar_nodes = 0;
		path(start_node, finish_node);
		shortest_path_distance = path_matrix[start_node][finish_node];
		intermediar_nodes = intermediar_nodes - 1;
		if (numberOfIntermediarNodes(start_node, finish_node) == 0)//we don't have an intermediar node
		{
			cout << "\t" << "Short path is the same as direct path." << endl;
		}
		else
		{
			cout << "\t" << "Shortest path from " << start_node << " to " << finish_node << endl;
			if (intermediar_nodes > 1)
			{
				cout << "\t" << "This path has " << intermediar_nodes << " intermediar nodes." << endl;
			}
			else
			{
				cout << "\t" << "This path has " << intermediar_nodes << " intermediar node." << endl;
			}

			cout << "\t" << "Distance: " << shortest_path_distance << " km" << endl;
		}
	}

	else
	{
		cout << "\t" << "There is no path from " << start_node << " to " << finish_node << endl;
	}
}

int check(int start_node, int finish_node)
{
	//determine if the direct path is valid: nodes should be different and it should be the correct distance between them
	int ok = 0; //the path is not correct
	if ((start_node != finish_node) && (path_matrix[start_node][finish_node] != inf))
	{
		ok = 1; //path is correct
	}
	return ok;
}

void directPath(int start_node, int finish_node)
{
	//check if there is a direct path between nodes
	if (check(start_node, finish_node) == 1) //path is correct/valid
	{
		cout << "\t" << "Direct path found! " << endl;
		cout << "\t" << "Distance: " << path_matrix[start_node][finish_node] << " km" << endl;
		direct_path_distance = path_matrix[start_node][finish_node];
	}
	else
	{
		cout << "\t" << "Sorry, there is no direct path.";
	}

}

int input_trunk_tires()
{
	//check how heavy the car is
	string car_loaded, trunk_loaded, rooftop_loaded, new_tires;
	int load = 0; //stores how many extra weights are in/on the car and also stores the tire condition (if the tires are old, it will increase by 1)
	cout << endl;
	cout << "How full is your car?" << endl;

	cout << endl << "Do you have 5 or less people in your car? Press Y for 'YES' or N for 'NO'. "; cin >> car_loaded;
	//checks if user input is valid
	while (car_input_check(car_loaded) == 0)
	{
		cout << "Input not valid. Please try again." << endl;
		cout << endl << "Do you have 5 or less people in your car? Press Y for 'YES' or N for 'NO'. "; cin >> car_loaded;
	}

	cout << endl << "Is your trunk loaded? Press Y for 'YES' or N for 'NO'. "; cin >> trunk_loaded;
	//checks if user input is valid
	while (car_input_check(trunk_loaded) == 0)
	{
		cout << "Input not valid. Please try again." << endl;
		cout << endl << "Is your trunk loaded? Press Y for 'YES' or N for 'NO'. "; cin >> trunk_loaded;
	}

	cout << endl << "Do you have a rooftop cargo carrier on your car? Press Y for 'YES' or N for 'NO'. "; cin >> rooftop_loaded;
	//checks if user input is valid
	while (car_input_check(rooftop_loaded) == 0)
	{
		cout << "Input not valid. Please try again." << endl;
		cout << endl << "Do you have a rooftop cargo carrier on your car? Press Y for 'YES' or N for 'NO'. "; cin >> rooftop_loaded;
	}


	cout << endl << "Are your tires new? Press Y for 'YES' or N for 'NO'. "; cin >> new_tires;
	//checks if user input is valid
	while (car_input_check(new_tires) == 0)
	{
		cout << "Input not valid. Please try again." << endl;
		cout << endl << "Are your tires new? Press Y for 'YES' or N for 'NO'. "; cin >> new_tires;
	}

	//if the user answer is positive (yes) it will consider that as an extra weight on the car or there is an extra consumption factor
	if ((car_loaded == "Y") || (car_loaded == "y"))
	{
		load = load + 1;
	}

	if ((trunk_loaded == "Y") || (trunk_loaded == "y"))
	{
		load = load + 1;
	}

	if ((rooftop_loaded == "Y") || (rooftop_loaded == "y"))
	{
		load = load + 1;
	}

	if ((new_tires == "N") || (new_tires == "n"))
	{
		load = load + 1;
	}
	return load;
}

int AC_on()
{
	//checks if the AC is on or off
	string AC;
	int AC_car = 0; //the AC is initially off
	cout << endl << "The AC is going to be on during the trip? Press Y for 'YES' or N for 'NO'. "; cin >> AC;
	//checks if user input is valid
	while (car_input_check(AC) == 0)
	{
		cout << "Input not valid. Please try again." << endl;
		cout << "The AC is going to be on during the trip? Press Y for 'YES' or N for 'NO'. "; cin >> AC;
	}

	if ((AC == "Y") || (AC == "y"))
	{
		AC_car = 1; //AC is on
	}
	return AC_car;
}

void time(double distance, int intermediar_nodes)
{
	//calculates the time required for the journey
	double average_speed, travel_time, new_distance;
	/*average_speed-stores the speed limit
	  travel_time-stores the time required to get from the first destination to the second destination
	  new_distance-stores the initial distance between destinations but it is converted into meters*/
	int hours = 0; //will count the number of hours
	if (intermediar_nodes != 0)
	{
		average_speed = 90;
	}
	else
	{
		average_speed = 130;
	}
	new_distance = distance * 1000; //convert from km to m
	average_speed = average_speed / 3.6; //convert km/h in m/s
	travel_time = (new_distance / average_speed) / 60; //convert seconds into minutes
	//count how many hours the trip lasts
	while (travel_time >= 60)
	{
		travel_time = travel_time - 60;
		hours++;
	}
	cout << hours << " h " << (int)travel_time << " m" << endl;
}

car::car(int nr, string brand, string model, float engine, double avg_cons)
{
	Number = nr;
	Brand = brand;
	Model = model;
	EngineSize = engine;
	AverageConsumption = avg_cons;
}

car::car()
{
	Brand = "Unspecified";
}

void car::display()
{
	cout << setfill(' ') << setw(5) << Number << setfill(' ') << setw(22) << Brand << setfill(' ') << setw(12) << Model << setfill(' ') << setw(15) << EngineSize << setfill(' ') << setw(15) << AverageConsumption << endl;
}

void car::load(ifstream& carFile)
{
	carFile >> Number >> Brand >> Model >> EngineSize >> AverageConsumption;

}

float car::consumption()
{
	//claculates the car consumption
	consumptionForDistance = AverageConsumption; //AverageConsumption is taken from carFile.txt
	int trunk_tire_state = input_trunk_tires(); //trunk_tire_state refers to the state of the tires: if they are new or old. Depending on the value stored in this variable it will add extra fuel consumption
	consumptionAC = 0; //AC is off, meaning that the consumption is 0
	if (trunk_tire_state != 0) //the trunk is loaded or/and the tires are old
	{
		/* Depending on the user answers from the input_trunk_tires() function, the value of the trunk_tire_state variable is different.
		Possible scenarios: there are maximum 5 people in the car, the trunk is loaded, there is a rooftop cargo carrier or the tires are old.
		*/
		if (trunk_tire_state == 1) // increase the consumption by 10%
		{
			consumptionForDistance = consumptionForDistance + 0.01*consumptionForDistance;
		}
		else
			if (trunk_tire_state == 2) //increase the consumption by 15%
			{
				consumptionForDistance = consumptionForDistance + 0.15*consumptionForDistance;
			}
			else
				if (trunk_tire_state == 3)//increase the consumption by 20%
				{
					consumptionForDistance = consumptionForDistance + 0.2*consumptionForDistance;
				}
				else //increase the consumption by 25%
				{
					consumptionForDistance = consumptionForDistance + 0.25*consumptionForDistance;
				}
	}

	if (AC_on() != 0) //AC is on
	{
		consumptionAC = 1;
	}

	if (intermediar_nodes != 0) //we have intermediare nodes meaning that the speed will not be constant due to different speed limits
	{
		consumptionForDistance = consumptionForDistance + (10 / 100)*consumptionForDistance; //an increase by 10% in the fuel consumption
	}
	return consumptionForDistance; //the consumption based on how full is the car and the state of the AC (on/off)
}

float car::consumptionWithAC(double distance)
{
	//calculates the consumption when AC is on
	int distance_AC = distance;
	consumptionSumForAC = 0; //Adds 1 liter of fuel consumption for every 100 km
	if (consumptionAC != 0) //AC is on
	{
		while (distance_AC > 99)
		{
			//consumption increases by 1 liter every 100 km
			consumptionSumForAC = consumptionSumForAC + 1;
			distance_AC = distance_AC / 100;
		}
	}
	return consumptionSumForAC;
}

float car::totalAverageConsumption(double distance)
{
	int distance_for_total_consumption = distance;
	int full_distance = 0, half_distance = 0; //full_distance: if the distance is > 100 km; half_distance: if the distance is <100 km
	float temporary_consumption = 0; //stores the total consumption
	while (distance_for_total_consumption > 49)
	{
		if ((distance_for_total_consumption > 49) && (distance_for_total_consumption < 99))
		{
			temporary_consumption = (temporary_consumption + consumptionForDistance + consumptionSumForAC) / 2; //it will add only half of the consumption for 100 km
			half_distance++; //counts how many "half distances" we have in a "full" distance
		}
		else
		{
			temporary_consumption = temporary_consumption + consumptionForDistance + consumptionSumForAC; //adds the whole consumption for 100 km
			full_distance++; //counts how many "full" distances we have
		}
		distance_for_total_consumption = distance_for_total_consumption - 100; //taking out 100 from the distance
	}

	temporary_consumption = (temporary_consumption * half_distance) + (temporary_consumption * full_distance);
	return temporary_consumption;
}