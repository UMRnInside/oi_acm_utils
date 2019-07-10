#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <algorithm>
#include <string>

#define _NUM 10
// very large int, may be overkill :)
class vl_int
{
    public:
        vl_int() {};
        vl_int(int i);
        vl_int(std::string);
        
        void operator=(const vl_int&);
        
        // TODO: interation with istream/ostream
        //friend std::istream& operator>>(std::istream& in, vl_int& vl);
        //friend std::ostream& operator<<(std::ostream& out, vl_int& vl);
        int what(int) const;
        int ncwhat(int);
        
        bool operator<(const vl_int&) const;
        bool operator>(const vl_int&) const;
        bool operator==(const vl_int&) const;
        vl_int operator+(const vl_int&) const;
        vl_int operator-(const vl_int&) const;
        vl_int operator*(const vl_int&) const;
        vl_int operator/(const vl_int&) const;
        
        void add(const vl_int&);
        void subtract(const vl_int&);
        void multiply(const vl_int&);
        void divide(const vl_int&);
        void dump(std::ostream&) const;
        long long toll() const;
        std::string tostring() const;
    //private:
        //bool is_negative;
        std::vector<int> v;
};

vl_int::vl_int(int n)
{
    bool is_negative = false;
    if (n<0) is_negative = true;
    //if (is_negative) std::cout<<"-------\n";
    if (n==0)
    {
        v.erase(v.begin(), v.end());
        return;
    }
    for (;n!=0;n/=_NUM)
        v.push_back(is_negative ? n%_NUM : n%_NUM);
}

vl_int::vl_int(std::string ts)
{
    bool is_negative = (ts[0] == '-');
    //if (is_negative) std::cout<<"-------\n";
    if (is_negative) ts = ts.substr(1);
    
    v.erase(v.begin(), v.end());
    
    std::string::iterator it;
    for (it=ts.begin();it!=ts.end();it++)
        v.push_back(*it - '0');
    
    if (is_negative)
        for (unsigned int i=0;i<v.size();i++) v[i] = -v[i];
    std::reverse(v.begin(), v.end());
}

std::string vl_int::tostring() const
{
    std::string ts, tr(" ");
    
    std::vector<int>::iterator it;
    for (auto i : v)
    {
        tr = std::abs((char)i) + '0';
        ts = tr + ts;
    }
    
    if (v[0] < 0) ts = "-" + ts;
    return ts;
}

void vl_int::operator=(const vl_int& vl)
{
    v = vl.v;
}

void vl_int::dump(std::ostream& out) const
{
    for (auto i : v)
        out<<i<<" ";
    out<<std::endl;
}
/*
std::istream& vl_int::operator>>(std::istream& in, vl_int& vl)
{
    std::string ts;
    std::vector<int>& v = vl.v;
    in>>ts;
    v.erase(v.begin(), v.end());
    
    std::string::iterator it;
    for (it=ts.begin();it!=ts.end();it++)
        v.push_back(*it - '0');
    std::reverse(v.begin(), v.end());
    return in;
}

std::ostream& vl_int::operator<<(std::ostream& out) const
{
    std::string ts;
    std::vector<int>::reverse_iterator rit;
    for (rit=v.rbegin();rit!=v.rend();rit++)
        ts += *rit + '0';
    
    out<<ts;
    return out;
}
*/
int vl_int::what(int i) const
{
    if ((unsigned)i >= v.size() )
    {
        return 0;
    }
    return v[i];
}

int vl_int::ncwhat(int i)
{
    if ((unsigned)i >= v.size() )
    {
        while ( (unsigned)i >= v.size()) v.push_back(0);
        return 0;
    }
    return v[i];
}

void vl_int::add(const vl_int& vl)
{
    int limit = std::max(vl.v.size(), v.size()); 
    for (int i=0;i<limit;i++)
    {
        int sum = ncwhat(i) + vl.what(i);
        v[i] = sum % _NUM;
        
        if (sum/_NUM)
        {
            ncwhat(i+1);
            v[i+1] = sum/_NUM;
        }
    }
}

int main()
{
    using namespace std;
    vl_int a(233), b("345");
    //cin>>a>>b;
    a.add(b);
    cout<<a.tostring()<<endl;
    cout<<vl_int(-233).tostring()<<endl;
    vl_int(-233).dump(cout);
    return 0;
}
