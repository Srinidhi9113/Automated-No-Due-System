// Automated no-dues system
/*
Profiles:  ==> Classes
1) Student
2) Admin
3) Librarian
4) Lab Coordinator
Files to hold their info
*/
/*
Process:
1) Student 
  * Initiate request for no-due
  * Check status of no-due
  * Get printout of no-due certifcate
2) Admin,Librarian,Lab Coordinator
  * Get list of all requests for no-due issue
  * Accept or reject request
*/
/*
 ** Folder Structure **
 Database
    Login
        Students.txt
        Admin.txt
    Requests
        Requests.txt
    Certificates
        <srn>.txt
*/

#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>

class Student{
    /*
    name - Name of student
    srn - Unique ID for student
    */
    private:
        std::string name;
        std::string srn;

    public:
        Student():Student("",""){}
        Student(std::string name,std::string srn):name(name),srn(srn){}
        Student(std::string name,std::string srn,int role):Student(){}

        ~Student(){
            std::cout<<"Logout Successful!!!"<<std::endl;
        }

        int SendRequest(){
            /*
            Append a new line name,srn,000 to the requests file  ==> Refer folder structure
            Here  act as status codes for Admin Librarian Lab Coordinator respectively
            0 => Not reviewed yet
            1 => Rejected
            2 => Accepted            
            */
           std::fstream fd;
           std::string line,word;
           std::vector<std::string> row;

           fd.open("Database/Requests/Requests.csv",std::ios::in);

           while(getline(fd,line,'\n')){
                row.clear();
                std::stringstream str(line);
                while(getline(str,word,',')){
                    row.push_back(word);
                }
                if(row[1]==srn){
                    std::cout<<"\nRequest already exists!!\n"<<std::endl;
                    fd.close();
                    return 0;
                }
           }
           fd.close();
           fd.open("Database/Requests/Requests.csv",std::ios::app);
           if(fd<<name<<","<<srn<<",000"<<"\n") std::cout<<"\nRequest Registered Successfully!!!\n"<<std::endl;
           else std::cout<<"Error";
           fd.close();
           return 0;
        }

        int CheckStatus(){
            /*
            Check status codes from the requests file and print to screen according to the status codes as
            Admin - <Accepted/Rejected>
            Librarian - <Accepted/Rejected>
            Lab Coordinator - <Accepted/Rejected>
            */
           std::fstream fin,temp;
           std::string line,word;
           std::vector<std::string> row,results;
           int status = 0;

           fin.open("Database/Requests/Requests.csv", std::ios::in);

           while(getline(fin,line,'\n')){
                row.clear();

                std::stringstream str(line);

                while(getline(str,word,',')){
                    row.push_back(word);
                }
                if(row[1]==srn){
                    for(auto x:row[2]){
                        if(x=='0') results.push_back("Pending");
                        else if(x=='1') results.push_back("Rejected");
                        else{
                            results.push_back("Accepted");
                            status++;
                        }
                    }
                    fin.close();

                    std::cout<<"\nAdministrator : "<<results[0]<<std::endl;
                    std::cout<<"Librarian : "<<results[1]<<std::endl;
                    std::cout<<"Lab Coordinator : "<<results[2]<<"\n"<<std::endl;

                    if(status==3){
                        std::cout<<"\nYour Certificate is ready!!!\n"<<std::endl;
                        temp.open("Database/Requests/temp.csv",std::ios::out);
                        fin.open("Database/Requests/Requests.csv", std::ios::in);
                        while(getline(fin,line,'\n')){
                            int found = line.find(srn);
                            if(found==std::string::npos) temp<<line<<"\n";
                        }
                        fin.close();
                        temp.close();

                        remove("Database/Requests/Requests.csv");

                        rename("Database/Requests/temp.csv","Database/Requests/Requests.csv");

                    }
                    return 0;
                }
           }
           std::cout<<"\nRequest does not exist!!\n"<<std::endl;
           return 0;
        }

        int PrintCertificate(){
            /*
            Check whether <srn>.txt exists in certificates folder and print to screen if it exists or else print Pending
            */
            std::fstream fin;
            std::string line;
            fin.open("Database/Certificates/"+srn+".txt", std::ios::in);

            if(!fin){
                std::cout<<"\nCertificate not found!!!\n"<<std::endl;
                return -1;
            }
            std::cout<<std::endl;
            while(getline(fin,line,'\n')) std::cout<<line<<std::endl;
            std::cout<<std::endl;
        }

        friend class Application;

};

class Admin{
    /*
    name - Admin Name
    empID - unique ID for employee
    role - 
        0 - Admin
        1 - Librarian
        2 - Lab Coordinator
    */
    private:
        std::string name;
        std::string empID;
        int role;
    public:
        Admin():Admin("","",-1){}
        Admin(std::string name,std::string empID,int role):name(name),empID(empID),role(role){}
        Admin(std::string name,std::string empID):Admin(){}

        ~Admin(){
            std::cout<<"Logout Successful!!!"<<std::endl;
        }
        

        int GetAllRequests(){
            /*
            Read all lines from requests file
            */
            int index = 0,prints;
            std::fstream fin;
            std::string line,word;

            fin.open("Database/Requests/Requests.csv",std::ios::in);

            std::cout<<"\n All Requests :\n"<<std::endl;

            while(getline(fin,line,'\n')){
                index++;
                std::stringstream str(line);
                prints = 2;
                std::cout<<index<<") ";
                while(prints--){
                    getline(str,word,',');
                    std::cout<<word<<" ";
                }
                std::cout<<std::endl;
            }
            fin.close();
            return index;
        }

        void GenerateCertificate(std::vector<std::string> row){
            std::fstream fout;
            if(fout){
                fout.open("Database/Certificates/"+row[1]+".txt",std::ios::out);

                fout<<"=============== NO - DUE CERTIFICATE ==============="<<std::endl;
                fout<<"===================================================="<<std::endl;
                fout<<"    This is to certify that "<<row[0]<<"\n with the SRN "<<row[1]<<" has cleared all dues and is\n eligible to pass this semester.";

                std::cout<<"\nCertificate generated successfully\n"<<std::endl;
            }
            else std::cout<<"Error";

            fout.close();
        }

        int AcceptRequest(std::string accept){
            /*
            Change status code to 1 based on role
            If all status codes are 1, write a new file to certificates folder with folder name as srn
            */
           int max = GetAllRequests();
           int index,iter=0;
           while(std::cout<<"\nEnter the index of request to accept : " && 
           (!(std::cin>>index) || index<1 || index>max)){
                std::cin.clear();
                std::cin.ignore(1000,'\n');
                std::cout<<"\nInvalid Entry!!!\n"<<std::endl;
           }
           std::fstream fin,temp;
           std::string line,word;
           std::vector<std::string> row;
           fin.open("Database/Requests/Requests.csv", std::ios::in);
           temp.open("Database/Requests/temp.csv", std::ios::out);

           while(getline(fin,line,'\n')){
                row.clear();
                iter++;
                std::stringstream str(line);
                while(getline(str,word,',')){
                    row.push_back(word);
                }
                if(iter==index){
                    row[2].replace(role,1,accept);
                    if(row[2]=="222"){
                        GenerateCertificate(row);
                    }
                }
                temp<<row[0]<<","<<row[1]<<","<<row[2]<<"\n";
           }
           fin.close();
           temp.close();

           remove("Database/Requests/Requests.csv");
           rename("Database/Requests/temp.csv","Database/Requests/Requests.csv");
        }

        friend class Application;
};

namespace Auth{

    template <class profile> profile Authenticate(std::string type){
        std::fstream fin;
        std::vector<std::string> row;
        int tries = 3;

        while(tries--){
            std::string ID,line,word,passwrd;
            if(type=="Students") std::cout<<"\n Login \nEnter your SRN : ";
            else std::cout<<"\n Login \nEnter your Employee ID : ";
            std::cin>>ID;
            std::cin.ignore(1000,'\n');
            std::cout<<"Enter your password : ";
            std::cin>>passwrd;
            std::cin.ignore(1000,'\n');

            fin.open("Database/Login/"+type+".csv",std::ios::in);

            while(getline(fin,line,'\n')){
                row.clear();

                std::stringstream str(line);

                while(getline(str, word, ',')){
                    row.push_back(word);
                }

                if(ID==row[1] && passwrd==row[2]){
                    std::cout<<"\nSuccessful!!!\n"<<std::endl;
                    if(type=="Students") return profile(row[0],ID);
                    if(type=="Admins") return profile(row[0],ID,std::stoi(row[3]));
                }
            }

            std::cout<<"\nWrong Credentials!!!\n"<<std::endl;
            fin.close();
        }
        std::cout<<"\nSorry!! Record not found\n"<<std::endl;
        exit(0);
    }

    // Student StudentAuth(){
    //     std::fstream fin;
    //     std::vector<std::string> row;
    //     int tries = 3;

    //     while(tries--){
    //         std::string srn,line,word,passwrd;
    //         std::cout<<"\n Login \nEnter your SRN : ";
    //         std::cin>>srn;
    //         std::cin.ignore(1000,'\n');
    //         std::cout<<"Enter your password : ";
    //         std::cin>>passwrd;
    //         std::cin.ignore(1000,'\n');

    //         fin.open("Database/Login/Students.csv",std::ios::in);

    //         while(getline(fin,line,'\n')){
    //             row.clear();

    //             std::stringstream str(line);

    //             while(getline(str, word, ',')){
    //                 row.push_back(word);
    //             }

    //             if(srn==row[1] && passwrd==row[2]){
    //                 std::cout<<"\nSuccessful!!!\n"<<std::endl;
    //                 // Student obj(row[0],srn);
    //                 // return obj;
    //                 return Student(row[0],srn);
    //             }
    //         }

    //         std::cout<<"\nWrong Credentials!!!\n"<<std::endl;
    //         fin.close();
    //     }
    //     std::cout<<"\nSorry!! Record not found\n"<<std::endl;
    //     exit(0);

    // }

    // Admin AdminAuth(){
    //     std::fstream fin;
    //     std::vector<std::string> row;
    //     int tries = 3;

    //     while(tries--){
    //         std::string empID,line,word,passwrd;
    //         std::cout<<"\n Login \nEnter your Employee ID : ";
    //         std::cin>>empID;
    //         std::cin.ignore(1000,'\n');
    //         std::cout<<"Enter your password : ";
    //         std::cin>>passwrd;
    //         std::cin.ignore(1000,'\n');

    //         fin.open("Database/Login/Admins.csv",std::ios::in);

    //         while(getline(fin,line,'\n')){
    //             row.clear();

    //             std::stringstream str(line);

    //             while(getline(str, word, ',')){
    //                 row.push_back(word);
    //             }

    //             if(empID==row[1] && passwrd==row[2]){
    //                 std::cout<<"\nSuccessful!!!\n"<<std::endl;
    //                 return Admin(row[0],empID,std::stoi(row[3]));
    //             }
    //         }

    //         std::cout<<"\nWrong Credentials!!!\n"<<std::endl;
    //         fin.close();
    //     }
    //     std::cout<<"\nSorry!! Record not found\n"<<std::endl;
    //     exit(0);

    // }
}

class Application{
    public:
        int PrintWelcomeMessage(){
            int option;
            std::cout<<"\n<================= Automated No-Due System =================>\n"<<std::endl;
            std::cout<<"Welcome to the Application!!!\n"<<std::endl;
            while(std::cout<<"Choose an option : "<<std::endl<<"    1) Login"<<std::endl<<"    2) Exit"<<std::endl<<"Choice : " && 
                (!(std::cin>>option) || option<1 || option>2)){
                    std::cin.clear();
                    std::cin.ignore(1000,'\n');
                    std::cout<<"Invalid Entry!!!\n"<<std::endl;
                }
            return option;
        }

        int PrintOptions(int type){
            int choice;
            std::cout<<"Choose an option : "<<std::endl;
            switch(type){
                case 1:
                std::cout<<"    1) Issue Request"<<std::endl;
                std::cout<<"    2) Check Status"<<std::endl;
                std::cout<<"    3) Print Certificate"<<std::endl;
                std::cout<<"    4) Logout"<<std::endl;
                break;
                case 2:
                std::cout<<"    1) List Requests"<<std::endl;
                std::cout<<"    2) Accept Request"<<std::endl;
                std::cout<<"    3) Reject Request"<<std::endl;
                std::cout<<"    4) Logout"<<std::endl;
                break;
            }
            while(std::cout<<"Enter your choice : " &&
                (!(std::cin>>choice) || choice<1 || choice>4)){
                    std::cin.clear();
                    std::cin.ignore(1000,'\n');
                    std::cout<<"Invalid Input!!!"<<std::endl;
                }
            return choice;
        }

        int LoginOptions(){
            int option;
            while(std::cout<<"\nChoose login type : "<<std::endl<<"   1) Student"<<std::endl<<"   2) Admin"<<std::endl<<"   3) Exit"<<std::endl<<"Choice : " &&
                (!(std::cin>>option) || option<1 || option>3)){
                    std::cin.clear();
                    std::cin.ignore(1000,'\n');
                    std::cout<<"Invalid Entry!!!\n"<<std::endl;
                }
            return option;
        }

        void StudentLoop(){
            Student obj = Auth::Authenticate<Student>("Students");
            int choice;
            std::cout<<"\n Welcome "<<obj.name<<"\n"<<std::endl;
            do{
                choice = PrintOptions(1);
                switch(choice){
                    case 1:
                    obj.SendRequest();
                    break;
                    case 2:
                    obj.CheckStatus();
                    break;
                    case 3:
                    obj.PrintCertificate();
                    break;
                    case 4: return;
                }
            }while(true);
        }

        void AdminLoop(){
            Admin obj = Auth::Authenticate<Admin>("Admins");
            int choice;
            std::cout<<"Welcome "<<obj.name<<std::endl;
            do{
                choice = PrintOptions(2);
                switch(choice){
                    case 1:
                    obj.GetAllRequests();
                    break;
                    case 2:
                    obj.AcceptRequest("2");
                    break;
                    case 3:
                    obj.AcceptRequest("1");
                    break;
                    case 4: return;
                }
            }while(true);
        }

        friend void AppLoop(Application app);
};

void AppLoop(Application app){
            int login,loginType;
            do{
                login = app.PrintWelcomeMessage();

                if(login==2) break;

                else{
                    loginType = app.LoginOptions();
                    switch(loginType){
                        case 1:
                        app.StudentLoop();
                        break;
                        case 2:
                        app.AdminLoop();
                        break;
                        case 3: exit(0);
                    }
                }
            }while(true);
        }

int main(){
    Application App;
    AppLoop(App);
}