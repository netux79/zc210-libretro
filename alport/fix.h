#ifndef ALPORT_FIX_H
#define ALPORT_FIX_H

#ifdef __cplusplus

typedef int32_t fixed;

class fix      /* C++ wrapper for the fixed point routines */
{
public:
   fixed v;

   fix();
   fix(const fix &x);
   explicit fix(const int x);
   explicit fix(const long x);
   explicit fix(const unsigned int x);
   explicit fix(const unsigned long x);
   explicit fix(const float x);
   explicit fix(const double x);

   operator int() const;
   operator long() const;
   operator unsigned int() const;
   operator unsigned long() const;
   operator float() const;
   operator double() const;

   fix &operator = (const fix &x);
   fix &operator = (const int x);
   fix &operator = (const long x);
   fix &operator = (const unsigned int x);
   fix &operator = (const unsigned long x);
   fix &operator = (const float x);
   fix &operator = (const double x);

   fix &operator += (const fix x);
   fix &operator += (const int x);
   fix &operator += (const long x);
   fix &operator += (const float x);
   fix &operator += (const double x);

   fix &operator -= (const fix x);
   fix &operator -= (const int x);
   fix &operator -= (const long x);
   fix &operator -= (const float x);
   fix &operator -= (const double x);

   fix &operator *= (const fix x);
   fix &operator *= (const int x);
   fix &operator *= (const long x);
   fix &operator *= (const float x);
   fix &operator *= (const double x);

   fix &operator /= (const fix x);
   fix &operator /= (const int x);
   fix &operator /= (const long x);
   fix &operator /= (const float x);
   fix &operator /= (const double x);

   fix &operator <<= (const int x);
   fix &operator >>= (const int x);

   fix &operator ++ ();
   fix &operator -- ();

   fix operator ++ (int);
   fix operator -- (int);

   fix operator - () const;

   inline friend fix operator + (const fix x, const fix y);
   inline friend fix operator + (const fix x, const int y);
   inline friend fix operator + (const int x, const fix y);
   inline friend fix operator + (const fix x, const long y);
   inline friend fix operator + (const long x, const fix y);
   inline friend fix operator + (const fix x, const float y);
   inline friend fix operator + (const float x, const fix y);
   inline friend fix operator + (const fix x, const double y);
   inline friend fix operator + (const double x, const fix y);

   inline friend fix operator - (const fix x, const fix y);
   inline friend fix operator - (const fix x, const int y);
   inline friend fix operator - (const int x, const fix y);
   inline friend fix operator - (const fix x, const long y);
   inline friend fix operator - (const long x, const fix y);
   inline friend fix operator - (const fix x, const float y);
   inline friend fix operator - (const float x, const fix y);
   inline friend fix operator - (const fix x, const double y);
   inline friend fix operator - (const double x, const fix y);

   inline friend fix operator * (const fix x, const fix y);
   inline friend fix operator * (const fix x, const int y);
   inline friend fix operator * (const int x, const fix y);
   inline friend fix operator * (const fix x, const long y);
   inline friend fix operator * (const long x, const fix y);
   inline friend fix operator * (const fix x, const float y);
   inline friend fix operator * (const float x, const fix y);
   inline friend fix operator * (const fix x, const double y);
   inline friend fix operator * (const double x, const fix y);

   inline friend fix operator / (const fix x, const fix y);
   inline friend fix operator / (const fix x, const int y);
   inline friend fix operator / (const int x, const fix y);
   inline friend fix operator / (const fix x, const long y);
   inline friend fix operator / (const long x, const fix y);
   inline friend fix operator / (const fix x, const float y);
   inline friend fix operator / (const float x, const fix y);
   inline friend fix operator / (const fix x, const double y);
   inline friend fix operator / (const double x, const fix y);

   inline friend fix operator << (const fix x, const int y);
   inline friend fix operator >> (const fix x, const int y);

   inline friend int operator == (const fix x, const fix y);
   inline friend int operator == (const fix x, const int y);
   inline friend int operator == (const int x, const fix y);
   inline friend int operator == (const fix x, const long y);
   inline friend int operator == (const long x, const fix y);
   inline friend int operator == (const fix x, const float y);
   inline friend int operator == (const float x, const fix y);
   inline friend int operator == (const fix x, const double y);
   inline friend int operator == (const double x, const fix y);

   inline friend int operator != (const fix x, const fix y);
   inline friend int operator != (const fix x, const int y);
   inline friend int operator != (const int x, const fix y);
   inline friend int operator != (const fix x, const long y);
   inline friend int operator != (const long x, const fix y);
   inline friend int operator != (const fix x, const float y);
   inline friend int operator != (const float x, const fix y);
   inline friend int operator != (const fix x, const double y);
   inline friend int operator != (const double x, const fix y);

   inline friend int operator < (const fix x, const fix y);
   inline friend int operator < (const fix x, const int y);
   inline friend int operator < (const int x, const fix y);
   inline friend int operator < (const fix x, const long y);
   inline friend int operator < (const long x, const fix y);
   inline friend int operator < (const fix x, const float y);
   inline friend int operator < (const float x, const fix y);
   inline friend int operator < (const fix x, const double y);
   inline friend int operator < (const double x, const fix y);

   inline friend int operator > (const fix x, const fix y);
   inline friend int operator > (const fix x, const int y);
   inline friend int operator > (const int x, const fix y);
   inline friend int operator > (const fix x, const long y);
   inline friend int operator > (const long x, const fix y);
   inline friend int operator > (const fix x, const float y);
   inline friend int operator > (const float x, const fix y);
   inline friend int operator > (const fix x, const double y);
   inline friend int operator > (const double x, const fix y);

   inline friend int operator <= (const fix x, const fix y);
   inline friend int operator <= (const fix x, const int y);
   inline friend int operator <= (const int x, const fix y);
   inline friend int operator <= (const fix x, const long y);
   inline friend int operator <= (const long x, const fix y);
   inline friend int operator <= (const fix x, const float y);
   inline friend int operator <= (const float x, const fix y);
   inline friend int operator <= (const fix x, const double y);
   inline friend int operator <= (const double x, const fix y);

   inline friend int operator >= (const fix x, const fix y);
   inline friend int operator >= (const fix x, const int y);
   inline friend int operator >= (const int x, const fix y);
   inline friend int operator >= (const fix x, const long y);
   inline friend int operator >= (const long x, const fix y);
   inline friend int operator >= (const fix x, const float y);
   inline friend int operator >= (const float x, const fix y);
   inline friend int operator >= (const fix x, const double y);
   inline friend int operator >= (const double x, const fix y);
};

#endif          /* ifdef __cplusplus */

inline int fixfloor(fixed x)
{
   /* (x >> 16) is not portable */
   if (x >= 0)
      return (x >> 16);
   else
      return ~((~x) >> 16);
}

inline fixed ftofix(double x)
{
   if (x > 32767.0)
      return 0x7FFFFFFF;

   if (x < -32767.0)
      return -0x7FFFFFFF;

   return (fixed)(x * 65536.0 + (x < 0 ? -0.5 : 0.5));
}

inline double fixtof(fixed x)
{
   return (double)x / 65536.0;
}

inline fixed fixmul(fixed x, fixed y)
{
   long long lx = x;
   long long ly = y;
   long long lres = (lx * ly);

   if (lres > 0x7FFFFFFF0000LL)
      return 0x7FFFFFFF;
   else if (lres < -0x7FFFFFFF0000LL)
      return 0x80000000;
   else
   {
      int res = lres >> 16;
      return res;
   }
}

inline fixed fixdiv(fixed x, fixed y)
{
   if (y == 0)
      return (x < 0) ? -0x7FFFFFFF : 0x7FFFFFFF;
   else
      return ftofix(fixtof(x) / fixtof(y));
}

inline fixed itofix(int x)
{
   return x << 16;
}

inline int fixtoi(fixed x)
{
   return fixfloor(x) + ((x & 0x8000) >> 15);
}

/* sepapator */

inline fix operator + (const fix x, const fix y)
{
   fix t;
   t.v = x.v + y.v;
   return t;
}

inline fix operator + (const fix x, const int y)
{
   fix t;
   t.v = x.v + itofix(y);
   return t;
}

inline fix operator + (const int x, const fix y)
{
   fix t;
   t.v = itofix(x) + y.v;
   return t;
}

inline fix operator + (const fix x, const long y)
{
   fix t;
   t.v = x.v + itofix(y);
   return t;
}

inline fix operator + (const long x, const fix y)
{
   fix t;
   t.v = itofix(x) + y.v;
   return t;
}

inline fix operator + (const fix x, const float y)
{
   fix t;
   t.v = x.v + ftofix(y);
   return t;
}

inline fix operator + (const float x, const fix y)
{
   fix t;
   t.v = ftofix(x) + y.v;
   return t;
}

inline fix operator + (const fix x, const double y)
{
   fix t;
   t.v = x.v + ftofix(y);
   return t;
}

inline fix operator + (const double x, const fix y)
{
   fix t;
   t.v = ftofix(x) + y.v;
   return t;
}

inline fix operator - (const fix x, const fix y)
{
   fix t;
   t.v = x.v - y.v;
   return t;
}

inline fix operator - (const fix x, const int y)
{
   fix t;
   t.v = x.v - itofix(y);
   return t;
}

inline fix operator - (const int x, const fix y)
{
   fix t;
   t.v = itofix(x) - y.v;
   return t;
}

inline fix operator - (const fix x, const long y)
{
   fix t;
   t.v = x.v - itofix(y);
   return t;
}

inline fix operator - (const long x, const fix y)
{
   fix t;
   t.v = itofix(x) - y.v;
   return t;
}

inline fix operator - (const fix x, const float y)
{
   fix t;
   t.v = x.v - ftofix(y);
   return t;
}

inline fix operator - (const float x, const fix y)
{
   fix t;
   t.v = ftofix(x) - y.v;
   return t;
}

inline fix operator - (const fix x, const double y)
{
   fix t;
   t.v = x.v - ftofix(y);
   return t;
}

inline fix operator - (const double x, const fix y)
{
   fix t;
   t.v = ftofix(x) - y.v;
   return t;
}

inline fix operator * (const fix x, const fix y)
{
   fix t;
   t.v = fixmul(x.v, y.v);
   return t;
}

inline fix operator * (const fix x, const int y)
{
   fix t;
   t.v = x.v * y;
   return t;
}

inline fix operator * (const int x, const fix y)
{
   fix t;
   t.v = x * y.v;
   return t;
}

inline fix operator * (const fix x, const long y)
{
   fix t;
   t.v = x.v * y;
   return t;
}

inline fix operator * (const long x, const fix y)
{
   fix t;
   t.v = x * y.v;
   return t;
}

inline fix operator * (const fix x, const float y)
{
   fix t;
   t.v = ftofix(fixtof(x.v) * y);
   return t;
}

inline fix operator * (const float x, const fix y)
{
   fix t;
   t.v = ftofix(x * fixtof(y.v));
   return t;
}
inline fix operator * (const fix x, const double y)
{
   fix t;
   t.v = ftofix(fixtof(x.v) * y);
   return t;
}

inline fix operator * (const double x, const fix y)
{
   fix t;
   t.v = ftofix(x * fixtof(y.v));
   return t;
}

inline fix operator / (const fix x, const fix y)
{
   fix t;
   t.v = fixdiv(x.v, y.v);
   return t;
}

inline fix operator / (const fix x, const int y)
{
   fix t;
   t.v = x.v / y;
   return t;
}
inline fix operator / (const int x, const fix y)
{
   fix t;
   t.v = fixdiv(itofix(x), y.v);
   return t;
}

inline fix operator / (const fix x, const long y)
{
   fix t;
   t.v = x.v / y;
   return t;
}

inline fix operator / (const long x, const fix y)
{
   fix t;
   t.v = fixdiv(itofix(x), y.v);
   return t;
}

inline fix operator / (const fix x, const float y)
{
   fix t;
   t.v = ftofix(fixtof(x.v) / y);
   return t;
}

inline fix operator / (const float x, const fix y)
{
   fix t;
   t.v = ftofix(x / fixtof(y.v));
   return t;
}

inline fix operator / (const fix x, const double y)
{
   fix t;
   t.v = ftofix(fixtof(x.v) / y);
   return t;
}

inline fix operator / (const double x, const fix y)
{
   fix t;
   t.v = ftofix(x / fixtof(y.v));
   return t;
}

inline fix operator << (const fix x, const int y)
{
   fix t;
   t.v = x.v << y;
   return t;
}

inline fix operator >> (const fix x, const int y)
{
   fix t;
   t.v = x.v >> y;
   return t;
}

inline int operator == (const fix x, const fix y)
{
   return (x.v == y.v);
}

inline int operator == (const fix x, const int y)
{
   return (x.v == itofix(y));
}

inline int operator == (const int x, const fix y)
{
   return (itofix(x) == y.v);
}

inline int operator == (const fix x, const long y)
{
   return (x.v == itofix(y));
}

inline int operator == (const long x, const fix y)
{
   return (itofix(x) == y.v);
}

inline int operator == (const fix x, const float y)
{
   return (x.v == ftofix(y));
}

inline int operator == (const float x, const fix y)
{
   return (ftofix(x) == y.v);
}

inline int operator == (const fix x, const double y)
{
   return (x.v == ftofix(y));
}

inline int operator == (const double x, const fix y)
{
   return (ftofix(x) == y.v);
}

inline int operator != (const fix x, const fix y)
{
   return (x.v != y.v);
}

inline int operator != (const fix x, const int y)
{
   return (x.v != itofix(y));
}
inline int operator != (const int x, const fix y)
{
   return (itofix(x) != y.v);
}

inline int operator != (const fix x, const long y)
{
   return (x.v != itofix(y));
}

inline int operator != (const long x, const fix y)
{
   return (itofix(x) != y.v);
}

inline int operator != (const fix x, const float y)
{
   return (x.v != ftofix(y));
}

inline int operator != (const float x, const fix y)
{
   return (ftofix(x) != y.v);
}

inline int operator != (const fix x, const double y)
{
   return (x.v != ftofix(y));
}

inline int operator != (const double x, const fix y)
{
   return (ftofix(x) != y.v);
}

inline int operator < (const fix x, const fix y)
{
   return (x.v < y.v);
}

inline int operator < (const fix x, const int y)
{
   return (x.v < itofix(y));
}

inline int operator < (const int x, const fix y)
{
   return (itofix(x) < y.v);
}

inline int operator < (const fix x, const long y)
{
   return (x.v < itofix(y));
}

inline int operator < (const long x, const fix y)
{
   return (itofix(x) < y.v);
}

inline int operator < (const fix x, const float y)
{
   return (x.v < ftofix(y));
}

inline int operator < (const float x, const fix y)
{
   return (ftofix(x) < y.v);
}

inline int operator < (const fix x, const double y)
{
   return (x.v < ftofix(y));
}

inline int operator < (const double x, const fix y)
{
   return (ftofix(x) < y.v);
}

inline int operator > (const fix x, const fix y)
{
   return (x.v > y.v);
}

inline int operator > (const fix x, const int y)
{
   return (x.v > itofix(y));
}

inline int operator > (const int x, const fix y)
{
   return (itofix(x) > y.v);
}

inline int operator > (const fix x, const long y)
{
   return (x.v > itofix(y));
}

inline int operator > (const long x, const fix y)
{
   return (itofix(x) > y.v);
}

inline int operator > (const fix x, const float y)
{
   return (x.v > ftofix(y));
}

inline int operator > (const float x, const fix y)
{
   return (ftofix(x) > y.v);
}

inline int operator > (const fix x, const double y)
{
   return (x.v > ftofix(y));
}

inline int operator > (const double x, const fix y)
{
   return (ftofix(x) > y.v);
}

inline int operator <= (const fix x, const fix y)
{
   return (x.v <= y.v);
}

inline int operator <= (const fix x, const int y)
{
   return (x.v <= itofix(y));
}

inline int operator <= (const int x, const fix y)
{
   return (itofix(x) <= y.v);
}

inline int operator <= (const fix x, const long y)
{
   return (x.v <= itofix(y));
}

inline int operator <= (const long x, const fix y)
{
   return (itofix(x) <= y.v);
}

inline int operator <= (const fix x, const float y)
{
   return (x.v <= ftofix(y));
}

inline int operator <= (const float x, const fix y)
{
   return (ftofix(x) <= y.v);
}

inline int operator <= (const fix x, const double y)
{
   return (x.v <= ftofix(y));
}

inline int operator <= (const double x, const fix y)
{
   return (ftofix(x) <= y.v);
}

inline int operator >= (const fix x, const fix y)
{
   return (x.v >= y.v);
}

inline int operator >= (const fix x, const int y)
{
   return (x.v >= itofix(y));
}

inline int operator >= (const int x, const fix y)
{
   return (itofix(x) >= y.v);
}

inline int operator >= (const fix x, const long y)
{
   return (x.v >= itofix(y));
}

inline int operator >= (const long x, const fix y)
{
   return (itofix(x) >= y.v);
}

inline int operator >= (const fix x, const float y)
{
   return (x.v >= ftofix(y));
}

inline int operator >= (const float x, const fix y)
{
   return (ftofix(x) >= y.v);
}

inline int operator >= (const fix x, const double y)
{
   return (x.v >= ftofix(y));
}

inline int operator >= (const double x, const fix y)
{
   return (ftofix(x) >= y.v);
}

#endif          /* ifndef ALPORT_FIX_H */
