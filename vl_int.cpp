#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <algorithm>
#include <string>
#include <vector>
#include <math.h>

#define _NUM 10
#define _STORAGE 100000000 // 10^8
#define _STORED_DIGITS 8
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
        long long what(int) const;
        long long& operator[](int);
        
        bool operator<(const vl_int&) const;
        bool operator>(const vl_int&) const;
        bool operator==(const vl_int&) const;
        vl_int operator+(const vl_int&) const;
        vl_int operator-(const vl_int&) const;
        vl_int operator*(const vl_int&) const;
        vl_int operator*(int) const;
        vl_int operator/(const vl_int&) const;
        vl_int operator/(int) const;
        vl_int operator%(const vl_int&) const;
        
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
        void mod(const vl_int&);
        void truncate(int);
        
        void remove_tail0(bool force=false);
        void fix();
        void negative();
        bool is_negative() const;
        void dump(std::ostream&) const;
        long long toll() const;
        std::string tostring() const;
    //private:
        std::vector<long long> v;
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
    for (;n!=0;n/=_STORAGE)
        v.push_back(is_negative ? n%_STORAGE : n%_STORAGE);
}

vl_int::vl_int(std::string ts)
{
    bool is_negative = (ts[0] == '-');
    //if (is_negative) std::cout<<"-------\n";
    if (is_negative) ts = ts.substr(1);
    if (ts.length() % _STORED_DIGITS > 0)
    {
        size_t padding = _STORED_DIGITS - (ts.length() % _STORED_DIGITS);
        for (size_t i=0;i<padding;i++)
            ts = "0" + ts;
    }
    
    v.erase(v.begin(), v.end());
    
    for (size_t i=0;i<ts.length();i+=_STORED_DIGITS)
        v.push_back( stoll( ts.substr(i, _STORED_DIGITS) ) );
    
    if (is_negative)
        for (unsigned int i=0;i<v.size();i++) v[i] = -v[i];
    std::reverse(v.begin(), v.end());
}

std::string vl_int::tostring() const
{
    std::string ts, tr;
    bool neg = false;
    
    for (auto i=v.rbegin();i!=v.rend();i++)
    {
        // NOTE: "00000000000"...
        if (i != v.rbegin() )
        {
            tr = "00000000" + std::to_string(*i);
            tr = tr.substr( tr.length() - _STORED_DIGITS );
        }
        else
            tr = std::to_string(*i);
        ts += tr;
        if (*i < 0) neg = true;
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
    //std::vector<int>::iterator it;
    auto it = tmp.v.begin() + n;
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
    auto it = this->v.begin() + n;
    this->v.erase(this->v.begin(), it);
}

long long vl_int::what(int i) const
{
    if ((unsigned)i >= v.size() )
    {
        return 0;
    }
    return v[i];
}

long long& vl_int::operator[](int i)
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

    auto it = v.begin();
    for (it=v.begin();it!=v.end();it++)
        if (*it != 0) break;
     
    v.erase(v.begin(), it);
    if (v.size() == 0 && !force) v.push_back(0);
    std::reverse(v.begin(), v.end());
}

void vl_int::negative()
{
    for (auto it=v.begin();it!=v.end();it++)
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
        for (int d=0;d<_STORAGE;d++)
        {
            if (v[i] + d*_STORAGE > 0)
            {
                v[i] += d*_STORAGE;
                v[i+1] -= d;
                break;
            }
        }
    }
    // round 2
    for (unsigned int i=0;i<v.size();i++)
    {
        //std::cout<<i<<":"<<v[i]<<"\n";
        if (v[i] >= _STORAGE)
        {
            int d = 0;
            while (v[i] - d*_STORAGE>= _STORAGE) d++;
            v[i] -= d*_STORAGE;
            
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
        v[i] = sum % _STORAGE;
        
        if (sum/_STORAGE)
        {
            (*this)[i+1];
            v[i+1] += sum/_STORAGE;
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
        tmp[i+1] = n*(self[i]) / _STORAGE;
        self[i] = n*(self[i]) % _STORAGE;
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
        this->add(faq);
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
        {
            std::cout << "Passing " << v[i]%n * _STORAGE << " from " << i << " to " <<i-1 << std::endl;
            v[i-1] += (v[i]%n)*_STORAGE;
        }
        v[i] = v[i] / n;
        dump(std::cout<<"i= "<<i << " ");
    }
    if (needfix) fix();
}

void vl_int::divide(const vl_int& vl)
{
    vl_int result(0);
    bool result_is_negative = is_negative() ^ vl.is_negative();
    if (is_negative()) negative();
    
    // Not to touch vl
    vl_int basenum = vl;
    if (basenum.is_negative()) basenum.negative();
    
    if ((*this) < basenum)
    {
        // Must be 0
        this->v.erase(this->v.begin(), this->v.end());
        this->v.push_back(0);
        return;
    }
    
    int offset = v.size() - vl.v.size();
    basenum <<= offset;
    for (int i=offset;i>=0;i--)
    {
        result<<=1;
        if ( (*this) < 0 || (*this) == 0) continue;
        // Binary search!
        int l=1, r=_STORAGE;
        int ans = -1;

        while (l < r)
        {
            //std::cout << "l, r = " << l << ", " << r << std::endl;
            if (r - l <= 5)
            {
                ans = r;
                for (int j=r;j>=l;j--)
                {
                    if ( (*this) == basenum*j || (*this) > basenum * j)
                    {
                        ans = j;
                        break;
                    }
                }
                break;
            }

            int mid = (l+r)/2;
            vl_int midvl = basenum * mid;
            //std::cout<<(*this)<<"<--->"<<midvl<<std::endl;

            if (midvl < (*this)) // too small
                l = mid;
            else if (midvl > (*this)) // to large
                r = mid;
            else // break
                ans = mid, l=mid, r=mid;
        }
        result.add(ans);
        this->subtract(basenum*ans);
        basenum>>=1;
    }
    
    if (result_is_negative) result.negative();
    result.fix();
    *this = result;
}

void vl_int::mod(const vl_int& vl)
{
    bool result_is_negative = vl.is_negative();
    if (is_negative()) negative();
    vl_int tmp = (*this);

    this->subtract( (tmp/vl)*vl );
    if (result_is_negative) negative();
}

void vl_int::truncate(int n)
{
    if (v.size() < (unsigned)n) return;
    
    auto it = v.begin() + n;
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

vl_int vl_int::operator/(const vl_int& vl) const
{
    vl_int tmp = *this;
    tmp.divide(vl);
    return tmp;
}

vl_int vl_int::operator%(const vl_int& vl) const
{
    vl_int tmp = (*this);
    tmp.mod(vl);
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

bool vl_int::operator==(const vl_int& vl) const
{
    if (v.size() != vl.v.size()) return false;
    int xsize = v.size();
    for (int i=0;i<xsize;i++)
        if (v[i] != vl.v[i])
            return false;
    return true;
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
    cout<<a*b<<" "<<a/b<<endl;
    cout<<a%b<<endl;
    //cout<<vl_fastpow(a, b, 0)<<endl;
    return 0;
}
