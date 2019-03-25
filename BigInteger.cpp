#include <iostream>
#include <vector>
#include <cstring>
#include <cstdio>

using namespace std;

class BigInteger
{
    private:
        vector<int> saven;
        string n;
        //！！！定义时须赋初值！切记切记！
        bool symbol=0;  //判断输入是否为负数
        bool printfu=0; //判断结果是否为负，若为负则先输出负号


    public:
        /*
        BigInteger(short num=0);
        BigInteger(int num=0);
        BigInteger(long long num);
        BigInteger(const char *num);
        BigInteger(const BigInteger &num);
        */
        
        BigInteger (){}
        ~BigInteger (){}

        BigInteger (BigInteger &bi);

        friend istream& operator>> (istream &input, BigInteger &num);
        friend ostream& operator<< (ostream &output, BigInteger &num);
        friend bool operator< (BigInteger a, BigInteger b);
        friend bool operator> (BigInteger a, BigInteger b);
        friend BigInteger operator- (const BigInteger &num);
        friend BigInteger operator= (const BigInteger &num);
        friend BigInteger operator+ (const BigInteger &a, const BigInteger &b);
        friend BigInteger operator- (const BigInteger &a, const BigInteger &b);
        friend BigInteger operator* (BigInteger a, BigInteger b);
        friend BigInteger operator/ (BigInteger a, BigInteger b);

};
        /*
        BigInteger::BigInteger(short num) {
            *this = num;
        }
        
        BigInteger::BigInteger(int num) {
            *this = num;
        }

        BigInteger::BigInteger(long long num) {
            *this = num;
        }

        BigInteger::BigInteger(const char *num) {
            *this = num;
        }

        BigInteger::BigInteger(const BigInteger &num) {
            *this = num;
        }
        */

        BigInteger::BigInteger (BigInteger &num) {
            this->symbol=num.symbol;
            this->saven.assign(num.saven.begin(), num.saven.end());
        }

        //将大数以字符串的形式读入，在读入操作中一并完成符号处理和用vector存储
        istream& operator>> (istream &input, BigInteger &num) {
            input >> num.n;
            num.saven.clear();
            num.symbol=(num.n[0]=='-'?1:0);
            if (num.symbol) {
                num.n=num.n.substr(1,num.n.size()); //去掉第一位的负号
            }
            int len=num.n.length();
            for (int i=len-1; i>=0; i--) {
                num.saven.push_back(num.n[i]-'0');
            }
            return input;
        }

        ostream& operator<< (ostream &output, BigInteger &num) {
            int len=num.saven.size();
            if (num.printfu) output << "-";
            for (int i=len-1; i>=0; i--) {
                output << num.saven[i];
            }
            return output;
        }


        bool operator< (BigInteger a, BigInteger b) {
            int len1=a.saven.size();
            int len2=b.saven.size();
            if (a.symbol && !b.symbol) return true;
            if (!a.symbol && b.symbol) return false;
            if (!a.symbol && !b.symbol) {
                if (len1 < len2) return true;
                else if (len1 > len2) return false;
                else {
                    for (int i=len1-1; i>=0; i--) {
                        if(a.saven[i] < b.saven[i]) return true;
                        else if(a.saven[i] > b.saven[i]) return false;
                    }
                    return false;
                }
            } else return b<a;
        }

        bool operator> (BigInteger a, BigInteger b) {
            return b<a;
        }


        //写这个的动机是让底下加法和减法里处理一些情况比较方便
        BigInteger operator- (const BigInteger &num) {
            BigInteger temp;
            temp.symbol=!num.symbol;    //符号取反
            temp.saven.assign(num.saven.begin(), num.saven.end());  //将传入的大数的数字部分复制到新定义的一个大数中
            return temp;
        }

        BigInteger operator= (const BigInteger &num) {
            this->symbol=num.symbol;
            this->saven.assign(num.saven.begin(), num.saven.end());
        }


        BigInteger operator+ (const BigInteger &a, const BigInteger &b)
        {
            BigInteger temp;
            temp.saven.clear();
            if (a.symbol && b.symbol) {
                temp.printfu=1;
            }
            if (b.symbol) return a-(-b);
            if (a.symbol) return b-(-a);
            int len1=a.saven.size();
            int len2=b.saven.size();
            int i=0;
            int contain;
            int carry=0;
            while (i<len1 || i<len2) {
                int a0=(i<len1)?a.saven[i]:0;
                int b0=(i<len2)?b.saven[i]:0;
                contain=a0+b0+carry;
                temp.saven.push_back(contain%10);
                carry=contain/10;
                i++;
            }
            if (carry!=0) {
                temp.saven.push_back(1);
            }
            return temp;
        }

        BigInteger operator- (const BigInteger &a, const BigInteger &b)
        {
            BigInteger temp;
            temp.saven.clear();
            if (!a.symbol && b.symbol) return a+(-b);
            if (a.symbol && !b.symbol) return -((-a)+b);
            if (a<b) {
                return -(b-a);  //我好傻，在这里想了好久。。。
            }
            int len1=a.saven.size();
            int len2=b.saven.size();
            int i=0;
            int contain;
            int carry=0;
            while (i<len1) {
                int a0=a.saven[i];
                int b0=(i<len2)?b.saven[i]:0;
                contain=a0-b0-carry;
                temp.saven.push_back((contain+10)%10);
                if (a0<b0+carry) carry=1;
                else carry=0;
                i++;
            }
            if (carry!=0) {
                temp.saven.push_back(1);
            }
            return temp;
        }

        BigInteger operator* (BigInteger a, BigInteger b)
        {
            BigInteger temp;
            temp.saven.clear();
            int len1=a.saven.size();
            int len2=b.saven.size();
            temp.saven.resize(len1+len2);
            if ((a.symbol + b.symbol)==1) {
                temp.printfu=1;
            }
            for (int i=0; i<len1; i++) {
                for (int j=0; j<len2; j++) {
                    temp.saven[i+j+1] += a.saven[i]*b.saven[i];
                }
            }
            for (int i=len1+len2-1; i>=0; i--) {
                if (temp.saven[i]>=10) {
                    temp.saven[i-1] += temp.saven[i]/10;
                    temp.saven[i] %= 10;
                }
            }
            //★删除前导的多余的0
            if (temp.saven[0] == 0) {
                vector<int>::iterator ite1 = temp.saven.begin();
                temp.saven.erase(ite1);
            }
            /*int i=temp.saven.size()-1;
            while (!temp.saven[i]) i--;
            temp.saven.resize(i+1);*/   //△Segmentation fault
            
            return temp;
        }

int main()
{

    BigInteger p,q,r;
    cin>>p>>q;
    r=p+q;
    cout<<r<<endl;
    r=p-q;
    cout<<r<<endl;
    r=p*q;
    cout<<r<<endl;
    bool z=p<q;
    cout << z;

    getchar();
    getchar();
    return 0;
}
