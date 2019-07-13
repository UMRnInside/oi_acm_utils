#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <algorithm>
#include <string>
#include <vector>
#include <math.h>

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
        vl_int operator*(int) const;
        vl_int operator/(const vl_int&) const;
        vl_int operator/(int) const;
        
        vl_int operator>>(int) const;
        vl_int operator<<(int) const;
        void operator>>=(int);
        void operator<<=(int);
        
        void add(const vl_int&, bool needfix=true);
        void subtract(const vl_int&);
        void multiply(const vl_int&);
        void multiply(int n, bool needfix=true);
        void divide(const vl_int&);
        void divide(int n, bool needfix=true);
        void truncate(int);
        
        void remove_tail0(bool force=false);
        void fix();
        void negative();
        bool is_negative() const;
        void dump(std::ostream&) const;
        long long toll() const;
        std::string tostring() const;
    //private:
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

bool vl_int::is_negative() const
{
    //fix();
    for (auto i : v)
        if (i<0) return true;
    return false;
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

vl_int vl_int::operator<<(int n) const
{
    vl_int tmp = *this;
    for (int i=1;i<=n;i++)
        tmp.v.insert(tmp.v.begin(), 0);
    return tmp;
}

vl_int vl_int::operator>>(int n) const
{
    vl_int tmp = *this;
    std::vector<int>::iterator it;
    it = tmp.v.begin() + n;
    tmp.v.erase(tmp.v.begin(), it);
    return tmp;
}

void vl_int::operator<<=(int n)
{
    for (int i=1;i<=n;i++)
        this->v.insert(this->v.begin(), 0);
}

void vl_int::operator>>=(int n)
{
    std::vector<int>::iterator it;
    it = this->v.begin() + n;
    this->v.erase(this->v.begin(), it);
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

void vl_int::remove_tail0(bool force)
{
    std::reverse(v.begin(), v.end());
    
    std::vector<int>::iterator it;
    for (it=v.begin();it!=v.end();it++)
        if (*it != 0) break;
     
    v.erase(v.begin(), it);
    if (v.size() == 0 && !force) v.push_back(0);
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

void vl_int::add(const vl_int& vl, bool needfix)
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
    if (needfix) fix();
}

void vl_int::subtract(const vl_int& vl)
{
    vl_int nv = vl;
    nv.negative();
    //nv.dump(std::cout);
    add(nv);
    fix();
}

void vl_int::multiply(int n, bool needfix)
{
    //void multiply(int, bool needfix=true);
    vl_int& self = *this;
    vl_int tmp;
    
    int o_size = self.v.size();
    
    for (int i=0;i<o_size;i++)
    {
        tmp[i+1] = n*(self[i]) / _NUM;
        self[i] = n*(self[i]) % _NUM;
    }
    
    self.add(tmp, false);
    if (needfix) fix();
}

void vl_int::multiply(const vl_int& vl)
{
    int vlsize = vl.v.size();
    vl_int orig = *this;
    this->v.erase(this->v.begin(), this->v.end());
    
    for (int i=0;i<vlsize;i++)
    {
        vl_int faq = orig;
        //faq.multiply(vl.what(i));
        faq.multiply(vl.v[i]);
        faq<<=i;
        this->add(faq, i%17==0);
        //faq<<=1;
    }
    fix();
}

void vl_int::divide(int n, bool needfix)
{
    if (n == 0) return;
    size_t osize = v.size();
    for (int i=osize-1;i>=0;i--)
    {
        if (i-1 >= 0)
            v[i-1] += (v[i]%n)*_NUM;
        v[i] = v[i] / n;
    }
    if (needfix) fix();
}

void vl_int::truncate(int n)
{
    std::vector<int>::iterator it;
    if (v.size() < (unsigned)n) return;
    
    it = v.begin() + n;
    v.erase(it, v.end());
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

vl_int vl_int::operator*(int n) const
{
    vl_int tmp = *this;
    tmp.multiply(n);
    return tmp;
}

vl_int vl_int::operator*(const vl_int& vl) const
{
    vl_int tmp = *this;
    tmp.multiply(vl);
    return tmp;
}

vl_int vl_int::operator/(int n) const
{
    vl_int tmp = *this;
    tmp.divide(n);
    return tmp;
}

bool vl_int::operator<(const vl_int& vl) const
{
    vl_int tmp = *this;
    tmp.subtract(vl);
    return tmp.is_negative();
}

bool vl_int::operator>(const vl_int& vl) const
{
    vl_int tmp = *this;
    tmp.subtract(vl);
    tmp.remove_tail0(true);
    return (!tmp.is_negative()) && (tmp.v.size() > 0);
}

vl_int vl_fastpow(vl_int base, vl_int power, int sizelimit=0)
{
    vl_int result("1"), zero("0");
    for(;zero<power;power.divide(2))
    {
        if (power.v[0] & 1) result.multiply(base);
        vl_int tmp = base;
        base.multiply(tmp);
        if (sizelimit)
            result.truncate(sizelimit),base.truncate(sizelimit);
        //std::cout<<result<<" "<<base<<" "<<power<<" "<<(power>zero)<<std::endl;
        //char faq;
        //std::cin>>faq;
    }
    return result;
}

vl_int vl_fastpow(vl_int base, int power, int sizelimit=0)
{
    vl_int result("1"), zero("0");
    for(;power;power>>=1)
    {
        if (power & 1) result.multiply(base);
        vl_int tmp = base;
        base.multiply(tmp);
        if (sizelimit)
            result.truncate(sizelimit),base.truncate(sizelimit);
        //std::cout<<result<<" "<<base<<" "<<power<<" "<<(power>0)<<std::endl;
        //char faq;
        //std::cin>>faq;
    }
    return result;
}

int main()
{
    using namespace std;
    vl_int a, b;
    cin>>a>>b;
    //a.add(b);
    //a.dump(cout);
    cout<<a+b<<" "<<a-b<<endl;
    cout<<a*b<<endl;
    a.divide(2);
    cout<<a<<endl;
    //cout<<vl_fastpow(a, b, 0)<<endl;
    
    //cout<<a<<" "<<vl_int(-233).tostring()<<endl;
    //vl_int(-233).dump(cout);
    return 0;
}
