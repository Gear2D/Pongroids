#include "gear2d.h"
using namespace gear2d;

class mainmenu : public gear2d::component::base {
  public:
    virtual component::family family() { return "controller"; }
    virtual component::type type() { return "mainmenu"; }
    virtual std::string depends() { return "menu/mouseover"; }
    
    virtual void singleplayerhover(parameterbase::id pid, base * lastwrite, object::id owner);
    virtual void versushover(parameterbase::id pid, base * lastwrite, object::id owner);
    virtual void exithover(parameterbase::id pid, base * lastwrite, object::id owner);
    
    virtual void setup(object::signature & sig);
    
    
    virtual void handle(parameterbase::id pid, base * lastwrite, object::id owner);
    
  private:
    sigparser sp;
    gear2d::link<float> singleplayeralpha;
    gear2d::link<float> versusalpha;
    gear2d::link<float> exitalpha;
    
    gear2d::link<bool> singleplayerhovered;
    gear2d::link<bool> versushovered;
    gear2d::link<bool> exithovered;
};

void mainmenu::setup(object::signature & sig) {
  sp = sigparser(sig, this);
  hook("singleplayer.hover", (component::call) &mainmenu::singleplayerhover);
  hook("versus.hover", (component::call) &mainmenu::versushover);
  hook("exitgame.hover", (component::call) &mainmenu::exithover);
  
  singleplayerhovered = fetch<bool>("singleplayer.hover");
  versushovered = fetch<bool>("versus.hover");
  exithovered = fetch<bool>("exitgame.hovercd");
  
  singleplayeralpha = fetch<float>("singleplayer.alpha");
  versusalpha = fetch<float>("versus.alpha");
  exitalpha = fetch<float>("exitgame.alpha");
  
  hook("mouse.1");
}

void mainmenu::handle(parameterbase::id pid, component::base * lastwrite, object::id owner) {
  std::string t;
  static char buf[5];
  int mousex, mousey;
  mousex = read<int>("mouse.x");
  mousey = read<int>("mouse.y");
  std::sprintf(buf, "%d", mousex);
  t += buf;
  std::sprintf(buf, "%d", mousey);
  t = t + " " + buf;
  write("mousedbg.text", t);
  write("mousedbg.x", (float)mousex);
  write("mousedbg.y", (float)mousey);
}


void mainmenu::exithover(parameterbase::id pid, component::base * lastwrite, object::id owner) {
  if (exithovered) {
    exitalpha = 1.0f;
  } else {
    exitalpha = 0.5f;
  }
}

void mainmenu::singleplayerhover(parameterbase::id pid, component::base * lastwrite, object::id owner) {
  if (singleplayerhovered) {
    singleplayeralpha = 1.0f;
  } else {
    singleplayeralpha = 0.5f;
  }
}

void mainmenu::versushover(parameterbase::id pid, component::base * lastwrite, object::id owner) {
  if (versushovered) {
    versusalpha = 1.0f;
  } else {
    versusalpha = 0.5f;
  }
}



g2dcomponent(mainmenu, controller, mainmenu);