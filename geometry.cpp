#include <complex>

const double EPS = 1e-8;
const double INF = 1e+12;

//点は複素数で表す
typedef complex<double> P;

namespace std{
    //頂点同士の比較演算子をnamespace stdに追加
    //x軸の大小関係を優先する
    const bool operator < (const P &a, const P &b)const {
        return real(a) != real(b) ? real(a) < real (b) : imag(a) < imag(b);
    }
}

//2つのベクトルの外積axb
double cross(const P &a, const P &b){
    return imag(conj(a)*b);
}

//2つのベクトルの内積(a,b)
double dot(const P &a, const P &b){
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

P projection(const L &l, const P &p) {
  double t = dot(p-l[0], l[0]-l[1]) / norm(l[0]-l[1]);
  return l[0] + t*(l[0]-l[1]);
}

P reflection(const L &l, const P &p) {
  return p + 2 * (projection(l, p) - p);
}

double distanceLP(const L &l, const P &p) {
  return abs(p - projection(l, p));
}

double distanceLL(const L &l, const L &m) {
  return intersectLL(l, m) ? 0 : distanceLP(l, m[0]);
}

double distanceLS(const L &l, const L &s) {
  if (intersectLS(l, s)) return 0;
  return min(distanceLP(l, s[0]), distanceLP(l, s[1]));
}

double distanceSP(const L &s, const P &p) {
  const P r = projection(s, p);
  if (intersectSP(s, r)) return abs(r - p);
  return min(abs(s[0] - p), abs(s[1] - p));
}

double distanceSS(const L &s, const L &t) {
  if (intersectSS(s, t)) return 0;
  return min(min(distanceSP(s, t[0]), distanceSP(s, t[1])),
             min(distanceSP(t, s[0]), distanceSP(t, s[1])));
}

P crosspoint(const L &l, const L &m) {
  double A = cross(l[1] - l[0], m[1] - m[0]);
  double B = cross(l[1] - l[0], l[1] - m[0]);
  if (abs(A) < EPS && abs(B) < EPS) return m[0]; // same line
  if (abs(A) < EPS) assert(false); // !!!PRECONDITION NOT SATISFIED!!!
  return m[0] + B / A * (m[1] - m[0]);
}

//凸包を求める
//引数は3点以上含むことを仮定する
vector<P> convex_hull(vector<P> ps) {
    int n = ps.size(), k = 0;
    sort(ps.begin(), ps.end());
    vector<P> ch(2*n);
    for (int i = 0; i < n; ch[k++] = ps[i++]) // lower-hull
        while (k >= 2 && ccw(ch[k-2], ch[k-1], ps[i]) <= 0) --k;
    for (int i = n-2, t = k+1; i >= 0; ch[k++] = ps[i--]) // upper-hull
        while (k >= t && ccw(ch[k-2], ch[k-1], ps[i]) <= 0) --k;
    ch.resize(k-1);
    return ch;
}

//多角形の包含判定
#define curr(G, i) G[i]
#define next(G, i) G[(i+1)%G.size()]
enum { OUT, ON, IN };
int contains(const G &g, const P &p) {
    bool in = false;
    for (int i = 0; i < g.size(); ++i) {
        point a = curr(g,i) - p, b = next(g,i) - p;
        if (imag(a) > imag(b)) swap(a, b);
        if (imag(a) <= 0 && 0 < imag(b))
            if (cross(a, b) < 0) in = !in;
        if (cross(a, b) == 0 && dot(a, b) <= 0) return ON;
    }
    return in ? IN : OUT;
}
