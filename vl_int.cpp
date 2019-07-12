#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <algorithm>
#include <string>
#include <vector>

#define _NUM 10
// very large int, may be overkill :)
class vl_int
{
    public:
        vl_int() {};
        vl_int(int i);
        vl_int(std::string);
        
        void operator=(const vl_int&);
        
        friend std::istream& operator>>(std::istream& in, vl_int& vl);
        friend std::ostream& operator<<(std::ostream& out, const vl_int& vl);
        int what(int) const;
        int& operator[](int);
        
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
        void multiply(int n, int offset=0, bool needfix=true);
        void divide(const vl_int&);
        
        void remove_tail0();
        void fix();
        void negative();
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
    bool neg = false;
    
    for (auto i : v)
    {
        tr = std::abs((char)i) + '0';
        ts = tr + ts;
        if (i < 0) neg = true;
    }
    if (v.size() == 0) return "0";
    if (neg) ts = "-" + ts;
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

std::istream& operator>>(std::istream& in, vl_int& vl)
{
    std::string ts;
    in>>ts;
    vl = vl_int(ts);
    return in;
}

std::ostream& operator<<(std::ostream& out, const vl_int& vl)
{
    std::string ts = vl.tostring();
    out<<ts;
    return out;
}

int vl_int::what(int i) const
{
    if ((unsigned)i >= v.size() )
    {
        return 0;
    }
    return v[i];
}

int& vl_int::operator[](int i)
{
    if ((unsigned)i >= v.size() )
    {
        while ( (unsigned)i >= v.size()) v.push_back(0);
        //return 0;
    }
    return v[i];
}

void vl_int::remove_tail0()
{
    std::reverse(v.begin(), v.end());
    
    std::vector<int>::iterator it;
    for (it=v.begin();it!=v.end();it++)
        if (*it != 0) break;
     
    v.erase(v.begin(), it);
    std::reverse(v.begin(), v.end());
}

void vl_int::negative()
{
    std::vector<int>::iterator it;
    for (it=v.begin();it!=v.end();it++)
        *it = -(*it);
}

void vl_int::fix()
{
    if (v.back() == 0) remove_tail0();
    bool revflag = false;
    if (v.back() < 0)
    {
        revflag = true;
        negative();
    }
    // neg intergrity
    int limit = v.size();
    for (int i=0;i<limit-1;i++)
    {
        for (int d=0;d<_NUM;d++)
        {
            if (v[i] + d*_NUM > 0)
            {
                v[i] += d*_NUM;
                v[i+1] -= d;
                break;
            }
        }
    }
    // round 2
    for (unsigned int i=0;i<v.size();i++)
    {
        //std::cout<<i<<":"<<v[i]<<"\n";
        if (v[i] >= _NUM)
        {
            int d = 0;
            while (v[i] - d*_NUM>= _NUM) d++;
            v[i] -= d*_NUM;
            
            (*this)[i+1] += d;
            //v[i+1] += d;
        }
    }
    
    remove_tail0();
    if (revflag) negative();
}

void vl_int::add(const vl_int& vl)
{
    int limit = std::max(vl.v.size(), v.size()); 
    for (int i=0;i<limit;i++)
    {
        int sum = (*this)[i] + vl.what(i);
        v[i] = sum % _NUM;
        
        if (sum/_NUM)
        {
            (*this)[i+1];
            v[i+1] += sum/_NUM;
        }
    }
    fix();
}

void vl_int::subtract(const vl_int& vl)
{
    vl_int nv = vl;
    nv.negative();
    //nv.dump(std::cout);
    add(nv);
    fix();
}

void vl_int::multiply(int n, int offset, bool needfix)
{
    //void multiply(int , int offset=0, bool needfix=true);
    vl_int& self = *this;
    vl_int tmp;
    
    int o_size = self.v.size();
    
    for (int i=0;i<o_size;i++)
        tmp[i+offset] = n*(self[i]);
    
    //tmp.fix();
    tmp.dump(std::cout);
    dump(std::cout);
    
    for (size_t i=0;i<tmp.v.size();i++)
        self[i] = tmp[i];
    dump(std::cout);
    
    if (needfix) fix();
}

vl_int vl_int::operator+(const vl_int& vl) const
{
    vl_int tmp;
    tmp.add(*this);
    tmp.add(vl);
    return tmp;
}

vl_int vl_int::operator-(const vl_int& vl) const
{
    vl_int tmp=*this, tmp2=vl;
    tmp2.negative();
    tmp.add(tmp2);
    return tmp;
}

int main()
{
    using namespace std;
    vl_int a, b;
    cin>>a>>b;
    //a.add(b);
    //a.dump(cout);
    cout<<a+b<<" "<<a-b<<endl;
    
    a.multiply(-11);
    //a.dump(cout);
    cout<<a<<" "<<vl_int(-233).tostring()<<endl;
    vl_int(-233).dump(cout);
    return 0;
}
