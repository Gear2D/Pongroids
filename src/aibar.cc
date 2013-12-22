/**
 * @file aibar.cc
 * @b family aibar
 * @b type aibar
 *
 * Controls the aibar behaviours */

#include "gear2d.h"
using namespace gear2d;

class aibar : public component::base {
  public:
    aibar();
    virtual component::family family() { return "bar"; }
    virtual component::type type() { return "aibar"; }
    virtual std::string depends() { return "collider/collider2d audiosource"; }
    virtual void handle(parameterbase::id pid, component::base * last, object::id pidowner);
    virtual void setup(object::signature & sig);
    virtual void update(float dt);

  private:
    component::base * pong;
};

aibar::aibar() : pong(0) {
}

void aibar::handle(parameterbase::id pid, component::base * last, object::id pidowner) {
  if (pid == "collider.collision") {
    component::base * c = read<component::base *>("collider.collision");
    if (c->raw<std::string>("collider.tag") == "pong") write("blip.playing", true);
  }
}

void aibar::setup(object::signature & sig) {
  hook("collider.collision");
}

void aibar::update(float dt) {
  if (pong == 0) {
    object::id o = locate("pong");
    if (o == 0) return;
    pong = o->component("pong");
    if (pong == 0) return;
  }

  const float & y = read<float>("y");
  const float & h = read<float>("h");
  const float & x = read<float>("x");
  const float & w = read<float>("w");
  const float & px = pong->read<float>("x");
  const float & py = pong->read<float>("y");

  if (pong->read<float>("y.speed") > read<float>("y.speed")) {
    write("controller.vertical", 1);
  } else if (pong->read<float>("y.speed") < read<float>("y.speed")) {
    write("controller.vertical", -1);
  }


  if (py > y && py < y + h) write("controller.vertical", 0);
  else if (py > y + h - 15) write("controller.vertical", 1);
  else if (py < y + 15) write("controller.vertical", -1);

  if (px >= 320 && pong->read<float>("x.speed") > -3.0f) {
    if (px < x) // if its on front
      write("controller.horizontal", -1);
    else if (px > x) {
      write("controller.horizontal", 1);
      if (py > y && py < y + h) write("controller.vertical", 1);
    }
  } else {
    if (x < 500) write("controller.horizontal", 1);
  }
}


g2dcomponent(aibar, bar, aibar)
