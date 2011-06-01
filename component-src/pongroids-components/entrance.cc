#include <gear2d/gear2d.h>
using namespace gear2d;
using namespace std;

class entrance : public component::base  {
		virtual component::family family() { return "entrance"; }
		virtual component::type type() { return "entrance"; }
		virtual string depends() { return "keyboard/keyboard menu/singleselect"; }
		
		virtual void handle(parameterbase::id pid, component::base * last, object::id owns) {
			if (pid == "key.up") {
				if (raw<int>("key.up") == 1) {
					cout << "kup" << endl;
					write("menu.prev", true);
				}
			}
			if (pid == "key.down") {
				if (raw<int>("key.down") == 1) {
					cout << "kdown" << endl;
					write("menu.next", true);
				}
			}
			
			if (pid == "key.return") {
				if (raw<string>("menu.focus") == "newgame") load("gamescene.yaml");
			}
			if (pid == "menu.focus") {
				write(read<string>("menu.last") + ".alpha", 0.5f);
				write(read<string>("menu.focus") + ".alpha", 1.0f);
			}
		}
		
		virtual void setup(object::signature & sig) {
			hook("menu.focus");
			hook("key.return");
			hook("key.up");
			hook("key.down");
		}
		
		virtual void update(float dt) {
			
		}
};

extern "C" {
	component::base * build() { return new entrance; }
}