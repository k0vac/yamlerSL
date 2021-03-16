#include <iostream>
#include <fstream>

using namespace std;

class keyPair {
    string key;
    string *value;
    int valSize;
    int firstEmptyIndex = 0;
    //I'd use a constructor but the complexity of this made it hard to make one (or i'm a dumbass). This will work temporarily.
    public:
    void assignKP(string k, int valAmount){
        key = k;
        value = new string[valAmount];
    }
    //
    void aValue(string val) {
        value[firstEmptyIndex] = val;
        firstEmptyIndex++;
    }
};


int main()
{
    string buff;
    char buffChar;
    string inpName = "testcase1.yml";
    string outName = "testcase1.ysl";
    string buffKey;
    ifstream inp;
    ofstream out;
    int kpCount = 0;
    int readerLine = 0;
    bool writingKeyValue = false;
    bool isValidKeyPair = false;

    inp.open(inpName);
    out.open(outName);
    /**I used a simple while loop to iterate through all the lines in the file
    For loop iterates through every character in the line to find the colon line to signify it's an actual Key-Value Pair.
    If it doesn't find it, it's either a line as part of a list of variables in a YAML array.
    It counts how many pairs there are present in the file to generate an array of the keyPair() class.
    **/

    //First Iterator, counts number of valid pairs of keys and values.
    while(getline(inp, buff)) {
         for(int i = 0; i < buff.length(); i++)
            {
                buffChar = buff.at(i);
                if(buffChar != ':'){
                }else{
                    kpCount++;
                }
            }
    }

    keyPair *kp = new keyPair[kpCount];
    int curLineState[kpCount];
    //reset the readers state and position to the start of the file again.
    inp.clear();
    inp.seekg(0);
    cout << "[DEBUG] " << kpCount <<  " Key Pairs counted.";
    while(getline(inp,buff)){

        for(int i = 0; i < buff.length(); i++)
        {
            buffChar = buff.at(i);
                switch(buffChar) {
                case ':':

                case '-':
                    curLineState[readerLine];

                case '#':
                    continue;
                default:
                    cout << "[DEBUG] The program could not recognize the type of line. Line " << i;
                }
        }
        readerLine++;
    }

    inp.clear();
    inp.seekg(0);


    while(getline(inp, buff)) {
        buffKey = "";
        for(int i = 0; i < buff.length(); i++)
        {
            buffChar = buff.at(i);
            cout << buffChar;
            if(!writingKeyValue){
                if(buffChar != ':')
                buffKey += buffChar;
                else {
                    writingKeyValue = true;
                    }
            } else {

            }
        }
        cout << buffKey << endl;
        out << buffKey << endl;

    }
    return 0;
}
