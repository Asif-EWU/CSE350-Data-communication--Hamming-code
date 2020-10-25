#include<bits/stdc++.h>
using namespace std;

class Sender
{
    string message;
    int codewordsize;
    char parityType;

public:
    int getCodeWordSize()
    {
        //calculate the codewordsize
        return codewordsize;
    }

    void setMessage(string _message)
    {
        message = _message;
    }

    void setParityType(int _parityType)
    {
        parityType = _parityType;
    }

    string generateCodeword()
    {
        //generate the codeword from the message and parityType
        int a, data_bit_no, parity_bit_no;
        string codeword;

        data_bit_no = (int)message.size();
        parity_bit_no = 1;
        while(parity_bit_no++)
        {
            a = pow(2, parity_bit_no);
            if((data_bit_no + parity_bit_no + 1) <= a) break;
        }

        codewordsize = data_bit_no + parity_bit_no;
        codeword = message;
        reverse(codeword.begin(), codeword.end());
        codeword = '0' + codeword;
        for(int i=1; i<=codewordsize; i*=2)
            codeword.insert(codeword.begin() + i, '0');

        int zero, one, x = 1;
        for(int i=0; i<parity_bit_no; i++)
        {
            int y = 0, res = 0;
            bool check = true;
            zero = one = x;

            while(1)
            {
                for(int j=0; j<zero; j++)
                {
                    y++;
                    if(y > codewordsize)
                    {
                        check = false;
                        break;
                    }
                }
                if(!check) break;

                for(int j=0; j<one; j++)
                {
                    res ^= (codeword[y] - '0');           /// bitwise operator
                    y++;
                    if(y > codewordsize)
                    {
                        check = false;
                        break;
                    }
                }
                if(!check) break;
            }

            if((parityType == 'o' && !res) || (parityType == 'e' && res))
                codeword[x] = '1';

            x *= 2;
        }

        codeword.erase(codeword.begin());
        reverse(codeword.begin(), codeword.end());
        return codeword;
    }
};

class Receiver
{
    int codewordsize;
    char parityType;

public:
    void setCodeWordSize(int _codewordsize)
    {
        codewordsize = _codewordsize;
    }

    void setParityType(int _parityType)
    {
        parityType = _parityType;
    }

    string receiveCodeword(string codeword, string message)
    {
        /*** Check whether the codeword contains an error
        If it does, correct it irrespective of the number of bits in error
        However, more than one bit gets altered,
        the corrected stream will also be erroneous. ***/

        string error_bit;
        int parity_bit_no = codewordsize - (int)message.size();
        reverse(codeword.begin(), codeword.end());
        codeword = '0' + codeword;

        int zero, one, x = 1;
        for(int i=0; i<parity_bit_no; i++)
        {
            int y = 0, res = 0;
            bool check = true;
            zero = one = x;

            while(1)
            {
                for(int j=0; j<zero; j++)
                {
                    y++;
                    if(y > codewordsize)
                    {
                        check = false;
                        break;
                    }
                }
                if(!check) break;

                for(int j=0; j<one; j++)
                {
                    res ^= (codeword[y] - '0');           /// bitwise operator
                    y++;
                    if(y > codewordsize)
                    {
                        check = false;
                        break;
                    }
                }
                if(!check) break;
            }

            if((parityType == 'o' && !res) || (parityType == 'e' && res)) error_bit += '1';
            else error_bit += '0';

            x *= 2;
        }

        int pos = 0;
        for(int i=0, j=1; error_bit[i]; i++, j*=2)
            if(error_bit[i] == '1')
                pos += j;

        cout << "Error Detected in Position: " << pos << endl;
        pos = min(pos, codewordsize);
        codeword[pos] = '1' - codeword[pos] + '0';

        codeword.erase(codeword.begin());
        reverse(codeword.begin(), codeword.end());
        return codeword;
    }
};

string randomChange(string codeword, int num)
{
    // randomly alter 'num' number of bits in the codeword and return it
    int codeword_size = (int)codeword.size();
    int rand_bit[2];

    rand_bit[0] = rand() % codeword_size;
    while(num == 2)
    {
        rand_bit[1] = rand() % codeword_size;
        if(rand_bit[1] != rand_bit[0]) break;
    }

    codeword[rand_bit[0]] = '1' - codeword[rand_bit[0]] + '0';
    if(num == 2) codeword[rand_bit[1]] = '1' - codeword[rand_bit[1]] + '0';

    return codeword;
}


int main()
{
    srand(time(NULL));

    Sender sender;
    Receiver receiver;
    string message, correctedCW;
    char parityType;

    // Take the message and parityType as input
    // 'o' for odd parity, 'e' for even parity
    cout << "Input the message: ";
    cin >> message;
    cout << "Input parity type: ";
    cin >> parityType;

    sender.setMessage(message);
    sender.setParityType(parityType);
    string codeword = sender.generateCodeword();

    string correctCW = codeword;
    cout << "Codeword: " << correctCW << endl << endl;

    string singleErrorCW = randomChange(codeword, 1);
    cout << "Single Bit Error: " << singleErrorCW << endl;

    string doubleErrorCW = randomChange(codeword, 2);
    cout << "Double Bit Error: " << doubleErrorCW << endl << endl;

    int codewordsize = sender.getCodeWordSize();
    receiver.setCodeWordSize(codewordsize);
    receiver.setParityType(parityType);

    cout << "For No Change in codeword: " << endl;
    correctedCW = receiver.receiveCodeword(correctCW, message);
    cout << "No Error in Codeword" << endl << endl;

    cout << "For Single bit Error: " << endl;
    correctedCW = receiver.receiveCodeword(singleErrorCW, message);
    cout << "After Correcting, CODEWORD is: " << endl << correctedCW << endl << endl;

    cout << "For Double bit Error: " << endl;
    correctedCW = receiver.receiveCodeword(doubleErrorCW, message);
    cout << "After Correcting, CODEWORD is: " << endl << correctedCW << endl;

    return 0;
}
