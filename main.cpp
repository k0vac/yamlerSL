#include <iostream>
#include <fstream>
#define BADYAML 30
#define INPUTFAIL 10
#define OUTPUTFAIL 20

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
    string buff; // Line buffer, stores the string of the line currently being read by the file class
    char buffChar; // Used in for loop to temporarily store the current character in the line being read
    string inpName = "testcase1.yml"; //name of file to open and read/parse, will be replaced with launch argument on release
    string outName = inpName + ".ysl"; //name of file to convert and save to
    string buffKey; //in the final conversion it's gonna be part of the format
    //linetype
    ifstream inp;
    ofstream out;
    int kpCount = 0;
    int readerLine = 0;
    int lineCountTotal = 0;
    bool writingKeyValue = false;
    int curKP = 0;

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
                } else {
                    kpCount++;
                }
            }
    lineCountTotal++;
    }

    keyPair *kp = new keyPair[kpCount];
    int curLineState[lineCountTotal];
    //reset the readers state and position to the start of the file again.
    inp.clear();
    inp.seekg(0);
    cout << "[DEBUG] " << kpCount <<  " Key Pairs counted.";
    while(getline(inp,buff)){

        for(int i = 0; i < buff.length(); i++)
        {
            buffChar = buff.at(i);
                /**
                0 - Regular KeyPair
                1 - Array Key
                11 - First Array Value
                12 - Following Array Values after 1st value (to make it easier to figure out where the array starts)
                13 - Last Array Value
                2 - Comment
                **/
                switch(buffChar) {
                case ':':
                    if(curLineState[readerLine - 1] == 12 || curLineState[readerLine - 1] == 11){
                        curLineState[readerLine - 1] = 13;
                        curLineState[readerLine] = 0;
                    } else {
                        curLineState[readerLine] = 0;
                    }
                    break;
                case '-':
                    if(curLineState[readerLine] == 0)
                        return BADYAML;
                    if(curLineState[readerLine - 1] == 0){
                        curLineState[readerLine - 1] = 1;
                        curLineState[readerLine] = 11;
                        break;
                    }

                    if(curLineState[readerLine - 1] == 11)
                        curLineState[readerLine] = 12;
                    break;

                case '#':
                    curLineState[readerLine] = 2;
                    break;
                default:
                    break;
                }
        }
        readerLine++;
    }

    inp.clear();
    inp.seekg(0);

    while(getline(inp, buff)) {
        buffKey = "";
        writingKeyValue = false;
            switch(curLineState[i]){
            case 0:


                for(int i = 0; i < buff.length(); i++)
                {
                    buffChar = buff.at(i);
                    if(!writingKeyValue){
                        if(buffChar != ':')
                        buffKey += buffChar;
                        else {
                            writingKeyValue = true;
                            buffKey

                            }
                    } else {

                    }
                }

            curKP++;
            break;
            case 1:

            case 2:

            case 11:

            case 12:
        }
        cout << buffKey << endl;
        out << buffKey << endl;

    }
    return 0;
}



/**

Error Codes:

10 - Can't access input file / Input File does not exist
20 - Can't write output
30 - Bad YAML Formatting

**/
