#include <gear2d/gear2d.h>
#include <stack>
using namespace gear2d;
using namespace std;

class entrance : public component::base  {
	private:
		component::base * mainmenu;
		component::base * newgamemenu;
		component::base * menufocus;
		std::stack<component::base *> menustack;
		
	public:
		virtual component::family family() { return "entrance"; }
		virtual component::type type() { return "entrance"; }
		virtual string depends() { return "keyboard/keyboard"; }
		
		virtual void handle(parameterbase::id pid, component::base * last, object::id owns) {
			if (pid == "key.up") {
				if (raw<int>("key.up") == 1) {
					menufocus->write("menu.prev", true);
				}
			}
			if (pid == "key.down") {
				if (raw<int>("key.down") == 1) {
					menufocus->write("menu.next", true);
				}
			}
			
			if (pid == "key.return") {
				// check to see if its the main menu
				// custom menu handling. We will try to spawn the object that have the name.
				if (raw<int>("key.return") != 1) return;
				if (menufocus == mainmenu) {
					if (mainmenu->raw<string>("menu.focus") == "newgame") {
// 						mainmenu->add("x", -150.0f) ;
						newgamemenu = spawn("entrance/newgamemenu")->component("menu");
						hook(newgamemenu, "menu.focus");
						menufocus = newgamemenu;
						return;
					}
				}
				
				if (menufocus == newgamemenu) {
					if (newgamemenu->raw<string>("menu.focus") == "singleplayer") {
						load("singleplayer");
						return;
					}
					
					if (newgamemenu->raw<string>("menu.focus") == "versus") {
						load("versus");
						return;
					}
				}
			}
			
			if (pid == "key.escape") {
				if (raw<int>("key.escape") != 1) return;
				if (menufocus == newgamemenu) {
					menufocus = mainmenu;
					newgamemenu->destroy();
					newgamemenu = 0;
				}
			}
			if (pid == "menu.focus") {
				menufocus->write(menufocus->read<string>("menu.last") + ".alpha", 0.5f);
				menufocus->write(menufocus->read<string>("menu.focus") + ".alpha", 1.0f);
				cout << "menu.last " << menufocus->read<string>("menu.last") << endl;
				cout << "menu.focus " << menufocus->read<string>("menu.focus") << endl;
			}
		}
		
		virtual void setup(object::signature & sig) {
			hook("key.return");
			hook("key.up");
			hook("key.down");
			hook("key.escape");
			
			mainmenu = spawn("entrance/mainmenu")->component("menu");
			menufocus = mainmenu;
			hook(mainmenu, "menu.focus");
		}
		
		virtual void update(float dt) {
			if (menufocus == mainmenu) {
				mainmenu->write("x.speed", (350.0f - mainmenu->read<float>("x"))*10.0f);
			} else {
				mainmenu->write("x.speed", (200.0f - mainmenu->read<float>("x"))*10.0f);
			}
		}
};

extern "C" {
	component::base * build() { return new entrance; }
}