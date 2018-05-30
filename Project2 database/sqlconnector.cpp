/********************************************************
* FILE: project2.cpp                                    *
* AUTHOR: Keith Djouba                                  *
* DATE: 11/28/2017                                      *
* PURPOSE: SQL connector for CSC30500 Project #2        *
*********************************************************/

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <my_global.h>
#include <mysql.h>
#include <conio.h>

using namespace std;
// reads in a password without echoing it to the screen
string myget_passwd()
{
	string passwd;// declare password string
	for (;;)
	{
		char ch; // declare char
		ch = _getch(); // get char WITHIOUT echo!
		if (ch == 13 || ch == 10) // if done ...
			break;//  stop reading chars!
		cout << '*';  // dump * to screen
		passwd += ch;   // addd char to password
	}
	cin.sync(); // flush anything else in the buffer (remaining newline)
	cout << endl;  // simulate the enter that the user pressed

	return passwd;
}
int main()
{
	// mysql connection and query variables
	MYSQL *conn, // actual mysql connection
		mysql;   // local mysql data
	MYSQL_RES *result; // mysql query results
	MYSQL_ROW row;  // one row of a table (result)
					// strings for mysql hostname, userid, ...
	string db_host;
	string db_user;
	string db_password;
	string db_name;
	char command = 0;
	char request = 0;
	// set up the client (this machine) to use mysql
	cout << "initializing client DB subsystem ..."; cout.flush();
	mysql_init(&mysql);
	cout << "Done!" << endl;
	// get user credentials and mysql server info
	cout << "Enter MySQL database hostname (or IP adress):";
	cin >> db_host;
	cout << "Enter MySQL database username:";
	cin >> db_user;
	cout << "Enter MySQL database password:";
	db_password = myget_passwd();
	// could also prompt for this, if desired
	db_name = db_user;
	// go out and connect to the mysql server
	cout << "Connecting to remote DB ..."; cout.flush();
	conn = mysql_real_connect(&mysql,
		db_host.c_str(), db_user.c_str(), db_password.c_str(), db_user.c_str(),
		0, 0, 0); // last three are usually 0's
				  //if we couldn't setup the connection ...
	if (conn == NULL)
	{
		// print out the error message as to why ...
		cout << mysql_error(&mysql) << endl;
		return 1; // ... and exit the program. 
	}
	else
		cout << "DB connection established" << endl;
	// repeat (until user quits
	int status = 1;
	do
	{
		// primpt prompt
		cout << ">>>";
		cin >> command;
		switch (command)
		{
			// user wants to add ...
		case 'a':
			cin >> request;
			if (request == 'c' || request == 'C') // if user press a
			{
				// strings use to add information to databse
				string addCities;
				string citiesCode;
				string cities;
				cin >> citiesCode >> cities;
				//create new table on database if not already there
				addCities = "create table if not exists ";
				addCities += " citiesTable (citiesCode char(3), cities char (11) , primary key (cities));";
				status = mysql_query(conn, addCities.c_str());// check sql status
															  //send the sql to server
				addCities = "insert into citiesTable values(\"" + citiesCode + "\",\"" + cities + "\");";
				cout << "Your query: " << addCities << endl;
				status = mysql_query(conn, addCities.c_str()); // send the sql
				if (status != 0) cout << "\n\t\tError! " << mysql_error(&mysql) << endl << endl; //failed to send
				else cout << "\n\t\tYour query went through!\n" << endl; // show success message
			}
			else if (request == 't' || request == 'T') // if user press T
			{
				// strings use to add information to databse
				string addTeams;
				string teamName;
				string citiesCode;
				string cities;
				cin >> citiesCode >> teamName;
				//create new table on database if not already there
				addTeams = "create table if not exists ";
				addTeams += "teamTable (citiesCode char(3), cities char (11) , primary key (teamName));";
				status = mysql_query(conn, addTeams.c_str()); // check sql status
															  //send the sql to server
				addTeams = "insert into teamsTable values(\"" + citiesCode + "\",\"" + teamName + "\");";
				cout << "Your query: " << addTeams << endl;
				status = mysql_query(conn, addTeams.c_str());//send it
				if (status != 0) cout << "\n\t\tError! " << mysql_error(&mysql) << endl << endl;//fail to send
				else cout << "\n\t\tYour query went through!\n" << endl;//success message
			}
			else if (request == 'g') //if user press g
			{
				// strings use to add information to databse
				string addTeamGame;
				string teamName1;
				string teamScore1;
				string teamName2;
				string teamScore2;
				cin >> teamName1 >> teamScore1 >> teamName2 >> teamScore2;
				//create new table on database if not already there
				addTeamGame = "create table if not exists ";
				addTeamGame += "teamGameTable (teamName1 char(3), teamScore1 char(11), teamName2 char(3), teamScore2 char (11), primary key (teamName1, teamName2));";
				status = mysql_query(conn, addTeamGame.c_str());
				addTeamGame = "insert into teamGameTable values(\"" + teamName1 + "\",\"" + teamScore1 + "\",\"" + teamName2 +
					"\", \"" + teamScore2 + "\");";
				cout << "Your query: " << addTeamGame << endl;
				status = mysql_query(conn, addTeamGame.c_str());//send it
				if (status != 0) cout << "\n\t\tError! " << mysql_error(&mysql) << endl << endl;//fail to send
				else cout << "\n\t\tYour query went through!\n" << endl;//success message
			}
			break;
			// user press l ...
		case 'l':
			cin >> request;// declare user input
			if (request == 'c' || request == 'C')// if user press c or C
			{
				string listCommand; // declare string which list inputs
				listCommand = "select * from citiesCode order by cities, citiesTable;"; // send the sql  to sever
				cout << "Your query: " << listCommand << endl; // status
				cout << "\n\t\tRequest Sent!" << endl << endl; // sucess to send
				status = mysql_query(conn, listCommand.c_str());
				if (status != 0) cout << "\n\t\tError! " << mysql_error(&mysql) << endl; // if the query didn't work ...
				else
				{
					//print out required information
					result = mysql_store_result(conn);
					for (row = mysql_fetch_row(result); row != NULL; row = mysql_fetch_row(result))
					{
						// print out the first 2 colums; they are stored in
						cout << "\t" << row[0] << "\t" << row[1] << "\t" << endl;
					}
				}
				cout << endl;
			}
			if (request == 't' || request == 'T') // if user press t or T
			{
				string listCommand;// declare string which list inputs
				listCommand = "select * from citiesCode order by cities, citiesTable;"; // send the sql  to sever
				cout << "Your query: " << listCommand << endl; // status
				cout << "\n\t\tRequest Sent!" << endl << endl; // sucess to send
				status = mysql_query(conn, listCommand.c_str());
				if (status != 0) cout << "\n\t\tError! " << mysql_error(&mysql) << endl; // if the query didn't work
				else
				{
					//print out required information
					result = mysql_store_result(conn);
					for (row = mysql_fetch_row(result); row != NULL; row = mysql_fetch_row(result))
					{
						// print out the first 2 colums; they are stored in
						cout << "\t" << row[0] << "\t" << row[1] << "\t" << endl;
					}
				}
				cout << endl;
			}
			if (request == 'g' || request == 'G') // if press g
			{ // ... cities
				string listCommand; // declare string which list inputs
									// send the sql  to sever
				listCommand = "select * from teamName1 order by  teamScore1, teamName2, teamScore2, addTeamTable;";
				cout << "Your query: " << listCommand << endl;
				cout << "\n\t\tRequest Sent!" << endl << endl;
				status = mysql_query(conn, listCommand.c_str());
				if (status != 0) cout << "\n\t\tError! " << mysql_error(&mysql) << endl;
				else
				{
					//print out required information
					result = mysql_store_result(conn);
					for (row = mysql_fetch_row(result); row != NULL; row = mysql_fetch_row(result))
					{
						// print out the first 2 colums; they are stored in
						cout << "\t" << row[0] << "\t" << row[1] << row[2] << row[3] << "\t" << endl;
					}
				}
				cout << endl;
			}
			break;
		}
	} while (command != 'q'); // quit command
	mysql_close(conn); // clean up the connection
	return 0;
}
