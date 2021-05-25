#include <iostream>
#include <fstream>

using namespace std;

class keyPair {



    //I'd use a constructor but the complexity of this made it hard to make one (or i'm a dumbass). This will work temporarily.
    public:
    int firstEmptyIndex = 0;
    string key;
    string *value;
    int valSize;
    bool isArray = false;

    void writeKey(char k) {
        key += k;
    }

    void arrayCreate(int valAmount, string arr[]){
        value = new string[valAmount];
        cout << "Array Created. " << valAmount << " elements." << "\n";
        for (int i; i < valAmount; i++)
            value[i] = arr[i];
        valSize = valAmount;
    }

    void assignKP(string k, int valAmount, bool isArr){
        key = k;
        isArray = isArr;
        value = new string[valAmount];
        valSize = valAmount;
    }
    //
    void aValue(string val) {
        value[firstEmptyIndex] = val;
        firstEmptyIndex++;
    }

    void defineArray(bool isArr) {
        isArray = true;
    }
};


int main()
{
    string buff; // Line buffer, stores the string of the line currently being read by the file class
    char buffChar; // Used in for loop to temporarily store the current character in the line being read
    string inpName = "input.yml"; //name of file to open and read/parse, will be replaced with launch argument on release
    string outName = "output.ysl"; //name of file to convert and save to
    string buffKey; //in the final conversion it's gonna be part of the format
    //linetype
    ifstream inp;
    ofstream out;
    ofstream manifest;
    int kpCount = 0;
    int readerLine = 0;
    int lineCountTotal = 0;
    bool writingKeyValue = false;
    int arrayElementSize[100];
    int curKP = 0;

    inp.open(inpName);
    out.open(outName);
    manifest.open("input-manifest.ysm");
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
    int curLineStateSymbol[lineCountTotal];
    //reset the readers state and position to the start of the file again.
    inp.clear();
    inp.seekg(0);
    cout << "[DEBUG] " << kpCount <<  " Key Pairs counted.";
    while(getline(inp,buff)){
        bool symbolfound = false;
        for(int i = 0; i < buff.length(); i++)
        {
            if (symbolfound)
                break;
            buffChar = buff.at(i);
                /**
                0 - Regular KeyPair
                1 - Array Key
                11 - First Array Value
                12 - Following Array Values after 1st value (to make it easier to figure out where the array starts)
                13 - Last Array Value
                24 - First+Last Array Value (1 Value Array)
                2 - Comment
                3 - Unknown (invalid line)
                **/
                switch(buffChar) {
                case ':':
                    if(curLineState[readerLine - 1] == 12 || curLineState[readerLine - 1] == 11){
                            if (curLineState[readerLine - 1] == 11) {
                                curLineState[readerLine] = 24;
                                curLineStateSymbol[readerLine] = i;
                                symbolfound = true;
                            } else {
                            curLineState[readerLine - 1] = 13;
                            curLineState[readerLine] = 0;
                            curLineStateSymbol[readerLine] = i;
                            symbolfound = true;
                            }
                    } else {
                        curLineState[readerLine] = 0;
                        curLineStateSymbol[readerLine] = i;
                        symbolfound = true;
                    }
                    continue;

                case '-':
                    if(curLineState[readerLine - 1] == 0){
                        curLineState[readerLine - 1] = 1;
                        curLineState[readerLine] = 11;
                        curLineStateSymbol[readerLine] = i;
                        symbolfound = true;
                    continue;
                    }

                    if(curLineState[readerLine - 1] == 11){
                        curLineState[readerLine] = 12;
                        curLineStateSymbol[readerLine] = i;
                        symbolfound = true;
                        continue;
                    }


                    if(curLineState[readerLine - 1] == 12) {
                        curLineState[readerLine] = 12;
                        curLineStateSymbol[readerLine] = i;
                        symbolfound = true;
                        continue;
                    }

                case '#':
                    if(curLineState[readerLine - 1] == 12 || curLineState[readerLine - 1] == 11){
                            if (curLineState[readerLine - 1] == 11) {
                                curLineState[readerLine - 1] = 24;
                                curLineState[readerLine] = 2;
                                curLineStateSymbol[readerLine] = i;
                                symbolfound = true;
                                continue;
                            } else {
                            curLineState[readerLine - 1] = 13;
                            curLineState[readerLine] = 2;
                            curLineStateSymbol[readerLine] = i;
                            symbolfound = true;
                            continue;
                            }
                    } else {
                        curLineState[readerLine] = 2;
                        curLineStateSymbol[readerLine] = i;
                        symbolfound = true;
                        continue;
                    }

                default:
                    curLineState[readerLine] = 3;
                    continue;
                }
        }
        readerLine++;
    }

    if(curLineState[lineCountTotal - 1] == 11)
        curLineState[lineCountTotal - 1] = 24;
    if(curLineState[lineCountTotal - 1] == 12)
        curLineState[lineCountTotal - 1] = 13;

    inp.clear();
    inp.seekg(0);

    for (int i = 0; i < lineCountTotal; i++) {
        manifest << curLineState[i] << " " << curLineStateSymbol[i] <<"\n";
    }

    manifest.close();
    readerLine = 0;
    string curArray[100];
        int curArraySize = 0;
    while(getline(inp, buff)) {

        buffKey = "";
        writingKeyValue = false;
        cout << "[DEBUG] Reading Line " << readerLine << "\n";
            switch(curLineState[readerLine]){
            case 0:
                for(int i = 0; i < buff.length(); i++)
                {
                    buffChar = buff.at(i);
                    if(!writingKeyValue){
                        if(i != curLineStateSymbol[readerLine])
                            buffKey += buffChar;
                        else {
                            writingKeyValue = true;
                            kp[curKP].assignKP(buffKey, 1, false);
                            buffKey = "";
                            }
                    } else {
                        buffKey += buffChar;
                    }

                }
                if (writingKeyValue)
                    kp[curKP].aValue(buffKey);
            curKP++;
            break;
            case 1:
                cout << "[DEBUG] Array Detected. \n";
                curArraySize = 0;
                for(int i = 0; i < buff.length(); i++)
                {
                    buffChar = buff.at(i);
                    if(!writingKeyValue){
                        if(i != curLineStateSymbol[readerLine]) {
                            kp[curKP].writeKey(buffChar);
                            kp[curKP].defineArray(true);
                        }
                        else {
                            writingKeyValue = true;
                            buffKey = "";
                            }
                    } else {
                    }
                }

            break;
            case 2:
            break;
            case 11:
                cout << "[DEBUG] First Array Element Writing. \n";
                curArraySize++;
                for(int i = 0; i < buff.length(); i++)
                {
                    buffChar = buff.at(i);
                    if(!writingKeyValue){
                        if(i != curLineStateSymbol[readerLine]){}
                        else {
                            writingKeyValue = true;
                            buffKey = "";
                            }
                    }
                    else {
                                buffKey += buffChar;
                    }
                }



                cout << "\nWriting Value: " << buffKey << "\n";
                curArray[curArraySize - 1] = buffKey;
                break;
            case 12:
                cout << "[DEBUG] Second Array Element Writing.\n";
                curArraySize++;
                for(int i = 0; i < buff.length(); i++)
                {
                    buffChar = buff.at(i);
                    if(!writingKeyValue){
                        if(i != curLineStateSymbol[readerLine]){}
                        else {
                            writingKeyValue = true;
                            buffKey = "";
                            }
                    } else {
                        buffKey += buffChar;
                    }
                }
                cout << "\nWriting Value: " << buffKey << "\n";
                curArray[curArraySize - 1] = buffKey;

                break;
            case 13:
                cout << "[DEBUG] Last Array Element Writing.\n";
                curArraySize++;
                for(int i = 0; i < buff.length(); i++)
                {
                    buffChar = buff.at(i);
                    if(!writingKeyValue){
                        if(i != curLineStateSymbol[readerLine]){}
                        else {
                            writingKeyValue = true;
                            buffKey = "";
                            }
                    } else {
                        buffKey += buffChar;
                    }
                }
                cout << "\nWriting Value: " << buffKey << "\n";
                curArray[curArraySize - 1] = buffKey;
                kp[curKP].arrayCreate(curArraySize, curArray);
                curKP++;
                break;
            case 24:
                cout << "[DEBUG] First/Last Array Element Writing.\n";
                curArraySize++;
                for(int i = 0; i < buff.length(); i++)
                {
                    buffChar = buff.at(i);
                    if(!writingKeyValue){
                        if(i != curLineStateSymbol[readerLine]){}
                        else {
                            writingKeyValue = true;
                            buffKey = "";
                            }
                    } else {
                        buffKey += buffChar;
                    }
                }
                curArray[curArraySize - 1] = buffKey;
                kp[curKP].arrayCreate(curArraySize, curArray);
                cout << "\nWriting Value: " << buffKey << "\n";
                curKP++;
                break;
            default:
                break;
        }
        readerLine++;
    }

    cout << "Reading all KeyPairs loaded into memory..." << "\n";

    for(int i = 0; i < kpCount; i++) {
        cout << "ValSize = " << kp[i].valSize << " "<< "ID: " << i << " - " << kp[i].key << " - ";
        if(kp[i].isArray){
            cout << "[";
            for (int j = 0; j < kp[i].valSize; j++){
                cout << "(" << j << ")";
                cout << kp[i].value[j] << "|";
            }
        } else {
            cout << kp[i].value[0];
        }
        cout << "\n";
    }
    return 0;
}
