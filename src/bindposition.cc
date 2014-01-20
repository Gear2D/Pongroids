#include "gear2d.h"

using namespace gear2d;

class bound : public component::base {
  public:
    virtual void setup(object::signature & sig);
    virtual void update(timediff dt);
    virtual component::family family() { return "controller"; }
    virtual component::type type() { return "bindposition"; }
    virtual std::string depends() { return "spatial/space2d renderer/renderer2"; }
    
  private:
    gear2d::link<float> xmin, xmax, ymin, ymax, x, y;
};

void bound::setup(object::signature & sig) {
  sigparser sp(sig, this);
  xmin = sp.init("bound.x.min", 0.0f);
  ymin = sp.init("bound.y.min", 0.0f);
  xmax = sp.init("bound.x.max", (float)read<int>("renderer.w"));
  ymax = sp.init("bound.y.max", (float)read<int>("renderer.h"));
  x = fetch<float>("x");
  y = fetch<float>("y");
}

void bound::update(timediff dt) {
  if (x > xmax || x < xmin) { x = xmax/2.0f; }
  if (y > ymax || y < ymin) { y = ymax/2.0f; }
}

g2dcomponent(bound, controller, bindposition);

