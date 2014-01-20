#include "gear2d.h"
#include <cstdlib>
#include <cmath>

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

using namespace gear2d;

/**
 * @file asteroid.cc
 * Component to handle asteroids. Asteroids breaks when kicked
 * and spawns other asteroids
 *
 * @b parameters
 * @li @c asteroid.type Size of this asteroid (big, medium, small). @b string
 *  When hit, one big asteroid will split into medium. One medium into
 *  small, and small will be destroyed.
 * @li @c asteroid.fragmentation How many smaller asteroids this asteroid should be split @b int
 */

enum asteroidtype {
  big,
  medium,
  small,
  none
};

namespace gear2d {
  template<> asteroidtype eval<asteroidtype>(const std::string & t, const asteroidtype & def) {
    if (t == "big") return big;
    if (t == "medium") return medium;
    if (t == "small") return small;
    return def;
  }
}

class asteroid : public component::base {
  private:
    asteroidtype t;
    int frags;
    bool spawning;
    float timeleft;

    static int asteroidno;
    static int usedasteroids;
    
    
    // keep a list of available asteroids
    static std::set<gear2d::object::id> free8;
    static std::set<gear2d::object::id> free16;
    
    gear2d::link<float> x, y, w, h;
    gear2d::link<bool> active;

  private:
    void explode() {
      modinfo("asteroid");
      trace("Asteroid size", (t-3) * -1, "exploding!");
      std::string target;
      if (t == big) target = "asteroid16";
      else if (t == medium) target = "asteroid8";
      else {
        reset();
        return;
      }

      const int & side = raw<int>("collider.collision.side");
      const float & colspeedx = raw<float>("collider.collision.speed.x");
      const float & colspeedy = raw<float>("collider.collision.speed.y");

      object::id obj;
      asteroid * com;
      const float & xspeed = raw<float>("x.speed");
      const float & yspeed = raw<float>("y.speed");
      float increment = 60.0f / frags;

      for (int i = 0; i < frags; i++) {
        // set other asteroids rolling
        if (t == big) {
          if (free16.empty()) {
            trace("Big asteroid need to spawn medium ones. Why?");
            spawn(target);
          }
          
          trace("Got an free asteroid! Yay!");
          obj = *free16.begin();
          free16.erase(free16.begin());
        } else {
          if (free8.empty()) {
            trace("Medium asteroid need to spawn small ones. Why?");
            obj = spawn(target);
          } 
          obj = *free8.begin();
          free8.erase(free8.begin());
        }
        
        com = (asteroid *) obj->component("asteroid");
        if (com == 0) continue;

        // setup x and y
        com->write("x", read<float>("x"));
        com->write("y", read<float>("y"));

        // left or right
        trace("colspeedx and y",  colspeedx,  colspeedy, side);
        com->write("y.speed", sinf((i * increment)*M_PI / 180.0f) * colspeedy + rand() % 20);
        com->write("x.speed", cosf((i * increment)*M_PI / 180.0f) * colspeedx + rand() % 20);
        com->active = true;
      }

      if (t == big) {
        write("explode.playing", true);
      }
    }

  public:
    asteroid() : t(none), spawning(false) { }
    virtual component::family family() { return "asteroid"; }
    virtual component::type type() { return "asteroid"; }
    virtual std::string depends() { return "spatial/space2d collider/collider2d dynamics/rigidbody2d kinematics/kinematic2d renderer/renderer2"; }

    void collide(parameterbase::id pid, component::base * lastwrite, object::id pidowner) {
      component::base * other = read<component::base *>("collider.collision");
      if (lastwrite == this) return;
      explode();
      reset();
    }

    virtual void activechanged(parameterbase::id pid, component::base * lastwrite, object::id pidowner) {
      if (active) usedasteroids++;
      else usedasteroids--;
      if (usedasteroids < 0) usedasteroids = 0;
      modinfo("asteroid");
      static int max = 0;
      if (usedasteroids > max) max = usedasteroids;
      trace(free16.size(), "\t", free8.size(), "\t", asteroidno);
      write("roid.render", active);
      write("collider.active", active);
      write("kinematics.active", active);
      write("dynamics.active", active);
    }

    virtual void setup(object::signature & sig) {
      modinfo("asteroid");
      t = eval(sig["asteroid.type"], small);
      active = fetch<bool>("asteroid.active", t == big);
      //write("roid.render", active);
      write("collider.active", active);
      hook("asteroid.active", (component::call) &asteroid::activechanged);
      bind("asteroid.type", t);
      bind("asteroid.fragmentation", frags);
      frags = eval(sig["asteroid.fragmentation"], 0);
      
      // spawn predicted frags
      std::string target = (t == big) ? "asteroid16" : "asteroid8";
      for (int i = 0; i < frags; i++) {
        spawn(target);
        if (t == big)  {
          spawn(target);
          spawn(target);
        }
      }
      
      hook("collider.collision", (component::call)&asteroid::collide);
      x = fetch<float>("x");
      y = fetch<float>("y");
      w = fetch<float>("w");
      h = fetch<float>("h");

      reset();
      
      asteroidno++;
      if (t ==  big) trace("total asteroids by now: ", asteroidno);
    }

    // reset this asteroid: put it back on the empty list.
    void reset() {
      modinfo("asteroid");
      switch (t) {
        case small:
          timeleft = 5.0f;
          free8.insert(owner);
          active = false;
          trace("Resetting a small asteroid", free8.size());
          break;
        case medium:
          active = false;
          free16.insert(owner);
          trace("Resetting a medium asteroid", free16.size());
          break;
        case big:
          trace("Resetting a big asteroid!");
          x = rand() % read<int>("renderer.w");
          y = rand() % read<int>("renderer.h");
          write("roid.alpha", 0.0f);
          spawning = true;
          write("x.speed", (rand() % 2) == 0 ? -10.0f : 10.0f);
          write("y.speed", (rand() % 2) == 0 ? -10.0f : 10.0f);
        default:
          break;
      }
    }
    
    virtual void update(float dt) {
      if (!active) return;
      if (x + w < 0) x = raw<int>("renderer.w") - raw<float>("w");
      else if (x > raw<int>("renderer.w")) x = 0.0f;
      if (y + h < 0) y = raw<int>("renderer.h") - raw<float>("h");
      else if (y > raw<int>("renderer.h")) y = 0.0f;
      if (t == big) {
        if (spawning) {
          float alpha = read<float>("roid.alpha");
          alpha += dt;
          clamp(alpha, 0.0f, 1.0f);
          if (alpha >= 1.0f) spawning = false;
          write("roid.alpha", alpha);
        }
      } else if (t == small) {
        timeleft -= dt;
        clamp(timeleft, 0.0f, 5.0f);
        write("roid.alpha", timeleft / 5.0f);
        if (timeleft <= 0.0f) reset();
      } else {
      }
    }

    virtual ~asteroid() {
      asteroidno--;
    }
};

int asteroid::asteroidno = 0;
int asteroid::usedasteroids = 0;
std::set<object::id> asteroid::free8;
std::set<object::id> asteroid::free16;

g2dcomponent(asteroid, asteroid, asteroid)
