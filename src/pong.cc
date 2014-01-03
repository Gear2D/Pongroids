#include "gear2d.h"

#include <iostream>
using namespace std;
using namespace gear2d;


class pong : public component::base {
  private:
    link<int> rendererw;
    link<int> rendererh;
  public:
    virtual component::family family() { return "pong"; }
    virtual component::type type() { return "pong"; }
    virtual string depends() { return "renderer/renderer2 spatial/space2d kinematics/kinematic2d dynamics/rigidbody2d"; }
    virtual void setup(object::signature & sig) {
      hook("collider.collision");
      rendererw = fetch<int>("renderer.w");
      rendererh = fetch<int>("renderer.h");
    }

    virtual void handle(parameterbase::id pid, component::base * lastwrite, object::id owns) {
      if (pid == "collider.collision") {
        component::base * c = read<component::base *>(pid);
        if (c->read<string>("collider.tag") == "bar") {
          add("x.speed", 2.0f);
          add("y.speed", read<float>("collider.collision.speed.y") * 0.5f);
        }
      }
    }

    virtual void update(float dt) {
      write("grid.x", raw<float>("x") / 10.0f - rendererw);
      write("grid.y", raw<float>("y") / 10.0f - rendererh);
      add("x.speed",  raw<float>("x.speed") < 0 ? -dt : dt);
    }

};

g2dcomponent(pong, pong, pong);
