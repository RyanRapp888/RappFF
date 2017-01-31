#ifndef UTIL_RGB_H
#define UTIL_RGB_H

#define RGB_GREEN_BITS  8
#define RGB_BLUE_BITS   16
#define RGB_MASK        255

#ifdef RGB
#undef RGB
#endif

class RGB
{
   public:

   RGB() : m_Value(0) {}
   RGB(int r, int g, int b)  { SetRGB(r, g, b); }
   RGB(const RGB& colour) : m_Value(colour.m_Value) {}

   const RGB& operator=(const RGB& colour)
   {
     m_Value = colour.m_Value;
     return *this;
   }
   
   
   int GetRed()   const { return m_Value & RGB_MASK; }
   int GetGreen() const { return (m_Value >> RGB_GREEN_BITS) & RGB_MASK; }
   int GetBlue()  const { return (m_Value >> RGB_BLUE_BITS) & RGB_MASK; }

   void SetRed(int r)   { SetRGB(r, GetGreen(), GetBlue()); }
   void SetGreen(int g) { SetRGB(GetRed(), g, GetBlue()); }
   void SetBlue(int b)  { SetRGB(GetRed(), GetGreen(), b); }
   void SetRGB(int r, int g, int b)
   {
      m_Value = (r | (g << RGB_GREEN_BITS) | (b << RGB_BLUE_BITS));
   }

   //val should be an integer (0,255)
   void IncrRed(int val)
   {
      SetRed((GetRed() + val) % 256);
   }

   void IncrGreen(int val)
   {
      SetGreen((GetGreen() + val) % 256);
   }
   
   void IncrBlue(int val)
   {
      SetBlue((GetBlue() + val) % 256);
   }

   void Incr(int val)
   {
      m_Value += val;
      if(m_Value > 16777216) m_Value = (m_Value % 16777216);
   }

   void Print()
   {
      std::cout << "(" << GetRed() << "," << GetGreen() << ","
                << GetBlue() << ")" << std::endl;
   }

   private:
   int m_Value;  // Packed red, green and blue components
};

#endif
