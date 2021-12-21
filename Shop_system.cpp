#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <stdlib.h>
#include <conio.h>
#include <iomanip>
#include <time.h>
#include "logo.cpp"
using namespace std;

// Some of the constant variables
const int MAX_S = 5;																			// Max number of salesmen
const int MAX_I = 50;																			// Max number of items
int current_salesman;																		    // Number of hired salesmen
const string NAME = "giki"; 																	// Owner use name
const string PASSWORD = "giki123";																// Owner password

// Salesman structure
struct Sales_man
{
	int ID;																						// Stores salesman ID.no
	string Name;																				// Stores salesman names
	float total_salary;																			// Stores salesman salary
	
} s1 [ MAX_S ];

// Item structure
struct Item
{
	int item_no;																				// Stores item ID
	string item_type;																			// Stores item type
	string item_name;																			// Stores item name
	int buy_rate;																				// Stores the rate at which item is bought
	int sell_rate;																				// Stores the rate at which item is sold
	int stock_in_hand;																			// Stores the stock avalible
	int low_limit;																				// Stores the the lower lomit of items
	int sold_quantity;																			// Stores the amount of stock sold
	float net_sales;																			// Stores the money earned from selling stock
	
} items [ MAX_I ];

// Report structure
struct Report
{
	int served_customers;																		// Stores the total amount of served customers
	int unserved_customers;																		// Stores the total amount of unserved customers
	int items_sold;																				// Stores the total amount items sold
	long long int total_expense;																// Stores the total expense
	long long int total_earning;																// Stores the total earning from sales
	int profit;																					// Stores the profit after shop closes
	
} report;

// Prototyping
void welcome_screen ();																			// The main shop screen
int login ();																					// Owner login
void owner_screen ();																			// Options for owner's use

void start_sale ();																				// Start shop sale
void generate_report ();																		// Report can be seen by owner after every sale
void close_shop ();																				// When owner closes shop the day report pops up

void sales_man_managment ();																	// Select what owner wants to do with salesman
void add_salesman ();																			// Hire now salesman
void fire_salesman ();																			// Fire any one salesman
void fire_all_salesman ();																		// Fire all salesman
void present_salesman ();																		// Print all hired salesman

void refil_stock ();																			// Refill stock in store
void low_stock ();																				// Warning generator if stock is low
void stock_print ();																			// Print all avalible stock
void stock_sale_print ();																		// Print stock and the money it has earned through sale

void get_all_data ();																			// Get all previously stored data
void empty_all_data ();				   							  								// Empty junk values in all variables
void empty_salesman_array ();																	// Empty all junk values in salesman arrays

void salesman_file_update ();																	// Updates salesman text file
void stock_file_update ();																		// Updates avalible stock text file
void sold_data_file_update ();																	// Updates sold items text file

void delete_files ();																			// Delete all extra text files after day ends
void press ();																					// For a little pause


// Main function
int main ()
{
	// To change seed every run
	srand ( time ( 0 ) );
	
	// Calls empty_all_data to empty all junk values
	empty_all_data ();
	
	// Calls get_all_data () to get if any previously stored data
	get_all_data ();
	
	// Print login logo
	login_logo ();
	while ( !login () );																		// Continues to run this until successful login

	low_stock ();																				// If there is any low stock it prints warning
	welcome_screen ();																			// Shop welcome screen
	
	return 0;
}

// Function for welcome screen
void welcome_screen ()
{
	system ("CLS");
	
	// Prints Shop logo
	welcome_logo ();			
	owner_screen ();	
}

// Function for owner login
int login ()
{	
	// Owner username and password entery
	string name, password;
	cout << " Please enter owner name : ";
	getline ( cin, name );
	cout << " Enter your password : ";
	
	char c = ' ';	
	while ( 1 )
	{
		c = getch ();																			// Gets character from user
		if ( c == 13 )																			// Loop breaks when user presses enter as ASCII for enter is '13'
			break;
			
		password += c;																			// Stores entered password
		cout << "*";																			// Prints '*' instead of entered password
	}
	
	// If entered data equals to the original owner data, login is successful
	if ( NAME == name && PASSWORD == password )
	{
		return 1;																				// Return back to welcome screen
	}
	else
	{
		system ("CLS");																			// CLears screen
	
		// If entered data does not match the whole login process is repeated
		login_logo ();							
		cout << " Incorrect name or password." << endl;
		return 0;
	}
}

// Function for main owner screen
void owner_screen ()
{
	// Asks owner for what he whats to perform
	cout << " What would you like to do?\n Please enter a number.\n" << endl;	
	cout << " 1=> Salesmen managment.\n";
	cout << " 2=> Fill Inventory.\n";
	cout << " 3=> Start sale.\n";
	cout << " 4=> Generate Report.\n";
	cout << " 5=> Close Shop.\n";
	cout << " 6=> Quit program.\n";
	
	cout << "\n Your option : ";
	int response;
	cin >> response;
	
	// Switch to call relevant function
	switch ( response )
	{
		case 1 :
			sales_man_managment ();																// Calls sales_man_managment() for manegment of salesman
			break;	
		case 2 :
			refil_stock ();																		// Calls refil_stock() to refil all stock
			break;
		case 3 :
			start_sale ();																		// Calls start_sale() to start shop sale
			break;
		case 4 :
			generate_report ();																	// Calls generate_report() to generate report after shop sale
			break;		
		case 5 :
			close_shop ();																		// Calls close_shop() when owner want to close the shop	
			break;
		case 6 :
			exit ( 0 );																			// Exit program
			break;
		default :																				// If invalid number is entered, process is repeated
			cout << "\n Sorry wrong input." << endl;
			press ();
			welcome_screen ();	
	}
}

// The main process behind sale
void start_sale ()
{
	if ( current_salesman == 0 )																// If there are no salesman then it asks to hire salesman
	{
		system ( "CLS" );
		cout << "\n There are no hired salesman.\n Please hire salesmen to start selling stock.\n";
		press ();					
		welcome_screen ();
	}
	
	// Basic variable declariations for sale
	int sales_man_on_duty = current_salesman;
	int customers, unserved, served, unserved2, served2, buy_item , no_of_items;
	int count = 0;

	// Main sale process starts here
	for ( int sec=0; sec<300; sec++ )
	{
		unserved = 0, served = 0, unserved2 = 0, served2 = 0, buy_item = 0 , no_of_items = 0;	// Make everything equal to '0'
		
		customers = ( rand() % 6 );																// Generates random number of customers from "0-5"
		
		if ( customers > sales_man_on_duty )													// If the number of customers are greater than salesmen
		{																						// Customer does not get served
			unserved += ( customers - sales_man_on_duty );	
		}
		
		served += ( customers - unserved ) ;													// All those customers who will be served
		served2 = served;
		unserved2 = unserved;
		
		// Loop for number of customer
		for ( int i=0; i<served; i++ )
		{
			count = 0;
			no_of_items = ( rand() % 5 ) + 1;													// Generates random number of items that each customer will buy
			
			// Loop for number of item 
			for ( int k=0; k<no_of_items; k++ )
			{
				buy_item = ( rand() % MAX_I ) + 1;												// Generates random item that will be bought
				
				// Loop for searching requested item and its sale
				for ( int j=0; j<MAX_I; j++ )
				{
					if ( buy_item == items[ j ].item_no )										// Check if requested item is found in avalible stock
					{
						if ( items[ j ].stock_in_hand <= 0 )									// Checks if requested item is in stock
						{
							break;
						}
						
						report.items_sold++;													// Increments total sold items 
						items[ j ].sold_quantity++;												// Increments sold quantity of item
						items[ j ].stock_in_hand--;												// Decrements sold item from stock
						report.total_earning += ( items[ j ].sell_rate );						// Adds sell rate to total earning
						report.total_expense += ( items[ i ].buy_rate );						// Adds buy rate to total expense
						count++;																// count for item bough
						break;
					}
				}
			}
			if ( count == 0 )																	// After finishing with one customer loop, if no items were bought
			{																					// The customer is added to unserved customers
				served2--;
				unserved2++;
			}
		}
		
		report.served_customers += served2;														// Adds served customers to its total		
		report.unserved_customers += unserved2;													// Adds unserved customers to its total
	}
	
	// Calculates salesman pay after sale
	for ( int i=0; i<sales_man_on_duty ; i++ )
		s1[ i ].total_salary = ( 300 / 60 ) * 10.0;
	
	selling_logo ();
	
	sold_data_file_update ();
	stock_file_update ();																		// Updates all files with new data
	salesman_file_update ();
	
	empty_all_data ();																			// Empties all data
	get_all_data ();																			// Gets all new data from files	
	
	low_stock ();																				// If stock is low then it prints warning
	
	welcome_screen ();
}

// Function to generate report after sales
void generate_report ()
{
	system ( "CLS" );
	
	report_logo ();
	
	int total_customers = report.served_customers + report.unserved_customers;	
	
	cout << "\n Items sold = " << report.items_sold << endl;									// Prints total items that have been sold so far
	cout << " Total number of customers = " << total_customers << endl;							// Prints total number of customers that have visited the shop so far

	stock_print ();																				// Prints all avalible stock	

	press ();
	welcome_screen ();	
}

// Function to print data after closing of shop
void close_shop ()
{
	system ( "CLS" );
	
	report_logo ();
	
	report.profit = report.total_earning - report.total_expense;								// Calculates profit
	
	cout << " Number of entertained customers = " << report.served_customers << endl;			// Prints total number of served customers
	cout << " Number of returned customers    = " << report.unserved_customers << endl;			// Prints total number of unserved customers
		
	// Checks if after sale we had profit or loss
	if ( report.profit >= 0 )
	{
		cout << "\n Total earning     = " << report.total_earning << endl;						// Prints total earning after whole day
		cout << " Total expenditure = " << report.total_expense << endl;						// Prints total expense after whole day
		cout << " Net Profit        = " << report.profit << endl;								// Prints profit from sale
	}
	else
	{
		cout << "\n Total earning     = " << report.total_earning << endl;						// Prints total earning after whole day	
		cout << " Total expenditure = " << report.total_expense << endl;						// Prints total expense after whole day
		cout << " Loss from sale = " << report.profit << endl;									// Prints loss from sale
	}
	
	stock_sale_print ();																		// Prints net sale item vise	
	
//	empty_all_data ();																			// Empties all array
//	delete_files ();																			// Deletes all extra files
	
	cout << "\n Press any key to close shop";
	getch();
	exit (0);
}

// Main function for salesman managment
void sales_man_managment ()
{
	system ( "CLS" );
	
	salesman_logo ();

	// Asks owner for what he whats to perform
	cout << " What would you like to do?\n Please enter a number.\n" << endl;	
	cout << " 1=> Hire.\n";
	cout << " 2=> Fire.\n";
	cout << " 3=> Fire All.\n";
	cout << " 4=> View All.\n";
	cout << " 5=> Return back.\n";
	
	cout << "\n Your option : ";
	
	int response;
	cin >> response;
	
	// Switch to call relevant function 
	switch ( response )
	{
		case 1 :
			add_salesman ();																	// Calls add_salesman() to hire salesman
			break;
		case 2 :
			fire_salesman ();																	// Calls fire_salesman() to fire one salesman
			break;
		case 3 :
			fire_all_salesman ();																// Calls fire_all_salesman() to fire all salesman
			break;
		case 4 :
			present_salesman ();																// Calls present_salesman() to view all salesman
			break;	
		case 5 :
			welcome_screen ();																	// Calls welcome_screen() to return back to main screen
			break;
		default :																				// If invalid number is entered it repeats the process
			cout << " Invalid option selected.\n";					
			press ();
			sales_man_managment ();		
	}
}

// Function to hire new salesman
void add_salesman ()
{
	system ( "CLS" );
	
	if ( current_salesman == MAX_S )															// If there are already '5' salesmen present, then owner can not add more
		cout << "\n Max amount of salesmen present.\n";
	else
	{
		hire_logo ();
		int latest_id;									
			
		cout << "\n Please enter name : ";
		
		if ( current_salesman == 0 )															// If there are '0' hired salesmen
		{
			latest_id = 555;																	// Creates new id
			cin.ignore();	
			getline ( cin, s1[ 0 ].Name );														// Asks owner for name and stores in array
			s1[ 0 ].ID = latest_id;																// Stores new id in array			
		}
		else																					// If there are some hired salesmen previously		
		{
			latest_id = s1[ current_salesman-1 ].ID + 1;										// Creates id after previous given id
			
			cin.ignore();												
			getline ( cin, s1[ current_salesman ].Name );										// Asks owner for name and stores in array
			s1[ current_salesman ].ID = latest_id;												// Stores new id in array	
		}	

		cout << "\n Salesman given ID No. " << s1[ current_salesman ].ID << " has been hired.\n";
		
		current_salesman ++;																	// Increments total number of hired salesmen
		salesman_file_update ();																// Updates "Salesman.txt"
	}	

	press ();
	sales_man_managment ();																		// Goes back to main salesman lobby
}

// Function to fire one salesman
void fire_salesman ()
{
	system ( "CLS" );
	
	if ( current_salesman == 0 )																// If there are not hired salesmen, then owner can not fire any
		cout << "\n No Salesman present.\n Please hire Salesman.\n";
	else
	{
		fire_logo ();
		int fire_id, j = 0;											
		int *temp = new int [ current_salesman ];												// Creates new array temporarily
		
		// Prints all currently present salesmen
		cout << "Total number of salesmen present : " << current_salesman << endl << endl;
		cout << " ID" << "      " << "Name" << endl;
		cout << "-----" << setw(9) << "------" << endl;
				
		for ( int i=0; i<current_salesman; i++ )
		{
			cout << " " << s1[ i ].ID << "     " << s1[ i ].Name << endl;
		}
		
		cout << "\n Please enter id for salesman you want to fire : ";							// Asks owner for id of salesman he wants to fire
		cin >> fire_id;
		
		// Loop to search for requsted id in all salesman data
		for ( int i=0; i<current_salesman; i++ )
		{
			if ( fire_id == s1[ i ].ID )
			{	
				report.total_expense += s1[ i ].total_salary;									// Adds salary of fired salesman to expense
				current_salesman --;															// Decrements total number of salesmen
	
				// Moves all salesmen data to fill array
				for ( int j=i; j<current_salesman; j++ )
				{
					s1[ j ].ID = s1[ j+1 ].ID;
					s1[ j ].Name = s1[ j+1 ].Name; 
					s1[ j ].total_salary = s1[ j+1 ].total_salary;
				}
				
				cout << "\n Salesman has been fired.\n";
				
				salesman_file_update ();														// Updates "Salesman.txt"
				
				break;
			}
				
			if ( i == current_salesman-1 )														// If after all the process, the entered id is not found then no salesman id fired
				cout << "\n Salesman not found.\n";
		}
	}
		
	press ();
	sales_man_managment ();
}

// Function to fire all salesmen
void fire_all_salesman ()
{
	system ( "CLS" );
	// add please wait cycle
	if ( current_salesman == 0 )																// If there are no hired salesmen, owner can not fire any
		cout << "\n No Salesman present.\n Please hire Salesman.\n";
	else
	{
		fire_all_logo ();
		
		// Adds salary of all salesman to total expense
		for ( int i=0; i<current_salesman; i++ )
		{
			report.total_expense += s1[ i ].total_salary; 
		}
		
		empty_salesman_array ();																// Empties all salesmen arrays
		current_salesman = 0;																	// Current salesman have gone down to '0'
		
		ofstream del_all ( "All_data/Salesman.txt", ios::trunc );								// Deletes all data from "Salesman.txt"	
		del_all.close();											
		
		cout << "\n All salesmen fired.\n";
	}
	
	press ();
	sales_man_managment ();
}

// Function to view all salesmen
void present_salesman ()
{
	system ( "CLS" );
	
	if ( current_salesman == 0 )																// If there are no hired salesmen, then owner can not view them	
		cout << "\n No Salesman present.\n Please hire Salesman.\n";
	else
	{
		// Prints data of all salesmen
		cout << "\n Total number of salesmen present : " << current_salesman << endl << endl;
		cout << " ID" << "      " << "Name" << endl;
		cout << "-----" << setw(9) << "------" << endl;
				
		for ( int i=0; i<current_salesman; i++ )
		{
			cout << " " << s1[ i ].ID << "     " << s1[ i ].Name << endl;
		}
	}
	
	press ();
	sales_man_managment ();
}

// Function to refil all stock
void refil_stock ()
{
	system ( "CLS" );
	
	string temp;
	string line1, line2;
	
	ifstream main_check ( "All_data/Total_stock.txt", ios::in );
	ifstream refil_check ( "All_data/Avalible_stock.txt", ios::in );
	
	if ( main_check.is_open() && refil_check.is_open() )										// If both files opens correctly
	{
		// Checks if stock is already full
		for ( int i=0; i<MAX_I; i++ )
		{
			getline ( main_check, line1 );														// Gets data line by line from "Total_stock.txt"											
			getline ( refil_check, line2 );														// Gets data line by line from	"Avalible_stock.txt"
			
			if ( line1 != line2 )																// Compares both string
				break;																			// If even one is differnt it breaks because stock is not full
			else 	
				if ( i == MAX_I-1 )																// If all lines are are equal and loop reaches end of file then stock is full																				
				{
					full_stock_logo ();
					press ();
					welcome_screen ();
				}	
		}
		
		main_check.close();																		// Closes "Total_stock.txt"	
		refil_check.close();																	// Closes "Avalible_stock.txt"
	}
	else
	{
		cout << "\n Files could not be opened.\n";												// If file can not be opened properly programs exits
		exit(0);
	}
	
	ifstream main_data ( "All_data/Total_stock.txt", ios::in );	
	ofstream refil_data ( "All_data/Avalible_stock.txt", ios::out );
	
	if ( main_data.is_open() && refil_data.is_open() )											// If both files opens correctly
	{		
		// Copies all data from "Total_stock.txt" to "Avalible_stock.txt"	
		for ( int i=0; !main_data.eof(); i++ )
		{
			getline ( main_data, temp );														// Gets data from "Total_stock.txt"						
			refil_data << temp;																	// Stores data into "Avalible_stock.txt"
			
			if ( i != MAX_I-1 )																	// Does not store endl on the last data slot because it causes problems when reding the files
				refil_data << endl;	
		}
			
		main_data.close();																		// Closes "Total_stock.txt"																		
		refil_data.close();																		// Closes "Avalible_stock.txt" 
		
		refilling_logo ();
	}
	else
	{
		cout << "\n Files could not be opened.\n";												// If file can not be opened properly programs exits
		exit(0);
	}
	
	empty_all_data ();																			// Empties all data 
	get_all_data ();																			// Gets all new data from files
	
	cout << endl;
	welcome_screen ();
}

// Function to print warning for low stock
void low_stock ()
{
	system ( "CLS" );
	
	int low = 0, once = 0;
	
	// To print all remaining stock below low limit
	for ( int i=0; i<MAX_I; i++ )
	{
		if ( items[ i ].stock_in_hand <= items[ i ].low_limit )
		{
			// Only prints once	
			if ( once == 0 )																
			{
				once++;
				cout << " ID" << "       " << "Type" << "\t           " << "Name" << "\t        " << "Stock left ( kg )" << endl;
				cout << "----  ------------   ----------------  ------------------" << endl;
			}
			
			// Prints stock
			cout << left;
			cout << " " << setw(6) << items[ i ].item_no << setw(15) << items[ i ].item_type << setw(26) << items[ i ].item_name << setw(5) << items[ i ].stock_in_hand << endl;
			low++;			
		}
	}
	
	// If stock is low it prints warning
	if ( low > 0 )
	{
		cout << "\n WARNING !!!\n " << low << " items are low in stock.\n Please refil as soon as possible.\n"; 
		
		press ();
	}
}

// Function to print all avalible stock
void stock_print ()
{
	cout << endl << endl;
	cout << "  ID" << "       " << "Type" << "\t           " << "Name" << "\t         " << "Avalible Stock ( kg )" << endl;
	cout << " ----  ------------   ----------------  -----------------------" << endl;
		
	// Prints all avalible stock	
	for ( int i=0; i<MAX_I; i++ )
	{			
		cout << left;
		cout << "  " << setw(6) << items[ i ].item_no << setw(15) << items[ i ].item_type << setw(27) << items[ i ].item_name << setw(5) << items[ i ].stock_in_hand << endl;		
	}
}

//Function to print stock and the money it has earned through sale
void stock_sale_print ()
{
	// Calculates money earned item wise
	for ( int i=0; i<MAX_I; i++ )
		items[ i ].net_sales = items[ i ].sold_quantity * items[ i ].sell_rate;
	
	cout << endl << endl;
	cout << " ID" << "       " << "Type" << "\t           " << "Name" << "\t        " << "Sold Quantity ( kg )" << "\t" << "Earning (kg)" << endl;
	cout << "----  ------------   ----------------  ----------------------  --------------" << endl;
		
	// Prints all stock with money earned	
	for ( int i=0; i<MAX_I; i++ )
	{			
		cout << left;
		cout << " " << setw(6) << items[ i ].item_no << setw(15) << items[ i ].item_type << setw(26) << items[ i ].item_name << setw(20) << items[ i ].sold_quantity << items[ i ].net_sales << endl;		
	}
}

// Function to get all previously avalible data
void get_all_data ()
{
	// Opens "Avalible_stock" text file
	ifstream stock_data ( "All_data/Avalible_stock.txt", ios::in );
	if ( stock_data.is_open() )																	// If file opens properly
	{
		// Stores all data into arrays
		for ( int i=0; i<MAX_I; i++ )
		{					
			stock_data >> items[ i ].item_no >> items[ i ].item_type >> items[ i ].item_name >> items[ i ].buy_rate >> items[ i ].sell_rate >> items[ i ].stock_in_hand >> items[ i ].low_limit;
		}
		stock_data.close();																		// CLoses "Avalible_stock.txt"
	}
	else 																						// If there is no "Avalible_stock.txt" it creates a new one
	{
		string temp;
		
		ifstream main_data ( "All_data/Total_stock.txt", ios::in ); 
		ofstream new_data ( "All_data/Avalible_stock.txt", ios::out );
	
		if ( main_data.is_open() && new_data.is_open() )
		{	
			// Copies all data from "Total_stock.txt" to "Avalible_stock.txt"	
			for ( int i=0; i<MAX_I; i++ )
			{
				getline ( main_data, temp );													// Gets data from "Total_stock.txt"
				new_data << temp;																// Stores data into "Avalible_stock.txt"
			
				if ( i != MAX_I-1 )																// Does not store endl on the last data slot because it causes problems when reding the files
					new_data << endl;															
			}

			main_data.close();																	// Closes "Total_stock.txt"		
			new_data.close();																	// Closes "Avalible_stock.txt" 
		}
		else
		{
			cout << "\n Files could not be opened.\n";
			exit(0);
		}
		
		ifstream stock_data_2 ( "All_data/Avalible_stock.txt", ios::in );
		if ( stock_data_2.is_open() )															// If file opens properly
		{
			// Stores all data into arrays
			for ( int i=0; i<MAX_I; i++ )
			{			
				stock_data_2 >> items[ i ].item_no >> items[ i ].item_type >> items[ i ].item_name >> items[ i ].buy_rate >> items[ i ].sell_rate >> items[ i ].stock_in_hand >> items[ i ].low_limit;
			}
			stock_data_2.close();																// CLoses "Avalible_stock.txt"
		}
		else
		{
			cout << "\n Files could not be opened.\n";											// If file can not be opened properly programs exits
			exit(0);
		}
	}
 
	// Opens "Salesman" text file
	ifstream salesman_data ( "All_data/Salesman.txt", ios::in );
	if ( salesman_data.peek() == EOF )															// If data is found it continues, otherwise it skips the process			
		cout << "";
	else
	{ 
		if ( salesman_data.is_open() )															// If file opens properly
		{
			// Stores all data into arrays
			for ( int i=0; !salesman_data.eof(); i++ )
			{			
				salesman_data >> s1[ i ].ID >> s1[ i ].Name >> s1[ i ].total_salary;
				current_salesman++;
			}
			
			salesman_data.close();																// Closes "Salesman.txt"
		}
		else 
		{
			cout << "\n Files could not be opened.\n";											// If file can not be opened properly programs exits
			exit(0);
		}
	}
								
	ifstream item_sold_data ( "All_data/Data.txt", ios::in );
	if ( item_sold_data.peek() == EOF )															// If data is found it continues, otherwise it skips the process
		cout << "";
	else
	{
		if ( item_sold_data.is_open() )
		{	
			// Stores all data into arrays				
			item_sold_data >> report.served_customers >> report.unserved_customers >> report.items_sold >> report.total_earning >> report.total_expense;
			
			item_sold_data.close();																// Closes "Data.txt"
		}
		else 
		{
			cout << "\n Files could not be opened.\n";											// If file can not be opened properly programs exits
			exit(0);	
		}
	}	
	
	ifstream item_data ( "All_data/Sold_item.txt", ios::in );
	if ( item_data.peek() == EOF )																// If data is found it continues, otherwise it skips the process
		cout << "";	
	else
	{
		if ( item_data.is_open() )
		{	
			// Stores all data into arrays	
			for ( int i=0; i<MAX_I; i++ )			
				item_data >> items[ i ].item_no >> items[ i ].item_type >> items[ i ].item_name >> items[ i ].sold_quantity;
			
			item_data.close();																	// CLoses "Sold_item.txt"
		}
		else 
		{
			cout << "\n Files could not be opened.\n";											// If file can not be opened properly programs exits
			exit(0);
		}
	}	
}

// Function to empty all variables
void empty_all_data ()
{
	report.served_customers = 0;
	report.unserved_customers = 0;
	report.items_sold = 0;									
	report.total_expense = 0;																	// All report data
	report.total_earning = 0;
	report.profit = 0;
	current_salesman = 0;
	
	for ( int y=0; y<MAX_I; y++ )
	{
		items[ y ].item_no = 0;
		items[ y ].item_type = " ";
		items[ y ].item_name = " ";
		items[ y ].buy_rate = 0;
		items[ y ].sell_rate = 0;																// All item arrays
		items[ y ].stock_in_hand = 0;	
		items[ y ].low_limit = 0;
		items[ y ].sold_quantity = 0;
		items[ y ].net_sales = 0;
	}
	
	for ( int y=0; y<MAX_S; y++ )
	{
		s1[ y ].ID = 0;
		s1[ y ].Name = " ";																		// All salesman arrays
		s1[ y ].total_salary = 0;	
	}
}

// Function to empty all salesman array
void empty_salesman_array ()
{
	// Empties all salesman array
	for ( int y=0; y<MAX_S; y++ )
	{
		s1[ y ].ID = 0;
		s1[ y ].Name = " ";																		
		s1[ y ].total_salary = 0;	
	}
}

// Function to update "Salesman.txt"
void salesman_file_update ()
{
	ofstream update ( "All_data/Salesman.txt", ios::out );
	if ( update.is_open() )																		// If file opens correctly
	{			
		// Adds new data from arrays to "Salesman.txt"
		for ( int i=0; i<current_salesman; i++ )
		{
			update << s1[ i ].ID << " " << s1[ i ].Name << " " << s1[ i ].total_salary;
							
			if ( i != current_salesman-1 )														// Does not store endl on the last data slot because it causes problems when reding the files
				update << endl;																	
		}
						
		update.close();																			// Closes "Salesman.txt"
	}
	else 
	{
		cout << "\n File could not be opened.\n";												// If file can not be opened properly programs exits
		exit(0);
	}
}

// Function to update "Avalible_stock.txt"
void stock_file_update ()
{
	ofstream update ( "All_data/Avalible_stock.txt", ios::out );
	if ( update.is_open() )																		// If file opens correctly
	{	
		// Adds new data from arrays to "Avalible_stock.txt"			
		for ( int i=0; i<MAX_I; i++ )
		{
			update << items[ i ].item_no << " " << items[ i ].item_type << " " << items[ i ].item_name << " " << items[ i ].buy_rate << " " << items[ i ].sell_rate << " " << items[ i ].stock_in_hand << " " << items[ i ].low_limit;
							
			if ( i != MAX_I-1 )																	// Does not store endl on the last data slot because it causes problems when reding the files
				update << endl;								
		}
						
		update.close();																			// Closes "Avalible_stock.txt"
	}
	else 
	{
		cout << "\n File could not be opened.\n";												// If file can not be opened properly programs exits
		exit(0);
	}	
}

// Function to update "Data.txt" and "Sold_item.txt"
void sold_data_file_update ()
{
	ofstream update ( "All_data/Data.txt", ios::out );
	if ( update.is_open() )																		// If file opens correctly
	{
		// Adds new data from arrays to "Data.txt"
		update << report.served_customers << " " << report.unserved_customers << " " << report.items_sold << " " << report.total_earning << " " << report.total_expense;
		
		update.close();																			// Closes "Data.txt"
	}
	else
	{
		cout << "\n File could not be opened.\n" << endl;										// If file can not be opened properly programs exits
		exit (0);
	}
	
	ofstream change ( "All_data/Sold_item.txt", ios::out );
	if ( change.is_open() )																		// If file opens correctly
	{
		// Adds new data from arrays to "Sold_item.txt.txt"
		for ( int i=0; i<MAX_I; i++ )
			{
				change << items[ i ].item_no << " " << items[ i ].item_type << " " <<  items[ i ].item_name << " " << items[ i ].sold_quantity;
							
				if ( i != MAX_I-1 )
					change << endl;
			}
		change.close();																			// Closes "Sold_item.txt.txt"
	}
	else
	{
		cout << "\n File could not be opened.\n" << endl;										// If file can not be opened properly programs exits
		exit (0);
	}
}

// Function to delete all extra text files
void delete_files ()
{
	remove ( "All_data/Avalible_stock.txt" );													// Deletes "Avalible_stock.txt"
	remove ( "All_data/Salesman.txt" );															// Deletes "Salesman.txt"		
	remove ( "All_data/Sold_item.txt" );														// Deletes "Sold_item.txt"
	remove ( "All_data/Data.txt" );																// Deletes "Data.txt"		
}

// Function for a little pause
void press ()
{
	cout << "\n Press any key to continue";
	getch();																					// Gets character but does not display it on screen
}
