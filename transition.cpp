#include "gif.h"
#include <cmath>
using namespace std;

const int width = 16;
const int height = 16;
uint8_t image[width*height*4];

void SetPixel(int xx, int yy, uint8_t red, uint8_t grn, uint8_t blu)
{
  uint8_t* pixel = &image[(yy*width+xx)*4];
  pixel[0] = red;
  pixel[1] = blu;
  pixel[2] = grn;
  pixel[3] = 0; // 255 = opaque, 0 = transparent
}

void ClearImage()
{
  for (int i = 0; i<width; ++i) {
    for (int j = 0; j<height; ++j) {
      //SetPixel(i,j,0,0,0); // all black
      SetPixel(i,j,255,255,255); // all white
    }
  }
}

int main()
{
  // starting point
  const int np = 6;
  double y[np] = { 7, 8, 7, 8, 7, 8 };
  double x[np] = { 0, 0, 1, 1, 2, 2 };
  // center of rotation
  double x0 = 7.5, y0 = 7.5;
  // convert x,y -> r,phi
  double r[np], f[np];
  for (int i = 0; i<np; ++i) {
    r[i] = sqrt(pow(x[i]-x0,2) + pow(y[i]-y0,2));
    f[i] = atan2(y[i]-y0, x[i]-x0);
  }
  int delay = 20;
  GifWriter g;
  GifBegin(&g, "test.gif", width, height, delay);
  // number of steps
  const int ns = 6;
  // number of rays
  const int nr = 8;
  for (int is = 0; is<ns; ++is) {
    ClearImage();
    for (int ir = 0; ir<nr; ++ir) {
      double df = 2*M_PI*(double(is)/ns + double(ir)/nr);
      for (int i = 0; i<np; ++i) {
	int ix = int(x0+r[i]*cos(f[i]+df)+0.5);
	int iy = int(y0+r[i]*sin(f[i]+df)+0.5);
	if (ix<0) ix = 0;
	if (ix>=width) ix = width-1;
	if (iy<0) iy = 0;
	if (iy>=height) iy = height-1;
	SetPixel(ix,iy,255,0,0);
      }
    }
    GifWriteFrame(&g, image, width, height, delay);
  }
  GifEnd(&g);
  return 0;
}
