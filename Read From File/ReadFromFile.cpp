// reading a text file
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {

    int x, y;

    // declare
    ifstream myfile("example.txt");

    // open
    if (myfile.is_open())
    {
        // read from file
        while (!myfile.eof())
        {
            // only works with tab separated values
            myfile >> x >> y;

            cout << x << "\t" << y << endl;
        }

        // close
        myfile.close();
    }
    else {
        cout << "Unable to open file";
    }

    return 0;


    // TASK: Read the position of a single brick from file,
    // In Crimson Zombie Framework
}