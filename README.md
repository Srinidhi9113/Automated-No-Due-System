# Automated-No-Due-System
An automated no due certificate generating system created as a programming project to use various features of Object Oriented Programming with C++.

The project.cc has the source code for the whole project. Save the Database folder in the same folder as project.cc.

The application initially allows login for 2 types of profiles
* Student
* Admin (Administrator, Librarian, Lab Coordinator)

The profile for Student has the following features:
1. Initiate a request for no-due certificate - A new request is added to the Database/Requests/Requests.csv file with status codes 000 representing the status of acceptance for Administrator, Librarian, Lab Coordinator respectively.
2. Check status of his request - Checks the status codes of his request
   * 0 - Not seen yet
   * 1 - Rejected
   * 2 - Accepted
3. Print Certificate - Display certificate if the status codes of his request are 222.

The profile for Admin has the following features:
1. Check list of all requests - Reads the Database/Requests/Requests.csv.
2. Accept a request - Updates the status code for a particular request to 2.
3. Reject a request - Updates the status code for a particular request to 1.

Features of OOP used:
1. Friend Function.
2. Friend Class.
3. Namespace.
4. Delegating Constructor.
5. Initialisation List for Constructors.
6. Template function.
