#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED

#include <vector>
#include <string>
#include <cstdint>

struct Point
{
  int16_t x;
  int16_t y;
};

struct Dimension
{
  uint16_t width;
  uint16_t height;
};

class Display
{
public:
  Display(const std::string& name);
  ~Display() = default;

  Dimension size() const;
  void draw(Point point, const uint16_t* pixels, uint16_t width);
  void present();

private:
  const std::string m_name;
  std::vector<uint16_t> m_frameBuffer;
};

#endif
