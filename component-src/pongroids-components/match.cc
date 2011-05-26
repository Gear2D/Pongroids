/**
 * @file match.cc
 * @b family match
 * @b type match
 * @author Leonardo Guilherme de Freitas
 * 
 * Controls aspects of the match like score, asteroids on screen,
 * and other code */

#include <gear2d/gear2d.h>
using namespace gear2d;

class match : public component::base {
	private:
		class player {
			public:
				int score;
				player()
				: score(0)
				{
					
				}
		};
	private:
		component::base * leftwall;
		component::base * rightwall;
		component::base * leftfield;
		component::base * rightfield;
		component::base * pong;
		player p1;
		player p2;
		
	private:
		void placepong(float x = 316.0f, float y = 256.0f) {
			pong->write("x", x);
			pong->write("y", y);
			pong->write("x.speed", 1.0f);
			pong->write("y.speed", 1.0f);
		}
	public:
		match() 
		: leftwall(0)
		, rightwall(0)
		, leftfield(0)
		, rightfield(0)
		, pong(0)
		{
			
		}
		virtual component::family family() { return "match"; }
		virtual component::type type() { return "match"; }
		virtual void handle(parameterbase::id pid, component::base * last, object::id pidowner) {
			// pong collided with leftwall
			if (pidowner == leftwall->owner) {
				p2.score++;
				write("score2.text", lexical_cast<std::string>(p2.score));
				placepong(140, 236);
			} else if (pidowner == rightwall->owner) {
				p1.score++;
				write("score1.text", lexical_cast<std::string>(p1.score));
				placepong(500, 236);
			}
		}
		virtual void setup(object::signature & sig) {
			object::id oid = 0;
			oid = spawn("left");
			leftwall = oid->component("spatial");
			hook(leftwall, "collider.collision");
			
			oid = spawn("right");
			rightwall = oid->component("spatial");
			hook(rightwall, "collider.collision");
			
			oid = spawn("pong");
			pong = oid->component("spatial");
		}
		
		virtual void update(float dt) {
		}
};

extern "C" {
	component::base * build() { return new match; }
}