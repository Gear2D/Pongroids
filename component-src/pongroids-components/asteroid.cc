#include <gear2d/gear2d.h>
#include <cstdlib>
#include <cmath>


using namespace gear2d;

/**
 * @file asteroid.cc
 * Component to handle asteroids. Asteroids breaks when kicked
 * and spawns other asteroids
 *
 * @b parameters
 * @li @c asteroid.type Size of this asteroid (big, medium, small). @b string
 * 	When hit, one big asteroid will split into medium. One medium into
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
	namespace component {
		template<> asteroidtype base::eval<asteroidtype>(std::string t, asteroidtype def) {
			if (t == "big") return big;
			if (t == "medium") return medium;
			if (t == "small") return small;
			return def;
		}
	}
}

class asteroid : public component::base {
	private:
		asteroidtype t;
		int frags;
		bool spawning;
		float timeleft;
		
	private:
		void explode() {
			std::string target;
			if (t == big) target = "asteroid16";
			else if (t == medium) target = "asteroid8";
			else return;
			
			const int & side = raw<int>("collider.collision.side");
			
			object::id obj;
			asteroid * com;
			const float & xspeed = raw<float>("x.speed");
			const float & yspeed = raw<float>("y.speed");
			float increment = 360.0f/frags;
			
			for (int i = 0; i < frags; i++) {
				obj = spawn(target);
				com = (asteroid *) obj->component("asteroid");
				if (com == 0) continue;

				// setup x and y
				com->write("x", read<float>("x"));
				com->write("y", read<float>("y"));
				
				// left or right
				com->write("y.speed", sinf((i*increment)*M_PI/180.0f)*10.0f + rand() % 20);
				com->write("x.speed", cosf((i*increment)*M_PI/180.0f)*10.0f + rand() % 20);
			}
			
			if (t == big) {
				object::id otherbig = spawn("asteroid32");
// 				otherbig->component("asteroid")->write<float>("x", -31);
// 				otherbig->component("asteroid")->write<float>("y.speed", 10.0f);
			}
		}
		
	public:
		asteroid() : t(none), spawning(false) { }
		virtual component::family family() { return "asteroid"; }
		virtual component::type type() { return "asteroid"; }
		virtual std::string depends() { return "spatial collider dynamics kinetics renderer"; }
		
		virtual void handle(parameterbase::id pid, component::base * lastwrite, object::id pidowner) {
			if (pid == "collider.collision") {
				component::base * other = read<component::base *>("collider.collision");
				if (lastwrite == this) return;
				explode();
				destroy();
			}
		}
		
		virtual void setup(object::signature & sig) {
			bind("asteroid.type", t);
			bind("asteroid.fragmentation", frags);
			t = eval(sig["asteroid.type"], small);
			frags = eval(sig["asteroid.fragmentation"], 2);
			hook("collider.collision");
			int w = rand() % read<int>("renderer.w"); cout << w << endl;
			int h = rand() % read<int>("renderer.h"); cout << h << endl;
			write<float>("x", w);
			write<float>("y", h);
			if (t == big) {
				write("roid.alpha", 0.0f);
				spawning = true;
				write("x.speed", (rand() % 2) == 0 ? -10.0f : 10.0f);
				write("y.speed", (rand() % 2) == 0 ? -10.0f : 10.0f);
			}
			if (t == small) {
				timeleft = 5.0f;
			}
		}
		
		virtual void update(float dt) {
			if (raw<float>("x") + raw<float>("w") < 0) write<float>("x", raw<int>("renderer.w") - raw<float>("w"));
			else if (raw<float>("x") > raw<int>("renderer.w")) write("x", 0.0f);
			if (raw<float>("y") + raw<float>("h") < 0) write<float>("y", raw<int>("renderer.h") - raw<float>("h"));
			else if (raw<float>("y") > raw<int>("renderer.h")) write("y", 0.0f);
			if (t == big) {
				if (spawning) {
					float alpha = read<float>("roid.alpha");
					alpha += dt;
					clamp(alpha, 0.0f, 1.0f);
					if (alpha >= 1.0f) spawning = false;
					write("roid.alpha", alpha);
				}
			}
			if (t == small) {
				timeleft -= dt;
				clamp(timeleft, 0.0f, 5.0f);
				write("roid.alpha", timeleft / 5.0f);
				if (timeleft <= 0.0f) destroy();
			}
		}
};

extern "C" {
	component::base * build() { return new asteroid; }
}