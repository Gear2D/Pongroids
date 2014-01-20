#include "gear2d.h"
#include <string>
using namespace gear2d;

class menumouseover : public component::base {
  public:
    virtual void setup(gear2d::object::signature & sig);
    virtual void update(gear2d::timediff dt);
    virtual gear2d::component::family family() { return "menu"; }
    virtual gear2d::component::type type() { return "mouseover"; }
    virtual std::string depends() { return "mouse/mouse"; }
    virtual void optionschanged(parameterbase::id pid, base * lastwrite, object::id owner);
    virtual void orderchanged(parameterbase::id pid, base * lastwrite, object::id owner);
    
    
  private:
    sigparser sp;
    gear2d::link<std::string> menuoptions;

    struct menuopt {
      std::string id;
      gear2d::link<int> order;
      gear2d::link<bool> hover;
      gear2d::link<bool> selected;
      gear2d::link<float> x, y;
      gear2d::link<int> w, h;
    };
    
    // position of the object
    gear2d::link<float> x, y;
    
    gear2d::link<int> mousex, mousey, mouse1;
    
    std::vector<menuopt *> options;
    
    void setoptions();
    
};

void menumouseover::optionschanged(parameterbase::id pid, component::base * lastwrite, object::id owner) {
  setoptions();
}

void menumouseover::orderchanged(parameterbase::id pid, component::base * lastwrite, object::id owner) {
  std::sort(options.begin(), options.end(), [](menuopt * first, menuopt * second) { return first->order < second->order; });
}

void menumouseover::setoptions() {
  std::set<std::string> opts = split<std::set<std::string>>(menuoptions, ' ');
  for (auto opt : opts) {
    for (menuopt * found : options) {
      // do nothing for existend menus.
      if (found->id == opt) continue;
    }
    
    menuopt * o = new menuopt();
    o->id = opt;
    o->order = sp.init(opt + ".order", 0);
    hook(opt + ".order", (component::call) &menumouseover::orderchanged);
    o->hover = sp.init(opt + ".hover", false);
    o->selected = sp.init(opt + ".selected", false);
    o->x = sp.init<float>(opt + ".x");
    o->y = sp.init<float>(opt + ".y");
    o->w = sp.init<int>(opt + ".w");
    o->h = sp.init<int>(opt + ".h");
    options.push_back(o);
  }
  
  std::sort(options.begin(), options.end(), [](menuopt * first, menuopt * second) { return first->order < second->order; });
}

void menumouseover::setup(gear2d::object::signature & sig) {
  sp = sigparser(sig, this);
  menuoptions = sp.init("menu.options");
  hook("menu.options", (component::call) &menumouseover::optionschanged);
  mousex = sp.init("mouse.x", 0);
  mousex = sp.init("mouse.y", 0);
  mouse1 = sp.init("mouse.1", 0);
  setoptions();
}

void menumouseover::update(timediff dt) {
  for (menuopt * opt : options) {
    if (mousex > opt->x + x &&
        mousex < opt->x + x + opt->w &&
        mousey > opt->y + y &&
        mousey < opt->y + y + opt->h)
    {
      if (!opt->hover) opt->hover = true;
      if (mouse1 == 1)
        if (!opt->selected) opt->selected = true;
    }
  }
}


g2dcomponent(menumouseover, menu, mouseover);

