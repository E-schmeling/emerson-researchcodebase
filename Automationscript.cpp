#include <iostream>
#include <cstdio>
#include <ctime>
#include <string>
#include <cstring>


using namespace std;

int main(){
    //steps:
    //  connect to RPI
    //  request file
    //  Await internet connection
    //  Access ONU weather station
    //  create companion file with weather data
    //  rename and transfer files to google drive   Mostly check


    time_t ctime;
    tm * curr_tm;
    time(&ctime);
	curr_tm = localtime(&ctime);
    char stime[20];
    strftime(stime,20,"%m_%d_%Y_%H-%M",curr_tm);



    string newstringname="G:\\My Drive\\ME-2991 Research Experience 1\\Raw Data\\ERS_"+(string)stime+".csv";
    cout<<newstringname;

    
    const int lenght=newstringname.length();
    char* newfile=new char[lenght+1];
    strcpy(newfile, newstringname.c_str()); 


    char oldfile[]="D:\\Emerson Data\\sensor_data.csv";
    rename(oldfile,newfile);
   
}