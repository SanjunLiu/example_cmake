#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]){

    using namespace std;
    string input_file = "test_readtxt.txt";
    if (argc == 2){
        input_file = argv[1];
    }
    cout << "the input txt file is: " << input_file << endl;

    ifstream fin;
    fin.open(input_file.c_str());

    if (fin.is_open() == false){
        cerr << input_file.c_str() << " open failed\n";
        fin.close();
        return -1;
    }

    string str;
    int count = 0;

    //read file by the break symbol with ':' or 'newline'
    getline(fin, str, ':');
    string tmp;
    while(fin){
        cout << count << ":" << str << endl;
        getline(fin, tmp, ':');
        str += tmp;
        ++count;
    }
    cout << count << ":" << str << endl;

    fin.close();

    cout << "read done!"<<endl;
    return 0;
}
