/*
This is a system that allows some company to manage their engine maintance operations. Employees may
register as new users, login, add an delete engines, update engine operating times, and add and complete 
jobs. Engines and users are stored in seperate vector containers while the engine object also contains 
a queue of jobs as an attribute. Users may register any user id they wish as long as there are no spaces. 
Passwords may be simple but are confirmed before registration. Menu options 3-7 are only available once
login is complete. In general user entries are not validated and unexpected entries will break the system.
There are no requirements that any entry is unique. Engines may be added or removed and the only update allowed
is to the engine operating times. Jobs may only be added to engines that already exist and may only be completed
in the order they have been added due to them being stored in a queue container.
*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <exception>

using namespace std;

// allows use of logged in menu options
bool allowed = false;

struct DneException : public exception {
	const char * what () const throw () {
      return "Runtime Exception: Engine id does not exist!";
   }
};
struct RegistrationException : public exception {
	const char * what () const throw () {
      return "Runtime Exception: There are no Engines registered!";
   }
};
struct JobException : public exception {
	const char * what () const throw () {
      return "Runtime Exception: Engine already contains no jobs!";
   }
};

// Job
class Job{	
public:
	int job_id;
	string job_description;	
};

// Engine 
class Engine{
public:
	int engine_id;
	int engine_hrs;
	queue<Job> jobs;

	void set_engine_hrs(int hrs){ engine_hrs = hrs; }

	int get_engine_id(){ return engine_id; }

	void enqueue_job(Job job){
		jobs.push(job);
	}

	void dequeue_job(){
		Job job;

		// throw exception if jobs is empty
		if(jobs.empty()){
			throw JobException();
		} else {
			job = jobs.front();
			jobs.pop();
		}
	}
};
vector<Engine> engines;

/*
Creates an engine object. Gets engine id and engine hours from user.
Engine id and Engine hours should be integers.
Adds the engine object to the engines vector. 
*/
void add_engine(){
	Engine engine;
	int eid;

	cout << "Enter engine id: ";
	cin >> engine.engine_id;
	cout << endl;

	cout << "Enter engine operating hours: ";
	cin >> engine.engine_hrs;
	cout << endl;

	engines.push_back(engine);
}

/*
Deletes a specified engine object. Gets engine id from user.
Engine id should be and integer and should exist.
If engine id does not exist user will be returned to menu.
*/
void delete_engine(){
	int eid;
	bool does_exist = false;

	if(engines.empty()){
		throw RegistrationException();
	}

	cout << "Enter engine id to be deleted: ";
	cin >> eid;
	cout << endl;

	for(int i = 0; i < engines.size(); i++){
		if(eid == engines[i].get_engine_id()){
			does_exist = true;
			engines.erase(engines.begin()+i);
			cout << "Engine id: " << eid << " erased! " << endl;
			break;
		}
	}
	if(!does_exist){
		throw DneException();
	}
}

/*
Updates engine operating hours on a specified engine. Gets engine id and engine hours from user.
Engine id and Engine hours should be and integer and Engine id should exist.
If engine id does not exist user will be returned to menu.
*/
void update_engine_hrs(){
	int eid;
	int hrs;
	bool does_exist = false;

	if(engines.empty()){
		throw RegistrationException();
	}

	cout << "Enter engine id to be updated: ";
	cin >> eid;
	cout << endl;

	cout << "Enter new engine operating hours for " << eid <<": ";
	cin >> hrs;
	cout << endl;

	// try catch throw if eid does not exist
	for(int i = 0; i < engines.size(); i++){
		if(eid == engines[i].get_engine_id()){
			does_exist = true;
			engines[i].set_engine_hrs(hrs);
			cout << "The new operating time for engine id: " << eid << " is: " << hrs << " hours." << endl;
			break;
		}
	}
	if(!does_exist){
		throw DneException();
	}
}

/*
Creates job object and adds it to an existing engine object. Gets Engine id, job id, and job description from user.
Engine id should be an integer and exist. Job id should be integer. Job description is a string.
Does not check that job id is unique, but user should use uniquie job id. 
If engine id does not exist user will be returned to menu.
*/
void add_job(){

	Job job;

	int eid;
	int jid;
	string j_desc;
	bool does_exist = false;

	if(engines.empty()){
		throw RegistrationException();
	}

	cout << "Enter engine id: ";
	cin >> eid;
	cout << endl;


	for(int i = 0; i < engines.size(); i++){
		if(eid == engines[i].get_engine_id()){
			does_exist = true;
			
			cout << "Enter job id: ";
			cin >> jid;
			cout << endl;
			job.job_id = jid;

			cout << "Enter job descripition: ";
			cin.ignore();
			getline(cin, j_desc); 
			cout << endl;
			job.job_description = j_desc;

			cout << "Job number: " << jid << " scheduled for engine id: " << eid << endl;

			engines[i].enqueue_job(job);
			break;
		}
	}
	if(!does_exist){
		throw DneException();
	}
}

/*
Deletes the oldest job in the queue for a specified engine. Gets engine id from user.
Engine id should be an integer. 
If engine id does not exist user will be returned to menu.
*/
void complete_job(){

	int eid;
	bool does_exist = false;

	if(engines.empty()){
		throw RegistrationException();
	}

	cout << "Enter engine id: ";
	cin >> eid;
	cout << endl;

	for(int i = 0; i < engines.size(); i++){
		if(eid == engines[i].get_engine_id()){
			does_exist = true;
			engines[i].dequeue_job();
			break;
		}
	}
	if(!does_exist){
		throw DneException();
	}

}

// User
class User{	
public:
	string user_id;
	string password;
	string f_name;
	string l_name;
	string email;

	string get_user_id(){ return user_id; }
	string get_password(){ return password; }
};
vector<User> users;


/*
Creates a User object. Gets user id, password, first and last name, and email from employee.
User id should be a string without spaces. User id is not checked for uniqueness, but should be unqiue.
Password may be simple or strong string password without spaces.
First and last names should be strings without spaces.
Email should be a string wihout spaces.
Validates that user enters matching password and confirm password fields.
User is added to the users vector.
*/
void register_user(){
	User user;

	string pwd, confirm_pwd;
	pwd = "password";
	confirm_pwd = "confirm";

	cout << "Enter user id: ";
	cin >> user.user_id;
	cout << endl;
	
	while(pwd != confirm_pwd){

		cout << "Enter password: ";
		cin >> pwd;
		cout << endl;

		cout << "Confirm password: ";
		cin >> confirm_pwd;
		cout << endl;
		if(pwd != confirm_pwd){
			cout << "Passwords do not match! Try again." << endl;
		} else {
			user.password = pwd;
		}
	}

	cout << "Enter first name: ";
	cin >> user.f_name;
	cout << endl;

	cout << "Enter last name: ";
	cin >> user.l_name;
	cout << endl;


	cout << "Enter email: ";
	cin >> user.email;
	cout << endl;

	users.push_back(user);
}

/*
Logs user into system unlocking menu options 3-7. Gets user id and password from user.
User id should be a string without spaces. Password should be a string without spaces.
Iterates the users vector until end is reached or a matching user id and password is found.
Returns true for a successful login and false otherwise.
*/
bool login(){

	string user_id;
	string password;

	cout << "Enter user id: ";
	cin >> user_id;
	cout << endl;

	cout << "Enter password: ";
	cin >> password;
	cout << endl;

	for(int i = 0; i < users.size(); i++){
		if(user_id == users[i].get_user_id() && password == users[i].get_password()){
			return true;
		}
	}
	return false;
}

// Menu
class Menu{
public:
	void clear_screen(){
		for(int i = 0; i < 100; i++){
			cout << endl;
		}
	}

	void launch_screen(){

		clear_screen();

		cout << "		Welcome to Some Company Engine Managment" << endl;
		cout << "			Press enter to continue" << endl;
		cout << "\n\n\n\n\n\n";
		cin.ignore(numeric_limits<streamsize>::max(),'\n');

		clear_screen();

	}

	void menu_screen(){

		cout << " ______________________________________________"  << endl;
		cout << "|                  User menu                   |" << endl;
		cout << "|  1. Register User                            |" << endl;
		cout << "|  2. Login                                    |" << endl;
		cout << "|  3. Add Engine                               |" << endl;
		cout << "|  4. Delete Engine                            |" << endl;
		cout << "|  5. Update Engine Operating Hours            |" << endl;
		cout << "|  6. Add Job                                  |" << endl;
		cout << "|  7. Complete Job                             |" << endl;
		cout << "|  8. Exit                                     |" << endl;
		cout << "|______________________________________________|" << endl;

	}

	int get_user_input(){
		int choice = 0;
		cout << "Enter menu choice (1-8): ";
		cin >> choice;
		cout << endl;
		clear_screen();

		while(choice < 1 || choice > 8){
			clear_screen();
			menu_screen();
			cout << "Sorry that is not a valid menu choice!" << endl;
			cout << "Enter a menu choice (1-8): " << endl;
			cin >> choice;
			cout << endl;
		}
		return choice;
	}
	
	int user_menu(int choice){

		// try catch menu choice throw out of menu range exception
		try{
			switch(choice) {
				case 1:
					register_user();
					break;
				case 2:
					if(login()) { cout << "Welcome!\n"; allowed = true;
					} else { cout << "Login Failed.\n"; } 
					break;	
					case 3:
						if(allowed) {
							add_engine();
						} else { cout << "Please login first.\n"; }
						break;
					case 4:
						if(allowed) {
							delete_engine();
						} else { cout << "Please login first.\n"; }
						break;
					case 5:
						if(allowed) {
							update_engine_hrs();
						} else { cout << "Please login first.\n"; }
						break;
					case 6:
						if(allowed) {
							add_job();
						} else { cout << "Please login first.\n"; }
						break;
					case 7:
						if(allowed) {
							complete_job();
						} else { cout << "Please login first.\n"; }
						break;
				case 8:
					exit(0);	
				}
			} catch (DneException &e) {
				cout << e.what() << endl;
			} catch (RegistrationException &e) {
				cout << e.what() << endl;
			} catch (JobException &e) {
				cout << e.what() << endl;
			}

		return choice;
	}

};

int main(){

	Menu menu;
	
	menu.launch_screen();

	while(true){
		menu.menu_screen();
		menu.user_menu(menu.get_user_input());
	}
}


