#include <complex>

const double EPS = 1e-8;
const double INF = 1e+12;

//点は複素数で表す
typedef complex<double> P;

namespace std{
    //頂点同士の比較演算子をnamespace stdに追加
    //x軸の大小関係を優先する
    bool operator < (const P &a, const P &b){
        return real(a) != real(b) ? real(a) < real (b) : imag(a) < imag(b);
    }
}

//2つのベクトルの外積axb
double cross(const P &a, const P &b){
    return iamg(conj(a)*b);
}

//2つのベクトルの内積(a,b)
double dor(const P &a, const P &b){
    return real(conj(a) * b);
}


//線分は点のvectorで表す
struct L : public vector<P> {
    L(const P &a, const P &b){
        push_back(a);
        push_back(b);
    }
};

//多角形G
typedef vector<P> G;

//円Cは(中心、半径)
struct C{
    P p;
    double r;
    C(const P &p, double r) : p(p), r(r) { }
};

//ccw
int ccw(P a, P b, P c) {
    b -= a; c -= a;
    if (cross(b, c) > 0)   return +1;       // counter clockwise
    if (cross(b, c) < 0)   return -1;       // clockwise
    if (dot(b, c) < 0)     return +2;       // c--a--b on line
    if (norm(b) < norm(c)) return -2;       // a--b--c on line
    return 0;                               // b==c || a--c--b on line
}

//is intersected including on a point, the same line

//line and line
bool intersectLL(const L &l, const L &m) {
    return abs(cross(l[1]-l[0], m[1]-m[0])) > EPS || // non-parallel
        abs(cross(l[1]-l[0], m[0]-l[0])) < EPS;   // same line
}

//line and segment
bool intersectLS(const L &l, const L &s) {
    return cross(l[1]-l[0], s[0]-l[0])*       // s[0] is left of l
        cross(l[1]-l[0], s[1]-l[0]) < EPS; // s[1] is right of l
}

//line and point
bool intersectLP(const L &l, const P &p) {
    return abs(cross(l[1]-p, l[0]-p)) < EPS;
}

//segment and segment
bool intersectSS(const L &s, const L &t) {
    return ccw(s[0],s[1],t[0])*ccw(s[0],s[1],t[1]) <= 0 &&
        ccw(t[0],t[1],s[0])*ccw(t[0],t[1],s[1]) <= 0;
}

//segment and point
bool intersectSP(const L &s, const P &p) {
    return abs(s[0]-p)+abs(s[1]-p)-abs(s[1]-s[0]) < EPS; // triangle inequality
}
