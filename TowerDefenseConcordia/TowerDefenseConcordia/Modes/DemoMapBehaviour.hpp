
#include "../WindowManager.hpp"
#include "../Map.hpp"
#include "../Message.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include "../MapMessages.hpp"
#include "../Critter.hpp"
#include "IModeBehaviour.hpp"
#include <memory>

namespace TDC
{
	class DemoMap : public IModeBehaviour
	{
	public:
		DemoMap();
		virtual ~DemoMap();
		virtual void init();
		void generate();
		void load(const std::string &path);
		virtual void update(const sf::Time &dt, sf::RenderWindow *window);
	private:
		void _events(const sf::Event &event);
		void computeCellSizeRatio(sf::Vector2u size);
		Map _map;
		std::size_t _cellSizeRatio;
		sf::Font _arial;
		std::vector < Critter > _critters;
	};
}