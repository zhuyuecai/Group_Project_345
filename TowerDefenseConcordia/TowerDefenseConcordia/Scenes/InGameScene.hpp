#pragma once

#include "../WindowManager.hpp"
#include "../Map.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include "../MapMessages.hpp"
#include "../Critter.hpp"
#include "IModeBehaviour.hpp"
#include <memory>
#include "../Tower/ITower.hpp"
#include "../TextButton.hpp"
#include "../Tower/ITower.hpp"

namespace TDC
{
	class InGameScene : public Scene
	{
	public:
		InGameScene();
		virtual ~InGameScene();
		virtual void init();
		void generate();
		void load(const std::string &path);
		virtual void update(const sf::Time &dt, sf::RenderWindow *window);
	private:

		struct IState
		{
			IState(std::size_t _id)
				: id(_id)
			{}
			const std::size_t id;
		protected:
			static std::size_t __idCounter;
		};

		template <typename T>
		struct State : public IState
		{
		public:
			State()
				: IState(getId())
			{}

			static const std::size_t getId()
			{
				static std::size_t id = __idCounter++;
				return id;
			}
		};

		struct PlaceMinigun : public State<PlaceMinigun>
		{};

		struct PlaceRocket : public State <PlaceRocket>
		{};

		struct ShowTowerInfos : public State < ShowTowerInfos >
		{
			ShowTowerInfos(std::size_t _index)
				: index(_index)
			{}
			std::size_t index;
		};

		virtual bool _event(const sf::Event &event);
		Map _map;
		int _money;
		std::unique_ptr<TextButton> _moneyBtn;
		std::unique_ptr<TextButton> _createTowerBtnType1;
		std::unique_ptr<TextButton> _createTowerBtnType2;
		std::unique_ptr<TextButton> _createTowerBtnType3;
		std::unique_ptr<IState> _state;
		std::unique_ptr<TextButton> _upgradeBtn;
		std::unique_ptr<TextButton> _sellBtn;
		std::unique_ptr<TextButton> _towerInfos;

		std::size_t _towerIndex;
		std::size_t _cellSizeRatio;
		sf::Font _arial;
		std::vector < Critter > _critters;
		std::map < std::size_t, std::unique_ptr < ITower >> _towers;
	};
}